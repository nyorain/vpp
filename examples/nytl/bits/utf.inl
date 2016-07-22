/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 nyorain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#pragma once

std::size_t charCount(const std::string& utf8)
{
	std::size_t count = 0u;
	for(auto& byte : utf8)
    	if((byte & 0xc0) != 0x80) ++count;

	return count;
}

std::array<char, 4> nth(const std::string& utf8, std::size_t n)
{
	std::array<char, 4> ret {};

	std::size_t count = 0u;
	std::size_t charNum = 0u;
	for(auto& byte : utf8)
	{
		if(count == n)
		{
			ret[charNum] = byte;
			++charNum;
		}

      	if((byte & 0xc0) != 0x80) ++count;
		if(count > n) break;
	}

	if(!charNum) throw std::out_of_range("ny::nth(utf8)");
	return ret;
}

const char& nth(const std::string& utf8, std::size_t n, std::uint8_t& size)
{
	const char* ret = nullptr;
	size = 0;

	std::size_t count = 0u;
	std::size_t charNum = 0u;
	for(auto& byte : utf8)
	{
		if(count == n)
		{
			if(size == 0) ret = &byte;
			++size;
		}

      	if((byte & 0xc0) != 0x80) ++count;
		if(count > n) break;
	}

	if(!ret) throw std::out_of_range("ny::nth(utf8)");
	return *ret;
}

char& nth(std::string& utf8, std::size_t n, std::uint8_t& size)
{
	char* ret = nullptr;
	size = 0;

	std::size_t count = 0u;
	std::size_t charNum = 0u;
	for(auto& byte : utf8)
	{
		if(count == n)
		{
			if(size == 0) ret = &byte;
			++size;
		}

      	if((byte & 0xc0) != 0x80) ++count;
		if(count > n) break;
	}

	if(!ret) throw std::out_of_range("ny::nth(utf8)");
	return *ret;
}

std::u16string utf8to16(const std::string& utf8)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	return converter.from_bytes(utf8);
}
std::u32string utf8to32(const std::string& utf8)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
	return converter.from_bytes(utf8);
}
std::string utf16to8(const std::u16string& utf16)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	return converter.to_bytes(utf16);
}
std::u32string utf16to32(const std::u16string& utf16)
{
	return utf8to32(utf16to8(utf16));
}
std::string utf32to8(const std::u32string& utf32)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
	return converter.to_bytes(utf32);
}
std::u16string utf32to16(const std::u32string& utf32)
{
	return utf8to16(utf32to8(utf32));
}
