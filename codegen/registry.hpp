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

#pragma once

#include <vector>
#include <deque>
#include <string>
#include <cstdint>
#include <utility>
#include <fstream>

#include "pugixml/pugixml.hpp"

class Entry
{
public:
	pugi::xml_node node;

public:
	Entry() = default;
	Entry(const pugi::xml_node& xnode) : node(const_cast<pugi::xml_node&>(xnode)) {}
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
		: Entry(node), category(cat), name(xname) {}
};

class Enum : public Type
{
public:
	std::vector<std::pair<std::string, std::int64_t>> values;
	int unusedStart = 0;
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
	Enum* bits = nullptr;

public:
	Bitmask() = default;
	Bitmask(const std::string& name, const pugi::xml_node& node)
		: Type(Category::bitmask, name, node) {}
};

class Handle : public Type
{
public:
	std::string type;
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
	bool reference = false;
	unsigned int pointerlvl = 0;
	std::vector<std::string> arraylvl;
};

struct Param : public Entry
{
public:
	QualifiedType type;
	std::string name;
	bool optional = false;

public:
	Param() = default;
	Param(const pugi::xml_node& node) : Entry(node) {}
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

class Requirements
{
public:
	std::vector<Command*> commands;
	std::vector<Command*> funcPtr;
	std::vector<Type*> types;
	std::vector<Constant*> constants;
	std::vector<Constant> extraConstants;

public:
	void add(Requirements& reqs);
};

class Extension : public Entry
{
public:
	Requirements reqs;
	std::string protect;
	std::string name;
	int number;
};

class Feature : public Entry
{
public:
	Requirements reqs;
	std::string name;
	std::string number;
	std::string api;
	std::vector<Extension*> extensions;
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

	std::string copyright;

public:
	Type* findType(const std::string& name);
	Struct* findStruct(const std::string& name);
	ExternalType* findExternalType(const std::string& name);
	Bitmask* findBitmask(const std::string& name);
	Enum* findEnum(const std::string& name);
	BaseType* findBaseType(const std::string& name);
	Handle* findHandle(const std::string& name);
	FuncPtr* findFuncPtr(const std::string& name);

	Command* findCommand(const std::string& name);
	Constant* findConstant(const std::string& name);

	Feature* findFeatureByApi(const std::string& name);
	Feature* findFeatureByName(const std::string& name);
	Extension* findExtension(const std::string& name);
};

class RegistryLoader
{
public:
	RegistryLoader(const std::string& xmlPath);
	Registry& parse();

	void loadTypes(const pugi::xml_node& node);
	void loadEnums(const pugi::xml_node& node);
	void loadCommands(const pugi::xml_node& node);

	void loadFeature(const pugi::xml_node& node);
	void loadExtension(const pugi::xml_node& node);

	Requirements parseRequirements(const pugi::xml_node& node, bool extension = false);
	void parseTypeReqs(Type& type, Requirements& reqs);
	void parseTypeReqs(QualifiedType& type, Requirements& reqs);
	void parseCommandReqs(Command& cmd, Requirements& reqs, bool extension = false);

	Param parseParam(const pugi::xml_node& node);

protected:
	Registry registry_;
	pugi::xml_document doc_;
	bool prototypes_ = false;
};
