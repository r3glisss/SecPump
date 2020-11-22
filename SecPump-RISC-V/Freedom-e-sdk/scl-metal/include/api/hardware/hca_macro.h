/******************************************************************************
 *
 * SiFive Cryptographic Library (SCL)
 *
 ******************************************************************************
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/

/**
 * @file hca_macro.h
 * @brief macro definition specific to hca
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_HCA_MACRO_H
#define SCL_BACKEND_HCA_MACRO_H

#include <metal/io.h>

#include <api/scl_api.h>

#include <api/hardware/hca_utils.h>

/**
 * @addtogroup HCA
 * @addtogroup HCA_MACRO
 * @ingroup HCA
 *  @{
 */

/*! @brief Macro to access a 64 bits register */
#define METAL_REG64(base, offset)                                              \
    (__METAL_ACCESS_ONCE((uint64_t *)((base) + (offset))))
/*! @brief Macro to access a 32 bits register */
#define METAL_REG32(base, offset)                                              \
    (__METAL_ACCESS_ONCE((uint32_t *)((base) + (offset))))

/*! @brief Macro to copy 32 (no alignement constraint) to 32 (aligned) */
#define GET_32BITS(data, k)                                                    \
    ((((uint32_t) * ((const uint8_t *)(data) + (k) + 3)) << 24) +              \
     (((uint32_t) * ((const uint8_t *)(data) + (k) + 2)) << 16) +              \
     (((uint32_t) * ((const uint8_t *)(data) + (k) + 1)) << 8) +               \
     ((uint32_t) * ((const uint8_t *)(data) + (k))))
/*! @brief Macro to copy 64 (no alignement constraint) to 64 (aligned) */
#define GET_64BITS(data, k)                                                    \
    ((((uint64_t)GET_32BITS((const uint8_t *)data, (k + 4))) << 32) +          \
     (uint64_t)GET_32BITS((const uint8_t *)data, k))

/** @}*/

#endif /* SCL_BACKEND_HCA_MACRO_H */
