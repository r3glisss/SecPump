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
 * @file scl_init.c
 * @brief
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <stdint.h>
#include <stdio.h>

#include <api/scl_api.h>
#include <scl_cfg.h>

#include <scl/scl_utils.h>

#define UINT32(data)                                                           \
    (((uint32_t) * ((const uint8_t *)data) << 24) +                            \
     ((uint32_t) * ((const uint8_t *)data + 1) << 16) +                        \
     ((uint32_t) * ((const uint8_t *)data + 2) << 8) +                         \
     ((uint32_t) * ((const uint8_t *)data + 3)))
#define UINT64(data)                                                           \
    (((uint64_t)UINT32((const uint8_t *)data) << 32) +                         \
     (uint64_t)UINT32((const uint8_t *)data + 4))

SCL_DATA metal_scl_t *scl_ctx = NULL;

int32_t scl_format_key(const uint8_t *const key, const size_t key_byte_len,
                       uint64_t *key_formated)
{
    int32_t ret;

    if ((NULL == key) || (NULL == key_formated))
    {
        return (SCL_INVALID_INPUT);
    }

    switch (key_byte_len)
    {
    case SCL_KEY128:
        key_formated[0] = 0;
        key_formated[1] = 0;
        key_formated[2] = UINT64(&key[8]);
        key_formated[3] = UINT64(&key[0]);
        ret = SCL_OK;
        break;
    case SCL_KEY192:
        key_formated[0] = 0;
        key_formated[1] = UINT64(&key[16]);
        key_formated[2] = UINT64(&key[8]);
        key_formated[3] = UINT64(&key[0]);
        ret = SCL_OK;
        break;
    case SCL_KEY256:
        key_formated[0] = UINT64(&key[24]);
        key_formated[1] = UINT64(&key[16]);
        key_formated[2] = UINT64(&key[8]);
        key_formated[3] = UINT64(&key[0]);
        ret = SCL_OK;
        break;
    default:
        ret = SCL_INVALID_INPUT;
    }

    return (ret);
}

void scl_format_iv(const uint8_t *const iv, const size_t iv_byte_len,
                   uint64_t *iv_formated)
{
    size_t i;

    iv_formated[0] = 0;
    iv_formated[1] = 0;

    if (iv_byte_len < 8)
    {
        for (i = 0; i < iv_byte_len; i++)
        {
            iv_formated[1] += ((uint64_t) * (iv + i)) << ((7 - i) << 3);
        }
    }
    else
    {
        iv_formated[1] = UINT64(iv);

        if (iv_byte_len > 8)
        {
            for (i = 0; i < (iv_byte_len - 8); i++)
            {
                iv_formated[0] += ((uint64_t) * (iv + 8 + i)) << ((7 - i) << 3);
            }
        }
    }
}
