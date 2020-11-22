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
 * @file scl_aes.h
 * @brief defines the AES.
 * AES is NIST FIPS-197
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_AES_H
#define SCL_AES_H

#include <stddef.h>
#include <stdint.h>

#include <scl_cfg.h>

#include <scl/scl_defs.h>
#include <scl/scl_retdefs.h>

#include <api/blockcipher/aes/aes.h>
#include <api/scl_api.h>

/**
 * @addtogroup SCL
 * @addtogroup SCL_AES
 * @ingroup SCL
 *  @{
 */

SCL_FUNCTION int32_t scl_aes(const metal_scl_t *const scl_ctx, uint8_t *dst,
                             uint8_t *src, uint8_t *key, int key_byte_len,
                             scl_process_t mode);

/** @}*/

#endif /* SCL_AES_H */
