#include "codegen.hpp"

Registry RegistryLoader::parse()
{
	//types
	auto regNode = doc_.child("registry");
	auto types = regNode.child("types");
	loadTypes(types);

	//enums
	auto enums = regNode.child("enums");
	loadEnums(enums);

	//commands
	auto commands = regNode.child("commands");
	loadCommands(commands);

	return registry_;
}

void RegistryLoader::loadTypes(const pugi::xml_node& node)
{
	auto& bulk = registry_.defaultBulk_;
	for(auto typeit = node.child("type"); typeit; typeit = typeit.next_sibling("type"))
	{
		auto category = typeit.attribute("category").as_string();
		auto name = typeit.attribute("name").as_string();
		if(category == "enum")
		{
			bulk.enums.emplace_back(name);
		}
		else if(category == "handle")
		{
			//auto parent = typeit.attribute("parent").as_string();
			Handle handle(name);
			bulk.handles.push_back(handle);
		}
		else if(category == "struct")
		{
			Struct s(name);
			for(auto member = typeit.child("member"); member; member = member.next_sibling("memeber"))
			{
				auto type = member.attribute("type").as_string();
				auto name = member.attribute("name").as_string();
				s.members.emplace_back(type, name);
			}
			bulk.structs.push_back(s);
		}
	}
}

void RegistryLoader::loadEnums(const pugi::xml_node& node)
{
	for(auto enumit = node.child("enum"); enumit; enumit = enumit.next_sibling("type"))
	{

	}
}

void RegistryLoader::loadCommands(const pugi::xml_node& node)
{
	auto& bulk = registry_.defaultBulk_;

	for(auto cmd = node.child("command"); cmd; cmd = cmd.next_sibling("command"))
	{
		Command command{};
		command.returnType = {registry_.findType(cmd.child("proto").child("type").value())};
		command.name = cmd.child("proto").child("name").value();
		bulk.commands.push_back(command);
	}
}
