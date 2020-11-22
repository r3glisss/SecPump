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
 * @file sha.h
 * @brief sha implementation/wrapper
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_SHA_H
#define SCL_BACKEND_SHA_H

#include <api/hash/sha224.h>
#include <api/hash/sha256.h>
#include <api/hash/sha384.h>
#include <api/hash/sha512.h>

/** 
 * @addtogroup COMMON
 * @addtogroup SHA
 * @ingroup COMMON
 *  @{
 */

/*! @brief Hash mode supported */
typedef enum
{
    /*! @brief Define SHA224 mode */
    SCL_HASH_SHA224 = 0,
    /*! @brief Define SHA256 mode */
    SCL_HASH_SHA256 = 1,
    /*! @brief Define SHA384 mode */
    SCL_HASH_SHA384 = 2,
    /*! @brief Define SHA512 mode */
    SCL_HASH_SHA512 = 3
} hash_mode_t;

/*! @brief Unified SHA context */
typedef struct { 
    /*! @brief supported SHA contexts union */
    union {
        sha224_ctx_t sha224;
        sha256_ctx_t sha256;
        sha384_ctx_t sha384;
        sha512_ctx_t sha512;
    } ctx;
    /*! Hash mode  */
    hash_mode_t mode;
} sha_ctx_t;

/** @}*/

#endif /* SCL_BACKEND_SHA_H */
