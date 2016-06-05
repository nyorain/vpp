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

#include "registry.hpp"

#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
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
			handle.type = typeit.child_value("type");
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
			if(std::string(typeit.attribute("returnedonly").as_string()) == "true") s.returnedonly = true;

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
				next.erase(std::remove(next.begin(), next.end(), '\n'), next.end());

				auto pos = next.find(',');
				if(pos == std::string::npos) pos = next.find(')');
				next = next.substr(0, pos);

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
			auto value = enumit.attribute("value").as_string();
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
	Param ret(node);

	auto optAttrib = node.attribute("optional");
	if(std::strcmp(optAttrib.as_string(), "true") == 0) ret.optional = true;

	auto currChild = pugi::xml_node{};
	for(auto child : node)
	{
		currChild = child;
		std::string n = child.name();
		if(n == "type")
		{
			ret.type.type = registry_.findType(child.text().get());
		}
		if(n == "name")
		{
			ret.name = child.text().get();
			break;
		}
		else
		{
			std::string txt = child.value();
			ret.type.pointerlvl += std::count(txt.begin(), txt.end(), '*');
			if(txt.find("const") != std::string::npos) ret.type.constant = true;
		}
	}

	//TODO: integrate this in loop above?
	auto child = currChild;
	bool bracketOpen = false;
	while((child = child.next_sibling()))
	{
		std::string txt = child.text().get();
		while(!txt.empty())
		{
			if(!bracketOpen)
			{
				auto pos = txt.find('[');
				if(pos != std::string::npos)
				{
					bracketOpen = true;
					if(txt.size() > pos + 1) txt = txt.substr(pos + 1);
					else break;
				}
				else break;
			}

			auto pos = txt.find(']');
			if(txt.empty()) break;
			if(pos != 0) ret.type.arraylvl.push_back(txt.substr(0, pos));

			if(pos != std::string::npos)
			{
				bracketOpen = false;
				if(txt.size() > pos + 1) txt = txt.substr(pos + 1);
				else break;
			}
			else break;
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
	extension.reqs = parseRequirements(node, true);
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

Requirements RegistryLoader::parseRequirements(const pugi::xml_node& node, bool extension)
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
			parseCommandReqs(*cmd, ret, extension);
		}

		//enums
		for(auto& req : require.children("enum"))
		{
			auto enumName = req.attribute("name").as_string();
			auto value = std::string(req.attribute("value").value());
			auto extAttrib = req.attribute("extends");
			if(!extAttrib && (value[0] == '\"' || value.find("VK") == std::string::npos))
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
			parseTypeReqs(member.type, reqs);
	}
	else if(type.category == Type::Category::funcptr)
	{
		auto& ptr = static_cast<FuncPtr&>(type);
		for(auto& param : ptr.signature.params)
			parseTypeReqs(param.type, reqs);
	}
	else if(type.category == Type::Category::bitmask)
	{
		auto& mask = static_cast<Bitmask&>(type);
		if(mask.bits) parseTypeReqs(*mask.bits, reqs);
	}

	reqs.types.push_back(&type);
}

void RegistryLoader::parseTypeReqs(QualifiedType& type, Requirements& reqs)
{
	for(auto& arrLvl : type.arraylvl)
	{
		auto constant = registry_.findConstant(arrLvl);
		if(constant)
		{
			bool found = false;
			for(auto& c : reqs.constants)
			{
				if(c == constant)
				{
					found = true;
					break;
				}
			}

			if(!found) reqs.constants.push_back(constant);
		}
	}

	if(type.type) parseTypeReqs(*type.type, reqs);
}

void RegistryLoader::parseCommandReqs(Command& cmd, Requirements& reqs, bool extension)
{
	for(auto& param : cmd.signature.params)
		parseTypeReqs(*param.type.type, reqs);

	parseTypeReqs(*cmd.signature.returnType.type, reqs);

	if(!extension || prototypes_) reqs.commands.push_back(&cmd);
	if(extension) reqs.funcPtr.push_back(&cmd);
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

//Requirements
void Requirements::add(Requirements& reqs)
{
	commands.insert(commands.end(), reqs.commands.begin(), reqs.commands.end());
	types.insert(types.end(), reqs.types.begin(), reqs.types.end());
	constants.insert(constants.end(), reqs.constants.begin(), reqs.constants.end());
	funcPtr.insert(funcPtr.end(), reqs.funcPtr.begin(), reqs.funcPtr.end());

	for(auto& c : reqs.extraConstants)
		constants.push_back(&c);
}
