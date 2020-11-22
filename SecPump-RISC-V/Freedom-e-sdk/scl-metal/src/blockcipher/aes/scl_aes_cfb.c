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
 * @file scl_aes_cfb.c
 * @brief CBC mode for the AES.
 * AES is NIST FIPS-197,
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <scl_cfg.h>

#include <api/scl_api.h>
#include <api/utils.h>

#include <scl/scl_defs.h>
#include <scl/scl_retdefs.h>
#include <scl/scl_utils.h>

#include <api/blockcipher/aes/aes.h>

#include <scl/scl_aes_cfb.h>

int32_t scl_aes_cfb_init(const metal_scl_t *const scl_ctx,
                         const uint8_t *const key, size_t key_byte_len,
                         const uint8_t *const iv, size_t iv_byte_len,
                         scl_process_t mode)
{
    int32_t ret;
    uint64_t formated[4] = {0};
    uint64_t tmp[2] = {0};

    if (NULL == scl_ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    ret = scl_format_key(key, key_byte_len, formated);

    if (SCL_OK != ret)
        return (ret);

    switch (key_byte_len)
    {
    case SCL_KEY128:
        ret = scl_ctx->aes_func.setkey(scl_ctx, SCL_AES_KEY128, formated, mode);
        break;
    case SCL_KEY192:
        ret = scl_ctx->aes_func.setkey(scl_ctx, SCL_AES_KEY192, formated, mode);
        break;
    case SCL_KEY256:
        ret = scl_ctx->aes_func.setkey(scl_ctx, SCL_AES_KEY256, formated, mode);
        break;
    default:
        ret = SCL_INVALID_INPUT;
    }

    if (SCL_OK == ret)
    {
        scl_format_iv(iv, iv_byte_len, tmp);

        ret = scl_ctx->aes_func.setiv(scl_ctx, tmp);
    }

    /* @FIXME: */
    /* key_formated should be secure erased */

    return (ret);
}

// for any input length, multiple of blocks
int32_t scl_aes_cfb_core(const metal_scl_t *const scl_ctx, uint8_t *const dst,
                         const uint8_t *const src, size_t src_byte_len,
                         scl_process_t mode)
{
    int32_t ret;

    if (NULL == scl_ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    if (src_byte_len & 0xF)
        return (SCL_INVALID_INPUT);

    ret = scl_ctx->aes_func.cipher(scl_ctx, SCL_AES_CFB, mode,
                                   SCL_BIG_ENDIAN_MODE, src, src_byte_len, dst);

    return (ret);
}

int32_t scl_aes_cfb(const metal_scl_t *const scl_ctx, uint8_t *const dst,
                    const uint8_t *const src, size_t src_byte_len,
                    const uint8_t *const key, size_t key_byte_len,
                    const uint8_t *const iv, size_t iv_byte_len,
                    scl_process_t mode)
{
    int32_t ret;

    if (NULL == src || NULL == key || NULL == dst)
    {
        return (SCL_INVALID_INPUT);
    }

    if ((src_byte_len % BLOCK128_NB_BYTE) != 0)
    {
        return (SCL_INVALID_INPUT);
    }

    if ((SCL_ENCRYPT != mode) && (SCL_DECRYPT != mode))
    {
        return (SCL_INVALID_MODE);
    }

    if ((SCL_KEY128 != key_byte_len) && (SCL_KEY192 != key_byte_len) &&
        (SCL_KEY256 != key_byte_len))
    {
        return (SCL_INVALID_INPUT);
    }

    ret = scl_aes_cfb_init(scl_ctx, key, key_byte_len, iv, iv_byte_len, mode);
    if (SCL_OK != ret)
    {
        return (ret);
    }

    ret = scl_aes_cfb_core(scl_ctx, dst, src, src_byte_len, mode);
    if (SCL_OK != ret)
    {
        return (ret);
    }

    return (SCL_OK);
}
