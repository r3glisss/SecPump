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
 * @file hca_sha224.h
 * @brief software sha224 implementation
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_HCA_SHA224_H
#define SCL_BACKEND_HCA_SHA224_H

#include <stdint.h>

#include <crypto_cfg.h>

#include <api/defs.h>
#include <api/hash/sha224.h>

/**
 * @addtogroup HCA
 * @addtogroup HCA_SHA
 * @ingroup HCA
 *  @{
 */

/**
 * @brief Compute final hash value of the concatenated block pass to
 * hca_sha256_core()
 *
 * @param[in] scl               metal scl context
 * @param[in] ctx               sha context
 * @param[out] hash             hash output buffer
 * @param[in,out] hash_len      length of the hash buffer/length of the hash
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t hca_sha224_finish(const metal_scl_t *const scl,
                                          sha_ctx_t *const ctx,
                                          uint8_t *const hash,
                                          size_t *const hash_len);

/**
 * @brief Read hash result from HCA output FIFO
 *
 * @param[in] scl               metal scl context
 * @param[out] data_out         output buffer
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 * @warning data_out buffer should be long enough to contain SHA output
 * (28 bytes)
 */
CRYPTO_FUNCTION int32_t hca_sha224_read(const metal_scl_t *const scl,
                                        uint8_t *const data_out);

/** @}*/

#endif /* SCL_BACKEND_HCA_SHA224_H */
