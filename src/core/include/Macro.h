#ifndef KT_MACRO_INCLUDE_H
#define KT_MACRO_INCLUDE_H

#ifndef KT_DEPRECATED
#   if (__GNUC__ >= 4)
#       define KT_DEPRECATED __attribute__((deprecated))
#   else
#       define KT_DEPRECATED
#   endif
#endif

#ifndef KT_UNUSED
#   ifdef __GNUC__
#       define KT_UNUSED __attribute__((unused))
#   else
#       define KT_UNUSED
#   endif
#endif

#ifndef KT_API
#   if defined(__WIN32__) || defined(__WINRT__) || defined(__CYGWIN__)
#       ifdef DLL_EXPORT
#           define KT_API __declspec(dllexport)
#       else
#           define KT_API
#       endif
#   elif defined(__OS2__)
#       ifdef BUILD_SDL
#           define KT_API    __declspec(dllexport)
#       else
#           define KT_API
#       endif
#   else
#       if defined(__GNUC__) && __GNUC__ >= 4
#           define KT_API __attribute__ ((visibility("default")))
#       else
#           define KT_API
#       endif
#   endif
#endif


#ifndef KT_INLINE
#   if defined(__GNUC__)
#       define KT_INLINE __inline__
#   elif defined(_MSC_VER)    || defined(__BORLANDC__) || \
         defined(__DMC__)     || defined(__SC__)       || \
         defined(__WATCOMC__) || defined(__LCC__)      || \
         defined(__DECC)      || defined(__CC_ARM)
#       define KT_INLINE __inline
#       ifndef __inline__
#           define __inline__ __inline
#       endif
#   else
#       define KT_INLINE inline
#       ifndef __inline__
#           define __inline__ inline
#       endif
#   endif
#endif

#ifndef KT_FORCE_INLINE
#   if defined(_MSC_VER)
#       define KT_FORCE_INLINE __forceinline
#   elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#       define KT_FORCE_INLINE __attribute__((always_inline)) static __inline__
#   else
#       define KT_FORCE_INLINE static KT_INLINE
#   endif
#endif

#endif//KT_MACRO_INCLUDE_H
