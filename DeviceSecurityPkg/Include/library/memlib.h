/**
    Copyright Notice:
    Copyright 2021 DMTF. All rights reserved.
    License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
**/

/** @file
  Provides copy memory, fill memory, zero memory, and GUID functions.

  The Base Memory Library provides optimized implementations for common memory-based operations.
  These functions should be used in place of coding your own loops to do equivalent common functions.
  This allows optimized library implementations to help increase performance.
**/

#ifndef __SPDM_MEMORY_LIB__
#define __SPDM_MEMORY_LIB__

#include <Library/BaseMemoryLib.h>

#define copy_mem    CopyMem
#define zero_mem    ZeroMem
#define const_compare_mem   CompareMem
#define set_mem     SetMem
#define random_bytes        RandomBytes

#endif
