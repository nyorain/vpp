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
)SRC";


#include "output.hpp"
#include "registry.hpp"
#include "header.hpp"

#include <iostream>
#include <algorithm>

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
CCOutputGenerator::CCOutputGenerator(Registry& reg, const CCOutputGeneratorSettings& settings)
	: OutputGenerator(reg)
{
	auto& fold = settings.outputFolder;

	main_.open(fold + "/vulkan.hpp");
	functions_.open(fold + "/functions.hpp");
	enums_.open(fold + "/enums.hpp");
	fwd_.open(fold + "/fwd.hpp");
	structs_.open(fold + "/structs.hpp");
}

void CCOutputGenerator::generate()
{
	//copyrights
	outputAll(copyright);
	outputAll("\n// The specification (vk.xml) itself is protected by the following license:\n");
	outputAll(registry().copyright);
	outputAll("\n// Automaitcally generated vulkan header file for the nyorain/vpp library.\n");
	outputAll("// Do not edit manually, rather edit the codegen files.\n");
	outputAll("\n#pragma once\n\n");

	//headers from header.hpp
	fwd_ << fwdHeader;
	main_ << mainHeader;
	functions_ << functionsHeader;
	structs_ << structsHeader;
	enums_ << enumsHeader;

	outputAllHeader("namespace vk\n{\n\n");

	//fwd dummy enum
	fwd_ << "enum class DummyEnum : std::int32_t {};\n\n";

	//all printed requirements
	Requirements fulfilled;

	//default vulkan feature
	auto& feature = *registry().findFeatureByApi("vulkan");
	auto& reqs = feature.reqs;

	printReqs(reqs, fulfilled);
	fulfilled.add(reqs);

	//extensions
	for(auto& ext : registry().extensions)
	{
		printReqs(ext.reqs, fulfilled, ext.protect);
		fulfilled.add(ext.reqs);
	}

	//end
	outputAllHeader("\n\n} //namespace vk");
}

void CCOutputGenerator::outputAll(const std::string& string)
{
	main_ << string;
	fwd_ << string;
	structs_ << string;
	enums_ << string;
	functions_ << string;
}

void CCOutputGenerator::outputAllHeader(const std::string& string)
{
	fwd_ << string;
	structs_ << string;
	enums_ << string;
	functions_ << string;
}

void CCOutputGenerator::printReqs(Requirements& reqs, const Requirements& fulfilled,
	const std::string& guard)
{
	auto fwdGuard = false;
	auto enumGuard = false;
	auto funcGuard = false;
	auto structGuard = false;

	//constants
	auto count = 0u;
	for(auto& constant : reqs.constants)
	{
		auto it = std::find(fulfilled.constants.cbegin(), fulfilled.constants.cend(), constant);
		if(it != fulfilled.constants.cend()) continue;

		assureGuard(fwd_, fwdGuard, guard);
		auto name = constantName(*constant);

		if(name == "true" || name == "false") continue;
		count++;

		fwd_ << "constexpr auto " << name << " = " << constant->value << ";\n";
	}

	for(auto& constant : reqs.extraConstants)
	{
		assureGuard(fwd_, fwdGuard, guard);
		auto name = constantName(constant);
		count++;

		fwd_ << "constexpr auto " << name << " = " << constant.value << ";\n";
	}

	if(count > 0) std::cout << "\tOutputted " << count << " constants\n";
	if(count > 0) fwd_ << "\n";

	//handles
	count = 0u;
	for(auto* typeit : reqs.types)
	{
		if(typeit->category != Type::Category::handle) continue;

		auto it = std::find(fulfilled.types.cbegin(), fulfilled.types.cend(), typeit);
		if(it != fulfilled.types.cend()) continue;

		auto& type = static_cast<Handle&>(*typeit);
		count++;

		assureGuard(fwd_, fwdGuard, guard);

		//fwd
		auto name = typeName(type);
		fwd_ << "using " << name << " = " << type.name << ";\n";
	}
	if(count > 0) std::cout << "\tOutputted " << count << " handles\n";
	if(count > 0) fwd_ << "\n";

	//basetypes
	count = 0u;
	for(auto* typeit : reqs.types)
	{
		if(typeit->category != Type::Category::basetype) continue;

		auto it = std::find(fulfilled.types.begin(), fulfilled.types.end(), typeit);
		if(it != fulfilled.types.end()) continue;

		auto& type = static_cast<BaseType&>(*typeit);
		count++;

		if(type.name == "VkFlags") continue;

		assureGuard(fwd_, fwdGuard, guard);

		//fwd
		auto name = typeName(type);
		fwd_ << "using " << name << " = " << type.original->name << ";\n";
	}

	if(count > 0) std::cout << "\tOutputted " << count << " typedefs\n";
	if(count > 0) fwd_ << "\n";

	//enums
	count = 0u;
	for(auto* typeit : reqs.types)
	{
		if(typeit->category != Type::Category::enumeration) continue;

		auto it = std::find(fulfilled.types.begin(), fulfilled.types.end(), typeit);
		if(it != fulfilled.types.end()) continue;

		auto& type = static_cast<Enum&>(*typeit);
		count++;

		auto name = typeName(type);

		assureGuard(fwd_, fwdGuard, guard);
		assureGuard(enums_, enumGuard, guard);

		fwd_ << "enum class " << name << " : std::int32_t;\n";

		//header
		enums_ << "enum class " << name << " : std::int32_t\n{\n";
		auto sepr = "";
		for(auto& value : type.values)
		{
			bool bit;
			auto n = enumName(type, value.first, &bit);
			enums_ << sepr << "\t" << n << " = ";
			if(bit) enums_ << "(1 << " << value.second << ")";
			else enums_ << value.second;
			sepr = ",\n";
		}

		enums_ << "\n};\n";

		if(type.bitmask) enums_<< "VPP_BITMASK_OPS(" << name << ")\n";
		enums_ << "\n";
	}

	if(count > 0) std::cout << "\tOutputted " << count << " enums\n";
	if(count > 0) fwd_ << "\n";
	if(count > 0) enums_ << "\n";

	//bitmasks
	count = 0u;
	for(auto* typeit : reqs.types)
	{
		if(typeit->category != Type::Category::bitmask) continue;

		auto it = std::find(fulfilled.types.begin(), fulfilled.types.end(), typeit);
		if(it != fulfilled.types.end()) continue;

		auto& type = static_cast<Bitmask&>(*typeit);
		count++;

		auto name = typeName(type);
		assureGuard(fwd_, fwdGuard, guard);

		std::string enumName;
		if(!type.bits) enumName = "DummyEnum";
		else enumName = typeName(*type.bits);

		fwd_ << "using " << name << " = Flags<" << enumName << ">;\n";
	}

	if(count > 0) std::cout << "\tOutputted " << count << " bitmasks\n";
	if(count > 0) fwd_ << "\n";

	//structs
	count = 0u;
	for(auto* typeit : reqs.types)
	{
		if(typeit->category != Type::Category::structure) continue;

		auto it = std::find(fulfilled.types.begin(), fulfilled.types.end(), typeit);
		if(it != fulfilled.types.end()) continue;

		auto& type = static_cast<Struct&>(*typeit);
		count++;

		assureGuard(fwd_, fwdGuard, guard);
		assureGuard(structs_, structGuard, guard);

		printStruct(type);

	}

	if(count > 0) std::cout << "\tOutputted " << count << " structs\n";
	if(count > 0) fwd_ << "\n";
	if(count > 0) structs_ << "\n";

	//funcptrs
	count = 0u;
	for(auto* typeit : reqs.types)
	{
		if(typeit->category != Type::Category::funcptr) continue;

		auto it = std::find(fulfilled.types.begin(), fulfilled.types.end(), typeit);
		if(it != fulfilled.types.end()) continue;

		auto& type = static_cast<FuncPtr&>(*typeit);
		count++;

		assureGuard(fwd_, fwdGuard, guard);

		fwd_ << "using " << typeName(type) << " = " << typeName(type.signature.returnType);
		fwd_ << "(*)(";

		auto sepr = "";
		for(auto& param : type.signature.params)
		{
			fwd_ << sepr << typeName(param.type) << " " << param.name;
			for(auto& lvl : param.type.arraylvl) fwd_ << "[" << lvl << "]";
			sepr = ", ";
		}

		fwd_ << ");\n";
	}

	if(count > 0) std::cout << "\tOutputted " << count << " funcptrs\n";
	if(count > 0) fwd_ << "\n";

	//commands
	count = 0u;
	for(auto* cmdit : reqs.commands)
	{
		auto it = std::find(fulfilled.commands.begin(), fulfilled.commands.end(), cmdit);
		if(it != fulfilled.commands.end()) continue;

		auto& cmd = *cmdit;
		count++;

		auto name = removeVkPrefix(cmd.name, nullptr);
		name[0] = std::tolower(name[0], std::locale());

		assureGuard(functions_, funcGuard, guard);

		functions_ << typeName(cmd.signature.returnType) << " " << name << "(";

		auto sepr = "";
		for(auto& param : cmd.signature.params)
		{
			functions_ << sepr << paramName(param);
			sepr = ", ";
		}

		std::string returnString = "";
		std::string returnStringEnd = "";
		auto& retType = cmd.signature.returnType;

		if(retType.type->name == "VkResult")
		{
			returnString = "return VPP_CALL(static_cast<Result>(";
			returnStringEnd = "))";
		}
		else if(retType.type->name != "void" || retType.pointerlvl > 0)
		{
			returnString = "return static_cast<" + typeName(cmd.signature.returnType) + ">(";
			returnStringEnd = ")";
		}

		functions_ << ")\n{\n\t" << returnString << cmd.name << "(";
		sepr = "";
		for(auto& param : cmd.signature.params)
		{
			if(param.type.pointerlvl > 0)
			{
				functions_ << sepr << "reinterpret_cast<" << param.type.string() << ">("
						<< param.name << ")";
			}
			else if(param.type.type->category == Type::Category::enumeration ||
				param.type.type->category == Type::Category::bitmask)
			{
				functions_ << sepr << "static_cast<" << param.type.string() << ">("
						<< param.name << ")";
			}
			else functions_ << sepr << param.name;
			sepr = ", ";
		}

		functions_ << ")" << returnStringEnd << ";\n}\n";
	}

	if(count > 0) std::cout << "\tOutputted " << count << " commands\n";
	if(count > 0) functions_ << "\n";

	endGuard(functions_, funcGuard, guard);
	endGuard(structs_, structGuard, guard);
	endGuard(fwd_, fwdGuard, guard);
	endGuard(enums_, enumGuard, guard);
}

void CCOutputGenerator::assureGuard(std::ofstream& of, bool& guardVar, const std::string& guard)
{
	if(guardVar) return;

	if(!guard.empty())of << "#ifdef " << guard << "\n\n";
	guardVar = true;
}

void CCOutputGenerator::endGuard(std::ofstream& of, bool guardVar, const std::string& guard)
{
	if(!guardVar) return;

	if(!guard.empty())of << "#endif //" << guard << "\n\n";
}

std::string CCOutputGenerator::enumName(const Enum& e, const std::string& name, bool* bit) const
{
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

		//basically find the prefix enum name and enum value name share and remove it
		//from the enum value name (means incerase removePrefix)
		auto fename = tolower(typeName(e)); //fixed enum name
		if(fename.substr(fename.size() - 4) == "bits") fename = fename.substr(0, fename.size() - 4);

		auto fname = tolower(ret); //fixed name

		auto s = 0u;
		while(fename.size() > s && fname.size() > s && fename[s] == fname[s]) ++s;

		removePrefix = s;
		removeSuffix = ext.size();
	}

	ret = ret.substr(removePrefix);
	ret = ret.substr(0, ret.size() - removeSuffix);
	ret[0] = tolower(ret[0], std::locale());

	//remove "Bit" from bitmask enums
	if(e.bitmask && ret.substr(ret.size() - 3) == "Bit")
	{
		if(bit) *bit = true;
		ret = ret.substr(0, ret.size() - 3);
	}

	//'e' prefix if it is a number
	if(std::isdigit(ret[0], std::locale())) ret.insert(0, 1, 'e');

	//make sure it is no keyword
	//keywords defined in header.hpp
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

std::string CCOutputGenerator::typeName(const Type& type) const
{
	auto ret = removeVkPrefix(type.name, nullptr);
	if(type.category == Type::Category::enumeration)
	{
		auto& e = static_cast<const Enum&>(type);
		std::size_t pos;
		if(e.bitmask && (pos = ret.find("FlagBits")) != std::string::npos)
			ret.erase(pos, 4); //erase "Flag" from the name
	}
	else if(type.category == Type::Category::funcptr)
	{
		ret.erase(0, 6); //PFN_vk
		ret.insert(0, "Pfn");
	}

	return ret;
}

std::string CCOutputGenerator::typeName(const QualifiedType& type) const
{
	std::string ret = type.type->name;
	if(type.type->category != Type::Category::external) ret = typeName(*type.type);

	if(type.constant) ret = "const " + ret;
	for(auto i = 0u; i < type.pointerlvl; ++i) ret += "*";
	return ret;
}

std::string CCOutputGenerator::constantName(const Constant& c) const
{
	auto ret = removeVkPrefix(c.name, nullptr);
	camelCaseip(ret);
	return ret;
}

void CCOutputGenerator::printStruct(const Struct& type)
{
	auto name = typeName(type);
	auto nameFirstLower = name;
	nameFirstLower[0] = std::tolower(nameFirstLower[0], std::locale());

	std::string metaType = "struct";
	if(type.isUnion) metaType = "union";

	fwd_ << metaType << " " << name << ";\n";
	structs_ << metaType << " " << name << "\n{\n";

	bool unionInit = false;
	auto sepr = "";
	std::string paramList;
	std::string initList;
	for(auto& member : type.members)
	{
		auto mtype = typeName(member.type);
		auto mname = member.name;

		//member declaration
		std::string init = "";
		if(member.name == "sType") init = "StructureType::" + nameFirstLower;
		structs_ << "\t" << paramName(member);
		if(!type.isUnion || !unionInit) structs_ << " {" << init << "}";

		unionInit = true;
		structs_ << ";\n";

		if(member.name == "sType" || member.name == "pNext") continue;

		paramList += sepr + mtype + " x" + mname + " = {}"; //ctor params
		initList += sepr + mname + "(x" + mname + ")"; //initializer

		sepr = ", ";
	}

	//init ctor
	if(!type.returnedonly)
	{
		structs_ << "\n\t" << name << "(" << paramList << ")";
		if(!initList.empty()) structs_ << " : " << initList;
		structs_ << " {}\n";
	}

	//explicit conversion function
	structs_ << "\n\tconst " << type.name << "& vkHandle() const { return reinterpret_cast<const "
			<< type.name << "&>(*this); }\n";

	structs_ << "\t" << type.name << "& vkHandle() { return reinterpret_cast<"
			<< type.name << "&>(*this); }\n";

	//conversion operator
	structs_ << "\n\toperator const " << type.name << "&() const { return vkHandle(); };\n";
	structs_ << "\toperator " << type.name << "&() { return vkHandle(); };\n";
	structs_ << "};\n";
}

std::string CCOutputGenerator::paramName(const Param& param) const
{
	std::string ret = typeName(param.type);
	ret += " " + param.name;

	for(auto& lvl : param.type.arraylvl)
	{
		std::string lvlName = lvl;

		auto pos = lvl.find("VK");
		if(pos != std::string::npos)
		{
			removeVkPrefix(lvlName);
			camelCaseip(lvlName);
		}

		ret += "[" + lvlName + "]";
	}

	return ret;
}
