#pragma once

///The RawBufferData is an explicit wrapper around custom aligned data to fill a buffer
///with.
struct RawBufferData
{
public:
	const std::uint8_t& data_;
	std::size_t size_;

public:
	template<typename T> constexpr RawBufferData(const T& obj, std::size_t count = 1)
		: data_(reinterpret_cast<const std::uint8_t&>(obj)), size_(sizeof(T) * count) {}

	const std::uint8_t& data() const { return data_; }
	std::size_t size() const { return size_; }
};

///\{
///Can be used to easily construct raw buffer wrappers around data to fill a buffer
template<typename T,
	typename = std::enable_if_t<!std::is_pointer<T>::value>,
	typename = std::enable_if_t<std::is_pod<T>::value>>
constexpr RawBufferData raw(const T& obj, std::size_t count = 1) { return {obj, count}; }

template<typename C,
	typename = decltype(std::declval<C>().data()),
	typename = decltype(std::declval<C>().size())>
constexpr RawBufferData raw(const C& container) { return {*container.data(), container.size()}; }
///\}

///The VulkanType template can be specialized for custom types to make vpp::BufferUpdate
///able to align them correctly.
template<typename T> struct VulkanType { static constexpr auto type = ShaderType::none; };
template<typename T> struct VulkanType<T&> : public VulkanType<T> {};
template<typename T> struct VulkanType<const T> : public VulkanType<T> {};
template<typename T> struct VulkanType<volatile T> : public VulkanType<T> {};

///Some VulkanType utility implementations that can be derived from when specializing for
///custom types.
///\{
///Major and minor always specify the size of the host matrix.
///If transpose is set to true the device matrix is transposed (size and components).
///Note that glsl (and spirv) lets you specify the layout of matrix in memory.
///Note that glsl matrix access is always column-major, only the memory layout can be changed.
///
///E.g. host matrix 4x3 row major transpose=true; device matrix 3x4 column major.
///host matrix 2x3 row major transpose=false, device matrix 2x3 row major.
///host matrix 3x4 column major transpose=false, device matrix 3x4 column major.
template<std::size_t MJ, std::size_t MN, bool TRP = false, bool S64 = false> struct VulkanTypeMat
{
	static constexpr auto type = ShaderType::mat;
	static constexpr auto major = MJ; //(transpose ? col : row) count of host mat
	static constexpr auto minor = MN; //(transpose ? row : col) count of host mat
	static constexpr auto transpose = TRP; //defines whether to transpose mat on device
	static constexpr auto size64 = S64; //double precision matrix componenets?
};

///Default vector base
template<std::size_t N, bool S64 = false> struct VulkanTypeVec
{
	static constexpr auto type = ShaderType::vec;
	static constexpr auto size64 = S64; //double precision vector compoenents?
	static constexpr auto dimension = N; //size of the vector
};

///Default scalar base
template<bool S64 = false>
struct VulkanTypeScalar
{
	static constexpr auto type = ShaderType::scalar;
	static constexpr auto size64 = S64;
};

using VulkanTypeScalar32 = VulkanTypeScalar<false>;
using VulkanTypeScalar64 = VulkanTypeScalar<true>;

///Default struct base. One must declare the "members" member as a tuple of pointers to the
///structs members in the order they should be transferred to the device.
template<bool Align>
struct VulkanTypeStruct
{
	static constexpr auto type = ShaderType::structure;
	static constexpr auto align = Align;
	//static constexpr auto members = std::make_tuple(members...); //must be custom specified
};

///Default raw buffer base.
struct VulkanTypeBuffer
{
	static constexpr auto type = ShaderType::buffer;
};
///\}

///\{
///Some VulkanType specializations for default types.
template<> struct VulkanType<float> : public VulkanTypeScalar32 {};
template<> struct VulkanType<bool> : public VulkanTypeScalar32 {};
template<> struct VulkanType<std::uint32_t> : public VulkanTypeScalar32 {};
template<> struct VulkanType<std::int32_t> : public VulkanTypeScalar32 {};
template<> struct VulkanType<double> : public VulkanTypeScalar64 {};
template<> struct VulkanType<RawBufferData> : public VulkanTypeBuffer {};
///\}
