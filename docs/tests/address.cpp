#include "bugged.hpp"
#include <vpp/util/allocation.hpp>
#include <vpp/imageOps.hpp>
#include <vpp/formats.hpp>
#include <vpp/vk.hpp>

TEST(align) {
	EXPECT(vpp::align(1023, 4), 1024);
	EXPECT(vpp::align(1023u, 2u), 1024u);
	EXPECT(vpp::align(1u, 1024u), 1024u);
	EXPECT(vpp::align(0u, 2u), 0u);
	EXPECT(vpp::align(233u, 0u), 233u);
	EXPECT(vpp::align(933u, 1u), 933u);
	EXPECT(vpp::align(933u, 2u), 934u);
}

TEST(contains) {
	using Allocation = vpp::BasicAllocation<vk::DeviceSize>;
	Allocation a {0, 1024};
	Allocation b {0, 10};

	EXPECT(contains(a, b), true);
	EXPECT(contains(b, a), false);

	b = {1, 1023};
	EXPECT(contains(a, b), true);
	EXPECT(contains(b, a), false);
	EXPECT(a == b, false);

	b = {0, 1024};
	EXPECT(contains(a, b), true);
	EXPECT(contains(b, a), true);
	EXPECT(a == b, true);
	EXPECT(a != b, false);

	b = {0, 1025};
	EXPECT(contains(a, b), false);
	EXPECT(contains(b, a), true);
	EXPECT(a == b, false);
	EXPECT(b == b, true);

	b = {10, 1200};
	EXPECT(contains(a, b), false);
	EXPECT(contains(b, a), false);
	EXPECT(a != b, true);
	EXPECT(b != b, false);

	b = {10, 20};
	EXPECT(contains(a, b), true);
	EXPECT(contains(b, a), false);

	b = {0, 0};
	EXPECT(contains(a, b), true);
	EXPECT(contains(b, a), false);

	b = {1024, 0};
	EXPECT(contains(a, b), true);
	EXPECT(contains(b, a), false);

	b = {10, 0};
	EXPECT(contains(a, b), true);
	EXPECT(contains(b, a), false);

	b = {1025, 10};
	EXPECT(contains(a, b), false);
	EXPECT(contains(b, a), false);

	b = {1025, 0};
	EXPECT(contains(a, b), false);
	EXPECT(contains(b, a), false);
}

// negative size is undefined behavior, no need to test that
TEST(sContains) {
	using Allocation = vpp::BasicAllocation<int>;
	Allocation a {-50, 100};
	Allocation b {-10, 10};

	EXPECT(contains(a, b), true);
	EXPECT(contains(b, a), false);
	EXPECT(a == b, false);
	EXPECT(a == a, true);
	EXPECT(b != a, true);
}

// tbh, not sure how to properly test it.
TEST(texelAddress) {
	vk::SubresourceLayout layout {};

	auto texelSize = 1u;
	layout.offset = 100u;
	layout.rowPitch = 15u;
	layout.depthPitch = 120u;
	layout.arrayPitch = 1010u;

	EXPECT(vpp::texelAddress(layout, texelSize, 0, 0, 0, 0), layout.offset);
	EXPECT(vpp::texelAddress(layout, texelSize, 1, 0, 0, 0), layout.offset + texelSize);
	EXPECT(vpp::texelAddress(layout, texelSize, 1, 1, 0, 0),
		layout.offset + layout.rowPitch + texelSize);
	EXPECT(vpp::texelAddress(layout, texelSize, 1, 2, 0, 0),
		layout.offset + 2 * layout.rowPitch + texelSize);
	EXPECT(vpp::texelAddress(layout, texelSize, 1, 2, 3, 0),
		layout.offset + 3 * layout.depthPitch + 2 * layout.rowPitch + texelSize);
	EXPECT(vpp::texelAddress(layout, texelSize, 1, 2, 3, 4),
		layout.offset + 4 * layout.arrayPitch + 3 * layout.depthPitch +
		2 * layout.rowPitch + texelSize);
}

TEST(imageBufferOffset) {
	EXPECT(vpp::tightTexelNumber({1u, 1u, 1u}, 2, 0, 0), 0u);
	EXPECT(vpp::tightTexelNumber({1u, 1u, 1u}, 2, 0, 1), 1u);
	EXPECT(vpp::tightTexelNumber({5u, 1u, 1u}, 2, 0, 1), 5u);
	EXPECT(vpp::tightTexelNumber({5u, 5u, 1u}, 2, 0, 1), 25u);
	EXPECT(vpp::tightTexelNumber({5u, 5u, 5u}, 2, 0, 1), 125u);

	EXPECT(vpp::tightTexelNumber({5u, 5u, 5u}, 2, 1, 0), 250u);
	EXPECT(vpp::tightTexelNumber({5u, 5u, 5u}, 2, 1, 1), 250u + 2 * 2 * 2);

	EXPECT(vpp::tightLayerTexelNumber({10u, 10u, 10u}, 1, 1, 1), 111u);
}
