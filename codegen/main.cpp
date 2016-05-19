#include "codegen.hpp"

int main()
{
	RegistryLoader loader("vk.xml");
	auto registry = loader.parse();

	CCOutputGenerator generator(registry, "vulkan.hpp", "fwd.hpp");
	generator.generate();
}
