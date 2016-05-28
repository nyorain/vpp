#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#include <vpp/vk.hpp>
#include <vpp/surface.hpp>
#include <vpp/context.hpp>

#include <windows.h>

namespace vpp
{

///Surface class for the windows wsi backend.
class Win32Surface : public Surface
{
public:
	Win32Surface() = default;
	Win32Surface(vk::Instance instance, HWND hwnds);
    Win32Surface(vk::Instance instance, HINSTANCE hinstance, HWND hwnd);

protected:
    void initSurface(HINSTANCE hinstance, HWND hwnd);
};

///Context class for the windows wsi backend.
class Win32Context : public Context
{
public:
	Win32Context() = default;
	Win32Context(const CreateInfo& info, HWND hwnds);
	Win32Context(const CreateInfo& info, HINSTANCE hinstance, HWND hwnd);
	virtual ~Win32Context();

	virtual const Surface& surface() const override { return surface_; }

protected:
	Win32Surface surface_;
};

}
