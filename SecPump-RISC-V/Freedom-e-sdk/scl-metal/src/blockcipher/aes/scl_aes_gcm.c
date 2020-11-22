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
 * @file scl_aes_gcm.c
 * @brief GCM mode for the AES.
 * AES is NIST FIPS-197, GCM follow NIST SP 800-38D
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <string.h>

#include <scl_cfg.h>

#include <api/scl_api.h>
#include <api/utils.h>

#include <scl/scl_defs.h>
#include <scl/scl_retdefs.h>
#include <scl/scl_utils.h>

#include <api/blockcipher/aes/aes.h>

#include <scl/scl_aes_gcm.h>

int32_t scl_aes_gcm_init(const metal_scl_t *const scl_ctx,
                         aes_auth_ctx_t *const ctx, const uint8_t *const key,
                         size_t key_byte_len, const uint8_t *const iv,
                         size_t iv_byte_len, const uint8_t *const aad,
                         size_t aad_byte_len, size_t pld_byte_len,
                         scl_process_t mode)
{
    int32_t ret;
    uint64_t formated[4] = {0};
    uint64_t tmp_iv[2] = {0};

    if (NULL == scl_ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    ret = scl_format_key(key, key_byte_len, formated);

    if (SCL_OK != ret)
    {
        return (ret);
    }

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
        scl_format_iv(iv, iv_byte_len, tmp_iv);

        ret = scl_ctx->aes_func.setiv(scl_ctx, tmp_iv);
    }

    /* @FIXME: */
    /* key_formated should be secure erased */

    if (SCL_OK != ret)
    {
        return (ret);
    }

    ret = scl_ctx->aes_func.auth_init(scl_ctx, ctx, SCL_AES_GCM, mode,
                                      SCL_BIG_ENDIAN_MODE, 0, aad, aad_byte_len,
                                      pld_byte_len);

    return (ret);
}

int32_t scl_aes_gcm_core(const metal_scl_t *const scl_ctx, aes_auth_ctx_t *const ctx,
                                      uint8_t *const dst, size_t *const dst_byte_len,
                                      const uint8_t *const src, size_t src_byte_len)
{
    if (NULL == scl_ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    return scl_ctx->aes_func.auth_core(scl_ctx, ctx, src, src_byte_len, dst, dst_byte_len);
}

int32_t scl_aes_gcm_finish(const metal_scl_t *const scl_ctx,
                           aes_auth_ctx_t *const ctx, uint8_t *const tag,
                           size_t tag_byte_len, uint8_t *const dst,
                           const uint8_t *const src, size_t src_byte_len)
{
    int32_t ret;
    uint8_t tmp_tag[BLOCK128_NB_BYTE] = {0};
    size_t i;
    size_t dst_byte_len = 0;

    if (NULL == scl_ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    if (NULL == ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    if ((NULL != src) && src_byte_len)
    {
        ret = scl_ctx->aes_func.auth_core(scl_ctx, ctx, src, src_byte_len, dst, &dst_byte_len);
        if (SCL_OK != ret)
        {
            return (ret);
        }
    }

    if (NULL == dst)
    {
        ret = scl_ctx->aes_func.auth_finish(scl_ctx, ctx, NULL, (uint64_t *)tmp_tag);
    }
    else
    {
        ret = scl_ctx->aes_func.auth_finish(scl_ctx, ctx, &dst[dst_byte_len], (uint64_t *)tmp_tag);
    }

    if (SCL_OK != ret)
    {
        return (ret);
    }

    for (i = 0; i < tag_byte_len; i++)
    {
        tag[i]=tmp_tag[sizeof(tmp_tag) - 1 - i];
    }

    /* @FIXME: */
    /* ctx should be erased and be sure compiler optimization do not remove this operation */
    memset(ctx,0,sizeof(aes_auth_ctx_t));

    return (ret);
}

int32_t scl_aes_gcm(const metal_scl_t *const scl_ctx, uint8_t *const tag,
                    size_t tag_byte_len, uint8_t *const dst,
                    const uint8_t *const src, size_t src_byte_len,
                    const uint8_t *const key, size_t key_byte_len,
                    const uint8_t *const iv, size_t iv_byte_len,
                    const uint8_t *const aad, size_t aad_byte_len,
                    scl_process_t mode)
{
    int32_t ret;
    aes_auth_ctx_t ctx_aes_auth = {0};
    size_t dst_byte_len;

    if (NULL == scl_ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    ret = scl_aes_gcm_init(scl_ctx, &ctx_aes_auth, key, key_byte_len, iv,
                           iv_byte_len, aad, aad_byte_len, src_byte_len, mode);
    if (SCL_OK != ret)
    {
        return (ret);
    }

    ret = scl_aes_gcm_core(scl_ctx, &ctx_aes_auth, dst, &dst_byte_len, src, src_byte_len);
    if (SCL_OK != ret)
    {
        return (ret);
    }

    ret = scl_aes_gcm_finish(scl_ctx, &ctx_aes_auth, tag, tag_byte_len, &dst[dst_byte_len], NULL, 0);

    return (ret);
}
