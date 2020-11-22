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
 * @file scl_sha.c
 * @brief implementation of the hash generic interface taking the hash function
 * algo reference as a parameter
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <scl/scl_retdefs.h>
#include <scl/scl_sha.h>

#include <api/scl_api.h>

int32_t scl_sha(const metal_scl_t *const scl_ctx, scl_hash_mode_t algo,
                const uint8_t *const data, size_t data_len, uint8_t *const hash,
                size_t *const hash_len)
{
    int32_t result;
    scl_sha_ctx_t ctx;

    if (NULL == scl_ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    result =
        scl_ctx->hash_func.sha_init(scl_ctx, &ctx, algo, SCL_BIG_ENDIAN_MODE);
    if (SCL_OK != result)
    {
        return (result);
    }

    result = scl_ctx->hash_func.sha_core(scl_ctx, &ctx, data, data_len);
    if (SCL_OK != result)
    {
        return (result);
    }

    result = scl_ctx->hash_func.sha_finish(scl_ctx, &ctx, hash, hash_len);
    if (SCL_OK != result)
    {
        return (result);
    }

    return (result);
}

int32_t scl_sha_init(const metal_scl_t *const scl_ctx, scl_sha_ctx_t *const ctx,
                     scl_hash_mode_t algo)
{
    if (NULL == scl_ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    return (
        scl_ctx->hash_func.sha_init(scl_ctx, ctx, algo, SCL_BIG_ENDIAN_MODE));
}

int32_t scl_sha_core(const metal_scl_t *const scl_ctx, scl_sha_ctx_t *const ctx,
                     const uint8_t *const data, size_t data_len)
{
    if (NULL == scl_ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    return (scl_ctx->hash_func.sha_core(scl_ctx, ctx, data, data_len));
}

int32_t scl_sha_finish(const metal_scl_t *const scl_ctx,
                       scl_sha_ctx_t *const ctx, uint8_t *const hash,
                       size_t *const hash_len)
{
    if (NULL == scl_ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    return (scl_ctx->hash_func.sha_finish(scl_ctx, ctx, hash, hash_len));
}

/**
 * this function is used to determine if a proposed integer is a valide hash
 * digest length it is used in ECDSA for checking
 */
int32_t scl_valid_hash_digest_length(size_t inputlength)
{
    switch (inputlength)
    {
    case SHA256_BYTE_HASHSIZE:
    case SHA384_BYTE_HASHSIZE:
    case SHA512_BYTE_HASHSIZE:
        return (SCL_OK);
    default:
        return (SCL_INVALID_INPUT);
    }

    return (SCL_INVALID_INPUT);
}
