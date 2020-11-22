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
 * @file sha384.h
 * @brief sha384 implementation/wrapper
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_SHA384_H
#define SCL_BACKEND_SHA384_H

#include <stddef.h>
#include <stdint.h>

#include <api/hash/sha512.h>

/** 
 * @addtogroup COMMON
 * @addtogroup SHA
 * @ingroup COMMON
 *  @{
 */

#define SHA384_BYTE_HASHSIZE 48

/**
 * @brief SHA384 context
 * @note Since SHA384 use SHA512 block computation, it can also use the same
 * context
 */
typedef sha512_ctx_t sha384_ctx_t;

/** @}*/

#endif /* SCL_BACKEND_SHA384_H */
