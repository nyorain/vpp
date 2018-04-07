// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

namespace detail {

/// Rounds up the given align to a multiple of 16 if the layout is std140
/// Otherwise just returns the given align value.
constexpr unsigned int roundAlign(unsigned int align, bool std140) {
	return std140 ? vpp::align(align, 16) : align;
}

/// Utility to get the member type of a pointer to member object (Used with decltype).
template<typename T> struct TypeDummy { using type = T; };
template<typename T, typename M> constexpr TypeDummy<M> memPtr(M T::*);

/// Utility template class that can be used to write an object of type T to a buffer update.
/// Implementations of this class for different ShaderTypes must implement the following
/// member functions:
///  - template<typename T, typename O> static void call(O& op, T& obj);
///  - template<typename T> static unsigned int align(bool std140); constexpr
///  - template<typename O> static void size(O& op); [optional; if possible] constexpr
/// The first function is used by BufferUpdate/BufferReader to read/write the actual data
/// while the second function is used by BufferSizer to determine the size that would be needed
/// to fit the given data on the buffer.
template<typename VT, ShaderType V = VT::type>
struct BufferApplier;

/// - Utility expressions to check -
/// Expression that checks if T has a static size(O&) function.
template<typename T, typename O> using HasSizeFunction =
	decltype(T::size(std::declval<O&>()));

/// - Utility shortcut functions -
/// Utility function using the different BufferApplier specializations.
/// Prefer to use this function instead of directly calling the BufferApplier Structure.
template<typename O, typename T, typename VT = VulkanType<T>>
void bufferApply(O& op, T&& obj) { BufferApplier<VT>::call(op, obj); }

/// Utillity function to using the different BufferApplier::size implementations.
template<typename VT, typename O>
constexpr void bufferSize(O& op) {
	static_assert(nytl::validExpression<HasSizeFunction, BufferApplier<VT>, O>,
		"Type is not static sized");
	BufferApplier<VT>::size(op);
}

/// Utility function to use the different BufferApplier::align implementations.
template<typename VT, typename T = void> constexpr
unsigned int bufferAlign(bool std140) { return BufferApplier<VT>::template align<T>(std140); }

/// Utility template class used to write the members of a structure to a buffer update.
/// \tparam VT VulkanType of the structure
template<typename VT, typename Seq = std::make_index_sequence<
	std::tuple_size<decltype(VT::members)>::value>> struct MembersOp;

/// Implements the BufferOperator dispatching for members of a structure type.
template<typename VT, std::size_t... I>
struct MembersOp<VT, std::index_sequence<I...>> {
	static constexpr decltype(VT::members) members = VT::members;

	template<typename O, typename T>
	static void call(O& op, T&& obj) {
		auto sa = VT::align ? align(op.std140()) : 0u;
		if(sa) op.align(sa);
		(bufferApply(op, obj.*(std::get<I>(members))), ...);
		if(sa) op.align(sa);
	}

	// applySize - utility for size
	template<typename MP, typename O>
	constexpr static void applySize(const MP& memptr, O& op) {
		bufferSize<VulkanType<typename decltype(memPtr(memptr))::type>>(op);
	}

	// applyAlign - utility for align
	template<typename MP>
	constexpr static unsigned int applyAlign(const MP& memptr, bool std140) {
		using V = typename decltype(memPtr(memptr))::type;
		return bufferAlign<VulkanType<V>, V>(std140);
	}

	// size
	template<typename O>
	constexpr static void size(O& op) {
		auto sa = VT::align ? align(op.std140()) : 0u;
		if(sa) op.align(sa);
		(applySize(std::get<I>(members), op), ...);
		if(sa) op.nextOffsetAlign(sa);
	}

	// align
	constexpr static unsigned int align(bool std140) {
		auto sa = 0u;
		for(auto elem : {(applyAlign(std::get<I>(members), std140), ...)})
			sa = std::max(sa, elem);
		return roundAlign(sa, std140);
	}
};

/// Definitions to avoid undefined reference
template<typename VT, std::size_t... I> constexpr
decltype(VT::members) MembersOp<VT, std::index_sequence<I...>>::members;

/// BufferApplier specialization for scalar types
template<typename VT>
struct BufferApplier<VT, ShaderType::scalar> {
	template<typename T, typename O>
	static void call(O& op, T&& obj) {
		op.align(align<void>(op.std140()));
		op.operate(&obj, sizeof(obj));
	}

	template<typename O>
	constexpr static void size(O& op) {
		op.align(align<void>(op.std140()));
		op.operate(nullptr, 4 + VT::size64 * 4);
	}

	template<typename T>
	static constexpr unsigned int align(bool) {
		return VT::size64 ? 8 : 4;
	}
};

/// BufferApplier specialization for vector types.
template<typename VT>
struct BufferApplier<VT, ShaderType::vec> {
	template<typename T, typename O>
	static void call(O& op, T&& obj) {
		op.align(align<void>(op.std140()));
		op.operate(&obj, sizeof(obj));
	}

	template<typename O>
	constexpr static void size(O& op) {
		op.align(align<void>(op.std140()));
		op.operate(nullptr, VT::dimension * (4 + VT::size64 * 4));
	}

	template<typename T>
	static constexpr unsigned int align(bool) {
		auto ret = ((VT::dimension) == 3 ? 4 : VT::dimension) * 4;
		if(VT::size64) ret *= 2;
		return ret;
	}
};

/// BufferApplier Specialization for custom shader types.
/// Simply forwards the calls to the functions specified in the custom
/// VulkanType.
template<typename VT>
struct BufferApplier<VT, ShaderType::custom> {
	using Impl = typename VT::impl;

	template<typename O, typename T>
	static void call(O& op, T&& obj) {
		Impl::call(op, obj);
	}

	template<typename O>
	static constexpr void size(O& op) {
		Impl::size(op);
	}

	template<typename T>
	static constexpr auto align(bool std140) {
		return Impl::template align<T>(std140);
	}
};

/// Specialization for raw buffers.
/// Buffers are generally dynamic sized and therefore this specialization does
/// not implement the size function.
/// Note that this simply implements 0 as alignment requirement since alignment
/// must also be manually managed for the buffer type.
template<typename VT>
struct BufferApplier<VT, ShaderType::buffer> {
	template<typename O, typename T,
		typename = decltype(std::declval<T>().size()),
		typename = decltype(std::declval<T>().data())>
	static void call(O& op, T&& obj) {
		if(obj.size() > 0) {
			op.operate(obj.data(), obj.size());
		}
	}

	template<typename T>
	static constexpr unsigned int align(bool) {
		return 0u;
	}
};

/// BufferApplier Specialization for matrix types.
/// Probably the most complex specialization in regard of specified rules.
template<typename VT>
struct BufferApplier<VT, ShaderType::mat> {
	static constexpr auto major = VT::major;
	static constexpr auto minor = VT::minor;
	static constexpr auto transpose = VT::transpose;
	static constexpr auto csize = 4 + VT::size64 * 4;

	/// Minor value for alignment-related uses.
	static constexpr auto minorAlign = minor == 3 ? 4 : minor;

	/// If this static assert fails, the VulkanType specialization for your type specified
	/// invalid dimensions. Glsl (and this implementation) does only allow
	/// matrices with dimensions 2x2 to 4x4
	static_assert(major > 1 && minor > 1 && major < 5 && minor < 5,
		"Invalid matrix dimensions!");

	template<typename O, typename T>
	static void call(O& op, T&& obj) {
		using Value = std::remove_cv_t<std::remove_reference_t<decltype(obj[0][0])>>;
		static_assert(
			std::is_same<Value, float>::value ||
			std::is_same<Value, double>::value,
			"Only floating point matrix types are supported!");

		auto sa = roundAlign(minorAlign * csize, op.std140());
		op.align(sa);

		// TODO: raw write sometimes possible?
		// current if condition probably not correct for std140 and mat2f (stride!)
		// if(minor != 3 && sizeof(obj) == major * minor * csize && !transpose) {
		// 	op.operate(&obj, sizeof(obj));
		// }

		if(!transpose) {
			for(auto mj = 0u; mj < major; ++mj) {
				op.align(sa);
				for(auto mn = 0u; mn < minor; ++mn)
					op.operate(&obj[mj][mn], csize);
			}
		} else {
			for(auto mn = 0u; mn < minor; ++mn) {
				op.align(sa);
				for(auto mj = 0u; mj < major; ++mj)
					op.operate(&obj[mj][mn], csize);
			}
		}

		// the member following is rounded up to the base alignment
		op.nextOffsetAlign(sa);
	}

	template<typename O>
	static constexpr void size(O& op) {
		auto sa = roundAlign(minorAlign * csize, op.std140());
		op.align(sa);
		op.operate(nullptr, major * sa);
		op.nextOffsetAlign(sa);
	}

	template<typename T>
	constexpr static unsigned int align(bool std140) {
		return roundAlign(minorAlign * csize, std140);
	}
};

/// BufferApplier specialization for structures.
/// Uses the MemrsOp specialization to simply forward operations to all
/// members of the structure.
template<typename VT>
struct BufferApplier<VT, ShaderType::structure> {
	template<typename O, typename T>
	static void call(O& op, T&& obj) {
		MembersOp<VT>::call(op, obj);
	}

	template<typename O>
	static constexpr void size(O& op) {
		MembersOp<VT>::size(op);
	}

	template<typename T>
	static constexpr unsigned int align(bool std140) {
		return MembersOp<VT>::align(std140);
	}
};

/// BufferApplier container/general specialization.
/// Will try to iterate over a given object and simply apply
/// all iterated values to the buffer, aligning it as array.
template<typename VT>
struct BufferApplier<VT, ShaderType::none> {

	template<
		typename O, typename T,
		typename B = decltype(*std::begin(std::declval<T>())),
		typename E = decltype(*std::end(std::declval<T>())),
		ShaderType V = VulkanType<B>::type>
	static void call(O& op, T&& obj) {
		auto sa = bufferAlign<VulkanType<B>, B>(op.std140());
		auto rounded = roundAlign(sa, op.std140());
		op.align(rounded);

		for(auto& a : obj) {
			if(op.std140()) op.align(rounded);
			bufferApply(op, a);
		}

		op.nextOffsetAlign(rounded);
	}

	template<typename T,
		typename B = decltype(*std::begin(std::declval<T>())),
		typename E = decltype(*std::end(std::declval<T>()))>
	static constexpr unsigned int align(bool std140) {
		return roundAlign(bufferAlign<VulkanType<B>, B>(std140), std140);
	}
};

} // namespace detail

template<typename B> template<typename T>
void BufferOperator<B>::addSingle(T&& obj) {
	auto& bself = *static_cast<B*>(this);
	detail::bufferApply(bself, obj);
}

template<typename B> template<typename... T>
void BufferOperator<B>::add(T&&... objs) {
	(addSingle(objs), ...);
}

template<typename... T>
constexpr void BufferSizer::add() {
	(detail::bufferSize<VulkanType<T>>(*this), ...);
}

constexpr void BufferSizer::operate(const void*, std::size_t size) {
	offset_ = std::max(nextOffset_, offset_) + size;
}
