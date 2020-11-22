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
 * @file hca_sha.c
 * @brief
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <stdint.h>
#include <stdio.h>

#include <scl/scl_retdefs.h>

#include <api/hash/sha.h>

#include <metal/io.h>
#include <metal/machine/platform.h>

#include <api/hardware/scl_hca.h>
#include <api/hardware/hca_macro.h>

#if METAL_SIFIVE_HCA_VERSION >= HCA_VERSION(0, 5, 0)
#include <api/hardware/v0.5/sifive_hca-0.5.x.h>

#include <api/hardware/v0.5/hash/hca_sha.h>
#include <api/hardware/v0.5/hash/hca_sha224.h>
#include <api/hardware/v0.5/hash/hca_sha256.h>
#include <api/hardware/v0.5/hash/hca_sha384.h>
#include <api/hardware/v0.5/hash/hca_sha512.h>

int32_t hca_sha_init(const metal_scl_t *const scl, sha_ctx_t *const ctx,
                     hash_mode_t hash_mode, endianness_t data_endianness)
{

    if ((NULL == ctx) || (NULL == scl))
    {
        return (SCL_INVALID_INPUT);
    }

    if (0 == METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_SHA_REV))
    {
        // revision of SHA is Zero so the SHA is not present.
        return SCL_ERROR;
    }

    ctx->mode = hash_mode;

    switch (ctx->mode)
    {
    case SCL_HASH_SHA224:
        // same context than sha256
        ctx->ctx.sha256.bitlen = 0;
        break;
    case SCL_HASH_SHA256:
        ctx->ctx.sha256.bitlen = 0;
        break;
    case SCL_HASH_SHA384:
        // same context than sha512
        ctx->ctx.sha512.bitlen = 0;
        break;
    case SCL_HASH_SHA512:
        ctx->ctx.sha512.bitlen = 0;
        break;
    default:
        return (SCL_ERROR);
        break;
    }

    // Set HCA_MODE to SHA
    hca_setfield32(scl, METAL_SIFIVE_HCA_CR, SCL_HCA_SHA_MODE,
                   HCA_REGISTER_CR_IFIFOTGT_OFFSET,
                   HCA_REGISTER_CR_IFIFOTGT_MASK);

    // Set endianness
    hca_setfield32(scl, METAL_SIFIVE_HCA_CR, data_endianness,
                   HCA_REGISTER_CR_ENDIANNESS_OFFSET,
                   HCA_REGISTER_CR_ENDIANNESS_MASK);

    // Set SHA mode
    hca_setfield32(scl, METAL_SIFIVE_HCA_SHA_CR, hash_mode,
                   HCA_REGISTER_SHA_CR_MODE_OFFSET,
                   HCA_REGISTER_SHA_CR_MODE_MASK);

    // Init SHA
    hca_setfield32(scl, METAL_SIFIVE_HCA_SHA_CR, 1,
                   HCA_REGISTER_SHA_CR_INIT_OFFSET,
                   HCA_REGISTER_SHA_CR_INIT_MASK);

    return (SCL_OK);
}

int32_t hca_sha_core(const metal_scl_t *const scl, sha_ctx_t *const ctx,
                     const uint8_t *const data, size_t data_byte_len)
{
    if (NULL == ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    switch (ctx->mode)
    {
    case SCL_HASH_SHA224:
        // same core than sha256
        return (hca_sha256_core(scl, ctx, data, data_byte_len));
        break;
    case SCL_HASH_SHA256:
        return (hca_sha256_core(scl, ctx, data, data_byte_len));
        break;
    case SCL_HASH_SHA384:
        // same core than sha384
        return (hca_sha512_core(scl, ctx, data, data_byte_len));
        break;
    case SCL_HASH_SHA512:
        return (hca_sha512_core(scl, ctx, data, data_byte_len));
        break;
    default:
        return (SCL_ERROR);
        break;
    }

    // just in case
    return (SCL_ERROR);
}

int32_t hca_sha_finish(const metal_scl_t *const scl, sha_ctx_t *const ctx,
                       uint8_t *const hash, size_t *const hash_len)
{
    (void)scl;

    if (NULL == ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    switch (ctx->mode)
    {
    case SCL_HASH_SHA224:
        return (hca_sha224_finish(scl, ctx, hash, hash_len));
        break;
    case SCL_HASH_SHA256:
        return (hca_sha256_finish(scl, ctx, hash, hash_len));
        break;
    case SCL_HASH_SHA384:
        return (hca_sha384_finish(scl, ctx, hash, hash_len));
        break;
    case SCL_HASH_SHA512:
        return (hca_sha512_finish(scl, ctx, hash, hash_len));
        break;
    default:
        return (SCL_ERROR);
        break;
    }

    // just in case
    return (SCL_ERROR);
}
#endif  /* METAL_SIFIVE_HCA_VERSION >= HCA_VERSION(0, 5, 0) */