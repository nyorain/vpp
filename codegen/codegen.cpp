//The below given string literal also defines the copyrights for this document

constexpr const auto copyright = 1 + R"SRC(
// Copyright © 2016 nyorain
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the “Software”), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// The implementation for the Flag tempalte class was taken from nvidias vkcpp project:
// Changes were made to it, which are protected by the above given license.
// link (May 2016): https://github.com/nvpro-pipeline/vkcpp
// license:

// Copyright(c) 2015-2016, NVIDIA CORPORATION. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
)SRC";

#include "codegen.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#include <locale>


RegistryLoader::RegistryLoader(const std::string& xmlPath)
{
	auto result = doc_.load_file(xmlPath.c_str());
	std::cout << "Load result: " << result.description() << "\n";
}

Registry& RegistryLoader::parse()
{
	//registry
	auto regNode = doc_.child("registry");

	auto& cr = registry_.copyright;
	cr = regNode.child_value("comment");
	auto pos = cr.find('\n');
	while(cr.find('\n', pos + 1) != std::string::npos) //exclude last newline
	{
		cr.insert(pos + 1, "// ");
		pos = cr.find('\n', pos + 1);
	}

	//vendors
	for(auto vendor : regNode.child("vendorids").children("vendorid"))
		registry_.vendors.push_back(vendor.attribute("name").as_string());

	//tags
	for(auto tag : regNode.child("tags").children("tag"))
		registry_.tags.push_back(tag.attribute("name").as_string());

	//types
	std::cout << "\ttypes:\n";
	auto types = regNode.child("types");
	loadTypes(types);

	//enums
	std::cout << "\tenums:\n";
	for(auto enumit : regNode.children("enums"))
		loadEnums(enumit);

	//commands
	std::cout << "\tcommands:\n";
	auto commands = regNode.child("commands");
	loadCommands(commands);

	//features
	std::cout << "\tfeatures:\n";
	for(auto feature : regNode.children("feature"))
		loadFeature(feature);

	//extensions
	std::cout << "\textensions:\n";
	for(auto extension : regNode.child("extensions").children("extension"))
		loadExtension(extension);

	return registry_;
}

void RegistryLoader::loadTypes(const pugi::xml_node& node)
{
	//all external types
	for(auto typeit : node.children("type"))
	{
		std::string category = typeit.attribute("category").as_string();
		std::string reqs = typeit.attribute("requires").as_string();
		if(category == "" && reqs != "")
		{
			auto name = typeit.attribute("name").as_string();
			ExternalType ext(name, typeit);
			registry_.externalTypes.push_back(ext);

			std::cout << "\t\texternal: " << name << "\n";
		}
	}

	//all enums and typedefs
	for(auto typeit : node.children("type"))
	{
		std::string category = typeit.attribute("category").as_string();
		if(category == "enum")
		{
			auto name = typeit.attribute("name").as_string();
			registry_.enums.emplace_back(name, typeit);

			std::cout << "\t\tenum: " << name << "\n";
		}
		else if(category == "basetype")
		{
			auto name = typeit.child_value("name");
			auto basetype = typeit.child_value("type");
			BaseType t(name, typeit);
			t.original = registry_.findType(basetype);
			registry_.baseTypes.push_back(t);

			std::cout << "\t\tbasetype: " << name << "\n";
		}
	}

	//then all bitmasks
	for(auto typeit : node.children("type"))
	{
		std::string category = typeit.attribute("category").as_string();
		if(category == "bitmask")
		{
			auto en = typeit.attribute("requires");
			auto name = typeit.child_value("name");

			Enum* e = nullptr;
			if(en) e = registry_.findEnum(en.as_string());

			registry_.bitmasks.emplace_back(name, typeit);
			if(e) registry_.bitmasks.back().bits = e;

			std::cout << "\t\tbitmask: " << name << "\n";
		}
	}

	//then all handles
	for(auto typeit : node.children("type"))
	{
		std::string category = typeit.attribute("category").as_string();
		if(category == "handle")
		{
			//auto parent = typeit.attribute("parent").as_string();
			auto name = typeit.child_value("name");
			Handle handle(name, typeit);
			registry_.handles.push_back(handle);

			std::cout << "\t\thandle: " << name << "\n";
		}
	}

	//then all structs prototypes
	for(auto typeit : node.children("type"))
	{
		std::string category = typeit.attribute("category").as_string();
		if(category == "struct" || category == "union")
		{
			auto name = typeit.attribute("name").as_string();
			Struct s(name, typeit);

			if(category == "union") s.isUnion = true;
			if(typeit.attribute("returnedonly").as_bool() == true) s.returnedonly = true;

			registry_.structs.push_back(s);

			std::cout << "\t\tstruct: " << name << "\n";
		}
	}

	//then all func pointers
	for(auto typeit : node.children("type"))
	{
		std::string category = typeit.attribute("category").as_string();
		if(category == "funcpointer")
		{
			auto name = typeit.child_value("name");
			FuncPtr ptr(name, typeit);

			//get rid of "typedef " suffix and " (VKAPI_PTR *"
			auto retString = std::string(typeit.text().as_string()).substr(8, std::string::npos);
			retString = retString.substr(0, retString.size() - 13);

			auto constPos = retString.find("const");
			if(constPos != std::string::npos)
			{
				ptr.signature.returnType.constant = true;
				retString = retString.substr(constPos + 6, std::string::npos);
			}

			ptr.signature.returnType.pointerlvl = std::count(retString.begin(), retString.end(), '*');
			retString.erase(std::remove(retString.begin(), retString.end(), '*'), retString.end());

			auto type = registry_.findType(retString);
			if(!type)
			{
				std::cout << "### Couldnt find type " << retString << "\n";
				continue;
			}

			ptr.signature.returnType.type = type;

			for(auto paramNode : typeit.children("type"))
			{
				Param param;
				auto type = registry_.findType(paramNode.text().get());
				if(!type)
				{
					std::cout << "### Couldnt find type " << paramNode.text().get() << "\n";
					continue;
				}

				param.type.type = type;
				std::string prev = paramNode.previous_sibling().value();
				std::string next = paramNode.next_sibling().value();

				if(prev.find("const") != std::string::npos) param.type.constant = true;
				param.type.pointerlvl = std::count(next.begin(), next.end(), '*');

				next.erase(std::remove(next.begin(), next.end(), '*'), next.end());
				next.erase(std::remove(next.begin(), next.end(), ' '), next.end());
				next.erase(std::remove(next.begin(), next.end(), '\t'), next.end());

				auto pos = next.find(',');
				if(pos == std::string::npos) pos = next.find(')');
				next = next.substr(pos - 1, std::string::npos);

				param.name = next;
				ptr.signature.params.push_back(param);
			}

			registry_.funcPtrs.push_back(ptr);
		}
	}

	//struct impl, members
	for(auto typeit : node.children("type"))
	{
		std::string category = typeit.attribute("category").as_string();
		if(category == "struct" || category == "union")
		{
			auto name = typeit.attribute("name").as_string();
			auto s = registry_.findStruct(name);
			if(!s)
			{
				std::cout << "### Couldnt find struct " << name << "\n";
				continue;
			}

			for(auto& member : typeit.children("member"))
				s->members.emplace_back(parseParam(member));
		}
	}
}

void RegistryLoader::loadEnums(const pugi::xml_node& node)
{
	std::string type = node.attribute("type").as_string();
	std::string name = node.attribute("name").as_string();

	std::cout << "\t\t" << name << "\n";
	if(type == "enum" || type == "bitmask")
	{
		auto retp = registry_.findEnum(name);
		if(!retp)
		{
			std::cout << "### Couldnt find " << name << "\n";
			return;
		}

		auto& ret = *retp;

		if(type == "bitmask") ret.bitmask = true;

		for(auto enumit : node.children("enum"))
		{
			std::pair<std::string, std::int64_t> value;

			value.first = enumit.attribute("name").as_string();

			auto attrib = enumit.attribute("value");
			if(!attrib) attrib = enumit.attribute("bitpos");

			auto str = std::string(attrib.as_string());
			if(str.size() > 1 && str.substr(0, 2) == "0x") value.second = std::stoul(str, nullptr, 16);
			else value.second = attrib.as_llong();

			ret.values.push_back(value);
		}

		auto unusedChild = node.child("unused");
		if(unusedChild) ret.unusedStart = unusedChild.attribute("start").as_int();
		else ret.unusedStart = ret.values.back().second;
	}
	else if(name == "API Constants")
	{
		for(auto enumit : node.children("enum"))
		{
			auto cname = enumit.attribute("name").as_string();
			auto value = enumit.attribute("value").as_llong();
			Constant constant(enumit);
			constant.name = cname;
			constant.value = value;
			registry_.constants.push_back(constant);
		}
	}
}

void RegistryLoader::loadCommands(const pugi::xml_node& node)
{
	for(auto cmd : node.children("command"))
	{
		Command command{};

		auto p = parseParam(cmd.child("proto"));
		command.signature.returnType = p.type;
		command.name = p.name;

		for(auto param : cmd.children("param"))
			command.signature.params.emplace_back(parseParam(param));

		registry_.commands.push_back(command);

		std::cout << "\t\t" << p.name << "\n";
	}
}

Param RegistryLoader::parseParam(const pugi::xml_node& node)
{
	Param ret;

	for(auto child : node)
	{
		std::string n = child.name();
		if(n == "type") ret.type.type = registry_.findType(child.text().get());
		if(n == "name") ret.name = child.text().get();
		else
		{
			std::string txt = child.value();
			ret.type.pointerlvl += std::count(txt.begin(), txt.end(), '*');
			if(txt.find("const") != std::string::npos) ret.type.constant = true;

			auto posbegin = txt.find("[");
			auto posend = txt.find("]");
			while(posbegin != std::string::npos && posend != std::string::npos)
			{
				auto arr = txt.substr(posbegin + 1, posend - 1);
				ret.type.arraylvl.push_back(arr);

				posbegin = txt.find("[", posend);
				posend = txt.find("]", posend);
			}
		}
	}

	return ret;
}

void RegistryLoader::loadFeature(const pugi::xml_node& node)
{
	Feature feature;
	feature.reqs = parseRequirements(node);
	feature.name = node.attribute("name").as_string();
	feature.api = node.attribute("api").as_string();
	feature.number = node.attribute("number").as_string();
	registry_.features.push_back(feature);

	std::cout << "\t\t" << feature.name << " " << feature.api << "\n";
}

void RegistryLoader::loadExtension(const pugi::xml_node& node)
{
	Extension extension;
	extension.reqs = parseRequirements(node);
	extension.number = node.attribute("number").as_int();
	extension.name = node.attribute("name").as_string();
	extension.protect = node.attribute("protect").as_string();
	registry_.extensions.push_back(extension);

	auto supportedAttrib = node.attribute("supported");
	if(supportedAttrib)
	{
		std::string supported = supportedAttrib.as_string();
		if(supported != "disabled")
		{
			auto feature = registry_.findFeatureByApi(supported);
			if(feature) feature->extensions.push_back(&registry_.extensions.back());
			else std::cout << "### couldnt find feature " << supported << "\n";
		}
	}

	std::cout << "\t\t" << extension.name << "\n";
}

Requirements RegistryLoader::parseRequirements(const pugi::xml_node& node)
{
	Requirements ret;

	int number = 0;
	auto numberAttrib = node.attribute("number");
	if(numberAttrib) number = numberAttrib.as_int() - 1;

	for(auto& require : node.children("require"))
	{
		//types
		for(auto& req : require.children("type"))
		{
			std::string name = req.name();
			auto type = registry_.findType(req.attribute("name").as_string());
			if(!type)
			{
				std::cout << "###couldnt find type " << req.attribute("name").as_string() << "\n";
				continue;
			}
			parseTypeReqs(*type, ret);
		}

		//commands
		for(auto& req : require.children("command"))
		{
			auto cmd = registry_.findCommand(req.attribute("name").as_string());
			if(!cmd)
			{
				std::cout << "###couldnt find cmd " << req.attribute("name").as_string() << "\n";
				continue;
			}
			parseCommandReqs(*cmd, ret);
		}

		//enums
		for(auto& req : require.children("enum"))
		{

			auto enumName = req.attribute("name").as_string();
			auto extAttrib = req.attribute("extends");
			if(!extAttrib)
			{
				auto valueAttrib = req.attribute("value");
				if(valueAttrib)
				{
					Constant constant(req);
					constant.name = req.attribute("name").as_string();
					constant.value = valueAttrib.as_string();
					ret.extraConstants.push_back(constant);
				}
				else
				{
					auto constant = registry_.findConstant(enumName);
					if(!constant)
					{
						std::cout << "### couldnt find constant " << enumName << "\n";
						continue;
					}
					ret.constants.push_back(constant);
				}
			}
			else
			{
				std::string dir = "+";
				auto dirAttrib = req.attribute("dir");
				if(dirAttrib) dir = dirAttrib.as_string();

				auto offset = req.attribute("offset").as_llong();
				offset += number * 1000; //extension number queried in the beginning

				auto extEnum = registry_.findEnum(extAttrib.as_string());
				if(!extEnum)
				{
					std::cout << "### couldnt find enum " << extEnum << "\n";
					continue;
				}

				std::int64_t value = 0;
				if(dir == "+") value = 1000000000 + offset;
				else if(dir == "-") value = -1000000000 - offset;

				auto v = std::make_pair(req.attribute("name").as_string(), value);
				extEnum->values.push_back(v);
			}
		}
	}

	return ret;
}

void RegistryLoader::parseTypeReqs(Type& type, Requirements& reqs)
{
	//check it is not already parsed
	for(auto& t : reqs.types)
		if(t->name == type.name) return;

	if(type.category == Type::Category::structure)
	{
		auto& s = static_cast<Struct&>(type);
		for(auto& member : s.members)
			if(member.type.type) parseTypeReqs(*member.type.type, reqs);
	}
	else if(type.category == Type::Category::funcptr)
	{
		auto& ptr = static_cast<FuncPtr&>(type);
		for(auto& param : ptr.signature.params)
			if(param.type.type) parseTypeReqs(*param.type.type, reqs);
	}
	else if(type.category == Type::Category::bitmask)
	{
		auto& mask = static_cast<Bitmask&>(type);
		if(mask.bits) parseTypeReqs(*mask.bits, reqs);
	}

	reqs.types.push_back(&type);
}

void RegistryLoader::parseCommandReqs(Command& cmd, Requirements& reqs)
{
	for(auto& param : cmd.signature.params)
		parseTypeReqs(*param.type.type, reqs);

	parseTypeReqs(*cmd.signature.returnType.type, reqs);

	reqs.commands.push_back(&cmd);
}


//registry
Feature* Registry::findFeatureByName(const std::string& name)
{
	for(auto& c : features) if(c.name == name) return &c;
	return nullptr;
}
Feature* Registry::findFeatureByApi(const std::string& name)
{
	for(auto& c : features) if(c.api == name) return &c;
	return nullptr;
}
Extension* Registry::findExtension(const std::string& name)
{
	for(auto& c : extensions) if(c.name == name) return &c;
	return nullptr;
}
Constant* Registry::findConstant(const std::string& name)
{
	for(auto& c : constants) if(c.name == name) return &c;
	return nullptr;
}
Command* Registry::findCommand(const std::string& name)
{
	for(auto& c : commands) if(c.name == name) return &c;
	return nullptr;
}

Enum* Registry::findEnum(const std::string& name)
{
	for(auto& e : enums) if(e.name == name) return &e;
	return nullptr;
}

BaseType* Registry::findBaseType(const std::string& name)
{
	for(auto& e : baseTypes) if(e.name == name) return &e;
	return nullptr;
}

FuncPtr* Registry::findFuncPtr(const std::string& name)
{
	for(auto& e : funcPtrs) if(e.name == name) return &e;
	return nullptr;
}

Type* Registry::findType(const std::string& name)
{
	Type* ret = nullptr;
	if((ret = findEnum(name))) return ret;
	if((ret = findStruct(name))) return ret;
	if((ret = findExternalType(name))) return ret;
	if((ret = findBitmask(name))) return ret;
	if((ret = findBaseType(name))) return ret;
	if((ret = findHandle(name))) return ret;
	if((ret = findFuncPtr(name))) return ret;
	return nullptr;
}

Struct* Registry::findStruct(const std::string& name)
{
	for(auto& e : structs) if(e.name == name) return &e;
	return nullptr;
}

Bitmask* Registry::findBitmask(const std::string& name)
{
	for(auto& e : bitmasks) if(e.name == name) return &e;
	return nullptr;
}

ExternalType* Registry::findExternalType(const std::string& name)
{
	for(auto& e : externalTypes) if(e.name == name) return &e;
	return nullptr;
}

Handle* Registry::findHandle(const std::string& name)
{
	for(auto& e : handles) if(e.name == name) return &e;
	return nullptr;
}



//types
std::string QualifiedType::string() const
{
	std::string ret = "NO TYPE";
	if(type) ret = type->name;
	if(constant) ret = "const " + ret;
	for(auto i = 0u; i < pointerlvl; ++i) ret += "*";
	return ret;
}

std::string QualifiedType::typestring(const CCOutputGenerator& gen) const
{
	std::string ret = "NO TYPE";
	if(type) ret = type->name;

	if(type->category != Type::Category::external) ret = gen.typeName(ret);

	if(constant) ret = "const " + ret;
	for(auto i = 0u; i < pointerlvl; ++i) ret += "*";
	return ret;
}


//utility
void tolowerip(std::string& str)
{
	static const auto& f = std::use_facet<std::ctype<char>>(std::locale());
	f.tolower(&str.front(), &str.back() + 1);
}

std::string tolower(const std::string& str)
{
	auto ret = str;
	tolowerip(ret);
	return ret;
}


void toupperip(std::string& str)
{
	static const auto& f = std::use_facet<std::ctype<char>>(std::locale());
	f.toupper(&str.front(), &str.back() + 1);
}

std::string toupper(const std::string& str)
{
	auto ret = str;
	toupperip(ret);
	return ret;
}



//generator
std::string OutputGenerator::removeVkPrefix(const std::string& string, bool* found) const
{
	auto ret = string;
	if(found) *found = removeVkPrefix(ret);
	else removeVkPrefix(ret);
	return ret;
}

bool OutputGenerator::removeVkPrefix(std::string& string) const
{
	if(string.size() < 2) return false;

	auto sub = string.substr(0, 2);
	tolowerip(sub);

	if(sub == "vk")
	{
		if(string[2] == '_') string = string.substr(3);
		else string = string.substr(2);
		return true;
	}

	return false;
}

std::string OutputGenerator::strip(const std::string& string) const
{
	auto ret = string;
	stripip(ret);
	return ret;
}

void OutputGenerator::stripip(std::string& string) const
{
	camelCase(string);
	removeVkPrefix(string);
	removeExtSuffix(string);
}

std::string OutputGenerator::removeExtSuffix(const std::string& string, std::string* extension) const
{
	auto ret = string;
	if(extension) *extension = removeExtSuffix(ret);
	else removeExtSuffix(ret);
	return ret;
}

std::string OutputGenerator::removeExtSuffix(std::string& string) const
{
	for(auto& ext : registry().vendors)
	{
		ext = tolower(ext);
		if(string.size() >= ext.size() && tolower(string.substr(string.size() - ext.size())) == ext)
		{
			string = string.substr(0, string.size() - ext.size());
			return ext;
		}
	}

	for(auto& ext : registry().tags)
	{
		ext = tolower(ext);
		if(string.size() >= ext.size() && tolower(string.substr(string.size() - ext.size())) == ext)
		{
			string = string.substr(0, string.size() - ext.size());
			return ext;
		}
	}

	std::string ext = "ext";
	if(string.size() >= ext.size() && tolower(string.substr(string.size() - ext.size())) == ext)
	{
		string = string.substr(0, string.size() - ext.size());
		return ext;
	}

	return "";
}

std::string OutputGenerator::camelCase(const std::string& string, bool firstupper) const
{
	auto ret = string;
	camelCaseip(ret, firstupper);
	return ret;
}

void OutputGenerator::camelCaseip(std::string& string, bool firstupper) const
{
	if(string.empty()) return;

	std::size_t pos = 0u;
	tolowerip(string);
	while((pos = string.find('_')) != std::string::npos)
	{
		string.erase(pos, 1);
		if(pos < string.size()) string[pos] = std::toupper(string[pos], std::locale());
	}

	auto next = false;
	for(auto& c : string)
	{
		if(std::isdigit(c, std::locale()))
		{
			next = true;
		}
		else if(next)
		{
			next = false;
			c = std::toupper(c, std::locale());
		}
	}

	if(firstupper) string[0] = std::toupper(string[0], std::locale());
	else string[0] = std::tolower(string[0], std::locale());
}

void OutputGenerator::upperCaseip(std::string& string) const
{
	std::size_t pos = 0u;
	while(pos < string.size())
	{
		for(auto& c : string.substr(pos))
		{
			if(std::isupper(c, std::locale())) break;
			pos++;
		}

		string.insert(pos, 1, '_');
		pos += 1;
	}

	toupperip(string);
}

std::string OutputGenerator::upperCase(const std::string& string) const
{
	auto ret = string;
	upperCaseip(ret);
	return ret;
}


//generator c++
CCOutputGenerator::CCOutputGenerator(Registry& reg, const std::string& header, const std::string& fwd)
	: OutputGenerator(reg), header_(header), fwd_(fwd)
{
}

void CCOutputGenerator::generate()
{
	//prototype file
	auto fwdHeader =
R"SRC(
//Automaitcally generated forward header file for the vulkan api for the nyorain/vpp library
//Conatains forward declarations for all vulkan types. Do not edit manually.

#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>

namespace vk
{

enum class DummyEnum {};

template <typename BitType, typename MaskType = std::uint32_t>
class Flags
{
public:
	Flags() = default;
	Flags(BitType bit) : m_mask(static_cast<MaskType>(bit)) {}
	Flags(const Flags<BitType>& rhs) : m_mask(rhs.m_mask) {}

	Flags<BitType> & operator=(const Flags<BitType>& rhs)
	{
		m_mask = rhs.m_mask;
		return *this;
	}

	Flags<BitType> & operator|=(const Flags<BitType>& rhs)
	{
		m_mask |= rhs.m_mask;
		return *this;
	}

    Flags<BitType> & operator&=(const Flags<BitType>& rhs)
    {
    	m_mask &= rhs.m_mask;
    	return *this;
    }

    Flags<BitType> & operator^=(const Flags<BitType>& rhs)
    {
    	m_mask ^= rhs.m_mask;
    	return *this;
    }

    Flags<BitType> operator|(const Flags<BitType>& rhs) const
    {
    	Flags<BitType> result(*this);
    	result |= rhs;
    	return result;
    }

    Flags<BitType> operator&(const Flags<BitType>& rhs) const
    {
    	Flags<BitType> result(*this);
    	result &= rhs;
    	return result;
    }

    Flags<BitType> operator^(const Flags<BitType>& rhs) const
    {
    	Flags<BitType> result(*this);
    	result ^= rhs;
    	return result;
    }

    operator bool() const { return (m_mask); }
    bool operator!() const { return !(m_mask); }
    bool operator==(const Flags<BitType>& rhs) const { return m_mask == rhs.m_mask; }
    bool operator!=(Flags<BitType> const& rhs) const { return m_mask != rhs.m_mask; }

    explicit operator MaskType() const { return m_mask; } //explicit?

	private:
    	MaskType  m_mask;
};

template <typename BitType>
Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags)
{
	return flags | bit;
}

template <typename BitType>
Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags)
{
	return flags & bit;
}

template <typename BitType>
Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags)
{
	return flags ^ bit;
}

)SRC";

	fwd_ << copyright;
	fwd_ << "\n//The specification (vk.xml) itself is protected by the following license:\n";
	fwd_ << registry().copyright;
	fwd_ << fwdHeader;

	//header file
	auto headerHeader =
R"SRC(
//Automaitcally generated header file for the vulkan api for the nyorain/vpp library
//Conatains all structure and enum declarations as well as command wrappers.
//Do not edit manually.

#pragma once

#include "fwd.hpp"

namespace vk
{

)SRC";

	header_ << copyright;
	header_ << "\n//The specification (vk.xml) itself is protected by the following license:\n";
	header_ << registry().copyright;
	header_ << headerHeader;


	//default vulkan feature
	auto& feature = *registry().findFeatureByApi("vulkan");
	auto& reqs = feature.reqs;

	printReqs(reqs);

	//extensions
	for(auto& ext : registry().extensions)
	{
		if(!ext.protect.empty())
		{
			fwd_ << "#ifdef " << ext.protect << "\n\n";
			header_ << "#ifdef " << ext.protect << "\n\n";
		}

		printReqs(ext.reqs);

		if(!ext.protect.empty())
		{
			fwd_ << "#endif //" << ext.protect << "\n\n";
			header_ << "#endif //" << ext.protect << "\n\n";
		}
	}

	//end
	fwd_ << "\n}";
	header_ << "\n}";
}

void CCOutputGenerator::printReqs(Requirements& reqs)
{
	//handles
	auto count = 0u;
	for(auto* typeit : reqs.types)
	{
		if(typeit->category != Type::Category::handle) continue;

		auto it = std::find(fulfilled_.types.begin(), fulfilled_.types.end(), typeit);
		if(it != fulfilled_.types.end()) continue;

		auto& type = static_cast<Handle&>(*typeit);
		count++;

		//fwd
		auto name = removeVkPrefix(type.name, nullptr);
		fwd_ << "using " << name << " = " << type.name << ";\n";
	}
	if(count > 0) std::cout << "\tOutputted " << count << " handles\n";
	if(count > 0) fwd_ << "\n";

	//basetypes
	count = 0u;
	for(auto* typeit : reqs.types)
	{
		if(typeit->category != Type::Category::basetype) continue;

		auto it = std::find(fulfilled_.types.begin(), fulfilled_.types.end(), typeit);
		if(it != fulfilled_.types.end()) continue;

		auto& type = static_cast<BaseType&>(*typeit);
		count++;

		if(type.name == "VkFlags") continue;

		//fwd
		auto name = removeVkPrefix(type.name, nullptr);
		fwd_ << "using " << name << " = " << type.original->name << ";\n";
	}

	if(count > 0) std::cout << "\tOutputted " << count << " typedefs\n";
	if(count > 0) fwd_ << "\n";

	//enums
	count = 0u;
	for(auto* typeit : reqs.types)
	{
		if(typeit->category != Type::Category::enumeration) continue;

		auto it = std::find(fulfilled_.types.begin(), fulfilled_.types.end(), typeit);
		if(it != fulfilled_.types.end()) continue;

		auto& type = static_cast<Enum&>(*typeit);
		count++;

		auto name = removeVkPrefix(type.name, nullptr);
		fwd_ << "enum class " << name << " : std::int32_t;\n";

		//header
		header_ << "enum class " << name << " : std::int32_t\n{\n";
		auto sepr = "";
		for(auto& value : type.values)
		{
			bool bit;
			auto n = enumName(type, value.first, &bit);
			header_ << sepr << "\t" << n << " = ";
			if(bit) header_ << "(1 << " << value.second << ")";
			else header_ << value.second;
			sepr = ",\n";
		}

		header_ << "};\n\n";
	}

	if(count > 0) std::cout << "\tOutputted " << count << " enums\n";
	if(count > 0) fwd_ << "\n";
	if(count > 0) header_ << "\n";

	//bitmasks
	count = 0u;
	for(auto* typeit : reqs.types)
	{
		if(typeit->category != Type::Category::bitmask) continue;

		auto it = std::find(fulfilled_.types.begin(), fulfilled_.types.end(), typeit);
		if(it != fulfilled_.types.end()) continue;

		auto& type = static_cast<Bitmask&>(*typeit);
		count++;

		auto name = removeVkPrefix(type.name, nullptr);

		std::string enumName;
		if(!type.bits) enumName = "DummyEnum";
		else enumName = removeVkPrefix(type.bits->name, nullptr);

		fwd_ << "using " << name << " = Flags<" << enumName << ">;\n";
	}

	if(count > 0) std::cout << "\tOutputted " << count << " bitmasks\n";
	if(count > 0) fwd_ << "\n";

	//structs
	count = 0u;
	for(auto* typeit : reqs.types)
	{
		if(typeit->category != Type::Category::structure) continue;

		auto it = std::find(fulfilled_.types.begin(), fulfilled_.types.end(), typeit);
		if(it != fulfilled_.types.end()) continue;

		auto& type = static_cast<Struct&>(*typeit);
		count++;

		auto name = removeVkPrefix(type.name, nullptr);
		auto nameFirstLower = name;
		nameFirstLower[0] = std::tolower(nameFirstLower[0], std::locale());

		std::string metaType = "struct";
		if(type.isUnion) metaType = "union";

		fwd_ << metaType << " " << name << ";\n";
		header_ << metaType << " " << name << "\n{\n";

		bool unionInit = false;
		for(auto& member : type.members)
		{
			std::string init = "";
			if(member.name == "sType") init = "StructureType::" + nameFirstLower;
			header_ << "\t" << member.type.typestring(*this) << " " << member.name;
			for(auto& lvl : member.type.arraylvl) header_ << "[" << lvl << "]";
			if(!type.isUnion || !unionInit) header_ << " {" << init << "}";

			unionInit = true;
			header_ << ";\n";
		}

		header_ << "\n\toperator const " << type.name << "&() const { return reinterpret_cast<const "
				<< type.name << "&>(*this); }\n";

		header_ << "};\n";
	}

	if(count > 0) std::cout << "\tOutputted " << count << " structs\n";
	if(count > 0) fwd_ << "\n";
	if(count > 0) header_ << "\n";

	//funcptrs
	/*
	count = 0u;
	for(auto* typeit : reqs.types)
	{
		if(typeit->category != Type::Category::funcptr) continue;

		auto it = std::find(fulfilled_.types.begin(), fulfilled_.types.end(), typeit);
		if(it != fulfilled_.types.end()) continue;

		auto it = std::find(fulfilled_.types.begin(), fulfilled_.types.end(), typeit);
		if(it != fulfilled_.types.end()) continue;

		auto& type = static_cast<FuncPtr&>(*typeit);
		count++;

		fwd_ << "Funcptr dummy for " << type.name << " \n";
	}

	if(count > 0) std::cout << "\tOutputted " << count << " funcptrs\n";
	if(count > 0) fwd_ << "\n";
	*/

	//commands
	count = 0u;
	for(auto* cmdit : reqs.commands)
	{
		auto it = std::find(fulfilled_.commands.begin(), fulfilled_.commands.end(), cmdit);
		if(it != fulfilled_.commands.end()) continue;

		auto& cmd = *cmdit;
		count++;

		auto name = removeVkPrefix(cmd.name, nullptr);
		name[0] = std::tolower(name[0], std::locale());

		header_ << cmd.signature.returnType.typestring(*this) << " " << name << "(";

		auto sepr = "";
		for(auto& param : cmd.signature.params)
		{
			header_ << sepr << param.type.typestring(*this) << " " << param.name;
			for(auto& lvl : param.type.arraylvl) header_ << "[" << lvl << "]";
			sepr = ", ";
		}

		std::string returnString = "";
		std::string returnStringEnd = "";
		if(cmd.signature.returnType.type->name != "void" || cmd.signature.returnType.pointerlvl > 0)
		{
			returnString = "return static_cast<" + cmd.signature.returnType.typestring(*this) + ">(";
			returnStringEnd = ")";
		}

		header_ << ")\n{\n\t" << returnString << cmd.name << "(";
		sepr = "";
		for(auto& param : cmd.signature.params)
		{
			if(param.type.pointerlvl > 0)
			{
				header_ << sepr << "reinterpret_cast<" << param.type.string() << ">("
						<< param.name << ")";
			}
			else if(param.type.type->category == Type::Category::enumeration ||
				param.type.type->category == Type::Category::bitmask)
			{
				header_ << sepr << "static_cast<" << param.type.string() << ">("
						<< param.name << ")";
			}
			else header_ << sepr << param.name;
			sepr = ", ";
		}

		header_ << ")" << returnStringEnd << ";\n}\n";
	}

	if(count > 0) std::cout << "\tOutputted " << count << " commands\n";
	if(count > 0) header_ << "\n";


	//insert printed reqs
	fulfilled_.commands.insert(fulfilled_.commands.end(), reqs.commands.begin(), reqs.commands.end());
	fulfilled_.types.insert(fulfilled_.types.end(), reqs.types.begin(), reqs.types.end());
	fulfilled_.constants.insert(fulfilled_.constants.end(), reqs.constants.begin(),
		reqs.constants.end());
	fulfilled_.extraConstants.insert(fulfilled_.extraConstants.end(), reqs.extraConstants.begin(),
		reqs.extraConstants.end());
}

std::string CCOutputGenerator::enumName(const Enum& e, const std::string& name, bool* bit) const
{

	constexpr const char* keywords[]= {"alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand",
		"bitor", "bool", "break", "case", "catch", "char", "char16_t", "char32_t", "class", "compl",
		"const", "constexpr", "const_cast", "continue", "decltype", "default", "delete", "do",
		"double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float",
		"for","friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new",
		"noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private",
		"protected public", "register", "reinterpret_cast","return","short","signed","sizeof",
		"static","static_assert","static_cast","struct", "switch", "template", "this","thread_local",
		"throw","true","try","typedef","typeid","typename","union","unsigned", "using","virtual",
		"void", "volatile","wchar_t","while","xor","xor_eq"};

	if(bit) *bit = false;

	auto ret = removeVkPrefix(name, nullptr);
	camelCaseip(ret);

	//prefixes and suffixes
	auto removePrefix = 0u;
	auto removeSuffix = 0u;
	if(!e.bitmask)
	{
		std::string ext;
		removeExtSuffix(e.name, &ext);

		if(e.name != "VkResult") removePrefix = e.name.size() - (2 + ext.size()); //2: vk
		removeSuffix = ext.size();
	}
	else
	{
		std::string ext;
		removeExtSuffix(e.name, &ext);

		removePrefix = e.name.size() - (2 + 8 + ext.size()); //2: vk 8: FlagBits
		removeSuffix = ext.size();

		if(ret.substr(ret.size() - 3) == "Bit")
		{
			if(bit) *bit = true;
			removeSuffix += 3;
		}
	}

	ret = ret.substr(removePrefix);
	ret = ret.substr(0, ret.size() - removeSuffix);
	ret[0] = tolower(ret[0], std::locale());

	//'e' prefix if it is a number
	if(std::isdigit(ret[0], std::locale())) ret.insert(0, 1, 'e');

	//make sure it is no keyword
	for(auto& word : keywords)
	{
		if(std::string(word) == ret)
		{
			ret.insert(0, 1, 'e');
			ret[0] = std::toupper(ret[0], std::locale());
			break;
		}
	}

	//capitalize extension prefixes
	std::string ext;
	removeExtSuffix(ret, &ext);
	if(!ret.empty())
		for(auto i = 0u; i < ext.size(); ++i)
			ret[ret.size() - i] = std::toupper(ret[ret.size() - i], std::locale());

	return ret;
}

std::string CCOutputGenerator::typeName(const std::string& name) const
{
	auto ret = removeVkPrefix(name, nullptr);
	return ret;
}
