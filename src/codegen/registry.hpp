// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vector>
#include <deque>
#include <string>
#include <cstdint>
#include <utility>
#include <fstream>

#include "pugixml/pugixml.hpp"

/// Base class for all registry entries.
/// Stores the associated xml node.
class Entry {
public:
	pugi::xml_node node;

public:
	Entry() = default;
	Entry(const pugi::xml_node& xnode) : node(const_cast<pugi::xml_node&>(xnode)) {}
};

/// Base class for all type defines.
/// Only used for types directly specified in the registry, for additional
/// qualifications, see QualifiedType.
class Type : public Entry {
public:
	/// The category of the type entry.
	/// Objects can be casted depending on this value.
	enum class Category {
		none,
		structure,
		enumeration,
		handle,
		bitmask,
		external,
		basetype,
		define,
		funcptr,
		plain
	};

public:
	Category category = Category::none; // the category
	std::string name {}; // the type name

public:
	Type() = default;
	Type(Category cat, const std::string& xname, const pugi::xml_node& node)
		: Entry(node), category(cat), name(xname) {}
};

/// Enumeration type.
/// Stores all enumeration values.
class Enum : public Type {
public:
	std::vector<std::pair<std::string, std::int64_t>> values;
	bool bitmask {}; /// Whether the enum is a bitmask type.

public:
	Enum() = default;
	Enum(const std::string& name, const pugi::xml_node& node)
		: Type(Category::enumeration, name, node) {};
};

/// Typedef type.
/// Stores a pointer to the original type, i.e. the real type behind the typedef.
class BaseType : public Type {
public:
	Type* original;

public:
	BaseType(const std::string& name, const pugi::xml_node& node)
		: Type(Category::basetype, name, node) {}
};

/// Type macro definition
class Define : public Type {
public:
	std::string define;
};

/// External type definition.
/// Usually used for platform specific types that are included per platform
/// headers.
class ExternalType : public Type {
public:
	ExternalType(const std::string& name, const pugi::xml_node& node)
		: Type(Category::external, name, node) {}
};

/// Bitmask type aka flags.
/// Holds the associated enumeration, or nullptr if there is none.
class Bitmask : public Type {
public:
	Enum* bits = nullptr;

public:
	Bitmask() = default;
	Bitmask(const std::string& name, const pugi::xml_node& node)
		: Type(Category::bitmask, name, node) {}
};

/// Handle type.
/// Holds the name of the macro type that defines it.
class Handle : public Type {
public:
	std::string type;

public:
	Handle() = default;
	Handle(const std::string& name, const pugi::xml_node& node)
		: Type(Category::handle, name, node) {};
};

/// Wrapper around a Type object for additional qualifiers.
/// Has always either the type or the pointer or the array member active.
class QualifiedType {
public:
	Type* type {}; // the plain type
	QualifiedType* pointer {}; // the reference/pointer/array type
	QualifiedType* array {}; // the reference/pointer/array type

	bool constant = false; // is the type contant? (this pointer/array level)
	std::string arraySize {}; // the size of the array (can be a constant)

	// TODO: remove them (only reference may be valid)
	bool reference = false;
	// unsigned int pointerlvl = 0;
	// std::vector<std::string> arraylvl;
};

/// A function parameter or struct member.
class Param : public Entry {
public:
	QualifiedType type {};
	std::string name {};
	bool optional = false;

public:
	Param() = default;
	Param(const pugi::xml_node& node) : Entry(node) {}
};

/// A Structure type
class Struct : public Type {
public:
	std::vector<Param> members;
	bool returnedonly = false;
	bool isUnion = false;

public:
	Struct(const std::string& name, const pugi::xml_node& node)
		: Type(Category::structure, name, node) {}
};

/// Function signature.
/// Holds return type and parameters.
class FuncSignature {
public:
	QualifiedType returnType {};
	std::vector<Param> params;
};

/// Function pointer type
class FuncPtr : public Type {
public:
	FuncSignature signature;

public:
	FuncPtr(const std::string& name, const pugi::xml_node& node)
		: Type(Category::funcptr, name, node) {}
};

/// Command registry entry.
/// Defines a certain vulkan api function.
/// Holds signature and name.
class Command : public Entry {
public:
	FuncSignature signature;
	std::string name;

public:
	Command() = default;
	Command(const pugi::xml_node& node) : Entry(node) {}
};

/// Api constant.
/// Used for symbolics such as vk::nullHandle.
class Constant : public Entry {
public:
	std::string name;
	std::string value;

public:
	Constant(const pugi::xml_node& node) : Entry(node) {}
};

/// Requirements of an extensions or feature.
/// Basically just groups all other entries together.
class Requirements {
public:
	std::vector<Command*> commands;
	std::vector<Command*> funcPtr;
	std::vector<Type*> types;
	std::vector<Constant*> constants;
	std::vector<Constant> extraConstants;

public:
	void add(Requirements& reqs);
};

/// Extensions entry holding its requirements as well as name and id.
/// If protect is set, the extensions will not be parsed.
class Extension : public Entry {
public:
	Requirements reqs;
	std::string protect;
	std::string name;
	std::string supported;
	int number;
};

/// A vulkan api feature.
/// Used for api versions and consists of requirements and extensions.
class Feature : public Entry {
public:
	Requirements reqs;
	std::string name;
	std::string number;
	std::string api;
	std::vector<Extension*> extensions;
};

/// The registry class holding the whole vulkan registry.
/// Does not load the registry, see RegistryLoader.
class Registry {
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
	Container<Define> defines;
	Container<FuncPtr> funcPtrs;
	Container<Type> plainTypes;

	Container<Feature> features;
	Container<Extension> extensions;

	Container<QualifiedType> qualifieds;

	Container<std::string> vendors;
	Container<std::string> tags;

	std::string copyright;
	std::string version; // header patch vesion (e.g. 42 for 1.0.42)

public:
	Type* findType(const std::string& name);
	Struct* findStruct(const std::string& name);
	ExternalType* findExternalType(const std::string& name);
	Bitmask* findBitmask(const std::string& name);
	Enum* findEnum(const std::string& name);
	BaseType* findBaseType(const std::string& name);
	Handle* findHandle(const std::string& name);
	FuncPtr* findFuncPtr(const std::string& name);
	Type* findPlainType(const std::string& name);

	Command* findCommand(const std::string& name);
	Constant* findConstant(const std::string& name);

	Feature* findFeatureByApi(const std::string& name);
	Feature* findFeatureByName(const std::string& name);
	Extension* findExtension(const std::string& name);
};

/// Loads a vulkan registry from a file.
/// Use the parse member function to parse and retrieve a Registry object
/// for the passed file.
class RegistryLoader {
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
