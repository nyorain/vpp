#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <utility>

#include <pugixml/pugixml.hpp>

class Entry
{
	pugi::xml_node node_;
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
	Category category;
	std::string name;

public:
	Type() = default;
	Type(Category cat, const std::string& xname) : category(cat), name(xname) {}
};

class Enum : public Type
{
public:
	std::vector<std::pair<std::string, std::uint64_t>> values;
	bool bitmask;

public:
	Enum() = default;
	Enum(const std::string& name) : Type(Category::enumeration, name) {};
};

class Handle : public Type
{
public:
	bool dispatchable = false;
	Handle* parent = nullptr;

public:
	Handle() = default;
	Handle(const std::string& name) : Type(Category::handle, name) {};
};

struct QualifiedType
{
public:
	Type type {};
	bool constant = false;
	unsigned int pointerlvl = 0;
};

class Struct : public Type
{
public:
	std::vector<std::pair<QualifiedType, std::string>> members;
	bool returnedonly = false;

public:
	Struct(const std::string& name) : Type(Category::structure, name) {}
};

struct Command : public Entry
{
public:
	QualifiedType returnType;
	std::string name;
	std::vector<std::pair<QualifiedType, std::string>> params;

public:
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

protected:
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
