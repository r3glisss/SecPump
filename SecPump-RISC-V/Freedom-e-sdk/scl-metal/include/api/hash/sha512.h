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
 * @file sha512.h
 * @brief sha512 implementation/wrapper
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_SHA512_H
#define SCL_BACKEND_SHA512_H

#include <stddef.h>
#include <stdint.h>

/** 
 * @addtogroup COMMON
 * @addtogroup SHA
 * @ingroup COMMON
 *  @{
 */

/*! @brief SHA512 block size */
#define SHA512_BYTE_BLOCKSIZE 128
/*! @brief Size of SHA512 in bytes */
#define SHA512_BYTE_HASHSIZE 64
/*! @brief number of rounds per SHA512 block computation */
#define SHA512_ROUNDS_NUMBER 80
/*! @brief number of words (64 bits) in hash */
#define SHA512_SIZE_WORDS 8
/*! @brief number of word (64 bits) in one block */
#define SHA512_BLOCK_WORDS 16
/*! @brief The nb of bytes for storing the size in the last block */
#define SHA512_BYTE_SIZE_BLOCKSIZE 16

/*! @brief SHA 256 context */
typedef struct
{
    /*! @brief Initial, intermediate and then final hash. */
    uint64_t h[SHA512_SIZE_WORDS];
    /*! @brief bit len */
    uint64_t bitlen;
    /*! @brief  block buffer */
    uint8_t block_buffer[SHA512_BYTE_BLOCKSIZE] __attribute__((aligned(8)));
} sha512_ctx_t;

/** @}*/

#endif /* SCL_BACKEND_SHA512_H */
