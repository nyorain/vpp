#include "codegen.hpp"
#include <iostream>
#include <locale>

int main()
{
	RegistryLoader loader("./vk.xml");

	std::cout << "loaded ... \n";
	auto& registry = loader.parse();

	std::cout << "parsed ... \n";
	CCOutputGenerator generator(registry, "vulkan.hpp", "fwd.hpp");
	generator.generate();

	std::cout << "generated ... \n";
}
