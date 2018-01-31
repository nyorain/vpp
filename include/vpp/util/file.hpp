// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/util/span.hpp> // nytl::Span
#include <vector> // std::vector
#include <cstdint> // std::uint8_t
#include <string_view> // std::string_view

namespace vpp {

/// Reads the file at the given filepath and returns a raw buffer with its contents.
/// \param binary Specifies whether the file should be read in binary mode.
/// \throws std::runtime_error If the given file path does not exist or cannot
/// be opened.
std::vector<std::uint8_t> readFile(std::string_view path, bool binary = true);

/// Writes the given buffer into the file at the given path.
/// \param binary Specifies whether the file should be written in binary mode.
/// \throws std::runtime_error If the given file path does not exist or cannot
/// be opened.
void writeFile(std::string_view path, nytl::Span<const std::uint8_t> buffer, bool binary = true);

} // namespace vpp
