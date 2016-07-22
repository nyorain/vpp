#pragma once

#include <vpp/fwd.hpp>
#include <vpp/utility/nonCopyable.hpp>

#include <vector>

//Can there any useful abstraction provided to the vulkan display api?
namespace vpp
{

class DisplayContext : public Context
{

};

///Represents a physical display that is connected to an vulkan physical device.
class Display : public NonCopyable
{
public:
	///Returns all supported display modes for this display.
	Range<vk::DisplayModeKHR> modes() const;

	///Returns all supported planes for this display.
	Range<vk::DisplayPlanePropertiesKHR> planes() const;

protected:
	vk::DisplayKHR handle_;
	std::vector<vk::DisplayPropertiesKHR> properties_;
};

///Creates a surface object for a given display, a given plane and a given displayMode.
Surface createSurface(vk::DisplayKHR display, unsigned int plane, vk::DisplayModeKHR displayMode);

DisplayContext createDisplayContext();

}

/*********************************
example for using the display class
===================================

vk::Instance instance = createInstance();
vk::PhysicalDevice phDev = choosePhysicalDevice(instance);

std::vector<vpp::Display> displays = vpp::queryDisplays(phDev);
vpp::Surface surface = vpp::createSurface(displays[0]); //default settings

**********************************/
