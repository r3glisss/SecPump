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
 * @file defs.h
 * @brief Low level API common definitions
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_DEFS_H
#define SCL_BACKEND_DEFS_H

/**
 * @addtogroup COMMON
 * @addtogroup DEFS
 * @ingroup COMMON
 *  @{
 */

/**
 * @brief endianess supported by the scl library
 * @note On classic cryptographic data representation, data are big endian words
 * for exemple a 128 bits AES key is a usually considered as a big number big
 * endian 128 bits word.
 * @warning The only mode supported by the software implementation is Big Endian
 */
typedef enum
{
    /*! @brief little endian words */
    SCL_LITTLE_ENDIAN_MODE = 0,
    /*! @brief big endian words */
    SCL_BIG_ENDIAN_MODE = 1
} endianness_t;

/** @}*/

#endif /* SCL_BACKEND_DEFS_H */
