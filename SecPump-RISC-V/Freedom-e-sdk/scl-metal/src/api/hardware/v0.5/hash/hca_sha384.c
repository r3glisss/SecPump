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
 * @file hca_sha384.c
 * @brief
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <string.h>

#include <metal/io.h>
#include <metal/machine/platform.h>

#include <scl/scl_retdefs.h>

#include <api/macro.h>
#include <api/utils.h>

#include <api/hardware/scl_hca.h>
#include <api/hardware/hca_macro.h>

#if METAL_SIFIVE_HCA_VERSION >= HCA_VERSION(0, 5, 0)
#include <api/hardware/v0.5/hash/hca_sha384.h>
#include <api/hardware/v0.5/hash/hca_sha512.h>
#include <api/hardware/v0.5/hash/hca_sha_miscellaneous.h>

int32_t hca_sha384_finish(const metal_scl_t *const scl, sha_ctx_t *const ctx,
                          uint8_t *const hash, size_t *const hash_len)
{
    size_t block_buffer_index;
    size_t block_remain;
    int32_t result;

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
    block_buffer_index =
        (size_t)((ctx->ctx.sha512.bitlen >> 3) % SHA512_BYTE_BLOCKSIZE);

    // add end of message
    ctx->ctx.sha512.block_buffer[block_buffer_index] = 0x80;

    block_buffer_index++;

    // compute the free remaining space in the block buffer (64-byte long)
    block_remain = SHA512_BYTE_BLOCKSIZE - block_buffer_index;

    if (block_remain >= SHA512_BYTE_SIZE_BLOCKSIZE)
    {
        memset(&ctx->ctx.sha512.block_buffer[block_buffer_index], 0,
               block_remain);
        block_buffer_index += block_remain - SHA512_BYTE_SIZE_BLOCKSIZE;
        hca_sha512_append_bit_len(
            &ctx->ctx.sha512.block_buffer[block_buffer_index],
            &ctx->ctx.sha512.bitlen);
        // this block is now complete,so it can be processed
        result = hca_sha_block(scl, ctx->mode, 2, ctx->ctx.sha512.block_buffer);
        if (SCL_OK != result)
        {
            return (result);
        }
    }
    else
    {
        if (block_remain != 0)
        {
            memset(&ctx->ctx.sha512.block_buffer[block_buffer_index], 0,
                   block_remain);
        }
        block_buffer_index = 0;
        block_remain = SHA512_BYTE_BLOCKSIZE;
        // this block is now complete,so it can be processed
        result = hca_sha_block(scl, ctx->mode, 2, ctx->ctx.sha512.block_buffer);
        if (SCL_OK != result)
        {
            return (result);
        }

        memset(&ctx->ctx.sha512.block_buffer[block_buffer_index], 0,
               block_remain);

        block_buffer_index += block_remain - SHA512_BYTE_SIZE_BLOCKSIZE;
        hca_sha512_append_bit_len(
            &ctx->ctx.sha512.block_buffer[block_buffer_index],
            &ctx->ctx.sha512.bitlen);
        // this block is now complete,so it can be processed
        result = hca_sha_block(scl, ctx->mode, 2, ctx->ctx.sha512.block_buffer);
        if (SCL_OK != result)
        {
            return (result);
        }
    }

    // retrieving the hash result
    result = hca_sha384_read(scl, hash);
    if (SCL_OK != result)
    {
        return (result);
    }

    *hash_len = SHA384_BYTE_HASHSIZE;

    return (SCL_OK);
}

int32_t hca_sha384_read(const metal_scl_t *const scl, uint8_t *const data_out)
{
    uint64_t *out64 = (uint64_t *)data_out;
    register uint64_t val;
    // Read hash
#if __riscv_xlen == 64
    if ((uint64_t)data_out & 0x7)
#elif __riscv_xlen == 32
    if ((uint32_t)data_out & 0x7)
#endif
    {
        val = METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_HASH);
        data_out[47] = (uint8_t)val;
        data_out[46] = (uint8_t)(val >> 8);
        data_out[45] = (uint8_t)(val >> 16);
        data_out[44] = (uint8_t)(val >> 24);
        data_out[43] = (uint8_t)(val >> 32);
        data_out[42] = (uint8_t)(val >> 40);
        data_out[41] = (uint8_t)(val >> 48);
        data_out[40] = (uint8_t)(val >> 56);
        val = METAL_REG64(scl->hca_base,
                          (METAL_SIFIVE_HCA_HASH + sizeof(uint64_t)));
        data_out[39] = (uint8_t)val;
        data_out[38] = (uint8_t)(val >> 8);
        data_out[37] = (uint8_t)(val >> 16);
        data_out[36] = (uint8_t)(val >> 24);
        data_out[35] = (uint8_t)(val >> 32);
        data_out[34] = (uint8_t)(val >> 40);
        data_out[33] = (uint8_t)(val >> 48);
        data_out[32] = (uint8_t)(val >> 56);
        val = METAL_REG64(scl->hca_base,
                          (METAL_SIFIVE_HCA_HASH + 2 * sizeof(uint64_t)));
        data_out[31] = (uint8_t)val;
        data_out[30] = (uint8_t)(val >> 8);
        data_out[29] = (uint8_t)(val >> 16);
        data_out[28] = (uint8_t)(val >> 24);
        data_out[27] = (uint8_t)(val >> 32);
        data_out[26] = (uint8_t)(val >> 40);
        data_out[25] = (uint8_t)(val >> 48);
        data_out[24] = (uint8_t)(val >> 56);
        val = METAL_REG64(scl->hca_base,
                          (METAL_SIFIVE_HCA_HASH + 3 * sizeof(uint64_t)));
        data_out[23] = (uint8_t)val;
        data_out[22] = (uint8_t)(val >> 8);
        data_out[21] = (uint8_t)(val >> 16);
        data_out[20] = (uint8_t)(val >> 24);
        data_out[19] = (uint8_t)(val >> 32);
        data_out[18] = (uint8_t)(val >> 40);
        data_out[17] = (uint8_t)(val >> 48);
        data_out[16] = (uint8_t)(val >> 56);
        val = METAL_REG64(scl->hca_base,
                          (METAL_SIFIVE_HCA_HASH + 4 * sizeof(uint64_t)));
        data_out[15] = (uint8_t)val;
        data_out[14] = (uint8_t)(val >> 8);
        data_out[13] = (uint8_t)(val >> 16);
        data_out[12] = (uint8_t)(val >> 24);
        data_out[11] = (uint8_t)(val >> 32);
        data_out[10] = (uint8_t)(val >> 40);
        data_out[9] = (uint8_t)(val >> 48);
        data_out[8] = (uint8_t)(val >> 56);
        val = METAL_REG64(scl->hca_base,
                          (METAL_SIFIVE_HCA_HASH + 5 * sizeof(uint64_t)));
        data_out[7] = (uint8_t)val;
        data_out[6] = (uint8_t)(val >> 8);
        data_out[5] = (uint8_t)(val >> 16);
        data_out[4] = (uint8_t)(val >> 24);
        data_out[3] = (uint8_t)(val >> 32);
        data_out[2] = (uint8_t)(val >> 40);
        data_out[1] = (uint8_t)(val >> 48);
        data_out[0] = (uint8_t)(val >> 56);
    }
    else
    {
        *out64++ = bswap64(METAL_REG64(
            scl->hca_base, (METAL_SIFIVE_HCA_HASH + 5 * sizeof(uint64_t))));
        *out64++ = bswap64(METAL_REG64(
            scl->hca_base, (METAL_SIFIVE_HCA_HASH + 4 * sizeof(uint64_t))));
        *out64++ = bswap64(METAL_REG64(
            scl->hca_base, (METAL_SIFIVE_HCA_HASH + 3 * sizeof(uint64_t))));
        *out64++ = bswap64(METAL_REG64(
            scl->hca_base, (METAL_SIFIVE_HCA_HASH + 2 * sizeof(uint64_t))));
        *out64++ = bswap64(METAL_REG64(
            scl->hca_base, (METAL_SIFIVE_HCA_HASH + 1 * sizeof(uint64_t))));
        *out64++ = bswap64(METAL_REG64(scl->hca_base, (METAL_SIFIVE_HCA_HASH)));
    }
    return (SCL_OK);
}
#endif /* METAL_SIFIVE_HCA_VERSION >= HCA_VERSION(0, 5, 0) */