#pragma once

namespace detail
{

///Utility template class that can be used to write an object of type T to a buffer update.
template<typename T, ShaderType V = VulkanType<T>::type>
struct BufferApplier;

///Utility function using the different BufferWriter specializations.
template<typename U, typename T, ShaderType V = VulkanType<T>::type>
void bufferApply(U& update, T&& obj)
{
	BufferApplier<T, V>::call(update, obj);
}

///Utility template class used to write the members of a structure to a buffer update.
template<typename T> struct MembersApplier;

template<std::size_t... I>
struct MembersApplier<std::index_sequence<I...>>
{
	template<typename U, typename T>
	static void call(U& update, T&& obj)
	{
		static auto constexpr tup = VulkanType<T>::members;

		auto sa = 0u;
		if(update.std140()) sa = 16u;

		int e1[] = {(sa = std::max(sa,
			align(VulkanType<decltype(obj.*(std::get<I>(tup)))>::type)), 0)...};
		if(sa) update.align(sa);

		int e2[] = {(bufferApply(update, obj.*(std::get<I>(tup))), 0) ...};
		if(sa) update.align(sa);
	}
};

///Default specialization for all default shader types.
template<typename T, ShaderType V>
struct BufferApplier
{
	template<typename U>
	static void call(U& update, T&& obj)
	{
		if(align(V)) update.align(align(V));
		update.operate(&obj, sizeof(obj));
	}
};

///Specialization for raw buffers.
template<typename T>
struct BufferApplier<T, ShaderType::buffer>
{
	template<
		typename U,
		typename = decltype(std::declval<T>().size),
		typename = decltype(std::declval<T>().data)>
	static void call(U& update, T&& obj)
	{
		update.operate(&obj.data, obj.size);
	}
};

///Specialization for matrix types.
template<typename T>
struct BufferApplier<T, ShaderType::matrix>
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

	template<typename U>
	static void call(U& update, T&& obj)
	{
		auto maligno = aligno; //needed for std::max since it takes a reference (otherwise link err)
		auto sa = (update.std140()) ? maligno : std::max(maligno, 16u);

		if(sa) update.align(sa);
		if(sa == sizeof(VT) && sizeof(obj) == major * minor * sizeof(VT) && !transpose)
		{
			update.operate(&obj, sizeof(obj));
		}
		else if(transpose)
		{
			for(auto mj = 0u; mj < major; ++mj)
			{
				if(sa) update.align(sa);
				for(auto mn = 0u; mn < minor; ++mn) update.operate(&obj[mj][mn], sizeof(VT));
			}
		}
		else
		{
			for(auto mn = 0u; mn < minor; ++mn)
			{
				if(sa) update.align(sa);
				for(auto mj = 0u; mj < major; ++mj) update.operate(&obj[mj][mn], sizeof(VT));
			}
		}

		if(sa) update.align(sa);
	}
};

///Specialization for structures.
template<typename T>
struct BufferApplier<T, ShaderType::structure>
{
	using V = VulkanType<T>;
	using Seq = std::make_index_sequence<std::tuple_size<decltype(V::members)>::value>;

	template<typename U>
	static void call(U& update, T&& obj)
	{
		MembersApplier<Seq>::call(update, obj);
	}
};

template<typename T>
struct BufferApplier<T, ShaderType::none>
{
	static constexpr auto V = VulkanType<decltype(std::declval<T>().begin())>::type;

	template<
		typename U,
		typename = decltype(std::begin(std::declval<T>())),
		typename = decltype(std::begin(std::declval<T>()))>
	static void call(U& update, T&& obj)
	{
		if(update.std140()) update.align(std::max(align(V), 16u));
		else if(align(V)) update.align(align(V));
		for(auto& a : obj)
		{
			if(update.std140()) update.align(16);
			bufferApply(update, a);
		}

		if(update.std140()) update.align(std::max(align(V), 16u));
	}
};

} //namespace detail

template<typename B> template<typename T>
void BufferOperator<B>::addSingle(T&& obj)
{
	auto& bself = *static_cast<B*>(this);
	detail::bufferApply(bself, obj);
}

template<typename B> template<typename... T>
void BufferOperator<B>::add(T&&... objs)
{
	int e1[] = {(addSingle(objs), 0)...};
}

template<typename... T> WorkPtr read(const Buffer& buf, BufferAlign align, T&... args)
{
	struct WorkImpl : public Work<void>
	{
		WorkImpl(const Buffer& buf, BufferAlign align, T&... args)
			: buffer_(buf), retrieveWork_(retrieve(buf)), align_(align), args_(args...) {}
		~WorkImpl() { finish(); }

		void submit() override { retrieveWork_->submit(); }
		void wait() override { retrieveWork_->wait(); }
		State state() override
		{
			if(!retrieveWork_) return WorkBase::State::finished;
			auto state = retrieveWork_->state();
			return (state == WorkBase::State::finished) ? WorkBase::State::executed : state;
		}
		void finish() override
		{
			if(!retrieveWork_) return;

			retrieveWork_->finish();
			BufferReader reader(buffer_.device(), align_, {&retrieveWork_->data(), buffer_.size()});
			nytl::apply([&](auto&... args){ reader.add(args...); }, args_); //std::apply c++17
			retrieveWork_.reset();
		}

		const Buffer& buffer_;
		DataWorkPtr retrieveWork_;
		BufferAlign align_;
		std::tuple<T&...> args_;
	};

	return std::make_unique<WorkImpl>(buf, align, args...);
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
