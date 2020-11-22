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
 * @file soft_sha384.c
 * @brief software sha384 implementation
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <api/software/hash/soft_sha512.h>

#include <string.h>

#include <scl/scl_retdefs.h>

#include <api/macro.h>
#include <api/utils.h>

#include <api/software/hash/soft_sha384.h>
#include <api/software/hash/soft_sha512.h>

static const uint64_t h_init[SHA512_BYTE_HASHSIZE] = {
    0xCBBB9D5DC1059ED8ULL, 0x629A292A367CD507ULL, 0x9159015A3070DD17ULL,
    0x152FECD8F70E5939ULL, 0x67332667FFC00B31ULL, 0x8EB44A8768581511ULL,
    0xDB0C2E0D64F98FA7ULL, 0x47B5481DBEFA4FA4ULL};

int32_t soft_sha384_init(sha384_ctx_t *const ctx, endianness_t data_endianness)
{
    size_t i = 0;

    if (NULL == ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    // for the moment only big endian mode is supported
    if (SCL_BIG_ENDIAN_MODE != data_endianness)
    {
        return (SCL_INVALID_MODE);
    }

    ctx->bitlen = 0;

    for (i = 0; i < SHA512_SIZE_WORDS; i++)
    {
        ctx->h[i] = h_init[i];
    }

    return (SCL_OK);
}

int32_t soft_sha384_core(sha384_ctx_t *const ctx, const uint8_t *const data,
                         size_t data_byte_len)
{
    return (soft_sha512_core(ctx, data, data_byte_len));
}

int32_t soft_sha384_finish(sha384_ctx_t *const ctx, uint8_t *const hash,
                           size_t *const hash_len)
{
    size_t block_buffer_index;
    size_t block_remain;

    if ((NULL == hash) || (NULL == hash_len))
    {
        return (SCL_INVALID_OUTPUT);
    }

    if (NULL == ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    if (*hash_len < SHA384_BYTE_HASHSIZE)
    {
        return (SCL_INVALID_OUTPUT);
    }

    // currently used nb of bytes in the block buffer
    block_buffer_index = (size_t)((ctx->bitlen >> 3) % SHA512_BYTE_BLOCKSIZE);

    // add end of message
    ctx->block_buffer[block_buffer_index] = 0x80;

    block_buffer_index++;

    // compute the free remaining space in the block buffer (64-byte long)
    block_remain = SHA512_BYTE_BLOCKSIZE - block_buffer_index;

    if (block_remain >= SHA512_BYTE_SIZE_BLOCKSIZE)
    {
        memset(&ctx->block_buffer[block_buffer_index], 0, block_remain);
        block_buffer_index += block_remain - SHA512_BYTE_SIZE_BLOCKSIZE;
        soft_sha512_append_bit_len(&ctx->block_buffer[block_buffer_index],
                                   &ctx->bitlen);
        // this block is now complete,so it can be processed
        soft_sha512_block(ctx, ctx->block_buffer);
    }
    else
    {
        if (block_remain != 0)
        {
            memset(&ctx->block_buffer[block_buffer_index], 0, block_remain);
        }
        block_buffer_index = 0;
        block_remain = SHA512_BYTE_BLOCKSIZE;
        // this block is now complete,so it can be processed
        soft_sha512_block(ctx, ctx->block_buffer);

        memset(&ctx->block_buffer[block_buffer_index], 0, block_remain);

        block_buffer_index += block_remain - SHA512_BYTE_SIZE_BLOCKSIZE;
        soft_sha512_append_bit_len(&ctx->block_buffer[block_buffer_index],
                                   &ctx->bitlen);
        // this block is now complete,so it can be processed
        soft_sha512_block(ctx, ctx->block_buffer);
    }

    // retrieving the hash result
    copy_u64_2_u8_be(hash, ctx->h, SHA384_BYTE_HASHSIZE);
    *hash_len = SHA384_BYTE_HASHSIZE;

    return (SCL_OK);
}
