There are two types of config options for vpp:
* Compile flags
* Macros defined when including the vpp headers

The Compile flags will change how the vpp library behaves. E.g. if you compile vpp in a debug
configuration, it will by default enable some vulkan debug layers at context creation and will perform
additional validity checks for passed arguments.

The config options specified at header include time, will only change the behaviour of inlined (e.g.
template) functions and classes. One can control e.g. if vulkan calls made by the program (NOT inside
the library) should throw errors or outputs warnings.
Usually, one wants to keep the vpp compile time settings for inlined code for consistency. Therefore
all headers include vpp/config.hpp which will define the compile-time settings.

Note that the generated vulkan headers do not pull in the config file, so it must be included
manually before including somthing from vpp/vulkan/ or vpp/vk.hpp must be used to pull in the
generated vulkan api bindings.

Those can be overridden though, see src/vpp/config.hpp.in (or include/vpp/config.hpp
if the library was already compiled).

* VPP_DEBUG If this macro is defined, additional checks will be performed (maybe worse performance)
* VPP_DEBUG_THROW If this macro and VPP_DEBUG are defined, an exception will be thrown if
	some additional check fails, otherwise a warning will be outputted to cout and
	maybe returned (if the error would lead to some invalid state).


* VPP_CALL_WARN If this macro is defined an error will be printed to cout if a vulkan function
	call returns a negative (i.e. error) result.
* VPP_CALL_THROW If this macro is defined an exception will be thrown if a vulkan function call
	return a negative (i.e. error) result
* VPP_CALL_NOCHECK If this macro is defined there will no result checking be performed on
	vulkan function call results and VPP_CALL_WARN and VPP_CALL_THROW are ignored.
