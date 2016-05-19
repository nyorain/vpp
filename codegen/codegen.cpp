#include "codegen.hpp"

Registry RegistryLoader::parse()
{
	Registry ret;

	//types
	auto types = doc_.child("types");

	for(auto typeit = types.child("type"); typeit; typeit = typeit.next_sibling("type"))
	{
		auto category = typeit.attribute("category").as_string();
		auto name = typeit.attribute("name").as_string();
		if(category == "enum")
		{
			ret.defaultBulk_.enums.emplace_back(name);
		}
		else if(category == "handle")
		{
			//auto parent = typeit.attribute("parent").as_string();
			Handle handle(name);
			ret.defaultBulk_.handles.push_back(handle);
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
		}
	}

	//enums
	auto enums = doc_.child("enums");

	for(auto enumit = enums.child("enum"); enumit; enumit = enumit.next_sibling("type"))
	{

	}

	//commands
	auto commands = doc_.child("commands");

	for(auto cmd = commands.child("command"); cmd; cmd = cmd.next_sibling("command"))
	{
		Command command{};
		command.returnType = {ret.findType(cmd.child("proto").child("type").value())};
		command.name = cmd.child("proto").child("name").value();
		ret.defaultBulk_.commands.push_back(command);
	}

	return ret;
}
