// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/descriptor.hpp>

namespace vpp {

/// Tracked DescriptorSetLayout, knows it layout.
/// NOTE: moving/destructing a TrDsLayout while it is referenced
/// by a TrDs results in undefined behavior.
/// TODO: probably best to make this not movable
///   instead offer a 'init' function.
class TrDsLayout : public DescriptorSetLayout {
public:
	TrDsLayout() = default;
	TrDsLayout(const Device&, const vk::DescriptorSetLayoutCreateInfo&);
	TrDsLayout(const Device&, nytl::Span<vk::DescriptorSetLayoutBinding>);
	TrDsLayout(const Device&,
		std::initializer_list<vk::DescriptorSetLayoutBinding>);

	const auto& bindings() const { return bindings_; }

protected:
	void init(nytl::Span<const vk::DescriptorSetLayoutBinding>);
	std::vector<vk::DescriptorPoolSize> bindings_;
};

/// Tracked DescriptorPool, knows how many resources it has left.
/// Always created with the freeDescriptorSet flag since otherwise it makes
/// no sense tracking the resources.
/// Not movable since it is referenced by TrDs objects.
class TrDsPool : public DescriptorPool {
public:
	TrDsPool() = default;
	TrDsPool(const Device&, vk::DescriptorPoolCreateInfo);
	TrDsPool(const Device&, unsigned maxSets,
		nytl::Span<const vk::DescriptorPoolSize> sizes);

	TrDsPool(TrDsPool&&) = delete;
	TrDsPool& operator=(TrDsPool&&) = delete;

	const auto& remaining() const { return remaining_; }
	const auto& remainingSets() const { return remainingSets_; }

protected:
	friend class TrDs;
	std::vector<vk::DescriptorPoolSize> remaining_; // types remaining
	unsigned int remainingSets_ {}; // how many sets to allocate are left
};

/// Tracked DescriptorSet, knows its associated pool and layout.
/// Will allocate/free the needed descriptors from the tracked pool it is
/// created with.
class TrDs : public DescriptorSet {
public:
	struct InitData {
		InitData() = default;
		InitData(InitData&&) noexcept;
		InitData& operator=(InitData&&) noexcept;
		~InitData();

		const TrDsLayout* layout {}; // required for canceln a reservation
		DescriptorAllocator* allocator {};
		size_t reservation {}; // number of pools in alloc at reserve time + 1
	};

public:
	TrDs() = default;

	/// Directly allocates a descriptor set with the given layout from
	/// the given pool.
	TrDs(TrDsPool&, const TrDsLayout&);

	/// Allocates a descriptor set with given alyout from the given allocator.
	TrDs(DescriptorAllocator&, const TrDsLayout&);

	/// Reserves the resources needed to allocate a descriptor set with the
	/// given layout from the given allocator.
	/// Only a call later to init() will really create the descriptor set.
	TrDs(InitData&, DescriptorAllocator&, const TrDsLayout&);
	~TrDs();

	TrDs(TrDs&& rhs) noexcept { swap(*this, rhs); }
	TrDs& operator=(TrDs rhs) noexcept  {
		swap(*this, rhs);
		return *this;
	}

	/// When this object was constructed with a deferred constructor,
	/// this will finish object initialization.
	/// Otherwise undefined behaviour.
	void init(InitData&);

	TrDsPool& pool() const { return *pool_; }
	const TrDsLayout& layout() const { return *layout_; }

	friend void swap(TrDs& a, TrDs& b) noexcept;

protected:
	TrDsPool* pool_ {};
	const TrDsLayout* layout_ {};
};

/// Dynamically allocates tracked descriptors.
/// Will create descriptor pools on the fly but try to create as few
/// as possible. Can't be moved since TrDs objects might reference it.
class DescriptorAllocator : public vpp::Resource {
public:
	DescriptorAllocator(const vpp::Device&);
	~DescriptorAllocator() = default;

	DescriptorAllocator(DescriptorAllocator&&) = delete;
	DescriptorAllocator& operator=(DescriptorAllocator&&) = delete;

	void reserve(nytl::Span<const vk::DescriptorPoolSize>, unsigned count = 1);
	void reserve(const TrDsLayout&, unsigned count = 1);
	void unreserve(const TrDsLayout&);
	TrDs alloc(const TrDsLayout&);

	const auto& pools() const { return pools_; }
	const auto& pending() const { return pending_; }

private:
	struct {
		std::vector<vk::DescriptorPoolSize> types;
		unsigned int count {};
	} pending_;

	// deque to not get reference invalidation on push_back
	std::deque<TrDsPool> pools_;
};

} // namespace vpp
