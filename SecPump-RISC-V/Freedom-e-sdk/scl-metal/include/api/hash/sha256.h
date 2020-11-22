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
 * @file sha256.h
 * @brief sha256 implementation/wrapper
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_SHA256_H
#define SCL_BACKEND_SHA256_H

#include <stddef.h>
#include <stdint.h>

/** 
 * @addtogroup COMMON
 * @addtogroup SHA
 * @ingroup COMMON
 *  @{
 */

/*! @brief SHA256 block size */
#define SHA256_BYTE_BLOCKSIZE 64
/*! @brief Size of SHA256 in bytes */
#define SHA256_BYTE_HASHSIZE 32
/*! @brief number of rounds per SHA256 block computation */
#define SHA256_ROUNDS_NUMBER 64
/*! @brief number of words (32 bits) in hash */
#define SHA256_SIZE_WORDS 8
/*! @brief number of word in one block */
#define SHA256_BLOCK_WORDS 16
/*! @brief the nb of bytes for storing the size in the last block */
#define SHA256_BYTE_SIZE_BLOCKSIZE 8

/*! @brief SHA 256 context */
typedef struct
{
    /*! @brief intermediate state and then final hash */
    uint32_t h[SHA256_SIZE_WORDS];
    /*! @brief total bits length computed */
    uint64_t bitlen;
    /*! @brief block buffer */
    uint8_t block_buffer[SHA256_BYTE_BLOCKSIZE] __attribute__((aligned(4)));
} sha256_ctx_t;

/** @}*/

#endif /* SCL_BACKEND_SHA256_H */
