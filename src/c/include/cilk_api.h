/*  cilk_api.h
 *
 *  Copyright (C) 2009-2015 Intel Corporation.  All Rights Reserved.
 *
 *  The source code contained or described herein and all documents related
 *  to the source code ("Material") are owned by Intel Corporation or its
 *  suppliers or licensors.  Title to the Material remains with Intel
 *  Corporation or its suppliers and licensors.  The Material is protected
 *  by worldwide copyright laws and treaty provisions.  No part of the
 *  Material may be used, copied, reproduced, modified, published, uploaded,
 *  posted, transmitted, distributed, or disclosed in any way without
 *  Intel's prior express written permission.
 *
 *  No license under any patent, copyright, trade secret or other
 *  intellectual property right is granted to or conferred upon you by
 *  disclosure or delivery of the Materials, either expressly, by
 *  implication, inducement, estoppel or otherwise.  Any license under such
 *  intellectual property rights must be express and approved by Intel in
 *  writing.
 */

#ifndef INCLUDED_CILK_API_H
#define INCLUDED_CILK_API_H


#ifndef CILK_STUB /* Real (non-stub) definitions */

#if ! defined(__cilk) && ! defined(USE_CILK_API)
#   ifdef _WIN32
#       error Cilk API is being used with non-Cilk compiler (or Cilk is disabled)
#   else
#       warning Cilk API is being used with non-Cilk compiler (or Cilk is disabled)
#   endif
#endif

#include "cilk/common.h"

#ifdef __cplusplus
#   include <cstddef>  /* Defines size_t */
#else
#   include <stddef.h> /* Defines size_t */
#endif

#ifdef _WIN32
#   ifndef IN_CILK_RUNTIME
/* Ensure the library is brought if any of these functions are being called. */
#       pragma comment(lib, "cilkrts")
#   endif

#   ifndef __cplusplus
#       include <wchar.h>
#   endif
#endif /* _WIN32 */

__CILKRTS_BEGIN_EXTERN_C

enum __cilkrts_set_param_status {
    __CILKRTS_SET_PARAM_SUCCESS = 0, 
    __CILKRTS_SET_PARAM_UNIMP   = 1, 
    __CILKRTS_SET_PARAM_XRANGE  = 2, 
    __CILKRTS_SET_PARAM_INVALID = 3, 
    __CILKRTS_SET_PARAM_LATE    = 4  
};

CILK_API(int) __cilkrts_set_param(const char *param, const char *value);

#ifdef _WIN32

CILK_API(int) __cilkrts_set_param_w(const wchar_t *param, const wchar_t *value);
#endif

CILK_API(void) __cilkrts_end_cilk(void);

CILK_API(void) __cilkrts_init(void);

CILK_API(int) __cilkrts_get_nworkers(void);

CILK_API(int) __cilkrts_get_total_workers(void);

CILK_API(int) __cilkrts_get_worker_number(void);

CILK_API(int) __cilkrts_get_force_reduce(void);

CILK_API(void)
    __cilkrts_metacall(unsigned int tool, unsigned int code, void *data);

#ifdef _WIN32
typedef struct _EXCEPTION_RECORD _EXCEPTION_RECORD;

typedef void (*__cilkrts_pfn_seh_callback)(const _EXCEPTION_RECORD *exception);

CILK_API(int) __cilkrts_set_seh_callback(__cilkrts_pfn_seh_callback pfn);
#endif /* _WIN32 */

#if __CILKRTS_ABI_VERSION >= 1
/* Pedigree API is available only for compilers that use ABI version >= 1. */



// @cond internal

CILK_API(__cilkrts_pedigree)
__cilkrts_get_pedigree_internal(__cilkrts_worker *w);

CILK_API(int)
__cilkrts_bump_worker_rank_internal(__cilkrts_worker* w);



__CILKRTS_INLINE
__cilkrts_pedigree __cilkrts_get_pedigree(void)
{
    return __cilkrts_get_pedigree_internal(__cilkrts_get_tls_worker());
}

typedef struct
{
    __STDNS size_t size;    
    void *data[3];          
} __cilkrts_pedigree_context_t;

CILK_API(int)
__cilkrts_get_pedigree_info(/* In/Out */ __cilkrts_pedigree_context_t *context,
                            /* Out */    uint64_t *sf_birthrank);

CILK_EXPORT_AND_INLINE
int __cilkrts_get_worker_rank(uint64_t *rank)
{
    *rank = __cilkrts_get_pedigree().rank;
    return 0;
}

CILK_EXPORT_AND_INLINE
int __cilkrts_bump_worker_rank(void)
{
    return __cilkrts_bump_worker_rank_internal(__cilkrts_get_tls_worker());
}

CILK_EXPORT_AND_INLINE
int __cilkrts_bump_loop_rank(void)
{
    return 0;
}


#endif /* __CILKRTS_ABI_VERSION >= 1 */

__CILKRTS_END_EXTERN_C

#else /* CILK_STUB */

// Programs compiled with CILK_STUB are not linked with the Intel Cilk Plus runtime
// library, so they should not have external references to runtime functions.
// Therefore, the functions are replaced with stubs.

#ifdef _WIN32
#define __cilkrts_set_param_w(name,value) ((value), 0)
#define __cilkrts_set_seh_callback(pfn) (0)
#endif
#define __cilkrts_set_param(name,value) ((value), 0)
#define __cilkrts_end_cilk() ((void) 0)
#define __cilkrts_init() ((void) 0)
#define __cilkrts_get_nworkers() (1)
#define __cilkrts_get_total_workers() (1)
#define __cilkrts_get_worker_number() (0)
#define __cilkrts_get_force_reduce() (0)
#define __cilkrts_metacall(tool,code,data) ((tool), (code), (data), 0)

#if __CILKRTS_ABI_VERSION >= 1
/* Pedigree stubs */
#define __cilkrts_get_pedigree_info(context, sf_birthrank) (-1)
#define __cilkrts_get_worker_rank(rank) (*(rank) = 0)
#define __cilkrts_bump_worker_rank() (-1)
#define __cilkrts_bump_loop_rank() (-1)

/*
 * A stub method for __cilkrts_get_pedigree.
 * Returns an empty __cilkrts_pedigree.
 */
__CILKRTS_INLINE
__cilkrts_pedigree __cilkrts_get_pedigree_stub(void)
{
    __cilkrts_pedigree ans;
    ans.rank = 0;
    ans.parent = NULL;
    return ans;
}

/* Renamed to an actual stub method. */
#define __cilkrts_get_pedigree() __cilkrts_get_pedigree_stub()

#endif /* __CILKRTS_ABI_VERSION >= 1 */

#endif /* CILK_STUB */


#endif /* INCLUDED_CILK_API_H */
