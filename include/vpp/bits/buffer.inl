#pragma once

namespace detail
{

///Utility template class that can be used to write an object of type T to a buffer update.
template<typename T, ShaderType V = VulkanType<T>::type>
struct BufferWriter;

///Utility function using the different BufferWriter specializations.
template<typename T, ShaderType V = VulkanType<T>::type>
void bufferWrite(BufferUpdate& update, const T& obj)
{
	BufferWriter<T, V>::call(update, obj);
}

///Utility function using the different BufferWriter specializations for the size.
template<typename T, ShaderType V = VulkanType<T>::type>
std::size_t bufferWriteSize(const T& obj, BufferAlign balign, std::size_t offset)
{
	return BufferWriter<T, V>::size(obj, balign, offset);
}

///Utility template class used to write the members of a structure to a buffer update.
template<typename T> struct MembersWriter;

template<std::size_t... I>
struct MembersWriter<std::index_sequence<I...>>
{
	template<typename T>
	static void call(BufferUpdate& update, const T& obj)
	{
		static auto constexpr tup = VulkanType<T>::members;

		auto sa = 0u;
		if(update.std140()) sa = 16u;

		int e1[] = {(sa = std::max(sa,
			align(VulkanType<decltype(obj.*(std::get<I>(tup)))>::type)), 0)...};
		update.align(sa);

		int e2[] = {(bufferWrite(update, obj.*(std::get<I>(tup))), 0) ...};
		update.align(sa);
	}

	template<typename T>
	static std::size_t size(const T& obj, BufferAlign balign, std::size_t offset)
	{
		static auto constexpr tup = VulkanType<T>::members;

		auto sa = 0u;
		if(balign == BufferAlign::std140) sa = 16u;

		int e1[] = {(sa = std::max(sa,
			align(VulkanType<decltype(obj.*(std::get<I>(tup)))>::type)), 0)...};
		offset = align(offset, sa);

		int e2[] = {(offset = bufferWriteSize(obj.*(std::get<I>(tup)), balign, offset), 0) ...};
		offset = align(offset, sa);

		return offset;
	}
};

///Default specialization for all default shader types.
template<typename T, ShaderType V>
struct BufferWriter
{
	static void call(BufferUpdate& update, const T& obj)
	{
		update.align(align(V));
		update.write(&obj, sizeof(obj));
	}

	static std::size_t size(const T& obj, BufferAlign balign, std::size_t size)
	{
		auto ret = align(size, align(V));
		ret += sizeof(obj);
		return ret;
	}
};

///Specialization for raw buffers.
template<typename T>
struct BufferWriter<T, ShaderType::buffer>
{
	template<
		typename = decltype(std::declval<T>().size),
		typename = decltype(std::declval<T>().data)>
	static void call(BufferUpdate& update, const T& obj)
	{
		update.write(&obj.data, obj.size);
	}

	template<
		typename = decltype(std::declval<T>().size),
		typename = decltype(std::declval<T>().data)>
	static std::size_t size(const T& obj, BufferAlign balign, std::size_t size)
	{
		return size + obj.size;
	}
};

///Specialization for matrix types.
template<typename T>
struct BufferWriter<T, ShaderType::matrix>
{
	using VT = std::decay_t<decltype(std::declval<T>()[0][0])>;
	static constexpr auto major = VulkanType<T>::major;
	static constexpr auto minor = VulkanType<T>::minor;
	static constexpr auto transpose = VulkanType<T>::transpose;
	static constexpr auto aligno = align(VulkanType<VT>::type) * (minor > 2 ? 4 : 2);

	static_assert(std::is_same<VT, float>::value || std::is_same<VT, double>::value,
		"vpp::BufferUpdate::add(matrix): Only floating point value types are supported!");
	static_assert(major > 1 && minor > 1 && major < 5 && minor < 5,
		"vpp::BufferUpdate::add(matrix): Invalid matrix dimensions!");

	static void call(BufferUpdate& update, const T& obj)
	{
		auto sa = (update.std140()) ? aligno : std::max(aligno, 16u);

		update.align(sa);
		if(sa == sizeof(VT) && sizeof(obj) == major * minor * sizeof(VT) && !transpose)
		{
			update.write(&obj, sizeof(obj));
		}
		else if(transpose)
		{
			for(auto mj = 0u; mj < major; ++mj)
			{
				update.align(sa);
				for(auto mn = 0u; mn < minor; ++mn) update.write(&obj[mj][mn], sizeof(VT));
			}
		}
		else
		{
			for(auto mn = 0u; mn < minor; ++mn)
			{
				update.align(sa);
				for(auto mj = 0u; mj < major; ++mj) update.write(&obj[mj][mn], sizeof(VT));
			}
		}

		update.align(sa);
	}

	static std::size_t size(const T& obj, BufferAlign balign, std::size_t size)
	{
		auto sa = (balign == BufferAlign::std140) ? aligno : std::max(aligno, 16u);
		auto ret = align(size, sa);

		if(sa == sizeof(VT) && sizeof(obj) == major * minor * sizeof(VT) && !transpose)
		{
			ret += sizeof(T);
		}
		else if(transpose)
		{
			for(auto mj = 0u; mj < major; ++mj)
			{
				ret = align(ret, sa);
				for(auto mn = 0u; mn < minor; ++mn) ret += sizeof(VT);
			}
		}
		else
		{
			for(auto mn = 0u; mn < minor; ++mn)
			{
				ret = align(ret, sa);
				for(auto mj = 0u; mj < major; ++mj) ret += sizeof(VT);
			}
		}

		ret = align(size, sa);
		return ret;
	}
};

///Specialization for structures.
template<typename T>
struct BufferWriter<T, ShaderType::structure>
{
	using V = VulkanType<T>;
	using Seq = std::make_index_sequence<std::tuple_size<decltype(V::members)>::value>;

	static void call(BufferUpdate& update, const T& obj)
	{
		MembersWriter<Seq>::call(update, obj);
	}

	static std::size_t size(const T& obj, BufferAlign balign, std::size_t offset)
	{
		return MembersWriter<Seq>::size(obj, balign, offset);
	}
};

template<typename T>
struct BufferWriter<T, ShaderType::none>
{
	static constexpr auto V = VulkanType<decltype(std::declval<T>().begin())>::type;

	template<
		typename = decltype(std::begin(std::declval<T>())),
		typename = decltype(std::begin(std::declval<T>()))>
	void call(BufferUpdate& update, const T& obj)
	{
		if(update.std140()) update.align(std::max(align(V), 16u));
		else update.align(align(V));
		for(auto& a : obj)
		{
			if(update.std140()) update.align(16);
			bufferWrite(update, a);
		}

		if(update.std140()) update.align(std::max(align(V), 16u));
	}

	template<
		typename = decltype(std::begin(std::declval<T>())),
		typename = decltype(std::begin(std::declval<T>()))>
	static std::size_t size(const T& obj, BufferAlign balign, std::size_t offset)
	{
		const auto is140 = (balign == BufferAlign::std140);

		if(is140) offset = align(offset, std::max(align(V), 16u));
		else offset = align(offset, align(V));
		for(auto& a : obj)
		{
			if(is140) offset = align(offset, 16u);
			offset = bufferWriteSize(obj, balign, offset);
		}

		if(is140) offset = align(offset, std::max(align(V), 16u));
	}

};

} //namespace detail

template<typename T>
void BufferUpdate::addSingle(const T& obj)
{
	detail::bufferWrite(*this, obj);
}

template<typename... T>
void BufferUpdate::add(const T&... objs)
{
	int e1[] = {(addSingle(objs), 0)...};
}

template<typename T>
void BufferSizeCalculator::addSingle(const T& obj)
{
	offset_ = detail::bufferWriteSize(obj, align_, offset_);
}

template<typename... T>
void BufferSizeCalculator::add(const T&... objs)
{
	int e1[] = {(addSingle(objs), 0)...};
}

constexpr unsigned int align(ShaderType type)
{
	switch(type)
	{
		case ShaderType::scalar: return 4;
		case ShaderType::scalar_64:
		case ShaderType::vec2: return 8;
		case ShaderType::vec3:
		case ShaderType::vec4:
		case ShaderType::vec2_64: return 16;
		case ShaderType::vec3_64:
		case ShaderType::vec4_64: return 32;
		default: return 0;
	}
}
