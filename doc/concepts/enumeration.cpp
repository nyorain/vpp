auto layers = vk::enumerateInstanceLayerProperties();
for(auto& l : layers)
{
	std::cout << "Layer " << l.layerName << " " << l.specVerison << " " << l.implementationVersion;
	std::cout << "\n\tDescription: " << l.description << "\n\n";
}

auto extensions = vk::enumerateInstanceExtensionProperties();
for(auto& e : extensions)
{
	std::cout << "Extension " << e.extensionName << " " << e.specVersion << "\n\n";
}
