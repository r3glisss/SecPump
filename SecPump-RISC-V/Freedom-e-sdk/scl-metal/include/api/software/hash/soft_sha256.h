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
 * @file soft_sha256.h
 * @brief software sha256 implementation
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_SOFT_SHA256_H
#define SCL_BACKEND_SOFT_SHA256_H

#include <stdint.h>

#include <crypto_cfg.h>

#include <api/defs.h>
#include <api/hash/sha256.h>

/**
 * @addtogroup SOFTWARE
 * @addtogroup SOFT_SHA
 * @ingroup SOFTWARE
 *  @{
 */

/**
 * @brief Compute one SHA256 bock
 *
 * @param[in] ctx               sha256 context
 * @param[in] words             64 bytes block to be treated
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t soft_sha256_block(sha256_ctx_t *const ctx,
                                          const uint8_t *const words);

/**
 * @brief Init software sha256 context
 *
 * @param[out] ctx              sha256 context
 * @param[in] data_endianness   endianess of the input data
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 * @warning only SCL_BIG_ENDIAN_MODE is supported
 */
CRYPTO_FUNCTION int32_t soft_sha256_init(sha256_ctx_t *const ctx,
                                         endianness_t data_endianness);

/**
 * @brief Compute intermediate sha256 value of the chunk of data in parameter
 *
 * @param[in,out] ctx           sha256 context
 * @param[in] data              data to hash
 * @param[in] data_byte_len     data lengtth to hash
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t soft_sha256_core(sha256_ctx_t *const ctx,
                                         const uint8_t *const data,
                                         size_t data_byte_len);

/**
 * @brief Compute final hash value of the concatenated block pass to
 * soft_sha256_core()
 *
 * @param[in] ctx               sha256 context
 * @param[out] hash             hash output buffer
 * @param[in,out] hash_len      length of the hash buffer/length of the hash
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t soft_sha256_finish(sha256_ctx_t *const ctx,
                                           uint8_t *const hash,
                                           size_t *hash_len);

/**
 * @brief append 64 bits bit length to a block buffer to complete padding
 *
 * @param buffer    pointer on block buffer last 64 bits
 * @param length    pointer on bit length to copy on the block's 64 last bits
 */
CRYPTO_FUNCTION void soft_sha256_append_bit_len(uint8_t *const buffer,
                                                uint64_t *const length);

/** @}*/

#endif /* SCL_BACKEND_SOFT_SHA256_H */
