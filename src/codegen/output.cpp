// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include "output.hpp"
#include "registry.hpp"
#include "header.hpp"

#include <iostream>
#include <algorithm>
#include <cstring>
#include <regex>

// utility
// the outputted copyright for the parsed header
constexpr const auto copyright = 1 + R"SRC(
// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
)SRC";

constexpr auto logEnabled = false;

/// log output function
template<typename... Args>
void log(Args... args)
{
	using Expand = const int[];
	if(logEnabled) {
		(void) Expand{(std::cerr << args, 0)...};
		std::cerr << "\n";
	}
}

/// string to lower in place
void tolowerip(std::string& str)
{
	static const auto& f = std::use_facet<std::ctype<char>>(std::locale());
	f.tolower(&str.front(), &str.back() + 1);
}

/// string to lower in copy
std::string tolower(const std::string& str)
{
	auto ret = str;
	tolowerip(ret);
	return ret;
}

/// string to upper in place
void toupperip(std::string& str)
{
	static const auto& f = std::use_facet<std::ctype<char>>(std::locale());
	f.toupper(&str.front(), &str.back() + 1);
}

/// string to upper in copy
std::string toupper(const std::string& str)
{
	auto ret = str;
	toupperip(ret);
	return ret;
}

// generator
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

	if(sub == "vk") {
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
	for(auto& ext : registry().vendors) {
		if(string.size() < ext.size()) continue;
		ext = tolower(ext);
		if(tolower(string.substr(string.size() - ext.size())) == ext) {
			string = string.substr(0, string.size() - ext.size());
			return ext;
		}
	}

	for(auto& ext : registry().tags) {
		if(string.size() < ext.size()) continue;
		ext = tolower(ext);
		if(tolower(string.substr(string.size() - ext.size())) == ext) {
			string = string.substr(0, string.size() - ext.size());
			return ext;
		}
	}

	std::string ext = "ext";
	if(string.size() < ext.size()) return "";
	if(tolower(string.substr(string.size() - ext.size())) == ext){
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
	while((pos = string.find('_')) != std::string::npos) {
		string.erase(pos, 1);
		if(pos < string.size()) string[pos] = std::toupper(string[pos], std::locale());
	}

	if(firstupper) string[0] = std::toupper(string[0], std::locale());
	else string[0] = std::tolower(string[0], std::locale());
}

void OutputGenerator::macroCaseip(std::string& string) const
{
	std::size_t pos = 0u;
	while(pos < string.size()) {
		for(auto& c : string.substr(pos)) {
			if(std::isupper(c, std::locale())) break;
			pos++;
		}

		string.insert(pos, 1, '_');
		pos += 1;
	}

	toupperip(string);
}

std::string OutputGenerator::macroCase(const std::string& string) const
{
	auto ret = string;
	macroCaseip(ret);
	return ret;
}

CCOutputGenerator::CCOutputGenerator(Registry& reg, const CCOutputGeneratorSettings& settings)
	: OutputGenerator(reg)
{
	auto& fold = settings.outputFolder;

	main_.open(fold + "/vulkan.hpp", std::ios_base::out | std::ios_base::binary);
	functions_.open(fold + "/functions.hpp", std::ios_base::out | std::ios_base::binary);
	enums_.open(fold + "/enums.hpp", std::ios_base::out | std::ios_base::binary);
	fwd_.open(fold + "/fwd.hpp", std::ios_base::out | std::ios_base::binary);
	structs_.open(fold + "/structs.hpp", std::ios_base::out | std::ios_base::binary);
}

void CCOutputGenerator::generate()
{
	// copyrights
	outputAll(copyright);
	outputAll("\n// Automaitcally generated vulkan header file for the nyorain/vpp library.\n");
	outputAll("// Do not edit manually, rather edit the codegen files.\n");
	outputAll("\n#pragma once\n\n");

	auto version = registry().version;

	// headers from header.hpp
	fwd_ << fwdHeader;
	main_ << mainHeader;
	functions_ << std::regex_replace(functionsHeader, std::regex("%v"), version);
	structs_ << std::regex_replace(structsHeader, std::regex("%v"), version);
	enums_ << enumsHeader;

	outputAllHeader("namespace vk {\n\n");

	// fwd dummy enum
	fwd_ << "enum class DummyEnum : int32_t {};\n";

	// XXX: use this?
	// fwd_ << "using nytl::Span; // span.hpp\n";
	// fwd_ << "using nytl::Flags; // flags.hpp\n\n";

	// all printed requirements
	Requirements fulfilled;

	// output the default vulkan feature
	auto& feature = *registry().findFeatureByApi("vulkan");
	auto& reqs = feature.reqs;

	printReqs(reqs, fulfilled);
	fulfilled.add(reqs);

	// output extensions
	for(auto& ext : feature.extensions) {
		printReqs(ext->reqs, fulfilled, ext->protect);
		fulfilled.add(ext->reqs);
	}

	// undef function macros
	functions_
		<< "#undef VEC_FUNC\n"
		<< "#undef VEC_FUNC_VOID\n"
		<< "#undef VEC_FUNC_RET\n"
		<< "#undef VEC_FUNC_RET_VOID\n";

	// end the header files
	outputAllHeader("\n} // namespace vk\n\n");

	outputAll("// The specification (vk.xml) itself is published under the following license:\n");
	outputAll(registry().copyright);

	fwd_ << "\n\n";
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

	// order:
	// - constants
	// - handles
	// - basetypes
	// - enums
	// - bitmasks
	// - structs

	// constants
	auto count = 0u;
	for(auto& constant : reqs.constants) {

		// only handle not yet fulfilled constants
		auto it = std::find(fulfilled.constants.cbegin(), fulfilled.constants.cend(), constant);
		if(it != fulfilled.constants.cend()) continue;

		assureGuard(fwd_, fwdGuard, guard);
		auto name = constantName(*constant);

		// NOTE: workaround for predefined constants (true/false)
		if(name == "true" || name == "false") continue;
		++count;

		fwd_ << "constexpr auto " << name << " = " << constant->value << ";\n";
	}

	if(count > 0) {
		log("\tOutputted ", count, " constants\n");
		fwd_ << "\n";
	}

	// handles
	count = 0u;
	auto types = reqs.types;
	for(auto i = 0; i < static_cast<int>(types.size()); ++i) {
		auto& type = *types[i];

		// remove duplicate types
		// only done in this loop, since this is the first type enitity loop
		auto it = std::find(fulfilled.types.cbegin(), fulfilled.types.cend(), &type);
		if(it != fulfilled.types.cend()) {
			types.erase(types.begin() + i);
			--i;
			continue;
		}

		if(type.category != Type::Category::handle)
			continue;

		auto& handle = static_cast<const Handle&>(type);
		++count;

		assureGuard(fwd_, fwdGuard, guard);

		// output handle macro to fwd
		auto name = typeName(type);
		fwd_ << handle.type << "(" << name << ")" << "\n";
	}

	if(count > 0) {
		log("\tOutputted ", count, " handles");
		fwd_ << "\n";
	}

	// basetypes
	count = 0u;
	for(auto* typeit : types) {
		if(typeit->category != Type::Category::basetype) continue;
		auto& basetype = static_cast<BaseType&>(*typeit);
		if(basetype.name == "VkFlags") continue;

		++count;
		assureGuard(fwd_, fwdGuard, guard);

		// fwd
		auto name = typeName(basetype);
		fwd_ << "using " << name << " = " << basetype.original->name << ";\n";
	}

	if(count > 0) {
		log("\tOutputted ", count, " typedefs");
		fwd_ << "\n";
	}

	// enums
	count = 0u;
	for(auto* typeit : types) {
		if(typeit->category != Type::Category::enumeration) continue;
		auto& enumeration = static_cast<Enum&>(*typeit);
		auto name = typeName(enumeration);

		++count;
		assureGuard(fwd_, fwdGuard, guard);
		assureGuard(enums_, enumGuard, guard);

		fwd_ << "enum class " << name << " : int32_t;\n";

		// output enum values
		enums_ << "enum class " << name << " : int32_t{\n";
		auto sepr = "";
		for(auto& value : enumeration.values) {
			bool bit;
			auto n = enumName(enumeration, value.first, &bit);
			enums_ << sepr << "\t" << n << " = ";
			if(bit) enums_ << "(1 << " << value.second << ")";
			else enums_ << value.second;
			sepr = ",\n";
		}

		enums_ << "\n};\n";

		if(enumeration.bitmask) enums_<< "NYTL_FLAG_OPS(" << name << ")\n";
		enums_ << "\n";
	}

	if(count > 0) {
		log("\tOutputted ", count, " enums");
		fwd_ << "\n";
		enums_ << "\n";
	}

	// bitmasks
	count = 0u;
	for(auto* typeit : types) {
		if(typeit->category != Type::Category::bitmask) continue;
		auto& bitmask = static_cast<Bitmask&>(*typeit);
		++count;

		auto name = typeName(bitmask);
		assureGuard(fwd_, fwdGuard, guard);

		std::string enumName;
		if(!bitmask.bits) enumName = "DummyEnum";
		else enumName = typeName(*bitmask.bits);

		fwd_ << "using " << name << " = nytl::Flags<" << enumName << ">;\n";
	}

	if(count > 0) {
		log("\tOutputted ", count, " bitmasks");
		fwd_ << "\n";
	}

	// structs
	count = 0u;
	for(auto* typeit : types) {
		if(typeit->category != Type::Category::structure) continue;
		auto& structure = static_cast<Struct&>(*typeit);
		++count;

		assureGuard(fwd_, fwdGuard, guard);
		assureGuard(structs_, structGuard, guard);
		printStruct(structure);
	}

	if(count > 0) {
		log("\tOutputted ", count, " structs");
		fwd_ << "\n";
		structs_ << "\n";
	}

	// funcptrs
	count = 0u;
	for(auto* typeit : types) {
		if(typeit->category != Type::Category::funcptr) continue;
		auto& funcptr = static_cast<FuncPtr&>(*typeit);
		++count;

		assureGuard(fwd_, fwdGuard, guard);

		fwd_ << "using " << typeName(funcptr) << " = " << typeName(funcptr.signature.returnType);
		fwd_ << "(*VKAPI_PTR)(";

		auto sepr = "";
		for(auto& param : funcptr.signature.params) {
			fwd_ << sepr << paramName(param, "", true);
			sepr = ", ";
		}

		fwd_ << ");\n";
	}

	if(count > 0) {
		log("\tOutputted ", count, " funcptrs");
		fwd_ << "\n";
	}

	// extra funcPointers
	// extension specific, used to load the commands
	count = 0;
	for(auto* typeit : reqs.funcPtr) {
		auto it = std::find(fulfilled.funcPtr.begin(), fulfilled.funcPtr.end(), typeit);
		if(it != fulfilled.funcPtr.end()) continue;

		auto& type = *typeit;
		count++;

		auto name = removeVkPrefix(type.name, nullptr);
		assureGuard(fwd_, fwdGuard, guard);

		fwd_ << "using Pfn" << name << " = " << typeName(type.signature.returnType);
		fwd_ << "(*VKAPI_PTR)(";

		auto sepr = "";
		for(auto& param : type.signature.params) {
			fwd_ << sepr << paramName(param, "", true);
			sepr = ", ";
		}

		fwd_ << ");\n";
	}

	if(count > 0) {
		log("\tOutputted ", count, " extra api funcptrs");
		fwd_ << "\n";
	}

	// commands
	count = 0u;
	for(auto* cmdit : reqs.commands) {
		auto it = std::find(fulfilled.commands.begin(), fulfilled.commands.end(), cmdit);
		if(it != fulfilled.commands.end()) continue;

		++count;
		assureGuard(functions_, funcGuard, guard);
		printCmd(*cmdit);
		// functions_ << "\n"; // insert blank line between seperate function (groups)
	}

	if(count > 0) {
		log("\tOutputted ", count, " commands");
		functions_ << "\n";
	}

	// end all open requirements guards
	endGuard(functions_, funcGuard, guard);
	endGuard(structs_, structGuard, guard);
	endGuard(fwd_, fwdGuard, guard);
	endGuard(enums_, enumGuard, guard);
}

void CCOutputGenerator::assureGuard(std::ofstream& of, bool& guardVar, const std::string& guard)
{
	if(!guardVar) {
		if(!guard.empty())of << "#ifdef " << guard << "\n\n";
		guardVar = true;
	}
}

void CCOutputGenerator::endGuard(std::ofstream& of, bool guardVar, const std::string& guard)
{
	if(guardVar && !guard.empty())of << "#endif //" << guard << "\n\n";
}

std::string CCOutputGenerator::enumName(const Enum& e, const std::string& name, bool* bit) const
{
	if(bit) *bit = false;

	auto ret = removeVkPrefix(name, nullptr);
	camelCaseip(ret);

	// prefixes and suffixes
	auto removePrefix = 0u;
	auto removeSuffix = 0u;

	{
		std::string ext;
		removeExtSuffix(e.name, &ext);

		auto fename = tolower(typeName(e)); // fixed enum name
		auto bpos = fename.rfind("bit");
		if(bpos != std::string::npos) fename = fename.substr(0, bpos);
		else fename = fename.substr(0, fename.size() - ext.size());

		// if(fename.substr(fename.size() - 4) == "bits") fename = fename.substr(0, fename.size() - 4);
		auto fname = tolower(ret); // fixed name

		// TODO: better check for common prefix: don't compare char for char but
		// rather word for word (words are seperated by an underscore) (?)

		auto s = 0u;
		while(fename.size() > s && fname.size() > s && fename[s] == fname[s]) ++s;

		removePrefix = s;
		removeSuffix = ext.size();


		// actuallly remove prefix and suffix, lower first char
		ret = ret.substr(removePrefix);
		ret = ret.substr(0, ret.size() - removeSuffix);
		ret[0] = tolower(ret[0], std::locale());

		// remove "Bit" from bitmask enums
		bpos = ret.rfind("Bit");
		if(e.bitmask && bpos != std::string::npos) {
			if(bit) *bit = true;
			ret = ret.erase(bpos, 3);
		}
	}

	// 'e' prefix if it is a number
	if(std::isdigit(ret[0], std::locale())) ret.insert(0, 1, 'e');

	// NOTE: hack/workaround for 'WordIDWord' struct and WORD_ID_WORD enum.
	// in this case the enum value name is wrongly generated (WordIdWord) and does
	// not match the (correct) structure name since this information is not
	// preserved. Rather a temporary workaround here.
	auto idPos = ret.find("Id");
	if(idPos != std::string::npos) {
		ret[idPos + 1] = 'D';
	}

	// make sure it is no keyword
	// keywords defined in header.hpp
	for(auto& word : keywords) {
		if(std::string(word) == ret) {
			ret.insert(0, 1, 'e');
			ret[1] = std::toupper(ret[1], std::locale());
			break;
		}
	}

	// capitalize extension prefixes
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
	if(type.category == Type::Category::enumeration) {
		auto& e = static_cast<const Enum&>(type);
		std::size_t pos;
		if(e.bitmask && (pos = ret.find("FlagBits")) != std::string::npos)
			ret.erase(pos, 4); // erase "Flag" from the name
	} else if(type.category == Type::Category::funcptr) {
		ret.erase(0, 6); // erase PFN_vk from beginning
		ret.insert(0, "Pfn");
	}

	return ret;
}

std::string CCOutputGenerator::typeName(const QualifiedType& type, bool change) const
{
	std::string ret;
	if(type.type) {
		ret = type.type->name;
		if(change && type.type->category != Type::Category::external) ret = typeName(*type.type);
		else ret = type.type->name;

		if(type.constant) ret = "const " + ret;
		if(type.reference) ret += "&";
	} else if(type.pointer) {
		ret = typeName(*type.pointer, change);
		ret += "*";
		if(type.constant) ret += " const";
	} else if(type.array) {
		ret = typeName(*type.array, change);
	} else {
		std::cerr << "### CCOG::typeName: invalid QualifiedType object\n";
		return "--- unknown ---";
	}

	return ret;
}

std::string CCOutputGenerator::constantName(const Constant& c) const
{
	auto ret = removeVkPrefix(c.name, nullptr);
	camelCaseip(ret);
	return ret;
}

std::string CCOutputGenerator::paramName(const Param& param, const std::string& namePrefix,
	bool plainArray) const
{
	std::string ret;

	auto it = &param.type;
	while(!plainArray && it->array) {
		ret += "std::array<";
		it = it->array;
	}

	ret += typeName(param.type);

	std::stack<std::string> ranges;
	it = &param.type;

	while(it->array) {
		std::string sizeName = it->arraySize;
		auto pos = sizeName.find("VK");
		if(pos != std::string::npos) {
			removeVkPrefix(sizeName);
			camelCaseip(sizeName);
		}

		ranges.push(sizeName);
		it = it->array;
	}

	while(!ranges.empty()) {
		if(plainArray) ret += "[" + ranges.top() + "]";
		else ret += ", " + ranges.top() + ">";
		ranges.pop();
	}

	ret += " " + namePrefix + param.name;

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
	structs_ << metaType << " " << name << " {\n";

	bool unionInit = false;
	auto sepr = "";
	std::string paramList;
	std::string initList;
	bool printCtor = false;

	// print all members
	for(auto& member : type.members) {
		auto mtype = typeName(member.type);
		auto mname = member.name;

		// member declaration
		std::string init = "";
		if(member.name == "sType") init = "StructureType::" + nameFirstLower;
		structs_ << "\t" << paramName(member);
		if(!type.isUnion || !unionInit)
			structs_ << " {" << init << "}";

		unionInit = true;
		structs_ << ";\n";

		if(member.name == "sType" || member.name == "pNext") {
			printCtor = true;
			continue;
		}

		paramList += sepr + paramName(member, "x") + " = {}"; //ctor params
		initList += sepr + mname + "(x" + mname + ")"; //initializer

		sepr = ", ";
	}

	// init ctor
	// there will only be one init constructor if the type isnt union, and there are members like
	// pNext or structureType, other wise uniform init can be used to init the members
	// if the struct is returnedonly there is no need for a constructor
	if(printCtor && !type.returnedonly && !type.isUnion) {
		structs_ << "\n\t" << name << "(" << paramList << ")";
		if(!initList.empty()) structs_ << " : " << initList;
		structs_ << " {}\n";
	}

	// TODO: union ctors that initialize the different members?
	// explicit conversion function (useful if passed as pointer)
	structs_ << "\n\tconst " << type.name << "& vkHandle() const { return reinterpret_cast<const "
			<< type.name << "&>(*this); }\n";

	structs_ << "\t" << type.name << "& vkHandle() { return reinterpret_cast<"
			<< type.name << "&>(*this); }\n";

	// conversion operator
	structs_ << "\n\toperator const " << type.name << "&() const { return vkHandle(); };\n";
	structs_ << "\toperator " << type.name << "&() { return vkHandle(); };\n";
	structs_ << "};\n\n";
}

ParsedCommand CCOutputGenerator::parseCommand(const Command& cmd) const
{
	ParsedCommand parsed {};
	parsed.command = &cmd;

	auto& params = cmd.signature.params;
	auto& pparams = parsed.parsedParams;
	pparams.reserve(params.size());

	// iterate through params to find array/count matches
	for(auto& param : params) {
		pparams.emplace_back();
		auto& pparam = pparams.back();
		pparam.param = &param;

		// check if output param
		if(param.type.pointer) {
			auto it = &param.type;
			pparam.out = true;
			while(it->pointer) {
				if(it->constant) {
					pparam.out = false;
					break;
				}

				it = it->pointer;
			}

			if(it->constant) pparam.out = false;
		}

		// array attrib, find the matching count param
		auto attr = param.node.attribute("len");
		std::string len = attr.value();
		if(len.empty())
			continue;

		std::string paramName = len;
		std::string memName;

#ifdef VPP_OLD_LENGTH_PARAM
		auto memAcc = len.find("->");
#else
		auto memAcc = len.find("::");
#endif
		if(memAcc != std::string::npos) {
			paramName = len.substr(0, memAcc);
			memName = len.substr(memAcc + 2);
		}

		// find the matching data pars
		for(auto& par : pparams) {
			if(par.param->name != paramName) continue;

			par.dataPars.push_back(&pparam);
			pparam.countPar = &par;

			if(!memName.empty()) {
				auto it2 = &par.param->type;
				while(!it2->type) {
					if(it2->array) it2 = it2->array;
					if(it2->pointer) it2 = it2->pointer;
				}

				auto& structType = static_cast<const Struct&>(*it2->type);
				for(auto& member : structType.members) {
					if(member.name == memName) {
						pparam.countMember = &member;
						par.memberAsCount = true;
						break;
					}
				}
			}

			break;
		}
	}

	// reverse iteration for checking which parameters can be optional
	// first check return pair then optoinal
	for(auto it = pparams.rbegin(); it < pparams.rend(); ++it) {

		// find param (pair) that should be returned
		if(it->dataPars.empty() && it->out) {
			auto it2 = &it->param->type;
			while(!it2->type) {
				if(it2->array) it2 = it2->array;
				if(it2->pointer) it2 = it2->pointer;
			}

			if(it2->type->category == Type::Category::structure && !it->countPar) {
				auto& structType = static_cast<const Struct&>(*it2->type);
				if(!structType.returnedonly) continue;
			}

			parsed.returnParam = &(*it);
			break;
		}
	}

	auto optional = true;
	auto optionalWithRet = true;
	for(auto it = pparams.rbegin(); it < pparams.rend(); ++it) {

		// optional
		if(&(*it) != parsed.returnParam && !it->param->optional) optional = false;
		else if(optional) it->optional = true;

		if(!it->param->optional) optionalWithRet = false;
		else if(optionalWithRet) it->optionalWithRet = true;
	}

	return parsed;
}


void CCOutputGenerator::printCmd(const Command& cmd)
{
	auto name = removeVkPrefix(cmd.name, nullptr);
	name[0] = std::tolower(name[0], std::locale());

	// parseCommand will analyse intput/output/optional/return attributes
	// of the commands parameters
	auto parsed = parseCommand(cmd);

	if(parsed.returnParam && !parsed.returnParam->countPar) {
		auto derefType = parsed.returnParam->param->type.pointer;
		functions_ << "inline " << typeName(*derefType) << " " << name << "(";
	} else {
		functions_ << "inline " << typeName(cmd.signature.returnType) << " " << name << "(";
	}

	auto printVecVersion = false;
	auto declSepr = "";
	auto callSepr = "";
	std::string args;

	// iterate over parameters and output their declarations
	// append thei call usage to args
	for(auto& pparam : parsed.parsedParams) {
		if(pparam.countPar)
			printVecVersion = true;

		auto decl = paramDecl(pparam, false, declSepr, parsed.returnParam);
		if(!decl.empty()) {
			functions_ << decl;
			declSepr = ", ";
		}

		args += paramCall(pparam, false, callSepr, parsed.returnParam);
		callSepr = ", ";
	}

	std::string before = "";
	std::string after = ";";
	auto& retType = cmd.signature.returnType;

	if(retType.type->name == "VkResult") {
		before = "VPP_CALL(";
		after = ");";
	}

	if(parsed.returnParam && !parsed.returnParam->countPar) {
		 auto derefType = parsed.returnParam->param->type.pointer;
		before = typeName(*derefType) + " ret = {}; " + before;
		after = after + " return ret;";
	} else if(retType.type->name != "void" || retType.pointer) {
		before = "return static_cast<" + typeName(cmd.signature.returnType) + ">(";
		after = ");";
	} else {
		before = "return ";
	}

	functions_ << "){ ";
	functions_ << before << cmd.name << "(" << args;
	functions_ << ")" << after << "}\n";

	// if possible/needed, output the std::vector version of the function
	if(printVecVersion) printVecCmd(parsed, name);
}

void CCOutputGenerator::printVecCmd(const ParsedCommand& pcmd, const std::string& name)
{
	auto& cmd = *pcmd.command;

	std::pair<const ParsedParam*, const ParsedParam*> vecRetStackVar {};
	std::pair<const ParsedParam*, const ParsedParam*>* vecRet = nullptr;

	if(pcmd.returnParam && pcmd.returnParam->countPar) {
		vecRet = &vecRetStackVar;
		vecRet->first = pcmd.returnParam;
		vecRet->second = pcmd.returnParam->countPar;
	}

	std::string retType = typeName(cmd.signature.returnType);
	if(vecRet) {
		auto typeCpy = *vecRet->first->param->type.pointer;
		typeCpy.constant = false;
		if(typeCpy.type->name != "void") retType = "std::vector<" + typeName(typeCpy) + ">";
		else retType = "std::vector<uint8_t>";
	}

	functions_ << "inline " << retType << " " << name << "(";

	auto declSepr = "";
	auto callSepr = "";
	std::string args;
	for(auto& pparam : pcmd.parsedParams) {
		auto decl = paramDecl(pparam, true, declSepr, pcmd.returnParam);
		if(!decl.empty()) {
			functions_ << decl;
			declSepr = ", ";
		}

		args += paramCall(pparam, true, callSepr, pcmd.returnParam);
		callSepr = ", ";
	}

	functions_ << "){ ";

	if(vecRet) {
		std::string code;
		auto& countPar = *pcmd.returnParam->countPar;
		if(vecRet->second->memberAsCount || !vecRet->second->out) {
			if(cmd.signature.returnType.type->name != "VkResult")
				code = vecFuncTemplateRetGivenVoid;
			else
				code = vecFuncTemplateRetGiven;

			// TODO: check for pointer
			std::string count;
			if(vecRet->first->countMember)
				count = vecRet->second->param->name + "." + vecRet->first->countMember->name;
			else if(countPar.dataPars[0] != pcmd.returnParam)
				count = countPar.dataPars[0]->param->name + ".size()";
			else
				count = countPar.param->name;

			code = std::regex_replace(code, std::regex("%c"), count);
		} else {
			if(cmd.signature.returnType.type->name != "VkResult") code = vecFuncTemplateVoid;
			else code = vecFuncTemplate;
		}

		auto typeCpy = *vecRet->first->param->type.pointer;
		typeCpy.constant = false;

		if(typeCpy.type->name != "void")
			code = std::regex_replace(code, std::regex("%t"), typeName(typeCpy));
		else
			code = std::regex_replace(code, std::regex("%t"), "uint8_t");

		code = std::regex_replace(code, std::regex("%a"), args);
		code = std::regex_replace(code, std::regex("%f"), cmd.name);

		auto it2 = &countPar.param->type;
		while(!it2->type) {
			if(it2->array) it2 = it2->array;
			if(it2->pointer) it2 = it2->pointer;
		}

		auto& countType = *it2->type;
		code = std::regex_replace(code, std::regex("%ct"), countType.name);

		functions_ << code;
		functions_ << " }\n";
	} else {
		auto& retType = cmd.signature.returnType;
		std::string returnString;
		std::string returnStringEnd;
		if(retType.type->name == "VkResult") {
			returnString = "return VPP_CALL(";
			returnStringEnd = ")";
		} else if(retType.type->name != "void" || retType.pointer) {
			returnString = "return static_cast<" + typeName(cmd.signature.returnType) + ">(";
			returnStringEnd = ")";
		}

		functions_ << returnString << cmd.name << "(" << args;
		functions_ << ")" << returnStringEnd << " ;}\n";
	}
}

std::string CCOutputGenerator::paramDecl(const ParsedParam& param, bool rangeify, const char* sepr,
	const ParsedParam* retParam) const
{
	if(retParam && &param == retParam && !retParam->countPar) return ""; //returnParam data part

	std::string ret = sepr;
	if(rangeify) {
		if(retParam && &param == retParam) return "";

		if(!param.dataPars.empty() && !param.memberAsCount) {
			if(param.out || !retParam || (retParam && retParam != param.dataPars[0])) return "";
		}

		if(param.countPar && !param.countMember) {
			auto type = &param.param->type;
			QualifiedType localqt;
			Type localt;
			if(type->pointer) type = param.param->type.pointer;
			if(type->type->name == "void") {
				localqt = *type;
				localqt.type = &localt;
				localt = *type->type;
				localt.name = "uint8_t";
				type = &localqt;
			}

			ret += "nytl::Span<" + typeName(*type) + "> " + param.param->name;
			return ret;
		}
	}

	bool optional = ((rangeify || (!retParam || !retParam->countPar)) && param.optional);
	optional |= (!rangeify && retParam && retParam->countPar && param.optionalWithRet);

	//reference for non optional pointer parameters
	auto namedParam = *param.param;

	if(namedParam.type.pointer && !param.param->optional) {
		auto it = namedParam.type.pointer;
		while(!it->type) {
			if(it->pointer) it = it->pointer;
			if(it->array) it = it->array;
		}

		if(it->type->name != "void" && it->type->name != "char") {
			namedParam.type = *it;
			namedParam.type.reference = true;
		}
	}

	ret += paramName(namedParam);
	if(optional) ret += " = {}";

	return ret;
}

std::string CCOutputGenerator::paramCall(const ParsedParam& param, bool rangeify, const char* sepr,
	const ParsedParam* retParam) const
{
	std::string ret = sepr;

	// check if return param (count or data part)
	if(retParam && &param == retParam && !param.countPar) {
		ret += "(" + typeName(param.param->type, false) + ")(&ret)";
		return ret;
	} else if(!param.memberAsCount && rangeify && retParam && &param == retParam->countPar) {
		if(param.out) {
			ret += "&count";
			return ret;
		} else if(param.dataPars[0] != retParam) {
			ret += param.dataPars[0]->param->name + ".size()";
			return ret;
		} else {
			ret += param.param->name;
			return ret;
		}
	}

	if(rangeify) {
		if(retParam && &param == retParam) {
			ret += "(" + typeName(param.param->type, false) + ")(ret.data())";
			return ret;
		} else if(!param.dataPars.empty()) {
			//is count part of normal range
			if(!param.memberAsCount) {
				ret += param.dataPars[0]->param->name;
				ret += ".size()";
				return ret;
			}
		} else if(param.countPar)  {
			//is data part of normal (nonRet) range
			std::string ret = sepr;
			ret += "(" + typeName(param.param->type, false) + ")(";
			ret += param.param->name;
			ret += ".data())";
			return ret;
		}
	}

	// normal params here that will not be returned and are not (directly) part of a range
	Type::Category category {};
	std::string name {};
	if(param.param->type.type) {
		category = param.param->type.type->category;
		name = param.param->type.type->name;
	} else {
		auto it = &param.param->type;
		while(!it->type) {
			if(it->pointer) it = it->pointer;
			if(it->array) it = it->array;
		}

		category = it->type->category;
		name = it->type->name;
	}

	if(param.param->type.pointer || category == Type::Category::handle) {
		bool optional = ((rangeify || (!retParam || !retParam->countPar)) && param.optional);
		optional |= (!rangeify && retParam && retParam->countPar && param.optionalWithRet);

		const char* ref = "";
		if(!param.param->optional && param.param->type.pointer &&
			name != "void" && name != "char") ref = "&";

		ret += "(";
		ret += typeName(param.param->type, false);
		ret += ")(";
		ret += ref;
		ret += param.param->name;
		ret += ")";
	} else if(category == Type::Category::enumeration || category == Type::Category::bitmask) {
		ret += "static_cast<";
		ret += typeName(param.param->type, false);
		ret += ">(";
		ret += param.param->name;
		ret += ")";
	} else if(param.param->type.array) {
		ret += param.param->name + ".data()";
	} else {
		ret += param.param->name;
	}

	return ret;
}
