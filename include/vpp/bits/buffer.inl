#pragma once

struct DataBuffer
{
	std::uint8_t& data;
	std::size_t size;
};

template<typename T> struct VulkanType { static constexpr auto type = ShaderType::none; };
template<typename T> struct VulkanType<T&> : public VulkanType<T> {};
template<typename T> struct VulkanType<const T> : public VulkanType<T> {};
template<typename T> struct VulkanType<volatile T> : public VulkanType<T> {};
template<typename T> struct VulkanType<const volatile T> : public VulkanType<T> {};
template<> struct VulkanType<float> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<bool> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<std::uint32_t> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<std::int32_t> { static constexpr auto type = ShaderType::scalar; };
template<> struct VulkanType<double> { static constexpr auto type = ShaderType::scalar_64; };
template<> struct VulkanType<DataBuffer> { static constexpr auto type = ShaderType::buffer; };

template<typename T> auto constexpr VulkanTypeV = VulkanType<T>::type;

unsigned int align(ShaderType type);
unsigned int align(const Range<ShaderType> types);

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
		int e1[] = {(sa = std::max(sa, align(VulkanType<decltype(obj.*(std::get<I>(tup)))>::type)), 0)...};
		update.align(sa);
		int e2[] = {(bufferWrite(update, obj.*(std::get<I>(tup))), 0) ...};
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

//foreach tuple
// template<typename T> struct AddTuple;
// template<std::size_t... I> struct AddTuple<std::index_sequence<I...>>
// {
// 	template<typename... T>
// 	void operator()(BufferUpdate& update, const std::tuple<T...>& tup)
// 	{
// 		int e1[] = {(update.add(std::get<I>(tup)), 0)...};
// 	}
// };

} //namespace detail

template<typename T>
void BufferUpdate::addSingle(const T& obj)
{
	// static_assert(sizeof(VulkanType<T>) > 0,
	// 	"Unkown vulkan type! Provide a specialization for vpp::VulkanType or use raw write!");

	detail::bufferWrite(*this, obj);
}

template<typename... T>
void BufferUpdate::add(const T&... objs)
{
	int e1[] = {(addSingle(objs), 0)...};
}


template<typename T, typename>
void BufferUpdate::write(const T& obj, std::size_t count, std::size_t align)
{
	write(&obj, count * sizeof(T), align);
}
