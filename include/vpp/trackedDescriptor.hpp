// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/descriptor.hpp>

namespace vpp {

/// Tracked DescriptorSetLayout, knows it layout.
class TrDsLayout : public DescriptorSetLayout, public nytl::NonMovable {
public:
	TrDsLayout() = default;
	TrDsLayout(const Device&, nytl::Span<const vk::DescriptorSetLayoutBinding>);

	const auto& bindings() const { return bindings_; }

protected:
	std::vector<vk::DescriptorPoolSize> bindings_;
};

/// Tracked DescriptorPool, knows how many resources it has left.
/// Always created with the freeDescriptorSet flag since otherwise it makes
/// no sense tracking the resources.
class TrDsPool : public DescriptorPool, public nytl::NonMovable {
public:
	TrDsPool() = default;
	TrDsPool(const Device&, vk::DescriptorPoolCreateInfo);
	TrDsPool(const Device&, unsigned maxSets,
		nytl::Span<const vk::DescriptorPoolSize> sizes);

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
	TrDs() = default;
	TrDs(TrDsPool&, const TrDsLayout&);
	TrDs(DeferTag, DescriptorAllocator&, const TrDsLayout&);
	~TrDs();

	TrDs(TrDs&& rhs) noexcept { swap(*this, rhs); }
	TrDs& operator=(TrDs rhs) noexcept  {
		swap(*this, rhs);
		return *this;
	}

	/// Only needed to call when this was constructed deferred.
	/// Will then actually allocate the descriptor set.
	/// Otherwise has no effect.
	void init();

	TrDsPool& pool() const { return *pool_; }
	const TrDsLayout& layout() const { return *layout_; }

	friend void swap(TrDs& a, TrDs& b) noexcept;

protected:
	union {
		TrDsPool* pool_ {};
		DescriptorAllocator* allocator_;
	};

	const TrDsLayout* layout_ {};
};

/// Dynamically allocates tracked descriptors.
/// Will create descriptor pools on the fly but try to create as few
/// as possible.
class DescriptorAllocator : public vpp::Resource {
public:
	DescriptorAllocator(const vpp::Device&);
	~DescriptorAllocator() = default;

	void reserve(nytl::Span<const vk::DescriptorPoolSize>, unsigned count = 1);
	void reserve(const TrDsLayout&, unsigned count = 1);
	TrDs allocate(const TrDsLayout&);

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
