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
 * @file scl_hca.h
 * @brief
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_SCL_HCA_H
#define SCL_BACKEND_SCL_HCA_H

#include <stdint.h>
#include <stdio.h>

#include <metal/io.h>
#include <metal/machine/platform.h>

#include <api/hardware/hca_macro.h>

/**
 * @addtogroup HCA
 *  @{
 */

typedef enum
{
    SCL_HCA_AES_MODE = 0,
    SCL_HCA_SHA_MODE = 1
} scl_hca_mode_t;

#define HCA_VERSION(a, b, c) (((a) << 16) + ((b) << 8) + (c))

#if METAL_SIFIVE_HCA_VERSION >= HCA_VERSION(0, 5, 0)
#include <api/hardware/v0.5/blockcipher/aes/hca_aes.h>
#include <api/hardware/v0.5/hash/hca_sha.h>
#include <api/hardware/v0.5/random/hca_trng.h>
#include <api/hardware/v0.5/sifive_hca-0.5.x.h>
#endif /* METAL_SIFIVE_HCA_VERSION */

/** @}*/

#endif /* SCL_BACKEND_SCL_HCA_H */
