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
 * @file soft_sha512.c
 * @brief software sha512 implementation
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <string.h>

#include <scl/scl_retdefs.h>

#include <api/macro.h>
#include <api/utils.h>

#include <api/software/hash/soft_sha512.h>

#define ROTR_512(x, n) (((x) >> (n)) | ((x) << (64 - n)))
#define SHR_512(x, n) ((x) >> (n))
#define CH_512(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ_512(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define CSIGMA0_512(x)                                                         \
    (((ROTR_512(x, 28)) ^ (ROTR_512(x, 34)) ^ (ROTR_512(x, 39))))
#define CSIGMA1_512(x)                                                         \
    (((ROTR_512(x, 14)) ^ (ROTR_512(x, 18)) ^ (ROTR_512(x, 41))))
#define SIGMA0_512(x) (((ROTR_512(x, 1)) ^ (ROTR_512(x, 8)) ^ (SHR_512(x, 7))))
#define SIGMA1_512(x) ((ROTR_512(x, 19) ^ ROTR_512(x, 61) ^ SHR_512(x, 6)))

static const uint64_t k[SHA512_ROUNDS_NUMBER] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL,
    0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
    0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL,
    0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
    0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
    0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL,
    0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL,
    0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL,
    0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL,
    0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
    0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL,
    0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL,
    0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
    0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL,
    0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL,
    0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
    0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL,
    0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL,
    0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
    0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL};

static const uint64_t h_init[SHA512_BYTE_HASHSIZE] = {
    0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL,
    0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
    0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL};

int32_t soft_sha512_block(sha512_ctx_t *ctx, const uint8_t *const words)
{
    size_t i;
    uint64_t w[SHA512_ROUNDS_NUMBER];
    uint64_t a, b, c, d, e, f, g, h, t1, t2;

    if (NULL == ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    copy_u8_2_u64_be(w, words, SHA512_BYTE_BLOCKSIZE);

    for (i = SHA512_BLOCK_WORDS; i < SHA512_ROUNDS_NUMBER; i++)
    {
        w[i] =
            SIGMA1_512(w[i - 2]) + w[i - 7] + SIGMA0_512(w[i - 15]) + w[i - 16];
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
    for (i = 0; i < SHA512_ROUNDS_NUMBER; i++)
    {
        t1 = h + CSIGMA1_512(e) + CH_512(e, f, g) + k[i] + w[i];
        t2 = CSIGMA0_512(a) + MAJ_512(a, b, c);
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

int32_t soft_sha512_init(sha512_ctx_t *const ctx, endianness_t data_endianness)
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

int32_t soft_sha512_core(sha512_ctx_t *const ctx, const uint8_t *const data,
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
    block_buffer_index = (size_t)((ctx->bitlen >> 3) % SHA512_BYTE_BLOCKSIZE);

    // compute the free remaining space in the block buffer (64-byte long)
    block_remain = SHA512_BYTE_BLOCKSIZE - block_buffer_index;

    ctx->bitlen += (uint64_t)(data_byte_len << 3);

    // if the input data size is larger than the block remaining size we'll be
    // able to process at least one block
    if (data_byte_len >= block_remain)
    {
        // we can add data,starting at the first available position in the block
        // buffer
        memcpy(&ctx->block_buffer[block_buffer_index], data, block_remain);

        // this block is now complete,so it can be processed
        soft_sha512_block(ctx, ctx->block_buffer);
        // block has been fully processed,so block buffer is empty
        block_buffer_index = 0;
        // processing full blocks as long as data are available
        for (data_index = block_remain;
             data_index + SHA512_BYTE_BLOCKSIZE - 1 < data_byte_len;
             data_index += SHA512_BYTE_BLOCKSIZE)
        {
            soft_sha512_block(ctx, &data[data_index]);
        }
    }
    // copying the remaining 'data' bytes to the block buffer
    memcpy(&ctx->block_buffer[block_buffer_index], &data[data_index],
           data_byte_len - data_index);

    return (SCL_OK);
}

int32_t soft_sha512_finish(sha512_ctx_t *const ctx, uint8_t *const hash,
                           size_t *hash_len)
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

    if (*hash_len < SHA512_BYTE_HASHSIZE)
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
    copy_u64_2_u8_be(hash, ctx->h, SHA512_BYTE_HASHSIZE);
    *hash_len = SHA512_BYTE_HASHSIZE;

    return (SCL_OK);
}

void soft_sha512_append_bit_len(uint8_t *const buffer, uint64_t *const length)
{
    size_t i;
    uint8_t *p_length = (uint8_t *)length;

    for (i = 0; i < sizeof(*length); i++)
    {
        buffer[SHA512_BYTE_SIZE_BLOCKSIZE - i - 1] = p_length[i];
    }
}
