// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include "registry.hpp"
#include "output.hpp"

#include <iostream>
#include <locale>

int main(int argc, const char** argv)
{
	if(argc < 2) return EXIT_FAILURE;
	RegistryLoader loader(argv[1]);

	std::cout << "loaded ... \n";
	auto& registry = loader.parse();

	std::cout << "parsed ... \n";
	CCOutputGenerator generator(registry);
	generator.generate();

	std::cout << "generated ... \n";
	return EXIT_SUCCESS;
}
