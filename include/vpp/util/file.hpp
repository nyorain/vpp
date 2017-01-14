// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/util/stringParam.hpp> // nytl::StringParam

#include <vector> // std::vector
#include <cstdint> // std::uint8_t

namespace vpp {

/// Reads the file at the given filepath and returns a raw buffer with its contents.
/// \param binary Specifies whether the file should be read in binary mode.
/// \throws std::runtime_error If the given file path does not exist or cannot
/// be opened.
std::vector<std::uint8_t> readFile(nytl::StringParam path, bool binary = true);

/// Writes the given buffer into the file at the given path.
/// \param binary Specifies whether the file should be written in binary mode.
/// \throws std::runtime_error If the given file path does not exist or cannot
/// be opened.
void writeFile(nytl::StringParam path, nytl::Span<const std::uint8_t> buffer, bool binary = true);

/// Returns whether the file at the given path exists.
/// Note that this does not check if it can be opened, read or written.
bool fileExists(nytl::StringParam path);

} // namespace vpp
