#include <vpp/trackedDescriptor.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

// TrDsLayout
TrDsLayout::TrDsLayout(const Device& dev,
	nytl::Span<const vk::DescriptorSetLayoutBinding> xbindings) :
		DescriptorSetLayout(dev, xbindings) {

	for(auto& b : xbindings) {
		auto it = std::find_if(bindings_.begin(), bindings_.end(),
			[&](const auto& s) { return s.type == b.descriptorType; });
		if(it != bindings_.end()) {
			it->descriptorCount += b.descriptorCount;
		} else {
			bindings_.push_back({});
			bindings_.back().type = b.descriptorType;
			bindings_.back().descriptorCount = b.descriptorCount;
		}
	}
}

// TrDsPool
TrDsPool::TrDsPool(const Device& dev, vk::DescriptorPoolCreateInfo info)
		: remainingSets_(info.maxSets) {

	info.flags |= vk::DescriptorPoolCreateBits::freeDescriptorSet;
	auto pool = vk::createDescriptorPool(dev, info, nullptr);
	DescriptorPool::operator=({dev, pool});

	remaining_.reserve(info.poolSizeCount);
	for(auto i = 0u; i < info.poolSizeCount; ++i) {
		remaining_.push_back(info.pPoolSizes[i]);
	}
}

TrDsPool::TrDsPool(const Device& dev, unsigned maxSets,
		nytl::Span<const vk::DescriptorPoolSize> sizes) :
		remaining_(sizes.begin(), sizes.end()), remainingSets_(maxSets) {

	vk::DescriptorPoolCreateInfo info;
	info.flags = vk::DescriptorPoolCreateBits::freeDescriptorSet;
	info.maxSets = maxSets;
	info.pPoolSizes = sizes.data();
	info.poolSizeCount = sizes.size();
	auto pool = vk::createDescriptorPool(dev, info, nullptr);
	DescriptorPool::operator=({dev, pool});
}

// Descriptor
TrDs::TrDs(TrDsPool& pool, const TrDsLayout& layout) :
		DescriptorSet(pool, layout), pool_(&pool), layout_(&layout) {

	dlg_assert(vkHandle());
	dlg_assert(pool);
	dlg_assert(layout);

	// remove from pool
	// TODO: throw when there are not enough resources left?
	auto& rem = pool.remaining_;
	for(auto& binding : layout.bindings()) {
		auto it = std::find_if(rem.begin(), rem.end(), [&](const auto& s)
			{ return s.type == binding.type; });
		dlg_assert(it != rem.end());
		dlg_assert(it->descriptorCount >= binding.descriptorCount);
		it->descriptorCount -= binding.descriptorCount;
	}

	--pool.remainingSets_;
}

TrDs::TrDs(DeferTag, DescriptorAllocator& alloc, const TrDsLayout& layout) :
		allocator_(&alloc), layout_(&layout) {
	allocator_->reserve(layout);
	reservation_ = allocator_->pools().size() + 1;
}

TrDs::TrDs(DescriptorAllocator& alloc, const TrDsLayout& layout)
		: TrDs(alloc.alloc(layout)) {
}

void TrDs::init() {
	if(vkHandle()) {
		dlg_assert(!reservation_);
		return;
	}

	reservation_ = {};
	dlg_assert(allocator_);
	dlg_assert(layout_);
	*this = allocator_->alloc(*layout_);
}

TrDs::~TrDs() {
	if(reservation_) {
		dlg_assert(!vkHandle());
		dlg_assert(allocator_ && layout_);
		if(allocator_->pools().size() + 1 == reservation_) {
			allocator_->unreserve(*layout_);
		}

		return;
	}

	if(!vkHandle()) {
		return;
	}

	dlg_assert(layout_);
	dlg_assert(pool_);

	// add bindings to pool
	auto& rem = pool().remaining_;
	for(auto& binding : layout().bindings()) {
		auto it = std::find_if(rem.begin(), rem.end(), [&](const auto& s)
			{ return s.type == binding.type; });
		dlg_assert(it != rem.end());
		it->descriptorCount += binding.descriptorCount;
	}

	++pool().remainingSets_;
	vk::freeDescriptorSets(device(), pool(), 1, vkHandle());
}

void swap(TrDs& a, TrDs& b) noexcept
{
	using std::swap;

	swap(static_cast<DescriptorSet&>(a), static_cast<DescriptorSet&>(b));
	swap(a.pool_, b.pool_);
	swap(a.layout_, b.layout_);
	swap(a.reservation_, b.reservation_);
}

// DescriptorAllocator
DescriptorAllocator::DescriptorAllocator(const Device& dev) :
	Resource(dev) {
}

void DescriptorAllocator::reserve(nytl::Span<const vk::DescriptorPoolSize> bs,
		unsigned count) {

	auto& t = pending_.types;
	for(auto& b1 : bs) {
		auto it = std::find_if(t.begin(), t.end(), [&](const auto& b2)
			{ return b2.type == b1.type; });

		if(it == t.end()) {
			t.push_back(b1);
		} else {
			it->descriptorCount += b1.descriptorCount;
		}
	}

	pending_.count += count;
}

void DescriptorAllocator::reserve(const TrDsLayout& layout, unsigned count) {
	reserve(layout.bindings(), count);
}

TrDs DescriptorAllocator::alloc(const TrDsLayout& layout) {
	dlg_assert(layout);

	// check if there is a pool with enough free space left
	for(auto& pool : pools_) {
		dlg_assert(pool);
		if(!pool.remainingSets()) {
			continue;
		}

		// whether the current pool can be used
		bool ok = true;

		// test for every binding if there are enough remaining
		auto& rem = pool.remaining();
		for(auto& b1 : layout.bindings()) {
			auto it = std::find_if(rem.begin(), rem.end(),
				[&](const auto& b2){ return b2.type == b1.type; });
			ok = (it != rem.end()) &&
				 (it->descriptorCount >= b1.descriptorCount);
			if(!ok) {
				break;
			}
		}

		if(ok) {
			return {pool, layout};
		}
	}

	// TODO: use a better allocation strat
	// allocate a new pool
	// reserve the default allocations additionally
	reserve(layout.bindings(), 1);
	reserve({
		{vk::DescriptorType::uniformBuffer, 30},
		{vk::DescriptorType::combinedImageSampler, 20}
	}, 20);

	pools_.emplace_back(device(), pending_.count, pending_.types);
	pending_ = {};
	return {pools_.back(), layout};
}

void DescriptorAllocator::unreserve(const TrDsLayout& layout) {
	auto& t = pending_.types;
	for(auto& b1 : layout.bindings()) {
		auto it = std::find_if(t.begin(), t.end(), [&](const auto& b2)
			{ return b2.type == b1.type; });

		dlg_assert(it != t.end());
		dlg_assert(it->descriptorCount > b1.descriptorCount);
		it->descriptorCount -= b1.descriptorCount;
	}

	dlg_assert(pending_.count);
	pending_.count -= 1;
}

} // namespace vpp
