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
		if(category == "struct")
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
			if(str.size() > 1 && str.substr(0, 2) == "0x")
				value.second = std::stoul(str, nullptr, 16);
			else
				value.second = attrib.as_llong();

			ret.values.push_back(value);
		}
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
	bool isConst = false;
	unsigned int pointerlvl = 0;

	std::string type = "";
	std::string name = "";

	for(auto child : node)
	{
		std::string n = child.name();
		if(n == "type") type = child.text().get();
		if(n == "name") name = child.text().get();
		else
		{
			std::string txt = child.value();
			pointerlvl += std::count(txt.begin(), txt.end(), '*');
			if(txt.find("const") != std::string::npos) isConst = true;
		}
	}

	QualifiedType t;
	t.type = registry_.findType(type);
	t.constant = isConst;
	t.pointerlvl = pointerlvl;

	return {t, name};
}

Requirements RegistryLoader::parseRequirements(const pugi::xml_node& node)
{
	for(auto& req : node.children("require"))
	{
		
	}
}




//registry
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
R"SRC(#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>

namespace vk
{

)SRC";

	fwd_ << fwdHeader;

	//header file
	auto headerHeader =
R"SRC(#pragma once

#include "fwd.hpp"

namespace vk
{

)SRC";

	header_ << headerHeader;

	//handle
	std::cout << "\tOutputting " << registry().handles.size() << " handles\n";
	for(auto& type : registry().handles)
	{
		auto name = removeVkPrefix(type.name, nullptr);

		fwd_ << "using " << name << " = " << type.name << ";\n";
	}

	fwd_ << "\n\n";
	header_ << "\n\n";

	//typedefs
	std::cout << "\tOutputting " << registry().baseTypes.size() << " typedefs\n";
	for(auto& type : registry().baseTypes)
	{
		fwd_ << "using " << type.name << " = " << type.original->name << ";\n";
	}

	fwd_ << "\n\n";
	header_ << "\n\n";

	//enums
	std::cout << "\tOutputting " << registry().enums.size() << " enums\n";
	for(auto& type : registry().enums)
	{
		auto name = removeVkPrefix(type.name, nullptr);

		//output enum prototypes
		fwd_ << "enum class " << name << " : std::int32_t;\n";

		//header
		header_ << "enum class " << name << " : std::int32_t\n{\n";
		for(auto& value : type.values)
		{
			bool bit;
			auto n = enumName(type, value.first, &bit);

			header_ << "\t" << n << " = ";

			if(bit) header_ << "(1 << " << value.second << "),\n";
			else header_ << value.second << ",\n";
		}

		header_ << "};\n\n";
	}

	fwd_ << "\n\n";
	header_ << "\n\n";

	//structs
	std::cout << "\tOutputting " << registry().structs.size() << " structs\n";
	for(auto& type : registry().structs)
	{
		auto name = removeVkPrefix(type.name, nullptr);

		std::string metaType = "struct";
		if(type.isUnion) metaType = "union";

		//fwd
		fwd_ << metaType << " " << name << ";\n";

		//header
		header_ << metaType << " " << name << "\n{\n";

		bool unionInit = false;
		for(auto& member : type.members)
		{
			std::string init = "";
			if(member.name == "sType") init = "StructureType::" + name;
			header_ << "\t" << member.type.typestring(*this) << " " << member.name;
			if(!type.isUnion || unionInit) header_ << " {" << init << "}";

			unionInit = true;
			header_ << ";\n";
		}

		header_ << "};\n\n";
	}

	fwd_ << "\n\n";
	header_ << "\n\n";

	//commands
	std::cout << "\tOutputting " << registry().commands.size() << " commands\n";
	for(auto& cmd : registry().commands)
	{
		auto name = removeVkPrefix(cmd.name, nullptr);
		name[0] = std::tolower(name[0], std::locale());

		header_ << cmd.signature.returnType.string() << " " << name << "(";

		auto sepr = "";
		for(auto& param : cmd.signature.params)
		{
			header_ << sepr << param.type.typestring(*this) << " " << param.name;
			sepr = ", ";
		}

		header_ << ")\n{\n\t" << cmd.name << "(";
		sepr = "";
		for(auto& param : cmd.signature.params)
		{
			header_ << sepr << param.name;
			sepr = ", ";
		}

		header_ << ");\n}\n";
	}

	//end
	fwd_ << "\n}";
	header_ << "\n}";
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
