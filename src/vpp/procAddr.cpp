#include <vpp/procAddr.hpp>

namespace vpp
{
    namespace detail
    {
        sup<VkInstance, sup<std::string, PFN_vkVoidFunction>> instanceProcs;
        sup<VkDevice, sup<std::string, PFN_vkVoidFunction>> deviceProcs;
    }
}
