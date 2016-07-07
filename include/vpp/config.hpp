#pragma once

//Macros:
//Those two macros are only important at vpp compile time
//* VPP_DEBUG If this macro is defined, additional checks will be performed (maybe worse performance)
//		This macro will be defined by default, if NDEBUG is not defined.
//* VPP_DEBUG_THROW If this macro and VPP_DEBUG are defined, an exception will be thrown if
//		some additional check fails, otherwise a warning will be outputted to cout and
//		maybe returned (if the error would lead to some invalid state). Defined by default.

//These macros can be manually set by application using vpp, but they will have to effect on
//the vulkan calls made by the vpp library (those will be checked depending on the macros
//at vpp compile time).
//* VPP_CALL_WARN If this macro is defined an error will be printed to cout if a vulkan function
//		call returns a negative (i.e. error) result
//* VPP_CALL_THROW If this macro is defined an exception will be thrown if a vulkan function call
//		return a negative (i.e. error) result
//* VPP_CALL_NOCHECK If this macro is defined there will no result checking be performed on
//		vulkan function call results and VPP_CALL_WARN and VPP_CALL_THROW are ignored.
//If none of the three macros defined above if defined by the user, VPP_CALL_NOCHECK will be
//defined by default if (!defined(VPP_DEBUG)), VPP_CALL_WARN will be defined by default
//if (defined(VPP_DEBUG)) and VPP_CALL_THROW will be defined by default
//if (defined(VPP_DEBUG) && defined(VPP_DEBUG_THROW)).

//VPP_CALL_WARN will be defined
//by default in non-debug mode (VPP_DEBUG = 0) and VPP_CALL_THROW by default if

#if !defined(VPP_DEBUG) && !defined(NDEBUG)
  #define VPP_DEBUG
#endif

#ifndef VPP_DEBUG_THROW
 #define VPP_DEBUG_THROW
#endif

#if !defined(VPP_CALL_THROW) && !defined(VPP_CALL_WARN) && !defined(VPP_CALL_NOCHECK)
 #ifdef VPP_DEBUG
  #define VPP_CALL_WARN
  #ifdef VPP_DEBUG_THROW
   #define VPP_CALL_THROW
  #endif //VPP_DEBUG_THROW
 #else //VPP_DEBUG
  #define VPP_CALL_NOCHECK
 #endif //VPP_DEBUG
#endif //!throw, !warn, !nochck
