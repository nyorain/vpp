//codegen
class XYBase
{
protected:
	vk::XYHandle handle_;

protected:
	XYBase() = default;
	void create(vk::XYHadle handle) { handle_ = handle; }

public:
	XYBase(vk::XYHadle handle) : handle_(handle) {}
	XYBase() = default;
};

class XY : public Resource, public XYBase
{
public:
	XY(const Device& device, const XYCreateInfo& info) : Resource(device) { vk::createXY(vkDevice(), &info, nullptr, &handle_); }
	~XY(){ if(handle_) vk::destroyXY(vkDevice(), nullptr, handle_); }
};
