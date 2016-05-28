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

#include <string>
#include <fstream>

class Registry;
class Param;
class Type;
class QualifiedType;
class Enum;
class Constant;
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
	std::string outputFolder = "output";
};

class CCOutputGenerator : public OutputGenerator
{
public:
	CCOutputGenerator(Registry& reg, const CCOutputGeneratorSettings& settings = {});
	void generate();
	void printReqs(Requirements& reqs, const Requirements& fulfilled, const std::string& guard = "");
	void printStruct(const Struct& type);

	std::string enumName(const Enum& e, const std::string& name, bool* bit = nullptr) const;
	std::string constantName(const Constant& c) const;
	std::string typeName(const Type& type) const;
	std::string typeName(const QualifiedType& type) const;
	std::string paramName(const Param& param) const;

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
