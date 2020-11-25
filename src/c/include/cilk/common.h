#ifndef INCLUDED_CILK_COMMON
#define INCLUDED_CILK_COMMON

#ifdef __cplusplus

namespace cilk {
    
    namespace internal {}
}
#endif

#define CILK_LIBRARY_VERSION 102

#ifdef __cplusplus
#   include <cassert>
#else
#   include <assert.h>
#endif

#ifdef __cplusplus
#   define __STDNS std::
#else
#   define __STDNS
#endif

#ifdef _WIN32
#   ifdef IN_CILK_RUNTIME
#       define CILK_EXPORT      __declspec(dllexport)
#       define CILK_EXPORT_DATA __declspec(dllexport)
#   else
#       define CILK_EXPORT      __declspec(dllimport)
#       define CILK_EXPORT_DATA __declspec(dllimport)
#   endif  /* IN_CILK_RUNTIME */
#elif defined(__CYGWIN__) || defined(__APPLE__) || defined(_DARWIN_C_SOURCE)
#   define CILK_EXPORT      /* nothing */
#   define CILK_EXPORT_DATA /* nothing */
#else /* Unix/gcc */
#   if defined(IN_CILK_RUNTIME) && defined(HAVE_ATTRIBUTE_VISIBILITY)
#       define CILK_EXPORT      __attribute__((visibility("protected")))
#       define CILK_EXPORT_DATA __attribute__((visibility("protected")))
#   else
#       define CILK_EXPORT      /* nothing */
#       define CILK_EXPORT_DATA /* nothing */
#   endif  /* IN_CILK_RUNTIME */
#endif /* Unix/gcc */

#ifdef __cplusplus
#   define __CILKRTS_BEGIN_EXTERN_C     extern "C" {
#   define __CILKRTS_END_EXTERN_C       }
#   define __CILKRTS_EXTERN_C           extern "C"
#else
#   define __CILKRTS_BEGIN_EXTERN_C
#   define __CILKRTS_END_EXTERN_C
#   define __CILKRTS_EXTERN_C
#endif

#ifdef __cplusplus
#   ifdef _WIN32
#       define __CILKRTS_NOTHROW __declspec(nothrow)
#   else /* Unix/gcc */
#       define __CILKRTS_NOTHROW __attribute__((nothrow))
#   endif /* Unix/gcc */
#else
#   define __CILKRTS_NOTHROW /* nothing */
#endif /* __cplusplus */

#define __CILKRTS_CACHE_LINE__ 64

#ifdef _WIN32
#   define CILK_ALIGNAS(n) __declspec(align(n))
#else /* Unix/gcc */
#   define CILK_ALIGNAS(n) __attribute__((__aligned__(n)))
#endif

#define __CILKRTS_CACHE_ALIGN CILK_ALIGNAS(__CILKRTS_CACHE_LINE__)

#ifdef _WIN32
#   define CILK_ALIGNAS_TYPE(t) __declspec(align(__alignof(t)))
#else /* Unix/gcc */
#   define CILK_ALIGNAS_TYPE(t) __attribute__((__aligned__))
#endif

#ifdef _WIN32
#   define CILK_API(RET_TYPE) CILK_EXPORT RET_TYPE __CILKRTS_NOTHROW __cdecl
#   define CILK_ABI(RET_TYPE) CILK_EXPORT RET_TYPE __CILKRTS_NOTHROW __cdecl
#   define CILK_ABI_THROWS(RET_TYPE) CILK_EXPORT RET_TYPE __cdecl
#else
#   define CILK_API(RET_TYPE) CILK_EXPORT RET_TYPE __CILKRTS_NOTHROW
#   define CILK_ABI(RET_TYPE) CILK_EXPORT RET_TYPE __CILKRTS_NOTHROW
#   define CILK_ABI_THROWS(RET_TYPE) CILK_EXPORT RET_TYPE
#endif

#if defined(_WIN32) && defined(_DEBUG)
    /* Windows debug */
#   define __CILKRTS_ASSERT(e) assert(e)
#elif (! defined(_WIN32)) && ! defined(__OPTIMIZE__)
    /* Unix non-optimized */
#   define __CILKRTS_ASSERT(e) assert(e)
#elif defined __cplusplus
    /* C++ non-debug */
#   define __CILKRTS_ASSERT(e) static_cast<void>(0)
#else
    /* C non-debug */
#   define __CILKRTS_ASSERT(e) ((void) 0)
#endif

#ifdef __cpluspus
    // C++
#   define __CILKRTS_INLINE inline
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    // C99
#   define __CILKRTS_INLINE static inline
#elif defined(_MSC_VER)
    // C89 on Windows
#   define __CILKRTS_INLINE __inline
#else
    // C89 on GCC-compatible systems
#   define __CILKRTS_INLINE extern __inline__
#endif

#ifdef COMPILING_CILK_API_FUNCTIONS
#   define CILK_EXPORT_AND_INLINE  CILK_EXPORT
#else
#   define CILK_EXPORT_AND_INLINE  __CILKRTS_INLINE
#endif

#if defined(__cplusplus) && !defined(__CILKRTS_RVALUE_REFERENCES)
#   if __cplusplus >= 201103L // C++11
#       define __CILKRTS_RVALUE_REFERENCES 1
#   elif defined(__GXX_EXPERIMENTAL_CXX0X__)
#       define __CILKRTS_RVALUE_REFERENCES 1
#   elif __cplusplus >= 199711L && __cplusplus < 201103L
        // Compiler recognizes a language version prior to C++11
#   elif __INTEL_COMPILER == 1200 && defined(__STDC_HOSTED__)
        // Intel compiler version 12.0
        // __cplusplus has a non-standard definition.  In the absence of a
        // proper definition, look for the C++0x macro, __STDC_HOSTED__.
#       define __CILKRTS_RVALUE_REFERENCES 1
#   elif __INTEL_COMPILER > 1200 && defined(CHAR16T)
        // Intel compiler version >= 12.1
        // __cplusplus has a non-standard definition.  In the absence of a
        // proper definition, look for the Intel macro, CHAR16T
#       define __CILKRTS_RVALUE_REFERENCES 1
#   endif
#endif

/*
 * Include stdint.h to define the standard integer types.
 *
 * Unfortunately Microsoft doesn't provide stdint.h until Visual Studio 2010,
 * so use our own definitions until those are available
 */

#if ! defined(_MSC_VER) || (_MSC_VER >= 1600)
#   include <stdint.h>
#else
#   ifndef __MS_STDINT_TYPES_DEFINED__
#       define __MS_STDINT_TYPES_DEFINED__
        typedef signed char int8_t;
        typedef short int16_t;
        typedef int int32_t;
        typedef __int64 int64_t;

        typedef unsigned char uint8_t;
        typedef unsigned short uint16_t;
        typedef unsigned int uint32_t;
        typedef unsigned __int64 uint64_t;
#   endif  /* __MS_STDINT_TYPES_DEFINED__ */
#endif  /* ! defined(_MSC_VER) || (_MSC_VER >= 1600) */

#ifndef __CILKRTS_ABI_VERSION
#   ifdef IN_CILK_RUNTIME
#       define __CILKRTS_ABI_VERSION 1
#   elif defined(__INTEL_COMPILER) && (__INTEL_COMPILER <= 1200)
        // Intel compilers prior to version 12.1 support only ABI 0
#       define __CILKRTS_ABI_VERSION 0
#   else
        // Non-Intel compiler or Intel compiler after version 12.0.
#       define __CILKRTS_ABI_VERSION 1
#   endif
#endif

// These structs are exported because the inlining of
// the internal version of API methods require a worker
// structure as parameter. 
__CILKRTS_BEGIN_EXTERN_C
    struct __cilkrts_worker;

    typedef struct __cilkrts_worker __cilkrts_worker;     

    typedef struct __cilkrts_worker *__cilkrts_worker_ptr; 
    
    
    CILK_ABI(__cilkrts_worker_ptr) __cilkrts_get_tls_worker(void);

    typedef void * __cilkrts_void_ptr;

__CILKRTS_END_EXTERN_C

                                   
#if __CILKRTS_ABI_VERSION >= 1
// Pedigree API is available only for compilers that use ABI version >= 1.

typedef struct __cilkrts_pedigree
{
    uint64_t rank;
                                         
    const struct __cilkrts_pedigree *parent;
} __cilkrts_pedigree;

#endif // __CILKRTS_ABI_VERSION >= 1


#endif /* INCLUDED_CILK_COMMON */
