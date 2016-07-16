/*
	ddf_font.h v0.1 public domain. No warranty; use at your own risk.
	-- created by nyorain --

	Simple single header file to parse multi-channel signed distance field fonts and their glyph
	properties.

	Can be used like this:

		#define DDF_FONT_IMPLEMENTATION
		#include "ddf_font.h"

		...

		ddf_font font;
		if(!ddf_font_create(&font, "myFont.ddf")) {
			printf("ddf_font error %d", ddf_get_last_error());
			exit(-1);
		}

		unsigned int width = font.texture_width;
		unsigned int height = font.texture_height;
		unsigned int format = font.texture_format; //1 for alpha, 3 for rgb

		//The texture data stored in the font is a tightly-packed row-major raw rgb buffer.
		//Therefore the size of the data is width * height * 3.
		const uint8_t* data = font.texture_data;

		const ddf_glyph_properties* props = ddf_get_glyph_properties(&font, 'A');
		if(!props) {
			printf("ddf error %d", ddf_get_last_error());
			exit(-1);
		}

		//Now the font can be rendered using the textureData and the glyph properties.
		//The returned types are not opaque, therefore one can simply access their members.

	Note that this file does ONLY parse fonts in the described-distance-field-font (".ddf")
	format and can not be used for ttf or similiar.
	Those files basically store a distance field image and a description about its glyph layout.

	The texture data can either be of format ddf_format_a for a default (signed) distance field
	or can be of format ddf_format_rgb for a multichannel signed distacne field.

	For more information of multichannel signed distance fields see:
	https://github.com/Chlumsky/msdfgen


	Configuration
	=============

	This header file can be configured by multiple macros. Remember that all of those macros
	must be defined BEFORE including the header file.

		* DDF_FONT_IMPLEMENATION Will enable the implementation if defined.
		* DDF_FONT_ALLOC An allocate function like alloc. Can be set to something fancy
			like tcalloc or some custom memory pool function for more performance.
			By default malloc.
		* DDF_FONT_FREE A function like free. Must be able to free buffer allocated by
			DDF_FONT_ALLOC. By default free.
		* DDF_FONT_THREADLOCAL A threadlocal keyword.
			Will be used for a threadlocal error variable, so if the library may be used
			from multiple thread, it must be a valid keyword. Otherwise it might be
			benficial to define it empty.
			Default: For C11 thread_local otherwise empty

	This software is dual-licensed to the public domain and under the following
	license: you are granted a perpetual, irrevocable license to copy, modify,
	publish, and distribute this file as you see fit.

	Inspired by stb.
*/

#ifndef DDF_FONT_INCLUDE_H
#define DDF_FONT_INCLUDE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DDF_FONT_MAGIC_NUMBER 0xB00000B5

///All return errors are members of this enumeration.
typedef enum ddf_error {
	ddf_error_none = 0,
	ddf_error_invalid_file,
	ddf_error_invalid_magic_number,
	ddf_error_invalid_data,
	ddf_error_invalid_glyph,
	ddf_error_out_of_memory, //signals that DDF_FONT_ALLOC (malloc by default) returned 0
	ddf_error_invalid_format,
	ddf_error_png_not_enabled
} ddf_error;

///The format of a loaded ddf font.
typedef enum ddf_texture_format {
	ddf_texture_format_none = 0,
	ddf_texture_format_a = 1,
	ddf_texture_format_rgb = 3
} ddf_texture_format;

///The properties of a single glyph.
typedef struct ddf_glyph_properties {
	float offset_x;
	float offset_y;
	float texcoord_x;
	float texcoord_y;
	float texwidth; //The height in the texture in pixels
	float texheight; //The width in the texture in pixels
	float charwidth; //The real width of the character
	float charheight; //The real height of the character
	float advance; //The horizontal advance in pixels
} ddf_glyph_properties;

///A ddf font glyph. Structure just used for instal storage
typedef struct ddf_glyph {
	uint32_t unicode;
	ddf_glyph_properties properties;
} ddf_glyph;

///The ddf font class storing the loaded image as well as the glyph properties.
typedef struct ddf_font {
	uint32_t texture_width;
	uint32_t texture_height;
	uint8_t texture_format; //value of ddf_texture_format
	uint8_t* texture_data;

	uint32_t glyph_count;
	ddf_glyph* glyphs;
} ddf_font;

///Returns the last occurred error.
///If there was no error since start of this thread or ddf_reset_last_error was called,
///ddf_error_none will be returned.
ddf_error ddf_get_last_error();
void ddf_reset_last_error();

///Sets last error to invalid_file If the given file cannot be opened.
///Sets last error to invalid_magic_number If the contents of the given file do not start
///with the formats magic number.
///Sets last error to invalid_data If the contents of the file cannot be parsed.
///Sets the last error to out_of_memory if an allocation fails.
bool ddf_font_create(ddf_font* font, const char* filename);

///Sets the last error to invalid_magic_number If the given buffer does not start with the formats
///magic number.
///Sets the last error to invalid_data If the given data cannot be parsed.
///Sets the last error to out_of_memory if an allocation fails.
bool ddf_font_create_from_memory(ddf_font* font, const uint8_t* buffer, size_t size);

///Return nullptr on failure.
///Sets the last error to invalid_glyph If the given glyph does not exist in the given font.
const ddf_glyph_properties* ddf_font_get_glyph_properties(const ddf_font* font, uint32_t unicode);

///Destoys the given font and frees all its allocated memory.
void ddf_font_destroy(ddf_font* font);

#ifdef __cplusplus
} //extern "C"
#endif

///End of the header file.
///--------------------------------------------------------------------------------------
///implementation
#ifdef DDF_FONT_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>

//macros
//one can provide custom macros to improve dynamic memory allocatoin
#ifndef DDF_FONT_FREE
 #define DDF_FONT_FREE(x) free(x)
#endif

#ifndef DDF_FONT_ALLOC
 #define DDF_FONT_ALLOC(x) malloc(x)
#endif

#ifndef DDF_FONT_THREADLOCAL
 #if __STDC_VERSION__ >= 201112L
  #define DDF_FONT_THREADLOCAL thread_local
 #else
  //we could try some custom compiler dependent macro using here
  //#error "Specify a valid threadlocal macro in DDF_FONT_THREADLOCAl"
  #define DDF_FONT_THREADLOCAL
 #endif
#endif

//global last error var
static DDF_FONT_THREADLOCAL ddf_error ddf_last_error_ = ddf_error_none;

ddf_error ddf_get_last_error()
{
	return ddf_last_error_;
}

void ddf_reset_last_error()
{
	ddf_last_error_ = ddf_error_none;
}

bool ddf_font_create(ddf_font* font, const char* filename)
{
	memset(font, 0, sizeof(*font));

	FILE* file = fopen(filename, "rb");
	if(!file) {
		ddf_last_error_ = ddf_error_invalid_file;
		return false;
	}

	fseek(file, 0, SEEK_END);
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	uint8_t* buffer = (uint8_t*) DDF_FONT_ALLOC(fsize + 1);
	if(!buffer){
		fclose(file);
		ddf_last_error_ = ddf_error_out_of_memory;
		return false;
	}

	fread(buffer, fsize, 1, file);
	fclose(file);

	bool ret = ddf_font_create_from_memory(font, buffer, fsize);
	DDF_FONT_FREE(buffer);

	return ret;
}

bool ddf_font_create_from_memory(ddf_font* font, const uint8_t* buffer, size_t size)
{
	memset(font, 0, sizeof(*font));

	uint32_t magic;
	size_t texture_size;

	const uint8_t* buffer_end = buffer + size;
	if(size <= sizeof(magic) + sizeof(font->glyph_count))
		goto error1;

	memcpy(&magic, buffer, sizeof(magic));
	buffer += sizeof(magic);

	if(magic != DDF_FONT_MAGIC_NUMBER) {
		ddf_last_error_ = ddf_error_invalid_magic_number;
		return false;
	}

	memcpy(&font->glyph_count, buffer, sizeof(font->glyph_count));
	buffer += sizeof(font->glyph_count);

	if(!font->glyph_count || buffer + font->glyph_count >= buffer_end)
		goto error1;

	font->glyphs = (ddf_glyph*) DDF_FONT_ALLOC(sizeof(*font->glyphs) * (font->glyph_count));
	if(!font->glyphs)
		goto error2;

	for(unsigned int i = 0; i < font->glyph_count; ++i) {
		memcpy(&font->glyphs[i], buffer, sizeof(font->glyphs[i]));
		buffer += sizeof(font->glyphs[i]);
	}

	if(buffer + sizeof(uint32_t) * 2 >= buffer_end)
		goto error1_free_glyphs;

	memcpy(&font->texture_format, buffer, sizeof(font->texture_format));
	buffer += sizeof(font->texture_format);

	if(font->texture_format != 1 && font->texture_format != 3) {
		DDF_FONT_FREE(font->glyphs);
		memset(&font, 0, sizeof(font));
		ddf_last_error_ = ddf_error_invalid_format;
		return false;
	}

	memcpy(&font->texture_width, buffer, sizeof(font->texture_width));
	buffer += sizeof(font->texture_width);

	memcpy(&font->texture_height, buffer, sizeof(font->texture_height));
	buffer += sizeof(font->texture_height);

	texture_size = font->texture_width * font->texture_height * font->texture_format;
	if(!font->texture_width || !font->texture_height || buffer + texture_size > buffer_end)
		goto error1_free_glyphs;

	font->texture_data = (uint8_t*) DDF_FONT_ALLOC(texture_size);
	if(!font->texture_data) {
		DDF_FONT_FREE(font->glyphs);
		goto error2;
	}

	memcpy(font->texture_data, buffer, texture_size);
	return true;

//invalid data
error1_free_glyphs:
	DDF_FONT_FREE(font->glyphs);

error1:
	memset(font, 0, sizeof(*font));
	ddf_last_error_ = ddf_error_invalid_data;
	return false;

//out_of_memory
error2:
	memset(font, 0, sizeof(*font));
	ddf_last_error_ = ddf_error_out_of_memory;
	return false;

}

const ddf_glyph_properties* ddf_get_glyph_properties(const ddf_font* font, uint32_t unicode)
{
	for(size_t i = 0; i < font->glyph_count; ++i)
		if(font->glyphs[i].unicode == unicode)
			return &font->glyphs[i].properties;

	ddf_last_error_ = ddf_error_invalid_glyph;
	return NULL;
}

void ddf_font_destroy(ddf_font* font)
{
	if(font->texture_data) DDF_FONT_FREE(font->texture_data);
	if(font->glyphs) DDF_FONT_FREE(font->glyphs);
	memset(font, 0, sizeof(*font));
}

#endif //DDF_FONT_IMPLEMENTATION
#endif //header guard
