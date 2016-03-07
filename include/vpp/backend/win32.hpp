#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#include <vpp/vk.hpp>
#include <vpp/surface.hpp>
#include <vpp/context.hpp>

#include <windows.h>

namespace vpp
{

//Surface
class Win32Surface : public Surface
{
protected:
    void initSurface(HINSTANCE hinstance, HWND hwnd);

public:
	Win32Surface() = default;
	Win32Surface(vk::Instance instance, HWND hwnds);
    Win32Surface(vk::Instance instance, HINSTANCE hinstance, HWND hwnd);
};

//Context
class Win32Context : public Context
{
protected:
	Win32Surface surface_;

public:
	Win32Context() = default;
	Win32Context(const CreateInfo& info, HWND hwnds);
	Win32Context(const CreateInfo& info, HINSTANCE hinstance, HWND hwnd);

	virtual const Surface& surface() const override { return surface_; }
};

}
