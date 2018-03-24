// Idea for a BufferRange rework.
// Renames BufferRange to SharedBufferRange and adds new BufferRange class/struct
// that represents a not owned view that can be passed to operations as
// abstraction over owned/not-owned/shared-owned buffers.
//
// Boils down to something like this:
struct BufferRange {
	vpp::Buffer* buf;
	vk::DeviceSize offset;
	vk::DeviceSize size;
};

// Or maybe make SharedBufferRange derive from BufferRange? Probably makes sense.
