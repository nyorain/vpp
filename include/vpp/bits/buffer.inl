#pragma once

namespace detail
{

template<typename T, ShaderType V>
struct BufferWriter;

template<typename T, ShaderType V = VulkanType<T>::type>
void bufferWrite(BufferUpdate& update, const T& obj)
{
	BufferWriter<T, V> writer;
	writer(update, obj);
}

template<typename T> struct PrintMembers;

template<std::size_t... I>
struct PrintMembers<std::index_sequence<I...>>
{
	template<typename T>
	void operator()(BufferUpdate& update, const T& obj)
	{
		auto constexpr tup = VulkanType<T>::members;

		auto sa = 0u;
		if(update.std140()) sa = 16u;
		int e1[] = {(sa = std::max(sa,
			align(VulkanType<decltype(obj.*(std::get<I>(tup)))>::type)), 0)...};
		update.align(sa);
		int e2[] = {(bufferWrite(update, obj.*(std::get<I>(tup))), 0) ...};
		update.align(sa);
	}
};

template<typename T, ShaderType V>
struct BufferWriter
{
	void operator()(BufferUpdate& update, const T& obj)
	{
		update.align(align(V));
		update.write(&obj, sizeof(obj));
	}
};

template<typename T>
struct BufferWriter<T, ShaderType::buffer>
{
	template<typename = decltype(std::declval<T>().size), typename = decltype(std::declval<T>().data)>
	void operator()(BufferUpdate& update, const T& obj)
	{
		update.write(&obj.data, obj.size);
	}
};

template<typename T>
struct BufferWriter<T, ShaderType::matrix>
{
	void operator()(BufferUpdate& update, const T& obj)
	{
		using VT = std::decay_t<decltype(obj[0][0])>;

		static constexpr auto major = VulkanType<T>::major;
		static constexpr auto minor = VulkanType<T>::minor;
		static constexpr auto transpose = VulkanType<T>::transpose;
		static constexpr auto aligno = align(VulkanType<VT>::type) * (minor > 2 ? 4 : 2);

		auto sa = (update.std140()) ? aligno : std::max(aligno, 16u);

		static_assert(std::is_same<VT, float>::value || std::is_same<VT, double>::value,
			"vpp::BufferUpdate::add(matrix): Only floating point value types are supported!");
		static_assert(major > 1 && minor > 1 && major < 5 && minor < 5,
			"vpp::BufferUpdate::add(matrix): Invalid matrix dimensions!");

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
				for(auto mn = 0u; mn < minor; ++mn)
				{
					update.write(&obj[mj][mn], sizeof(VT));
				}
			}
		}
		else
		{
			for(auto mn = 0u; mn < minor; ++mn)
			{
				update.align(sa);
				for(auto mj = 0u; mj < major; ++mj)
				{
					update.write(&obj[mj][mn], sizeof(VT));
				}
			}
		}

		update.align(sa);
	}
};

template<typename T>
struct BufferWriter<T, ShaderType::structure>
{
	void operator()(BufferUpdate& update, const T& obj)
	{
		using V = VulkanType<T>;
		using Seq = std::make_index_sequence<std::tuple_size<decltype(V::members)>::value>;

		PrintMembers<Seq> printer;
		printer(update, obj);
	}
};

template<typename T>
struct BufferWriter<T, ShaderType::none>
{
	template<typename = decltype(std::begin(std::declval<T>())), typename = decltype(std::begin(std::declval<T>()))>
	void operator()(BufferUpdate& update, const T& obj)
	{
		static constexpr auto V = VulkanType<decltype(*obj.begin())>::type;

		if(update.std140()) update.align(std::max(align(V), 16u));
		else update.align(align(V));
		for(auto& a : obj)
		{
			if(update.std140()) update.align(16);
			bufferWrite(update, a);
		}

		if(update.std140()) update.align(std::max(align(V), 16u));
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
