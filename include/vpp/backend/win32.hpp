// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#include <vpp/fwd.hpp>
#include <vpp/surface.hpp>
#include <vpp/context.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// We undef the shittiest win32 macros here since it is our reponsibiility. We have to pull
// in this big pile of shit for correct HWND, HINSTANCE definitions.
// TODO: Can they be safely (platform, version independent) forward-declared?
#undef near
#undef far
#undef max
#undef min

namespace vpp {

///Creates a win32 surface for the given vulkan instance, win32 window and module [optional].
///Does require the needed extensions to be enabled for the given instance.
Surface createSurface(vk::Instance instance, HWND window, HINSTANCE module = nullptr);

///Creates a win32 context for the given window.
Context createContext(HWND window, Context::CreateInfo info, HINSTANCE module = nullptr);
Context createContext(HWND window, HINSTANCE module = nullptr);

} // namespace vpp
