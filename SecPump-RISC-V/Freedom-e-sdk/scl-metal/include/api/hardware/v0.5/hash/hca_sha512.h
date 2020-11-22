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
 * @file hca_sha512.h
 * @brief software sha512 implementation
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_HCA_SHA512_H
#define SCL_BACKEND_HCA_SHA512_H

#include <stdint.h>

#include <crypto_cfg.h>

#include <api/defs.h>
#include <api/hash/sha512.h>

/**
 * @addtogroup HCA
 * @addtogroup HCA_SHA
 * @ingroup HCA
 *  @{
 */

/**
 * @brief Compute intermediate sha384/sha512 value of the chunk of data in
 * parameter
 *
 * @param[in] scl               metal scl context
 * @param[in,out] ctx           sha context
 * @param[in] data              data to hash
 * @param[in] data_byte_len     data length to hash
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t hca_sha512_core(const metal_scl_t *const scl,
                                        sha_ctx_t *const ctx,
                                        const uint8_t *const data,
                                        size_t data_byte_len);

/**
 * @brief Compute final hash value of the concatenated block pass to
 * hca_sha512_core()
 *
 * @param[in] scl               metal scl context
 * @param[in] ctx               sha context
 * @param[out] hash             hash output buffer
 * @param[in,out] hash_len      length of the hash buffer/length of the hash
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t hca_sha512_finish(const metal_scl_t *const scl,
                                          sha_ctx_t *const ctx,
                                          uint8_t *const hash,
                                          size_t *const hash_len);

/**
 * @brief append 128 bits bit length to a block buffer to complete padding
 *
 * @param buffer    pointer on block buffer last 128 bits
 * @param length    pointer on bit length to copy on the block's 128 last bits
 */
CRYPTO_FUNCTION void hca_sha512_append_bit_len(uint8_t *const buffer,
                                               uint64_t *const length);

/**
 * @brief Read hash result from HCA output FIFO
 *
 * @param[in] scl               metal scl context
 * @param[out] data_out         output buffer
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 * @warning data_out buffer should be long enough to contain SHA output
 * (64 bytes)
 */
CRYPTO_FUNCTION int32_t hca_sha512_read(const metal_scl_t *const scl,
                                        uint8_t *const data_out);

/** @}*/

#endif /* SCL_BACKEND_HCA_SHA512_H */
