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
 * @file hca_sha_miscellaneous.h
 * @brief hardware sha implementation/wrapper
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 *
 */

#ifndef SCL_BACKEND_HCA_SHA_MISCELLANEOUS_H
#define SCL_BACKEND_HCA_SHA_MISCELLANEOUS_H

#include <stddef.h>
#include <stdint.h>

#include <crypto_cfg.h>

#include <api/scl_api.h>

#include <api/hash/sha.h>
#include <scl/scl_retdefs.h>

/**
 * @addtogroup HCA
 * @addtogroup HCA_SHA
 * @ingroup HCA
 *  @{
 */

/**
 * @brief Compute one or more 512 blocks using HCA
 *
 * @param[in] scl               metal scl context
 * @param[in] hash_mode         hash mode
 * @param[in] NbBlocks512       number of 512 bits blocks
 * @param[in] data_in           Pointer on data to hash
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 * @note For SHA384 and SHA512 the number of blocks should be a multiple of 2
 */
CRYPTO_FUNCTION int32_t hca_sha_block(const metal_scl_t *const scl,
                                      hash_mode_t hash_mode,
                                      uint32_t NbBlocks512,
                                      const uint8_t *const data_in);

/** @}*/

#endif /* SCL_BACKEND_HCA_SHA_MISCELLANEOUS_H */
