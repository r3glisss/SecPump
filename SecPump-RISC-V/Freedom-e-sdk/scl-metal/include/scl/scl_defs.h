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
 * @file scl_defs.h
 * @brief contains various definitions for the cryptographic algorithms
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_DEFS_H
#define SCL_DEFS_H

#include <api/defs.h>
#include <api/hash/sha.h>

/**
 * @addtogroup SCL
 * @addtogroup SCL_DEFS
 * @ingroup SCL
 *  @{
 */

typedef endianness_t scl_endianness_t;

// symmetric-crypto encryption mode
typedef enum
{
    SCL_ENCRYPT = 0,
    SCL_DECRYPT = 1
} scl_process_t;

typedef enum
{
    /*! @brief Define for char lenght of 128 bits key */
    SCL_KEY128 = 16,
    /*! @brief Define for char lenght of 192 bits key */
    SCL_KEY192 = 24,
    /*! @brief Define for char lenght of 256 bits key */
    SCL_KEY256 = 32,
} scl_key_size_t;

typedef enum
{
    /*! @brief Define for 128 bits key lenght */
    SCL_AES_KEY128 = 0,
    /*! @brief Define for 192 bits key lenght */
    SCL_AES_KEY192 = 1,
    /*! @brief Define for 256 bits key lenght */
    SCL_AES_KEY256 = 2,
} scl_aes_key_type_t;

typedef enum
{
    /*! @brief Define ECB mode */
    SCL_AES_ECB = 0,
    /*! @brief Define CBC mode */
    SCL_AES_CBC = 1,
    /*! @brief Define CFB mode */
    SCL_AES_CFB = 2,
    /*! @brief Define OFB mode */
    SCL_AES_OFB = 3,
    /*! @brief Define CTR mode */
    SCL_AES_CTR = 4,
    /*! @brief Define GCM mode */
    SCL_AES_GCM = 5,
    /*! @brief Define CCM mode */
    SCL_AES_CCM = 6
} scl_aes_mode_t;

typedef hash_mode_t scl_hash_mode_t;

/** @}*/

#endif /* SCL_DEFS_H */
