// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/config.hpp> // VPP_API
#include <vpp/util/span.hpp> // nytl::Span
#include <vector> // std::vector
#include <cstdlib> // std::byte
#include <string_view> // std::string_view

namespace vpp {

/// Reads the file at the given filepath and returns a raw buffer with its contents.
/// binary: Specifies whether the file should be read in binary mode.
/// Throws directly from std::istream on error.
VPP_API std::vector<std::byte> readFile(std::string_view path, bool binary = true);

/// Writes the given buffer into the file at the given path.
/// binary: Specifies whether the file should be written in binary mode.
/// Throws directly from std::ostream on error.
VPP_API void writeFile(std::string_view path, nytl::Span<const std::byte> buffer,
	bool binary = true);

} // namespace vpp
