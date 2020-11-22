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
 * @file scl_aes_ofb.h
 * @brief defines the AES for the CBC mode.
 * AES is NIST FIPS-197
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_AES_OFB_H
#define SCL_AES_OFB_H

#include <stdint.h>
#include <stdio.h>

#include <scl/scl_defs.h>
#include <scl/scl_retdefs.h>
#include <scl_cfg.h>

#include <api/blockcipher/aes/aes.h>
#include <api/scl_api.h>

/**
 * @addtogroup SCL
 * @addtogroup SCL_AES
 * @ingroup SCL
 *  @{
 */

/**
 * @brief compute AES-OFB operation (according mode parameter) on the data in
 * parameter and return result
 *
 * @param[in] scl_ctx           scl context
 * @param[out] dst              output buffer - result of AES-OFB operation
 * @param[in] src               data to process
 * @param[in] src_byte_len      length in bytes of data
 * @param[in] key               Key to use for the AES-OFB operation
 * @param[in] key_byte_len      length in bytes of key
 * @param[in] iv                Initial Vector (iv) use for the AES-OFB
 * operation
 * @param[in] iv_byte_len       length in bytes of iv
 * @param[in] mode              type of operation @ref scl_process_t
 * @return 0    in case of SUCCESS
 * @return != 0 in case of errors @ref scl_errors_t
 */
SCL_FUNCTION int32_t scl_aes_ofb(const metal_scl_t *const scl_ctx,
                                 uint8_t *const dst, const uint8_t *const src,
                                 size_t src_byte_len, const uint8_t *const key,
                                 size_t key_byte_len, const uint8_t *const iv,
                                 size_t iv_byte_len, scl_process_t mode);

/**
 * @brief Initialize AES-OFB context (according mode parameter)
 *
 * @param[in] scl_ctx           scl context
 * @param[in] key               Key to use for the AES-OFB operation
 * @param[in] key_byte_len      length in bytes of key
 * @param[in] iv                Initial Vector (iv) use for the AES-OFB
 * operation
 * @param[in] iv_byte_len       length in bytes of iv
 * @param[in] mode              type of operation @ref scl_process_t
 * @return 0    in case of SUCCESS
 * @return != 0 in case of errors @ref scl_errors_t
 */
SCL_FUNCTION int32_t scl_aes_ofb_init(const metal_scl_t *const scl_ctx,
                                      const uint8_t *const key,
                                      size_t key_byte_len,
                                      const uint8_t *const iv,
                                      size_t iv_byte_len, scl_process_t mode);

/**
 * @brief compute AES-OFB operation (according mode parameter) with current
 * AES-OFB context
 *
 * @param[in] scl_ctx           scl context
 * @param[out] dst              output buffer - result of AES-OFB operation
 * @param[in] src               data to process
 * @param[in] src_byte_len      length in bytes of data
 * @param[in] mode              type of operation @ref scl_process_t
 * @return 0    in case of SUCCESS
 * @return != 0 in case of errors @ref scl_errors_t
 */
SCL_FUNCTION int32_t scl_aes_ofb_core(const metal_scl_t *const scl_ctx,
                                      uint8_t *const dst,
                                      const uint8_t *const src,
                                      size_t src_byte_len, scl_process_t mode);

/** @}*/

#endif /* SCL_AES_OFB_H */
