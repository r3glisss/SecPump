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
 * @file hca_sha.h
 * @brief hardware sha implementation/wrapper
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 *
 */

#ifndef SCL_BACKEND_HCA_SHA_H
#define SCL_BACKEND_HCA_SHA_H

#include <stddef.h>
#include <stdint.h>

#include <crypto_cfg.h>

#include <api/defs.h>
#include <api/scl_api.h>

#include <scl/scl_retdefs.h>

/**
 * @addtogroup HCA
 * @addtogroup HCA_API_SHA
 * @ingroup HCA
 *  @{
 */

/**
 * @brief Init hardware sha context
 *
 * @param[in] scl               metal scl context
 * @param[out] ctx              sha context
 * @param[in] hash_mode         hash mode
 * @param[in] data_endianness   endianess of the input data
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t hca_sha_init(const metal_scl_t *const scl,
                                     sha_ctx_t *const ctx,
                                     hash_mode_t hash_mode,
                                     endianness_t data_endianness);

/**
 * @brief Compute intermediate hash value of the chunk of data in parameter
 *
 * @param[in] scl               metal scl context
 * @param[in,out] ctx           sha context
 * @param[in] data              data to hash
 * @param[in] data_byte_len     data length to hash
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t hca_sha_core(const metal_scl_t *const scl,
                                     sha_ctx_t *const ctx,
                                     const uint8_t *const data,
                                     size_t data_byte_len);

/**
 * @brief Compute final hash value of the concatenated block pass to
 * hca_sha_core()
 *
 * @param[in] scl               metal scl context
 * @param[in] ctx               sha context
 * @param[out] hash             hash output buffer
 * @param[in,out] hash_len      length of the hash buffer/length of the hash
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t hca_sha_finish(const metal_scl_t *const scl,
                                       sha_ctx_t *const ctx,
                                       uint8_t *const hash,
                                       size_t *const hash_len);

/** @}*/

#endif /* SCL_BACKEND_HCA_SHA_H */
