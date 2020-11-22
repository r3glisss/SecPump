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
 * @file hca_aes.h
 * @brief hardware aes implementation/wrapper
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_HCA_AES_H
#define SCL_BACKEND_HCA_AES_H

#include <stddef.h>
#include <stdint.h>

#include <crypto_cfg.h>

#include <api/defs.h>
#include <api/scl_api.h>

#include <scl/scl_retdefs.h>

/**
 * @addtogroup HCA
 * @addtogroup HCA_API_AES
 * @ingroup HCA
 *  @{
 */

/**
 * @brief set AES key
 *
 * @param[in] scl               metal scl context @ref metal_scl_t
 * @param[in] type              key type (128, 192, 256) @ref scl_aes_key_type_t
 * @param[in] key               key value (256 bits in any case)
 * @param[in] aes_process       aes process (encrypt or decrypt)
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t hca_aes_setkey(const metal_scl_t *const scl,
                                       scl_aes_key_type_t type,
                                       const uint64_t *const key,
                                       scl_process_t aes_process);

/**
 * @brief set IV (Initial Value)
 *
 * @param[in] scl               metal scl context @ref metal_scl_t
 * @param[in] iv                iv value (128 bits)
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t hca_aes_setiv(const metal_scl_t *const scl,
                                      const uint64_t *const iv);

/**
 * @brief perform AES cipher operation
 *
 * @param[in] scl               metal scl context @ref metal_scl_t
 * @param[in] aes_mode          AES mode @ref scl_aes_mode_t
 * @param[in] aes_process       AES process (encrypt or decrypt)
 * @param[in] data_endianness   endianess of the input data
 * @param[in] data_in           data to process
 * @param[in] data_len          length of the data to process (in byte)
 * @param[out] data_out         data output buffer
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t hca_aes_cipher(
    const metal_scl_t *const scl, scl_aes_mode_t aes_mode,
    scl_process_t aes_process, scl_endianness_t data_endianness,
    const uint8_t *const data_in, size_t data_len, uint8_t *const data_out);

/**
 * @brief initiliaze AES cipher with authentication operation
 *
 * @param[in] scl               metal scl context @ref metal_scl_t
 * @param[in,out] ctx           aes authenticate context
 * @param[in] aes_mode          AES mode @ref scl_aes_mode_t
 * @param[in] aes_process       AES process (encrypt or decrypt)
 * @param[in] data_endianness   endianess of the input data
 * @param[in] auth_option       option for the ccm mode
 * @param[in] aad               add data
 * @param[in] aad_byte_len      length of the add data (in bytes)
 * @param[in] payload_len       length of the payload data (in bytes) 
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t hca_aes_auth_init(
    const metal_scl_t *const scl, aes_auth_ctx_t *const ctx, 
    scl_aes_mode_t aes_mode, scl_process_t aes_process,
    scl_endianness_t data_endianness, uint32_t auth_option,
    const uint8_t *const aad, size_t aad_byte_len, uint64_t payload_len);

/**
 * @brief perform AES cipher with authentication operation
 *
 * @param[in] scl               metal scl context @ref metal_scl_t
 * @param[in,out] ctx           aes authenticate context
 * @param[in] payload           data payload to process
 * @param[in] payload_len       length of the current data payload to process (in bytes)
 * @param[out] data_out         data output buffer
 * @param[out] out_len          length of data (in bytes) write into output buffer
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t hca_aes_auth_core(const metal_scl_t *const scl, 
                                          aes_auth_ctx_t *const ctx,
                                          const uint8_t *const payload, 
                                          uint64_t payload_len, 
                                          uint8_t *const data_out, 
                                          size_t *const out_len);

/**
 * @brief finish AES cipher with authentication operation
 *
 * @param[in] scl               metal scl context @ref metal_scl_t
 * @param[in,out] ctx           aes authenticate context
 * @param[out] data_out         data output buffer to complete operation
 * @param[out] tag              tag output buffer (128 bits)
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t hca_aes_auth_finish(const metal_scl_t *const scl, 
                                            aes_auth_ctx_t *const ctx, 
                                            uint8_t *const data_out, 
                                            uint64_t *const tag);

/** @}*/

#endif /* SCL_BACKEND_HCA_AES_H */
