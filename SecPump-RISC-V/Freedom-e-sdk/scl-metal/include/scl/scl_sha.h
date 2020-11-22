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
 * @file scl_sha.h
 * @brief defines the generic hash function interface, where the hash function
 * is transmitted as a parameter.
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_SHA_H
#define SCL_SHA_H

#include <stddef.h>
#include <stdint.h>

#include <scl_cfg.h>

#include <scl/scl_defs.h>
#include <scl/scl_retdefs.h>

#include <api/hash/sha.h>
#include <api/scl_api.h>

/**
 * @addtogroup SCL
 * @addtogroup SCL_SHA
 * @ingroup SCL
 *  @{
 */

/**
 * @brief SCL SHA context definition
 * @see sha_ctx_t
 */
typedef sha_ctx_t scl_sha_ctx_t;

/**
 * @brief compute SHA hash on the data inn parameter and return result
 *
 * @param[in] scl_ctx           scl context
 * @param[in] algo              hash algorithm to use
 * @param[in] data              data to hash
 * @param[in] data_byte_len     length in bytes of data to hash
 * @param[out] hash             hash output buffer
 * @param[in,out] hash_len      output buffer length / hash length
 * @return 0    in case of SUCCESS
 * @return != 0 in case of errors @ref scl_errors_t
 */
SCL_FUNCTION int32_t scl_sha(const metal_scl_t *const scl_ctx,
                             scl_hash_mode_t algo, const uint8_t *const data,
                             size_t data_byte_len, uint8_t *const hash,
                             size_t *const hash_len);

/**
 * @brief Initialize SHA computation
 *
 * @param[in] scl_ctx           scl context
 * @param[out] ctx              SHA context
 * @param[in] algo              hash algorithm to use
 * @return 0    in case of SUCCESS
 * @return != 0 in case of errors @ref scl_errors_t
 */
SCL_FUNCTION int32_t scl_sha_init(const metal_scl_t *const scl_ctx,
                                  scl_sha_ctx_t *const ctx,
                                  scl_hash_mode_t algo);

/**
 * @brief Perform intermediate sha comptation of data chunk
 *
 * @param[in] scl_ctx           scl context
 * @param[in] ctx               SHA context
 * @param[in] data              data to hash
 * @param[in] data_len     length in bytes of data to hash
 * @return 0    in case of SUCCESS
 * @return != 0 in case of errors @ref scl_errors_t
 */
SCL_FUNCTION int32_t scl_sha_core(const metal_scl_t *const scl_ctx,
                                  scl_sha_ctx_t *const ctx,
                                  const uint8_t *const data, size_t data_len);

/**
 * @brief Finalize SHA computation and returning result
 *
 * @param[in] scl_ctx           scl context
 * @param[in] ctx               SHA context
 * @param[out] hash             hash output buffer
 * @param[in,out] hash_len      output buffer length / hash length
 * @return 0    in case of SUCCESS
 * @return != 0 in case of errors @ref scl_errors_t
 */
SCL_FUNCTION int32_t scl_sha_finish(const metal_scl_t *const scl_ctx,
                                    scl_sha_ctx_t *const ctx,
                                    uint8_t *const hash,
                                    size_t *const hash_len);

/**
 * @brief   Check if the input length is a valid SHA length
 * @details  this function is used to determine if a proposed integer is a
 * valide hash digest length it is used in ECDSA for checking
 *
 * @param[in] inputlength              length in bytes to check
 * @return 0    in case of SUCCESS
 * @return != 0 in case of errors @ref scl_errors_t
 */
SCL_FUNCTION int32_t scl_valid_hash_digest_length(size_t inputlength);

/** @}*/

#endif /* SCL_SHA_H */
