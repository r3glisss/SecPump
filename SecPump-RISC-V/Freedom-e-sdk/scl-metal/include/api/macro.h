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
 * @file macro.h
 * @brief Low level API common macro
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_MACRO_H
#define SCL_BACKEND_MACRO_H

#include <machine/endian.h>

/**
 * @addtogroup COMMON
 * @addtogroup MACRO
 * @ingroup COMMON
 *  @{
 */

#ifndef bswap16
/*! @brief swap 16 bits words  */
#define bswap16(x) __bswap16(x)
#endif

#ifndef bswap32
/*! @brief swap 32 bits words  */
#define bswap32(x) __bswap32(x)
#endif

#ifndef bswap64
/*! @brief swap 64 bits words  */
#define bswap64(x) __bswap64(x)
#endif

#ifndef MAX
/**
 * @brief simple max macro
 * @warning Not safe in case of function call as inputs, double entry issue.
 */
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
/**
 * @brief simple min macro
 * @warning Not safe in case of function call as inputs, double entry issue.
 */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef SAFE_MAX
/**
 * @brief safe max macro
 * @note Safe in case of function call as inputs, but more ressource consuming
 */
#define SAFE_MAX(a, b)                                                         \
    ({                                                                         \
        __typeof__(a) _a = (a);                                                \
        __typeof__(b) _b = (b);                                                \
        _a > _b ? _a : _b;                                                     \
    })
#endif

#ifndef SAFE_MIN
/**
 * @brief simple min macro
 * @note Safe in case of function call as inputs, but more ressource consuming
 */
#define SAFE_MIN(a, b)                                                         \
    ({                                                                         \
        __typeof__(a) _a = (a);                                                \
        __typeof__(b) _b = (b);                                                \
        _a < _b ? _a : _b;                                                     \
    })
#endif

/** @}*/

#endif /* SCL_BACKEND_MACRO_H */
