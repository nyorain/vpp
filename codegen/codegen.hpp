#pragma once

#include <vector>
#include <deque>
#include <string>
#include <cstdint>
#include <utility>
#include <fstream>

#include <pugixml/pugixml.hpp>

class OutputGenerator;
class CCOutputGenerator;

class Entry
{
public:
	const pugi::xml_node* node_ = nullptr;

public:
	Entry() = default;
	Entry(const pugi::xml_node& node) : node_(&node) {}
};

class Type : public Entry
{
public:
	enum class Category
	{
		none,
		structure,
		enumeration,
		handle,
		bitmask,
		include,
		external,
		basetype,
		define,
		funcptr
	};

public:
	Category category = Category::none;
	std::string name {};

public:
	Type() = default;
	Type(Category cat, const std::string& xname, const pugi::xml_node& node)
		: category(cat), name(xname), Entry(node) {}
};

class Enum : public Type
{
public:
	std::vector<std::pair<std::string, std::int64_t>> values;
	bool bitmask;

public:
	Enum() = default;
	Enum(const std::string& name, const pugi::xml_node& node)
		: Type(Category::enumeration, name, node) {};
};

class BaseType : public Type
{
public:
	Type* original;

public:
	BaseType(const std::string& name, const pugi::xml_node& node)
		: Type(Category::basetype, name, node) {}
};

class Include : public Type
{

};

class Define : public Type
{
	std::string define;
};

class ExternalType : public Type
{
public:
	Include* requires;

public:
	ExternalType(const std::string& name, const pugi::xml_node& node)
		: Type(Category::external, name, node) {}
};

class Bitmask : public Type
{
public:
	Enum* bits;

public:
	Bitmask() = default;
	Bitmask(const std::string& name, const pugi::xml_node& node)
		: Type(Category::bitmask, name, node) {}
};

class Handle : public Type
{
public:
	bool dispatchable = false;
	Handle* parent = nullptr;

public:
	Handle() = default;
	Handle(const std::string& name, const pugi::xml_node& node)
		: Type(Category::handle, name, node) {};
};

struct QualifiedType
{
public:
	Type* type {};
	bool constant = false;
	unsigned int pointerlvl = 0;

public:
	std::string string() const;
	std::string typestring(const CCOutputGenerator& generator) const;
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
	bool isUnion = false;

public:
	Struct(const std::string& name, const pugi::xml_node& node)
		: Type(Category::structure, name, node) {}
};

class FuncSignature
{
public:
	QualifiedType returnType;
	std::vector<Param> params;
};

class FuncPtr : public Type
{
public:
	FuncSignature signature;

public:
	FuncPtr(const std::string& name, const pugi::xml_node& node)
		: Type(Category::funcptr, name, node) {}
};

struct Command : public Entry
{
public:
	FuncSignature signature;
	std::string name;

public:
	Command() = default;
	Command(const pugi::xml_node& node) : Entry(node) {}
};

class Constant : public Entry
{
public:
	std::string name;
	std::string value;

public:
	Constant(const pugi::xml_node& node) : Entry(node) {}
};

struct Requirements
{
	std::vector<Command*> commands;
	std::vector<Type*> types;
	std::vector<Constant*> constants;
	std::vector<Constant> extraConstants;
};

class Feature : public Entry
{
public:
	Requirements reqs;
	std::string name;
	std::string number;
};

class Extension : public Entry
{
public:
	Requirements reqs;
	std::string protect;
	unsigned int id;
};

class Registry
{
public:
	template<typename T> using Container = std::deque<T>;

	Container<Command> commands;
	Container<Handle> handles;
	Container<Enum> enums;
	Container<Struct> structs;
	Container<Bitmask> bitmasks;
	Container<Constant> constants;
	Container<ExternalType> externalTypes;
	Container<BaseType> baseTypes;
	Container<Include> includes;
	Container<Define> defines;
	Container<FuncPtr> funcPtrs;

	Container<Feature> features;
	Container<Extension> extensions;

	Container<std::string> vendors;
	Container<std::string> tags;

public:
	Type* findType(const std::string& name);
	Struct* findStruct(const std::string& name);
	ExternalType* findExternalType(const std::string& name);
	Bitmask* findBitmask(const std::string& name);
	Enum* findEnum(const std::string& name);
	BaseType* findBaseType(const std::string& name);
	Handle* findHandle(const std::string& name);
	FuncPtr* findFuncPtr(const std::string& name);
};

class RegistryLoader
{
public:
	RegistryLoader(const std::string& xmlPath);
	Registry& parse();

	void loadTypes(const pugi::xml_node& node);
	void loadEnums(const pugi::xml_node& node);
	void loadCommands(const pugi::xml_node& node);

	Requirements parseRequirements(const pugi::xml_node& node);

	Param parseParam(const pugi::xml_node& node);

protected:
	Registry registry_;
	pugi::xml_document doc_;
};

class OutputGenerator
{
public:
	std::string removeVkPrefix(const std::string& string, bool* found) const;
	bool removeVkPrefix(std::string& string) const;

	std::string removeExtSuffix(const std::string& string, std::string* extension) const;
	std::string removeExtSuffix(std::string& string) const;

	std::string camelCase(const std::string& string, bool firstupper = false) const;
	void camelCaseip(std::string& string, bool firstupper = false) const;

	std::string upperCase(const std::string& string) const;
	void upperCaseip(std::string& string) const;

	std::string strip(const std::string& string) const;
	void stripip(std::string& string) const;

	Registry& registry() const { return *registry_; }

protected:
	OutputGenerator(Registry& reg) : registry_(&reg) {}
	Registry* registry_;
};

class CCOutputGenerator : public OutputGenerator
{
public:
	CCOutputGenerator(Registry& reg, const std::string& header, const std::string& fwd);
	void generate();

	std::string enumName(const Enum& e, const std::string& name, bool* bit = nullptr) const;
	std::string typeName(const std::string& name) const;

protected:
	std::ofstream header_;
	std::ofstream fwd_;
};
