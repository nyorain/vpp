//Macros:
//If none of the three macros defined above if defined by the user, VPP_CALL_NOCHECK will be
//defined by default if (!defined(VPP_DEBUG)), VPP_CALL_WARN will be defined by default
//if (defined(VPP_DEBUG)) and VPP_CALL_THROW will be defined by default
//if (defined(VPP_DEBUG) && defined(VPP_DEBUG_THROW)).
//
//VPP_CALL_WARN will be defined
//by default in non-debug mode (VPP_DEBUG = 0) and VPP_CALL_THROW by default if
//
//For more information on vpp configuration, see doc/config.md

#pragma once

#define VPP_VMAJOR 0
#define VPP_VMINOR 1
#define VPP_VPATCH 0

#ifndef VPP_DEBUG
 #define VPP_DEBUG ON
#endif

#ifndef VPP_DEBUG_THROW
 #define VPP_DEBUG_THROW ON
#endif

#if !defined(VPP_CALL_THROW) && !defined(VPP_CALL_WARN) && !defined(VPP_CALL_NOCHECK)
 #ifdef VPP_DEBUG
  #define VPP_CALL_WARN
  #ifdef VPP_DEBUG_THROW
   #define VPP_CALL_THROW
  #endif
 #else
  #define VPP_CALL_NOCHECK
 #endif
#endif
