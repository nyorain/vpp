#pragma once

#include <vpp/vk.hpp>

namespace vpp
{

class Debugger : public Resource
{
protected:
	std::vector<vk::DebugReportCallbackEXT> debugCallback_ {};

public:
	Debugger(vk::Instance instance);
	~Debugger();
};

};
