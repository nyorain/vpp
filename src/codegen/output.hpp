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

/// OutputGenerator base class, used for outputting a Registry in a specific language
/// or format. Implements various utility functions.
class OutputGenerator {
public:
	// removes a vk or VK prefix (case does not matter)
	// if the string startes with VK_ also removes the first underscore.
	std::string removeVkPrefix(const std::string& string, bool* found) const;
	bool removeVkPrefix(std::string& string) const;

	// removes the extension or tag suffix from a string.
	// Uses the stored registry to query valid extensions (vendor) suffixes.
	// Will also remove tag suffixes (such as KHR or EXT)
	// The second (in-place) overload returns the found extension/tag.
	std::string removeExtSuffix(const std::string& string, std::string* extension) const;
	std::string removeExtSuffix(std::string& string) const;

	// brings the given string into camelCase.
	// If firstupper is true, also capitalizes the first char.
	// Example: camelCase(TEST_CASE_ABC_de, true) -> TestCaseAbcDe
	std::string camelCase(const std::string& string, bool firstupper = false) const;
	void camelCaseip(std::string& string, bool firstupper = false) const;

	// brings the given string from camel case from into macro caes, i.e. all chars
	// to uppercase and seperates words (previously seperated by a uppercase char)
	// with an underscore.
	// Example: macroCase(camelCaseWordABC) -> CAMEL_CASE_A_B_C
	std::string macroCase(const std::string& string) const;
	void macroCaseip(std::string& string) const;

	// strips the given string to the important information by removing the vk prefix,
	// andy extensions prefix and bringing it to camel case.
	std::string strip(const std::string& string) const;
	void stripip(std::string& string) const;

	// the associated registry
	Registry& registry() const { return *registry_; }

protected:
	OutputGenerator(Registry& reg) : registry_(&reg) {}
	Registry* registry_;
};

struct CCOutputGeneratorSettings {
	std::string outputFolder = ".";
};

struct ParsedParam {
	const Param* param;

	// if parsed param is array it references the count param
	ParsedParam* countPar = nullptr;
	const Param* countMember = nullptr;

	// if parsed param is count
	std::vector<ParsedParam*> dataPars;
	bool memberAsCount = false; // if it is count, whether the count is a struct member

	// whether the param is an out param
	bool out = false;

	// whether the param is optional
	bool optional = false;
	bool optionalWithRet = false; // whehter it is optional if return param is normal param
};

struct ParsedCommand {
	const Command* command = nullptr;
	std::vector<ParsedParam> parsedParams;
	ParsedParam* returnParam = nullptr; // the data part of the return param
};

class CCOutputGenerator : public OutputGenerator {
public:
	CCOutputGenerator(Registry& reg, const CCOutputGeneratorSettings& settings = {});

	// generates the header files from the registry and settings passed in constructor
	void generate();

	// outputs everything of the given Requirements to the matching header files.
	// the guard parameter can be passed for an additional macro guard around the reqs.
	// will skip requirements that are already in the passed fulfilled ones.
	void printReqs(Requirements&, const Requirements& fulfilled, const std::string& guard = "");

	// outputs the given entities to the matching headers files
	void printStruct(const Struct& type);
	void printCmd(const Command& command);
	void printVecCmd(const Command& command, const Param& count, const Param& data);
	void printVecCmd(const ParsedCommand& command, const std::string& name);

	// parses the command, i.e. tries to vectorize and analyze it.
	ParsedCommand parseCommand(const Command& cmd) const;

	// outputs the param declaration for the given parsed parameter.
	// if rangify is true will try to convert it into a span
	// sepr is the seperator to put in front of the parameter
	std::string paramDecl(const ParsedParam& param, bool rangeify, const char* sepr,
		const ParsedParam* retParam) const;
	std::string paramCall(const ParsedParam& param, bool rangeify, const char* sepr,
		const ParsedParam* retParam) const;

	/// returns the names of the given entities for c++
	std::string enumName(const Enum& e, const std::string& name, bool* bit = nullptr) const;
	std::string constantName(const Constant& c) const;
	std::string typeName(const Type& type) const;
	std::string typeName(const QualifiedType& type, bool change = true) const;
	std::string paramName(const Param&, const std::string& namePrefix = "",
		bool plainArray = false) const;

	// outputs to all headers
	// the second function does not output to main, i.e. the central header
	void outputAll(const std::string& output);
	void outputAllHeader(const std::string& output);

	// ensures the the given guard is set in guardVar, otherwise prints it to of and sets it
	void ensureGuard(std::ofstream& of, bool& guardVar, const std::string& guard);
	void endGuard(std::ofstream& of, bool guardVar, const std::string& guard);

protected:
	std::ofstream main_;
	std::ofstream functions_;
	std::ofstream enums_;
	std::ofstream structs_;
	std::ofstream fwd_;
};
