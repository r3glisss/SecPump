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
 * @file soft_sha224.c
 * @brief software sha224 implementation
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <string.h>

#include <scl/scl_retdefs.h>

#include <api/macro.h>
#include <api/utils.h>

#include <api/software/hash/soft_sha224.h>
#include <api/software/hash/soft_sha256.h>

static const uint32_t h_init[SHA256_SIZE_WORDS] = {
    0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
    0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4};

int32_t soft_sha224_init(sha224_ctx_t *const ctx, endianness_t data_endianness)
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

    for (i = 0; i < SHA256_SIZE_WORDS; i++)
    {
        ctx->h[i] = h_init[i];
    }

    return (SCL_OK);
}

int32_t soft_sha224_core(sha224_ctx_t *const ctx, const uint8_t *const data,
                         size_t data_byte_len)
{
    return (soft_sha256_core(ctx, data, data_byte_len));
}

int32_t soft_sha224_finish(sha224_ctx_t *const ctx, uint8_t *const hash,
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

    if (*hash_len < SHA224_BYTE_HASHSIZE)
    {
        return (SCL_INVALID_OUTPUT);
    }

    // currently used nb of bytes in the block buffer
    block_buffer_index = (size_t)((ctx->bitlen >> 3) % SHA256_BYTE_BLOCKSIZE);

    // add end of message
    ctx->block_buffer[block_buffer_index] = 0x80;

    block_buffer_index++;

    // compute the free remaining space in the block buffer (64-byte long)
    block_remain = SHA256_BYTE_BLOCKSIZE - block_buffer_index;

    if (block_remain >= SHA256_BYTE_SIZE_BLOCKSIZE)
    {
        memset(&ctx->block_buffer[block_buffer_index], 0, block_remain);
        block_buffer_index += block_remain - SHA256_BYTE_SIZE_BLOCKSIZE;
        soft_sha256_append_bit_len(&ctx->block_buffer[block_buffer_index],
                                   &ctx->bitlen);
        // this block is now complete,so it can be processed
        soft_sha256_block(ctx, ctx->block_buffer);
    }
    else
    {
        if (block_remain != 0)
        {
            memset(&ctx->block_buffer[block_buffer_index], 0, block_remain);
        }
        block_buffer_index = 0;
        block_remain = SHA256_BYTE_BLOCKSIZE;
        // this block is now complete,so it can be processed
        soft_sha256_block(ctx, ctx->block_buffer);

        memset(&ctx->block_buffer[block_buffer_index], 0, block_remain);

        block_buffer_index += block_remain - SHA256_BYTE_SIZE_BLOCKSIZE;
        soft_sha256_append_bit_len(&ctx->block_buffer[block_buffer_index],
                                   &ctx->bitlen);
        // this block is now complete,so it can be processed
        soft_sha256_block(ctx, ctx->block_buffer);
    }

    // retrieving the hash result
    copy_u32_2_u8_be(hash, ctx->h, SHA224_BYTE_HASHSIZE);
    *hash_len = SHA224_BYTE_HASHSIZE;

    return (SCL_OK);
}
