// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <string>
#include <fstream>
#include <vector>

class Registry;
class Param;
class Type;
class QualifiedType;
class Enum;
class Constant;
class Command;
class Requirements;
class Struct;

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

struct CCOutputGeneratorSettings
{
	std::string outputFolder = ".";
};

struct ParsedParam
{
	const Param* param;

	//if parsed param is array it references the count param
	ParsedParam* countPar = nullptr;
	const Param* countMember = nullptr;

	//if parsed param is count
	std::vector<ParsedParam*> dataPars;
	bool memberAsCount = false; //if it is count, whether the count is only a struct member

	//whether the param is an out param
	bool out = false;

	//whether the param is optional
	bool optional = false;
	bool optionalWithRet = false; //whehter it is optional if return param is normal param
};

struct ParsedCommand
{
	const Command* command = nullptr;
	std::vector<ParsedParam> parsedParams;
	ParsedParam* returnParam = nullptr; //the data part of the return param
};

class CCOutputGenerator : public OutputGenerator
{
public:
	CCOutputGenerator(Registry& reg, const CCOutputGeneratorSettings& settings = {});
	void generate();
	void printReqs(Requirements& reqs, const Requirements& fulfilled, const std::string& guard = "");

	void printStruct(const Struct& type);
	void printCmd(const Command& command);
	void printVecCmd(const Command& command, const Param& count, const Param& data);
	void printVecCmd(const ParsedCommand& command, const std::string& name);

	ParsedCommand parseCommand(const Command& cmd) const;
	std::string paramDecl(const ParsedParam& param, bool rangeify, const char* sepr,
		const ParsedParam* retParam) const;
	std::string paramCall(const ParsedParam& param, bool rangeify, const char* sepr,
		const ParsedParam* retParam) const;

	std::string enumName(const Enum& e, const std::string& name, bool* bit = nullptr) const;
	std::string constantName(const Constant& c) const;
	std::string typeName(const Type& type) const;
	std::string typeName(const QualifiedType& type, bool change = true) const;
	std::string paramName(const Param& param, const std::string& namePrefix = "") const;

	void outputAll(const std::string& output);
	void outputAllHeader(const std::string& output);

	void assureGuard(std::ofstream& of, bool& guardVar, const std::string& guard);
	void endGuard(std::ofstream& of, bool guardVar, const std::string& guard);

protected:
	std::ofstream main_;
	std::ofstream functions_;
	std::ofstream enums_;
	std::ofstream structs_;
	std::ofstream fwd_;
};
