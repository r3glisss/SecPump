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
 * @file scl_retdefs.h
 * @brief defines the values returned by the functions: that's mainly error
 * codes
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_RETDEFS_H
#define SCL_RETDEFS_H

/**
 * @addtogroup SCL
 * @addtogroup SCL_DEFS
 * @ingroup SCL
 *  @{
 */

/*! @brief SCL library error codes */
typedef enum
{
    /*! @brief no error */
    SCL_OK = 0,
    /*! @brief generic error code */
    SCL_ERROR = -1,
    /*! @brief error on input parameter */
    SCL_INVALID_INPUT = -2,
    /*! @brief error on output parameter */
    SCL_INVALID_OUTPUT = -3,
    /*! @brief error invalid operation mode selected */
    SCL_INVALID_MODE = -4,
    /*! @brief error invalid length */
    SCL_INVALID_LENGTH = -5,
    /*! @brief error potential overflow detected */
    SCL_STACK_OVERFLOW = -6,
    /*! @brief error stack not initialized (deprecated) */
    SCL_STACK_NOT_INITIALIZED = -7,
    /*! @brief error stack already initialized (deprecated) */
    SCL_STACK_ALREADY_INITIALIZED = -8,
    /*! @brief error initialization already performed */
    SCL_ALREADY_INITIALIZED = -9,
    /*! @brief error during stack initialization (deprecated) */
    SCL_STACK_INIT_ERROR = -10,
    /*! @brief error during stack free (deprecated) */
    SCL_STACK_FREE_ERROR = -11,
    /*! @brief generic error on stack (deprecated) */
    SCL_STACK_ERROR = -12,
    /*! @brief error on Random Number Generation */
    SCL_RNG_ERROR = -13,
    /*! @brief error new seed requested */
    SCL_RESEED_REQUIRED = -14,
    /*! @brief error ignored */
    SCL_IGNORED = -15,
    /*! @brief error busy */
    SCL_BUSY = -16,
    /*! @brief error zero division */
    SCL_ZERO_DIVISION = -17,
    /**
     * @brief One of the entry point, use by this function or in the call tree
     * use an API pointer undeclared or misplaced
     */
    SCL_ERROR_API_ENTRY_POINT = -18,
    /*! @brief error not modular inversion possible */
    SCL_NOT_INVERSIBLE = -19,
    /*! @brief error on parity */
    SCL_ERR_PARITY = -20,

    /*! @brief error functionnality not present */
    SCL_NOT_PRESENT = -30,
    /*! @brief error functionnality not yet supported */
    SCL_NOT_YET_SUPPORTED = -31,
} scl_errors_t;

/** @}*/

#endif /* SCL_RETDEFS_H */
