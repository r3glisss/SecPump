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
 * @file soft_sha.c
 * @brief software sha implementation
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <scl/scl_retdefs.h>

#include <api/hash/sha.h>
#include <api/software/hash/soft_sha.h>

#include <api/software/hash/soft_sha224.h>
#include <api/software/hash/soft_sha256.h>
#include <api/software/hash/soft_sha384.h>
#include <api/software/hash/soft_sha512.h>

int32_t soft_sha_init(const metal_scl_t *const scl, sha_ctx_t *const ctx,
                      hash_mode_t hash_mode, endianness_t data_endianness)
{
    (void)scl;

    if (NULL == ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    ctx->mode = hash_mode;

    switch (ctx->mode)
    {
    case SCL_HASH_SHA224:
        return (soft_sha224_init(&(ctx->ctx.sha224), data_endianness));
        break;
    case SCL_HASH_SHA256:
        return (soft_sha256_init(&(ctx->ctx.sha256), data_endianness));
        break;
    case SCL_HASH_SHA384:
        return (soft_sha384_init(&(ctx->ctx.sha384), data_endianness));
        break;
    case SCL_HASH_SHA512:
        return (soft_sha512_init(&(ctx->ctx.sha512), data_endianness));
        break;
    default:
        return (SCL_ERROR);
        break;
    }

    // just in case
    return (SCL_ERROR);
}

int32_t soft_sha_core(const metal_scl_t *const scl, sha_ctx_t *const ctx,
                      const uint8_t *const data, size_t data_byte_len)
{
    (void)scl;

    if (NULL == ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    switch (ctx->mode)
    {
    case SCL_HASH_SHA224:
        return (soft_sha224_core(&(ctx->ctx.sha224), data, data_byte_len));
        break;
    case SCL_HASH_SHA256:
        return (soft_sha256_core(&(ctx->ctx.sha256), data, data_byte_len));
        break;
    case SCL_HASH_SHA384:
        return (soft_sha384_core(&(ctx->ctx.sha384), data, data_byte_len));
        break;
    case SCL_HASH_SHA512:
        return (soft_sha512_core(&(ctx->ctx.sha512), data, data_byte_len));
        break;
    default:
        return (SCL_ERROR);
        break;
    }

    // just in case
    return (SCL_ERROR);
}

int32_t soft_sha_finish(const metal_scl_t *const scl, sha_ctx_t *const ctx,
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
        return (soft_sha224_finish(&(ctx->ctx.sha224), hash, hash_len));
        break;
    case SCL_HASH_SHA256:
        return (soft_sha256_finish(&(ctx->ctx.sha256), hash, hash_len));
        break;
    case SCL_HASH_SHA384:
        return (soft_sha384_finish(&(ctx->ctx.sha384), hash, hash_len));
        break;
    case SCL_HASH_SHA512:
        return (soft_sha512_finish(&(ctx->ctx.sha512), hash, hash_len));
        break;
    default:
        return (SCL_ERROR);
        break;
    }

    // just in case
    return (SCL_ERROR);
}
