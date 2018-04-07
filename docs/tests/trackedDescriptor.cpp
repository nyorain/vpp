#include "init.hpp"
#include "bugged.hpp"

#include <vpp/trackedDescriptor.hpp>

TEST(basic) {
	auto& dev = *globals.device;

	// layout
	auto bindings1 = {
		vpp::descriptorBinding(vk::DescriptorType::uniformBuffer),
		vpp::descriptorBinding(vk::DescriptorType::uniformBuffer),
		vpp::descriptorBinding(vk::DescriptorType::combinedImageSampler,
			vk::ShaderStageBits::fragment, 4, 10),
	};

	auto bindings2 = {
		vpp::descriptorBinding(vk::DescriptorType::storageBuffer,
			vk::ShaderStageBits::compute, -1, 3),
	};

	auto layout1 = vpp::TrDsLayout(dev, bindings1);
	auto layout2 = vpp::TrDsLayout(dev, bindings2);

	EXPECT(layout1.bindings().size(), 2u);
	EXPECT(layout1.bindings()[0].descriptorCount, 2u);
	EXPECT(layout1.bindings()[0].type, vk::DescriptorType::uniformBuffer);
	EXPECT(layout1.bindings()[1].descriptorCount, 10u);
	EXPECT(layout1.bindings()[1].type, vk::DescriptorType::combinedImageSampler);

	EXPECT(layout2.bindings().size(), 1u);
	EXPECT(layout2.bindings()[0].descriptorCount, 3u);
	EXPECT(layout2.bindings()[0].type, vk::DescriptorType::storageBuffer);

	// pool
	auto pool = vpp::TrDsPool(dev, 3, {
		{vk::DescriptorType::uniformBuffer, 4},
		{vk::DescriptorType::storageBuffer, 3},
		{vk::DescriptorType::combinedImageSampler, 20}
	});

	EXPECT(pool.remainingSets(), 3u);
	EXPECT(pool.remaining().size(), 3u);
	EXPECT(pool.remaining()[0].type, vk::DescriptorType::uniformBuffer);
	EXPECT(pool.remaining()[0].descriptorCount, 4u);
	EXPECT(pool.remaining()[1].descriptorCount, 3u);
	EXPECT(pool.remaining()[1].type, vk::DescriptorType::storageBuffer);
	EXPECT(pool.remaining()[2].descriptorCount, 20u);
	EXPECT(pool.remaining()[2].type, vk::DescriptorType::combinedImageSampler);

	// sets
	auto set1 = vpp::TrDs(pool, layout1);

	EXPECT(pool.remainingSets(), 2u);
	EXPECT(pool.remaining().size(), 3u);
	EXPECT(pool.remaining()[0].descriptorCount, 2u);
	EXPECT(pool.remaining()[1].descriptorCount, 3u);
	EXPECT(pool.remaining()[2].descriptorCount, 10u);

	auto set2 = vpp::TrDs(pool, layout1);

	EXPECT(pool.remainingSets(), 1u);
	EXPECT(pool.remaining().size(), 3u);
	EXPECT(pool.remaining()[0].descriptorCount, 0u);
	EXPECT(pool.remaining()[1].descriptorCount, 3u);
	EXPECT(pool.remaining()[2].descriptorCount, 0u);

	auto set3 = vpp::TrDs(pool, layout2);

	EXPECT(pool.remainingSets(), 0u);
	EXPECT(pool.remaining().size(), 3u);
	EXPECT(pool.remaining()[0].descriptorCount, 0u);
	EXPECT(pool.remaining()[1].descriptorCount, 0u);
	EXPECT(pool.remaining()[2].descriptorCount, 0u);

	EXPECT(&set1.pool(), &pool);
	EXPECT(&set2.pool(), &pool);
	EXPECT(&set3.pool(), &pool);

	EXPECT(&set1.layout(), &layout1);
	EXPECT(&set2.layout(), &layout1);
	EXPECT(&set3.layout(), &layout2);
}

TEST(allocator) {
	auto& dev = *globals.device;
	auto alloc = vpp::DescriptorAllocator(dev);

	auto bindings = {
		vpp::descriptorBinding(vk::DescriptorType::storageBuffer),
		vpp::descriptorBinding(vk::DescriptorType::combinedImageSampler)
	};

	auto layout = vpp::TrDsLayout(dev, bindings);

	// Check that with correct reserves, we don't use more than one pool
	for(auto i = 0u; i < 10; ++i) {
		alloc.reserve(layout);
	}

	auto defered = vpp::TrDs(vpp::defer, alloc, layout);
	EXPECT(defered.vkHandle(), vk::DescriptorSet {});

	auto set0 = vpp::TrDs(alloc, layout);
	EXPECT(&set0.layout(), &layout);

	std::vector<vpp::TrDs> sets;
	for(auto i = 0u; i < 10; ++i) {
		sets.push_back({alloc, layout});
		EXPECT(sets.back().vkHandle() != vk::DescriptorSet {}, true);
		EXPECT(&sets.back().layout(), &layout);
		EXPECT(&sets.back().pool(), &set0.pool());
	}

	EXPECT(alloc.pools().size(), 1u);

	defered.init();
	EXPECT(defered.vkHandle() != vk::DescriptorSet {}, true);
	EXPECT(&defered.pool(), &set0.pool());
	EXPECT(&defered.layout(), &layout);

	EXPECT(alloc.pools().size(), 1u);

	// check that resources are correctly freed and reused
	sets.clear();
	for(auto i = 0u; i < 100; ++i) {
		auto set = vpp::TrDs(alloc, layout);
		EXPECT(set.vkHandle() != vk::DescriptorSet {}, true);
		EXPECT(&set.layout(), &layout);
		EXPECT(&set.pool(), &set0.pool());
		EXPECT(alloc.pools().size(), 1u);
	}
}

TEST(devAllocator) {
	auto& dev = *globals.device;
	auto& alloc = dev.descriptorAllocator();

	auto bindings = {
		vpp::descriptorBinding(vk::DescriptorType::storageBuffer),
		vpp::descriptorBinding(vk::DescriptorType::combinedImageSampler)
	};

	auto layout = vpp::TrDsLayout(dev, bindings);

	alloc.reserve(layout, 1);

	auto ds2 = vpp::TrDs(vpp::defer, alloc, layout);

	for(auto i = 0u; i < 1000; ++i) {
		auto tmp = vpp::TrDs(vpp::defer, alloc, layout);
	}

	for(auto i = 0u; i < 1000; ++i) {
		auto tmp = vpp::TrDs(vpp::defer, alloc, layout);
		tmp = {};
	}

	auto ds = vpp::TrDs(alloc, layout);
	ds2.init();

	EXPECT(alloc.pools().size(), 1u);
	EXPECT(alloc.pools()[0].remainingSets() < 1000u, true);
}
