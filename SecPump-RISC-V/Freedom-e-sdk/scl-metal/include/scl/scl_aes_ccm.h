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
 * @file scl_aes_ccm.h
 * @brief defines the AES for the CCM mode.
 * AES is NIST FIPS-197
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_AES_CCM_H
#define SCL_AES_CCM_H

#include <stdint.h>
#include <stdio.h>

#include <scl_cfg.h>
#include <scl/scl_defs.h>
#include <scl/scl_retdefs.h>

#include <api/scl_api.h>
#include <api/blockcipher/aes/aes.h>

/**
 * \addtogroup SCL
 * \addtogroup SCL_AES
 * \ingroup SCL
 *  @{
 */

/**
 * @brief compute AES-CCM operation (according mode parameter) on the data in parameter and return result
 *
 * @param[in] scl_ctx           scl context
 * @param[out] tag              output tag buffer - result of AES-CCM operation
 * @param[in] tag_byte_len      length in bytes of tag
 * @param[out] dst              output buffer - result of AES-CCM operation
 * @param[in] pld               data to process
 * @param[in] pld_byte_len      length in bytes of data
 * @param[in] key               Key to use for the AES-CCM operation
 * @param[in] key_byte_len      length in bytes of key
 * @param[in] iv                Initial Vector (iv) use for the AES-CCM operation
 * @param[in] iv_byte_len       length in bytes of iv
 * @param[in] aad               Additional Authenticated Data (aad) use for the AES-CCM operation
 * @param[in] aad_byte_len      length in bytes of aad
 * @param[in] mode              type of operation @ref scl_process_t
 * @return 0    in case of SUCCESS
 * @return != 0 in case of errors @ref scl_errors_t
 */
SCL_FUNCTION int32_t scl_aes_ccm(const metal_scl_t *const scl_ctx,
                                 uint8_t *const tag, size_t tag_byte_len, 
                                 uint8_t *const dst, const uint8_t *const pld,
                                 uint64_t pld_byte_len, const uint8_t *const key,
                                 size_t key_byte_len, const uint8_t *const iv,
                                 size_t iv_byte_len, const uint8_t *const aad, size_t aad_byte_len, scl_process_t mode);

/**
 * @brief Initialize AES-CCM context (according mode parameter)
 *
 * @param[in] scl_ctx           scl context
 * @param[in,out] ctx           AES authenticate context
 * @param[in] key               Key to use for the AES-CCM operation
 * @param[in] key_byte_len      length in bytes of key
 * @param[in] iv                Initial Vector (iv) use for the AES-CCM operation
 * @param[in] iv_byte_len       length in bytes of iv
 * @param[in] aad               Additional Authenticated Data (aad) use for the AES-CCM operation
 * @param[in] aad_byte_len      length in bytes of aad
 * @param[in] pld_byte_len      length in bytes of data payload
 * @param[in] tag_byte_len      length in bytes of tag returned
 * @param[in] mode              type of operation @ref scl_process_t
 * @return 0    in case of SUCCESS
 * @return != 0 in case of errors @ref scl_errors_t
 */
SCL_FUNCTION int32_t scl_aes_ccm_init(const metal_scl_t *const scl_ctx, aes_auth_ctx_t *const ctx,
                                      const uint8_t *const key,
                                      size_t key_byte_len, const uint8_t *const iv,
                                      size_t iv_byte_len, const uint8_t *const aad, size_t aad_byte_len,
                                      uint64_t pld_byte_len, size_t tag_byte_len, scl_process_t mode);

/**
 * @brief Perform intermediate AES-CCM comptation of data chunk
 *
 * @param[in] scl_ctx           scl context
 * @param[in,out] ctx           AES authenticate context
 * @param[out] dst              output buffer - result of AES-CCM operation
 * @param[out] dst_byte_len     length of data (in bytes) write into output buffer
 * @param[in] pld               data payload to process
 * @param[in] pld_byte_len      length in bytes of data payload
 * @return 0    in case of SUCCESS
 * @return != 0 in case of errors @ref scl_errors_t
 */
SCL_FUNCTION int32_t scl_aes_ccm_core(const metal_scl_t *const scl_ctx, aes_auth_ctx_t *const ctx,
                                      uint8_t *const dst, size_t *const dst_byte_len,
                                      const uint8_t *const pld, uint64_t pld_byte_len);

/**
 * @brief Finalize AES-CCM computation and returning Authenticate tag
 *
 * @param[in] scl_ctx           scl context
 * @param[in,out] ctx           AES authenticate context
 * @param[out] tag              output tag buffer - result of AES-CCM operation
 * @param[in] tag_byte_len      length in bytes of tag
 * @param[out] dst              output buffer - result of AES-CCM operation
 * @param[in] pld               data payload to process
 * @param[in] pld_byte_len      length in bytes of data payload
 */
SCL_FUNCTION int32_t scl_aes_ccm_finish(const metal_scl_t *const scl_ctx, aes_auth_ctx_t *const ctx,
                                 uint8_t *const tag, size_t tag_byte_len, uint8_t *const dst,
                                 const uint8_t *const pld, uint64_t pld_byte_len);

/** @}*/
 
#endif /* SCL_AES_CCM_H */
