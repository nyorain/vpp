#include <vpp/procAddr.hpp>

namespace vpp
{
    namespace detail
    {
        sup<vk::Instance, sup<std::string, vk::PfnVoidFunction>> instanceProcs;
        sup<vk::Device, sup<std::string, vk::PfnVoidFunction>> deviceProcs;
    }
}
