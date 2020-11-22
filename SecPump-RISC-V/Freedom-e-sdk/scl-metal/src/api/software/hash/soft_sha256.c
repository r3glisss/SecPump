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
 * @file soft_sha256.c
 * @brief software sha256 implementation
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <string.h>

#include <scl/scl_retdefs.h>

#include <api/macro.h>
#include <api/utils.h>

#include <api/software/hash/soft_sha256.h>

#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - n)))
#define SHR(x, n) ((x) >> (n))
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SIGMA0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SIGMA1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define GAMMA0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3))
#define GAMMA1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

static const uint32_t k[SHA256_ROUNDS_NUMBER] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

static const uint32_t h_init[SHA256_SIZE_WORDS] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

int32_t soft_sha256_block(sha256_ctx_t *const ctx, const uint8_t *const words)
{
    size_t i;
    uint32_t w[SHA256_ROUNDS_NUMBER];
    uint32_t a, b, c, d, e, f, g, h, t1, t2;

    if (NULL == ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    copy_u8_2_u32_be(w, words, SHA256_BYTE_BLOCKSIZE);

    for (i = SHA256_BLOCK_WORDS; i < SHA256_ROUNDS_NUMBER; i++)
    {
        w[i] = GAMMA1(w[i - 2]) + w[i - 7] + GAMMA0(w[i - 15]) + w[i - 16];
    }

    // 2.
    a = ctx->h[0];
    b = ctx->h[1];
    c = ctx->h[2];
    d = ctx->h[3];
    e = ctx->h[4];
    f = ctx->h[5];
    g = ctx->h[6];
    h = ctx->h[7];

    // 3.
    for (i = 0; i < SHA256_ROUNDS_NUMBER; i++)
    {
        t1 = h + SIGMA1(e) + CH(e, f, g) + k[i] + w[i];
        t2 = SIGMA0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    // 4.
    ctx->h[0] += a;
    ctx->h[1] += b;
    ctx->h[2] += c;
    ctx->h[3] += d;
    ctx->h[4] += e;
    ctx->h[5] += f;
    ctx->h[6] += g;
    ctx->h[7] += h;

    return (SCL_OK);
}

int32_t soft_sha256_init(sha256_ctx_t *const ctx, endianness_t data_endianness)
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

int32_t soft_sha256_core(sha256_ctx_t *const ctx, const uint8_t *const data,
                         size_t data_byte_len)
{
    size_t block_buffer_index;
    size_t block_remain;
    size_t data_index = 0;

    if (NULL == ctx || NULL == data)
    {
        return (SCL_INVALID_INPUT);
    }

    // currently used nb of bytes in the block buffer
    block_buffer_index = (size_t)((ctx->bitlen >> 3) % SHA256_BYTE_BLOCKSIZE);

    // compute the free remaining space in the block buffer (64-byte long)
    block_remain = SHA256_BYTE_BLOCKSIZE - block_buffer_index;

    ctx->bitlen += (data_byte_len << 3);

    // if the input data size is larger than the block remaining size we'll be
    // able to process at least one block
    if (data_byte_len >= block_remain)
    {
        // we can add data,starting at the first available position in the block
        // buffer
        memcpy(&ctx->block_buffer[block_buffer_index], data, block_remain);

        // this block is now complete,so it can be processed
        soft_sha256_block(ctx, ctx->block_buffer);
        // block has been fully processed,so block buffer is empty
        block_buffer_index = 0;
        // processing full blocks as long as data are available
        for (data_index = block_remain;
             data_index + SHA256_BYTE_BLOCKSIZE - 1 < data_byte_len;
             data_index += SHA256_BYTE_BLOCKSIZE)
        {
            soft_sha256_block(ctx, &data[data_index]);
        }
    }
    // copying the remaining 'data' bytes to the block buffer
    memcpy(&ctx->block_buffer[block_buffer_index], &data[data_index],
           data_byte_len - data_index);

    return (SCL_OK);
}

int32_t soft_sha256_finish(sha256_ctx_t *const ctx, uint8_t *const hash,
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

    if (*hash_len < SHA256_BYTE_HASHSIZE)
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
    copy_u32_2_u8_be(hash, ctx->h, SHA256_BYTE_HASHSIZE);
    *hash_len = SHA256_BYTE_HASHSIZE;

    return (SCL_OK);
}

void soft_sha256_append_bit_len(uint8_t *const buffer, uint64_t *const length)
{
    size_t i;
    uint8_t *p_length = (uint8_t *)length;

    for (i = 0; i < sizeof(*length); i++)
    {
        buffer[SHA256_BYTE_SIZE_BLOCKSIZE - i - 1] = p_length[i];
    }
}
