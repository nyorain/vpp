// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Defines binary operators for enums as well as the nytl::Flags class.

#pragma once

#ifndef NYTL_INCLUDE_FLAGS
#define NYTL_INCLUDE_FLAGS

#include <type_traits> // std::underlaying_type_t

namespace nytl {

#ifndef NYTL_INCLUDE_FWD_FLAGS
#define NYTL_INCLUDE_FWD_FLAGS
	template<typename T, typename U = std::underlying_type_t<T>> class Flags;
#endif //header guard

/// \brief Can be used to invert the given value on Flags construction
/// Can be used like this: `nytl::Flags<Enum>(nytl::invertFlags, Enum::value)`.
/// \module utility
struct InvertFlags {};
constexpr InvertFlags invertFlags {};

/// \brief Can be used to combine multiple values from the same enumeration.
/// \details Use the [NYTL_FLAG_OPS]() macro to define binary operations on the
/// enumeration that result in a nytl::Flags object for it.
/// \requires Each value in the enumerations should have exactly one bit set and
/// all values should have different bits set so they can be combined.
/// \tparam T The enum type from which values should be combined.
/// \tparam U The raw type to store the values in. By default the underlying type of
/// the enum as reported by std::underlying_type<T>
/// \module utility
template<typename T, typename U>
class Flags {
public:
	constexpr Flags() noexcept = default;
	constexpr Flags(T bit) noexcept : value_(static_cast<U>(bit)) {}
	constexpr Flags(InvertFlags, T bit) noexcept : value_(~static_cast<U>(bit)) {}
	~Flags() noexcept = default;

	constexpr Flags& operator=(const Flags& r) noexcept { value_ = r.value(); return *this; }
	constexpr Flags& operator|=(const Flags& r) noexcept { value_ |= r.value(); return *this; }
	constexpr Flags& operator&=(const Flags& r) noexcept { value_ &= r.value_; return *this; }
	constexpr Flags& operator^=(const Flags& r) noexcept { value_ ^= r.value(); return *this; }

	constexpr Flags operator|(const Flags& r) const noexcept { return Flags(r) |= *this; }
	constexpr Flags operator&(const Flags& r) const noexcept { return Flags(r) &= *this; }
	constexpr Flags operator^(const Flags& r) const noexcept { return Flags(r) ^= *this; }

	constexpr bool operator==(const Flags& rhs) const noexcept { return value_ == rhs.value(); }
	constexpr bool operator!=(const Flags& rhs) const noexcept { return value_ != rhs.value(); }

	constexpr const U& value() const noexcept { return value_; }
	constexpr operator U() const noexcept { return value_; }

protected:
	U value_ {};
};

// - binary flags operators -
template<typename T> constexpr
Flags<T> operator|(T bit, const Flags<T>& flags) noexcept
	{ return flags | bit; }

template<typename T> constexpr
Flags<T> operator&(T bit, const Flags<T>& flags) noexcept
	{ return flags & bit; }

template<typename T> constexpr
Flags<T> operator^(T bit, const Flags<T>& flags) noexcept
	{ return flags ^ bit; }

} // namespace nytl

/// \brief Can be used for an enum to generate binary operations resulting in nytl::Flags.
/// Can be used like this: `enum class Enum {}; NYTL_FLAG_OPS(Enum)` which will
/// make results like `Enum::value1 | Enum::value2` automatically result in a
/// `nytl::Flags<Enum>` object holding the union of the given values.
/// \note Inversion of flags or enum values will actually the underlaying value.
/// Therefore equal comparisions with flags can be error prone and one should prefer to
/// just check whether flags contain a specific value. The follwing static_assertion will fail:
/// ```cpp
/// enum class Enum { value1 = 1, value2 = 2 };
/// NYTL_FLAG_OPS(Enum)
/// static_assert(~Enum::value1 == Enum::value2, "will fail due to nytl::Flags");
/// ```
/// \module utility
#define NYTL_FLAG_OPS(T) \
	constexpr nytl::Flags<T> operator|(T a, T b) noexcept { return nytl::Flags<T>(a) | b; } \
	constexpr nytl::Flags<T> operator&(T a, T b) noexcept { return nytl::Flags<T>(a) & b; } \
	constexpr nytl::Flags<T> operator^(T a, T b) noexcept { return nytl::Flags<T>(a) ^ b; } \
	constexpr nytl::Flags<T> operator~(T bit) noexcept { return {nytl::invertFlags, bit}; }

#endif // header guard
