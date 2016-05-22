#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <utility>

#include <pugixml/pugixml.hpp>

class Entry
{
public:
	pugi::xml_node* node_ = nullptr;

public:
	Entry() = default;
	Entry(pugi::xml_node& node) : node_(&node) {}
};

class Type : public Entry
{
public:
	enum class Category
	{
		none,
		structure,
		enumeration,
		handle
	};

public:
	Category category = Category::none;
	std::string name {};

public:
	Type() = default;
	Type(Category cat, const std::string& xname, pugi::xml_node& node)
		: category(cat), name(xname), Entry(node) {}
};

class Enum : public Type
{
public:
	std::vector<std::pair<std::string, std::uint64_t>> values;
	bool bitmask;

public:
	Enum() = default;
	Enum(const std::string& name, pugi::xml_node& node) : Type(Category::enumeration, name, node) {};
};

class Handle : public Type
{
public:
	bool dispatchable = false;
	Handle* parent = nullptr;

public:
	Handle() = default;
	Handle(const std::string& name, pugi::xml_node& node) : Type(Category::handle, name, node) {};
};

struct QualifiedType
{
public:
	Type type {};
	bool constant = false;
	unsigned int pointerlvl = 0;
};

struct Param
{
	QualifiedType type;
	std::string name;
};

class Struct : public Type
{
public:
	std::vector<Param> members;
	bool returnedonly = false;

public:
	Struct(const std::string& name, pugi::xml_node& node) : Type(Category::structure, name, node) {}
};

struct Command : public Entry
{
public:
	QualifiedType returnType;
	std::string name;
	std::vector<Param> params;

public:
	Command() = default;
	Command(pugi::xml_node& node) : Entry(node) {}
};

struct Bulk
{
	std::vector<Command> commands;
	std::vector<Handle> handles;
	std::vector<Enum> enums;
	std::vector<Struct> structs;
};

class Extension : public Entry
{
	Bulk features_;
};

class Registry
{
protected:
	friend class RegistryLoader;
	Bulk defaultBulk_;
	std::vector<Extension> extensions_;
	std::vector<std::string> extenders_;

protected:
	Type& findType(const std::string& name);
};

class RegistryLoader
{
public:
	RegistryLoader(const std::string& xmlPath);
	Registry parse();

	void loadTypes(const pugi::xml_node& node);
	void loadEnums(const pugi::xml_node& node);
	void loadCommands(const pugi::xml_node& node);

	Param parseParam(const pugi::xml_node& node);

protected:
	Registry registry_;
	pugi::xml_document doc_;
};

class OutputGenerator
{
public:
	std::string removeVkPrefix(const std::string& string, bool* found = nullptr) const;
	bool removeVkPrefix(std::string& string) const;

	std::string removeExtSuffix(const std::string& string, std::string* extension = nullptr) const;
	std::string removeExtSuffix(const std::string& string) const;

	std::string camelcase(const std::string& string) const;
	std::string uppercase(const std::string& string) const;

protected:
	Registry* registry_;
};

class CCOutputGenerator
{
public:
	CCOutputGenerator(Registry& reg, const std::string& header, const std::string& fwd);
	void generate();
};
