#ifndef KT_PLATFORM_INCLUDE_H
#define KT_PLATFORM_INCLUDE_H

#if (defined(linux) || defined(__linux) || defined(__linux__))
#   undef  __LINUX__
#   define __LINUX__   1
#endif

#if defined(ANDROID) || defined(__ANDROID__)
//#   undef  __LINUX__
#   undef  __ANDROID__
#   define __ANDROID__ 1
#endif


#if defined(__APPLE__)
#   include "AvailabilityMacros.h"
#   include "TargetConditionals.h"
#   if TARGET_OS_TV
#       undef  __TVOS__
#       define __TVOS__     1
#   endif
#   if TARGET_OS_IPHONE
#       undef  __IPHONEOS__
#       define __IPHONEOS__ 1
#       undef  __MACOSX__
#   else
#       undef  __MACOSX__
#       define __MACOSX__   1
#       if MAC_OS_X_VERSION_MIN_REQUIRED < 1060
#           error Mac OS X only supports deploying on 10.6 and above.
#       endif
#   endif
#endif


#endif//KT_PLATFORM_INCLUDE_H
