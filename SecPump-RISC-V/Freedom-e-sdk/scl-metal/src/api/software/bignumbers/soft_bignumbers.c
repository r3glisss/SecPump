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
 * @file soft_bignumbers.c
 * @brief arithmetic on bignumber, software implementation
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <string.h>

#include <api/macro.h>
#include <api/utils.h>

#include <scl/scl_retdefs.h>

#include <api/software/bignumbers/soft_bignumbers.h>

int32_t soft_bignum_compare(const metal_scl_t *const scl,
                            const uint64_t *const a, const uint64_t *const b,
                            size_t nb_32b_words)
{
    size_t i;
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/

    i = nb_32b_words / 2;

    if (0 != nb_32b_words % 2)
    {
        if ((uint32_t)a[i] > (uint32_t)b[i])
        {
            return (1);
        }
        if ((uint32_t)a[i] < (uint32_t)b[i])
        {
            return (-1);
        }
    }

    /* If word_size == 0 then the array are considered equals */
    while (i != 0)
    {
        i--;
        if (a[i] > b[i])
        {
            return (1);
        }
        if (a[i] < b[i])
        {
            return (-1);
        }
    }
    return (0);
}

int32_t soft_bignum_compare_len_diff(const metal_scl_t *const scl,
                                     const uint64_t *const a,
                                     size_t a_nb_32b_words,
                                     const uint64_t *const b,
                                     size_t b_nb_32b_words)
{
    int32_t result = 0;
    size_t i;
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/

    /* check if upper part of the longest array is null */
    if (a_nb_32b_words > b_nb_32b_words)
    {
        result =
            soft_bignum_is_null(scl, &((const uint32_t *)a)[b_nb_32b_words],
                                a_nb_32b_words - b_nb_32b_words);
        if (0 == result)
        {
            return (1);
        }
    }
    else if (a_nb_32b_words < b_nb_32b_words)
    {
        result =
            soft_bignum_is_null(scl, &((const uint32_t *)b)[a_nb_32b_words],
                                b_nb_32b_words - a_nb_32b_words);
        if (0 == result)
        {
            return (-1);
        }
    }

    i = MIN(a_nb_32b_words, b_nb_32b_words) / 2;

    if (0 != MIN(a_nb_32b_words, b_nb_32b_words) % 2)
    {
        if ((uint32_t)a[i] > (uint32_t)b[i])
        {
            return (1);
        }
        if ((uint32_t)a[i] < (uint32_t)b[i])
        {
            return (-1);
        }
    }

    /* If word_size == 0 then the array are considered equals */
    while (i != 0)
    {
        i--;
        if (a[i] > b[i])
        {
            return (1);
        }
        if (a[i] < b[i])
        {
            return (-1);
        }
    }
    return (0);
}

int32_t soft_bignum_is_null(const metal_scl_t *const scl,
                            const uint32_t *const array, size_t nb_32b_words)
{
    size_t i = 0;
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/

    if (NULL == array)
    {
        return (SCL_INVALID_INPUT);
    }

    if (0 == nb_32b_words)
    {
        return (SCL_INVALID_LENGTH);
    }

    for (i = 0; i < nb_32b_words; i++)
    {
        if (0 != array[i])
        {
            return ((int32_t) false);
        }
    }

    return ((int32_t) true);
}

int32_t soft_bignum_negate(const metal_scl_t *const scl, uint64_t *const array,
                           size_t nb_32b_words)
{
    int32_t result = 0;
    size_t i = 0;

    if ((NULL == scl) || (NULL == array))
    {
        return (SCL_INVALID_INPUT);
    }

    if (NULL == scl->bignum_func.inc)
    {
        return (SCL_ERROR_API_ENTRY_POINT);
    }

    if (0 == nb_32b_words)
    {
        return (SCL_INVALID_LENGTH);
    }

    for (i = 0; i < nb_32b_words / 2; i++)
    {
        array[i] = ~array[i];
    }

    if (0 != nb_32b_words % 2)
    {
        *((uint32_t *)&array[i]) = ~(uint32_t)array[i];
    }

    result = scl->bignum_func.inc(scl, array, nb_32b_words);
    if (SCL_OK > result)
    {
        return (result);
    }

    return SCL_OK;
}

int32_t soft_bignum_inc(const metal_scl_t *const scl, uint64_t *const array,
                        size_t nb_32b_words)
{
    size_t i = 0;
    uint64_t carry = 1;
    uint64_t previous = 0;
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/

    if (NULL == array)
    {
        return (SCL_INVALID_INPUT);
    }

    if (0 == nb_32b_words)
    {
        return (SCL_INVALID_LENGTH);
    }

    for (i = 0; i < nb_32b_words / 2; i++)
    {
        previous = array[i];
        array[i] += carry;
        carry = array[i] < previous ? (uint64_t)1 : (uint64_t)0;
    }

    if (0 != nb_32b_words % 2)
    {
        previous = *((uint32_t *)&array[i]);
        *((uint32_t *)&array[i]) += (uint32_t)carry;
        carry = *((uint32_t *)&array[i]) < previous ? (uint64_t)1 : (uint64_t)0;
    }

    /* carry equals 0 or 1 */
    return ((int32_t)carry);
}

int32_t soft_bignum_add(const metal_scl_t *const scl,
                        const uint64_t *const in_a, const uint64_t *const in_b,
                        uint64_t *const out, size_t nb_32b_words)
{
    size_t i = 0;
    uint64_t carry = 0;
    uint64_t previous = 0;
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/

    if ((NULL == in_a) || (NULL == in_b) || (NULL == out))
    {
        return (SCL_INVALID_INPUT);
    }

    if (0 == nb_32b_words)
    {
        return (SCL_INVALID_LENGTH);
    }

    for (i = 0; i < nb_32b_words / 2; i++)
    {
        previous = in_a[i];
        out[i] = in_a[i] + in_b[i];
        out[i] += carry;
        carry = out[i] < previous ? (uint64_t)1 : (uint64_t)0;
    }

    if (0 != nb_32b_words % 2)
    {
        previous = *((const uint32_t *)&in_a[i]);
        *((uint32_t *)&out[i]) =
            *((const uint32_t *)&in_a[i]) + *((const uint32_t *)&in_b[i]);
        *((uint32_t *)&out[i]) += (uint32_t)carry;
        carry = *((uint32_t *)&out[i]) < previous ? (uint64_t)1 : (uint64_t)0;
    }

    /* carry is equal to 0 or 1 */
    return ((int32_t)carry);
}

int32_t soft_bignum_sub(const metal_scl_t *const scl,
                        const uint64_t *const in_a, const uint64_t *const in_b,
                        uint64_t *const out, size_t nb_32b_words)
{
    size_t i = 0;
    uint64_t borrow = 0;
    uint64_t previous = 0;
    uint64_t current = 0;
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/

    if ((NULL == in_a) || (NULL == in_b) || (NULL == out))
    {
        return (SCL_INVALID_INPUT);
    }

    if (0 == nb_32b_words)
    {
        return (SCL_INVALID_LENGTH);
    }

    for (i = 0; i < nb_32b_words / 2; i++)
    {
        previous = in_a[i];
        current = in_a[i] - borrow;
        borrow = current > previous ? (uint64_t)1 : (uint64_t)0;

        previous = current;
        out[i] = current - in_b[i];
        borrow |= out[i] > previous ? (uint64_t)1 : (uint64_t)0;
    }

    if (0 != nb_32b_words % 2)
    {
        previous = *((const uint32_t *)&in_a[i]);
        current = *((const uint32_t *)&in_a[i]) - borrow;
        borrow = current > previous ? (uint64_t)1 : (uint64_t)0;

        previous = current;
        *((uint32_t *)&out[i]) =
            (uint32_t)current - *((const uint32_t *)&in_b[i]);
        borrow |= *((uint32_t *)&out[i]) > previous ? (uint64_t)1 : (uint64_t)0;
    }

    /* borrow is equal to 0 or 1 */
    return ((int32_t)borrow);
}

int32_t soft_bignum_mult(const metal_scl_t *const scl,
                         const uint64_t *const in_a, const uint64_t *const in_b,
                         uint64_t *const out, size_t nb_32b_words)
{
    size_t i, j;
    uint32_t carry;
    uint64_t ab;
    const uint32_t *a = (const uint32_t *)in_a;
    const uint32_t *b = (const uint32_t *)in_b;

    uint32_t *res = (uint32_t *)out;
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/

    if ((NULL == in_a) || (NULL == in_b) || (NULL == out))
    {
        return (SCL_INVALID_INPUT);
    }

    if (0 == nb_32b_words)
    {
        return (SCL_INVALID_LENGTH);
    }

    /**
     * carefull here, nb_32b_words is 2 * the number of 64 bits words of inputs
     */
    memset(out, 0, nb_32b_words * 2 * sizeof(uint32_t));

    // 1.
    for (i = 0; i < nb_32b_words; i++)
    {
        for (carry = 0, j = 0; j < nb_32b_words; j++)
        {
            ab = (uint64_t)b[i] * (uint64_t)a[j];

            res[i + j] = res[i + j] + carry;
            if (res[i + j] < carry)
            {
                carry = 1;
            }
            else
            {
                carry = 0;
            }

            res[i + j] += (uint32_t)ab;
            if (res[i + j] < (uint32_t)ab)
            {
                carry++;
            }

            /* load 32 bits msb into carry */
            carry += (uint32_t)(ab >> (sizeof(uint32_t) * __CHAR_BIT__));
        }
        res[i + nb_32b_words] += carry;
    }

    return (SCL_OK);
}

int32_t soft_bignum_leftshift(const metal_scl_t *const scl,
                              const uint64_t *const in, uint64_t *const out,
                              size_t shift, size_t nb_32b_words)
{
    size_t revshift, bit_shift_div64, bit_shift_mod64;
    size_t i;
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/

    if ((NULL == in) || (NULL == out))
    {
        return (SCL_INVALID_INPUT);
    }

    if ((0 == nb_32b_words) ||
        (shift >= nb_32b_words * sizeof(uint32_t) * __CHAR_BIT__))
    {
        return (SCL_INVALID_LENGTH);
    }

    bit_shift_div64 = shift / (sizeof(uint64_t) * __CHAR_BIT__);
    bit_shift_mod64 = shift & ((sizeof(uint64_t) * __CHAR_BIT__) - 1);

    i = nb_32b_words / 2;

    revshift = (sizeof(uint64_t) * __CHAR_BIT__) - bit_shift_mod64;

    if (0 != nb_32b_words % 2)
    {
        *((uint32_t *)&out[i]) =
            (uint32_t)(in[i - bit_shift_div64] << bit_shift_mod64);

        if (i >= bit_shift_div64 + 1)
        {
            *((uint32_t *)&out[i]) |=
                (uint32_t)(in[i - bit_shift_div64 - 1] >> revshift);
        }
    }

    while (i > bit_shift_div64 + 1)
    {
        i--;
        out[i] = in[i - bit_shift_div64] << bit_shift_mod64;
        out[i] |= in[i - bit_shift_div64 - 1] >> revshift;
    }

    if (i > bit_shift_div64)
    {
        i--;
        out[i] = in[i - bit_shift_div64] << bit_shift_mod64;
    }

    memset(out, 0, shift / __CHAR_BIT__);

    return (SCL_OK);
}

int32_t soft_bignum_rightshift(const metal_scl_t *const scl,
                               const uint64_t *const in, uint64_t *const out,
                               size_t shift, size_t nb_32b_words)
{
    size_t revshift, bit_shift_div64, bit_shift_mod64;
    size_t i;
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/

    uint32_t *out_32b = (uint32_t *)out;

    if ((NULL == in) || (NULL == out))
    {
        return (SCL_INVALID_INPUT);
    }

    if ((0 == nb_32b_words) ||
        (shift >= nb_32b_words * sizeof(uint32_t) * __CHAR_BIT__))
    {
        return (SCL_INVALID_LENGTH);
    }

    bit_shift_div64 = shift / (sizeof(uint64_t) * __CHAR_BIT__);
    bit_shift_mod64 = shift & ((sizeof(uint64_t) * __CHAR_BIT__) - 1);

    i = 0;

    revshift = (sizeof(uint64_t) * __CHAR_BIT__) - bit_shift_mod64;

    while (i + bit_shift_div64 + 1 < nb_32b_words / 2)
    {
        out[i] = in[i + bit_shift_div64] >> bit_shift_mod64;
        out[i] |= in[i + bit_shift_div64 + 1] << revshift;
        i++;
    }

    if (i + bit_shift_div64 < nb_32b_words / 2)
    {
        out[i] = in[i + bit_shift_div64] >> bit_shift_mod64;
        if (0 != nb_32b_words % 2)
        {
            out[i] |=
                (uint64_t)(*((const uint32_t *)&in[i + bit_shift_div64 + 1]))
                << revshift;
            i++;
        }
    }

    if (0 != nb_32b_words % 2)
    {
        *((uint32_t *)&out[i]) = (uint32_t)(
            (*((const uint32_t *)&in[i + bit_shift_div64])) >> bit_shift_mod64);
    }

    memset((void *)((uint8_t *)&out_32b[nb_32b_words] -
                    (uint8_t *)(shift / __CHAR_BIT__)),
           0, shift / __CHAR_BIT__);

    return (SCL_OK);
}

int32_t soft_bignum_msb_set_in_word(uint64_t word_64b)
{
    size_t i;

    for (i = 0; i < sizeof(word_64b) * __CHAR_BIT__; i++, word_64b >>= 1)
    {
        if (0 == word_64b)
        {
            break;
        }
    }

    return ((int32_t)i);
}

int32_t soft_bignum_get_msb_set(const metal_scl_t *const scl,
                                const uint64_t *const array,
                                size_t nb_32b_words)
{
    size_t i;
    size_t shift_word;
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/

    if (NULL == array)
    {
        return (SCL_INVALID_INPUT);
    }

    /**
     * Check length, to avoid conflict with error codes on return, in practice
     * this should never happen except for tests
     */
    if (((size_t)0x80000000 <=
         nb_32b_words * sizeof(uint64_t) * __CHAR_BIT__) ||
        (0 == nb_32b_words))
    {
        return (SCL_INVALID_LENGTH);
    }

    i = nb_32b_words / 2;

    if (0 != nb_32b_words % 2)
    {
        if (0 != (uint32_t)array[i])
        {
            shift_word = i * sizeof(uint64_t) * __CHAR_BIT__;
            shift_word +=
                (size_t)soft_bignum_msb_set_in_word((uint32_t)array[i]);
            return ((int32_t)shift_word);
        }
    }

    /* If word_size == 0 then the array are considered equals */
    while (i != 0)
    {
        i--;
        if (0 != array[i])
        {
            shift_word = i * sizeof(uint64_t) * __CHAR_BIT__;
            shift_word += (size_t)soft_bignum_msb_set_in_word(array[i]);
            return ((int32_t)shift_word);
        }
    }

    return (0);
}

int32_t soft_bignum_set_bit(const metal_scl_t *const scl, uint64_t *const array,
                            size_t nb_32b_words, size_t bit_2_set)
{
    uint32_t *array_32b = (uint32_t *)array;
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/

    if (NULL == array)
    {
        return (SCL_INVALID_INPUT);
    }

    /**
     * Check length, to avoid conflict with error codes on return, in practice
     * this should never happen except for tests
     */
    if (0 == nb_32b_words)
    {
        return (SCL_INVALID_LENGTH);
    }

    if (bit_2_set >= nb_32b_words * sizeof(uint32_t) * __CHAR_BIT__)
    {
        return (SCL_INVALID_INPUT);
    }

    array_32b[bit_2_set / (sizeof(uint32_t) * __CHAR_BIT__)] |=
        (uint32_t)(1 << bit_2_set % (sizeof(uint32_t) * __CHAR_BIT__));

    return (SCL_OK);
}

int32_t soft_bignum_div(const metal_scl_t *const scl,
                        const uint64_t *const dividend,
                        size_t dividend_nb_32b_words,
                        const uint64_t *const divisor,
                        size_t divisor_nb_32b_words, uint64_t *const remainder,
                        uint64_t *const quotient)
{
    int32_t result = 0;

    size_t p_len = 0;

    /* bitshift index use for dichotomy  */
    size_t bitshift_dico = 0;

    if ((NULL == scl) || (NULL == dividend) || (NULL == divisor))
    {
        return (SCL_INVALID_INPUT);
    }

    if ((0 == dividend_nb_32b_words) || (0 == divisor_nb_32b_words))
    {
        return (SCL_INVALID_LENGTH);
    }

    if ((NULL == scl->bignum_func.is_null) ||
        (NULL == scl->bignum_func.compare_len_diff) ||
        (NULL == scl->bignum_func.get_msb_set) ||
        (NULL == scl->bignum_func.leftshift) ||
        (NULL == scl->bignum_func.rightshift) ||
        (NULL == scl->bignum_func.add) || (NULL == scl->bignum_func.sub) ||
        (NULL == scl->bignum_func.set_bit))
    {
        return (SCL_ERROR_API_ENTRY_POINT);
    }

    /* dividor pointer has already been checked */
    result = scl->bignum_func.is_null(scl, (const uint32_t *)divisor,
                                      divisor_nb_32b_words);
    if (SCL_OK > result)
    {
        return (result);
    }
    else if ((int32_t) false != result)
    {
        return (SCL_ZERO_DIVISION);
    }

    /* if dividend < divisor, then reminder = dividend and quotient = 0 */
    result = scl->bignum_func.compare_len_diff(
        scl, dividend, dividend_nb_32b_words, divisor, divisor_nb_32b_words);
    if (0 > result)
    {
        if (NULL != quotient)
        {
            memset(quotient, 0, dividend_nb_32b_words * sizeof(uint32_t));
        }
        if (NULL != remainder)
        {
            memcpy(remainder, dividend,
                   divisor_nb_32b_words * sizeof(uint32_t));
        }
        return (SCL_OK);
    }
    else if (0 == result)
    {
        if (NULL != quotient)
        {
            memset(quotient, 0, dividend_nb_32b_words * sizeof(uint32_t));
            quotient[0]++;
        }
        if (NULL != remainder)
        {
            memset(remainder, 0, divisor_nb_32b_words * sizeof(uint32_t));
        }
        return (SCL_OK);
    }

    /**
     * the goal here is to have:
     * p = b << bitshift_dico
     * with
     * p <= dividend < p << 1
     */

    /* get highest bit of dividend */
    result = scl->bignum_func.get_msb_set(scl, dividend, dividend_nb_32b_words);
    if (SCL_OK > result)
    {
        return (result);
    }

    bitshift_dico = (size_t)result;

    /* We allocate +1 word to manage overflow */
    p_len = (bitshift_dico - 1) / (sizeof(uint32_t) * __CHAR_BIT__) + 1;

    p_len += (bitshift_dico - 1) % (sizeof(uint32_t) * __CHAR_BIT__) ? 1 : 0;

    /* get highest bit of divisor */
    result = scl->bignum_func.get_msb_set(scl, divisor, divisor_nb_32b_words);
    if (SCL_OK > result)
    {
        return (result);
    }

    bitshift_dico -= (size_t)result;

    {
        // p_len = bitshift_dico / (sizeof(uint32_t) * __CHAR_BIT__) +
        // divisor_nb_32b_words + 1;

        /** p : representative of the calculations on b * 2 ^ n (the second
         * column in example) on which we do our multiplication and division by
         * 2 via a "shift". */
        uint32_t p[p_len] __attribute__((aligned(8)));

        /** aux : representing the sum that should not exceed the dividend
         * (third column in the example). */
        uint32_t aux[p_len] __attribute__((aligned(8)));

        /**
         * We do a last comparison to make sure of:
         * p = b << bitshift_dico
         * with
         * p <= dividend < p << 1
         */

        /*
         * We only copy p_len since dividend > divisor (at ths point)
         */
        memset(p, 0, p_len * sizeof(uint32_t));
        memcpy(p, divisor, MIN(divisor_nb_32b_words, p_len) * sizeof(uint32_t));

        result = scl->bignum_func.leftshift(scl, (uint64_t *)p, (uint64_t *)p,
                                            bitshift_dico, p_len);
        if (SCL_OK > result)
        {
            return (result);
        }

        /**
         * Compare p to dividend and adjust bitshisft_dico (just in case p is
         * superior)
         */
        result = scl->bignum_func.compare_len_diff(
            scl, (const uint64_t *)dividend, dividend_nb_32b_words,
            (uint64_t *)p, p_len);
        if (0 > result)
        {
            result = scl->bignum_func.rightshift(scl, (uint64_t *)p,
                                                 (uint64_t *)p, 1, p_len);
            if (SCL_OK > result)
            {
                return (result);
            }
            bitshift_dico--;
        }

        memcpy(aux, p, p_len * sizeof(uint32_t));

        if (NULL != quotient)
        {
            memset(quotient, 0, dividend_nb_32b_words * sizeof(uint32_t));
            /**
             * Please note that here +1 is safe since at least 32bits have been
             * zeroise. So no chance to corrupt upper 32 bits in case the are
             * not part of the buffer.
             */
            quotient[0]++;
            result = scl->bignum_func.leftshift(
                scl, quotient, quotient, bitshift_dico, dividend_nb_32b_words);
            if (SCL_OK > result)
            {
                return (result);
            }
        }

        while (bitshift_dico > 0)
        {
            result = scl->bignum_func.rightshift(scl, (uint64_t *)p,
                                                 (uint64_t *)p, 1, p_len);
            if (SCL_OK > result)
            {
                return (result);
            }

            bitshift_dico--;

            result = scl->bignum_func.add(scl, (uint64_t *)aux, (uint64_t *)p,
                                          (uint64_t *)aux, p_len);
            if (SCL_OK > result)
            {
                return (result);
            }

            result = scl->bignum_func.compare_len_diff(
                scl, (const uint64_t *)dividend, dividend_nb_32b_words,
                (uint64_t *)aux, p_len);
            if (result >= 0)
            {
                if (NULL != quotient)
                {
                    result = scl->bignum_func.set_bit(scl, (uint64_t *)quotient,
                                                      dividend_nb_32b_words,
                                                      bitshift_dico);
                    if (SCL_OK > result)
                    {
                        return (result);
                    }
                }
            }
            else
            {
                result =
                    scl->bignum_func.sub(scl, (uint64_t *)aux, (uint64_t *)p,
                                         (uint64_t *)aux, p_len);
                if (SCL_OK > result)
                {
                    return (result);
                }
            }
        }

        if (NULL != remainder)
        {
            result = scl->bignum_func.sub(scl, (const uint64_t *)dividend,
                                          (uint64_t *)aux, (uint64_t *)aux,
                                          MIN(dividend_nb_32b_words, p_len));
            if (SCL_OK > result)
            {
                return (result);
            }
            memset(remainder, 0, divisor_nb_32b_words * sizeof(uint32_t));
            memcpy(remainder, aux,
                   MIN(divisor_nb_32b_words, p_len) * sizeof(uint32_t));
        }
    }

    return (SCL_OK);
}

int32_t soft_bignum_mod(const metal_scl_t *const scl, const uint64_t *const in,
                        size_t in_nb_32b_words, const uint64_t *const modulus,
                        size_t modulus_nb_32b_words, uint64_t *const remainder)
{
    if (NULL == scl)
    {
        return (SCL_INVALID_INPUT);
    }

    if (NULL == scl->bignum_func.div)
    {
        return (SCL_ERROR_API_ENTRY_POINT);
    }

    return (scl->bignum_func.div(scl, in, in_nb_32b_words, modulus,
                                 modulus_nb_32b_words, remainder, NULL));
}

int32_t soft_bignum_set_modulus(const metal_scl_t *const scl,
                                bignum_ctx_t *const ctx,
                                const uint64_t *const modulus,
                                size_t modulus_nb_32b_words)
{
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/

    if ((NULL == ctx) || (NULL == modulus))
    {
        return (SCL_INVALID_INPUT);
    }

    if (0 == modulus_nb_32b_words)
    {
        return (SCL_INVALID_LENGTH);
    }

    ctx->modulus = modulus;
    ctx->modulus_nb_32b_words = modulus_nb_32b_words;

    return (SCL_OK);
}

int32_t soft_bignum_clear_modulus(const metal_scl_t *const scl,
                                  bignum_ctx_t *const ctx)
{
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/

    if (NULL == ctx)
    {
        return (SCL_INVALID_INPUT);
    }

    ctx->modulus = NULL;
    ctx->modulus_nb_32b_words = 0;

    return (SCL_OK);
}

int32_t soft_bignum_mod_neg(const metal_scl_t *const scl,
                            const bignum_ctx_t *const ctx,
                            const uint64_t *const in, uint64_t *const out,
                            size_t nb_32b_words)
{
    int32_t result;
    uint32_t neg_result[nb_32b_words] __attribute__((aligned(8)));

    if ((NULL == scl) || (NULL == ctx) || (NULL == ctx->modulus))
    {
        return (SCL_INVALID_INPUT);
    }

    if ((NULL == scl->bignum_func.sub) || (NULL == scl->bignum_func.mod))
    {
        return (SCL_ERROR_API_ENTRY_POINT);
    }

    /* output should be modulus size */
    if (nb_32b_words != ctx->modulus_nb_32b_words)
    {
        return (SCL_INVALID_LENGTH);
    }

    result =
        scl->bignum_func.mod(scl, in, nb_32b_words, ctx->modulus,
                             ctx->modulus_nb_32b_words, (uint64_t *)neg_result);
    if (SCL_OK > result)
    {
        return (result);
    }

    /*@-compdef@*/
    result =
        scl->bignum_func.sub(scl, ctx->modulus, (uint64_t *)neg_result,
                             (uint64_t *)neg_result, ctx->modulus_nb_32b_words);
    /*@+compdef@*/
    if (SCL_OK > result)
    {
        return (result);
    }

    /*@-compdef@*/
    result = scl->bignum_func.mod(scl, (uint64_t *)neg_result, nb_32b_words,
                                  ctx->modulus, ctx->modulus_nb_32b_words, out);
    /*@+compdef@*/
    if (SCL_OK > result)
    {
        return (result);
    }

    return (SCL_OK);
}

int32_t soft_bignum_mod_add(const metal_scl_t *const scl,
                            const bignum_ctx_t *const ctx,
                            const uint64_t *const in_a,
                            const uint64_t *const in_b, uint64_t *const out,
                            size_t nb_32b_words)
{
    int32_t result;
    uint32_t add_result[nb_32b_words + 1] __attribute__((aligned(8)));

    if ((NULL == scl) || (NULL == ctx) || (NULL == ctx->modulus))
    {
        return (SCL_INVALID_INPUT);
    }

    if ((NULL == scl->bignum_func.add) || (NULL == scl->bignum_func.mod))
    {
        return (SCL_ERROR_API_ENTRY_POINT);
    }

    /* output should be modulus size */
    if (nb_32b_words != ctx->modulus_nb_32b_words)
    {
        return (SCL_INVALID_LENGTH);
    }

    add_result[nb_32b_words] = (uint32_t)scl->bignum_func.add(
        scl, in_a, in_b, (uint64_t *)add_result, nb_32b_words);
    if (SCL_OK > (int32_t)add_result[nb_32b_words])
    {
        return ((int32_t)add_result[nb_32b_words]);
    }

    result = scl->bignum_func.mod(scl, (uint64_t *)add_result, nb_32b_words + 1,
                                  ctx->modulus, ctx->modulus_nb_32b_words, out);
    if (SCL_OK > result)
    {
        return (result);
    }

    return (SCL_OK);
}

int32_t soft_bignum_mod_sub(const metal_scl_t *const scl,
                            const bignum_ctx_t *const ctx,
                            const uint64_t *const in_a,
                            const uint64_t *const in_b, uint64_t *const out,
                            size_t nb_32b_words)
{
    int32_t result;
    uint32_t sub_result[nb_32b_words] __attribute__((aligned(8)));

    if ((NULL == scl) || (NULL == ctx) || (NULL == ctx->modulus))
    {
        return (SCL_INVALID_INPUT);
    }

    if ((NULL == scl->bignum_func.sub) || (NULL == scl->bignum_func.negate) ||
        (NULL == scl->bignum_func.mod))
    {
        return (SCL_ERROR_API_ENTRY_POINT);
    }

    /* output should be modulus size */
    if (nb_32b_words != ctx->modulus_nb_32b_words)
    {
        return (SCL_INVALID_LENGTH);
    }

    result = scl->bignum_func.sub(scl, in_a, in_b, (uint64_t *)sub_result,
                                  nb_32b_words);
    if (SCL_OK > result)
    {
        return (result);
    }

    if (0 < result)
    {
        /*@-compdef@*/
        result =
            scl->bignum_func.negate(scl, (uint64_t *)sub_result, nb_32b_words);
        /*@+compdef@*/
        if (SCL_OK > result)
        {
            return (result);
        }

        /*@-compdef@*/
        result = scl->bignum_func.mod(scl, (uint64_t *)sub_result, nb_32b_words,
                                      ctx->modulus, ctx->modulus_nb_32b_words,
                                      (uint64_t *)sub_result);
        /*@+compdef@*/
        if (SCL_OK > result)
        {
            return (result);
        }

        /*@-compdef@*/
        result = scl->bignum_func.sub(scl, ctx->modulus, (uint64_t *)sub_result,
                                      out, ctx->modulus_nb_32b_words);
        /*@+compdef@*/
        if (SCL_OK > result)
        {
            return (result);
        }
    }
    else
    {
        /*@-compdef@*/
        result =
            scl->bignum_func.mod(scl, (uint64_t *)sub_result, nb_32b_words,
                                 ctx->modulus, ctx->modulus_nb_32b_words, out);
        /*@+compdef@*/
        if (SCL_OK > result)
        {
            return (result);
        }
    }

    return (SCL_OK);
}

int32_t soft_bignum_mod_mult(const metal_scl_t *const scl,
                             const bignum_ctx_t *const ctx,
                             const uint64_t *const in_a,
                             const uint64_t *const in_b, uint64_t *const out,
                             size_t nb_32b_words)
{
    int32_t result;
    uint32_t mult_result[nb_32b_words * 2] __attribute__((aligned(8)));

    if ((NULL == scl) || (NULL == ctx) || (NULL == ctx->modulus))
    {
        return (SCL_INVALID_INPUT);
    }

    if (NULL == scl->bignum_func.mult)
    {
        return (SCL_ERROR_API_ENTRY_POINT);
    }

    /* output should be modulus size */
    if (nb_32b_words != ctx->modulus_nb_32b_words)
    {
        return (SCL_INVALID_LENGTH);
    }

    result = scl->bignum_func.mult(scl, in_a, in_b, (uint64_t *)mult_result,
                                   nb_32b_words);
    if (SCL_OK > result)
    {
        return (result);
    }

    /*@-compdef@*/
    result =
        scl->bignum_func.mod(scl, (uint64_t *)mult_result, nb_32b_words * 2,
                             ctx->modulus, ctx->modulus_nb_32b_words, out);
    /*@+compdef@*/
    if (SCL_OK > result)
    {
        return (result);
    }

    return (SCL_OK);
}

int32_t soft_bignum_mod_inv(const metal_scl_t *const scl,
                            const bignum_ctx_t *const ctx,
                            const uint64_t *const input, uint64_t *const out,
                            size_t nb_32b_words)
{
    int32_t result = 0;
    /* x in Hoac, x = input*/
    const uint64_t *x = input;
    /* y in Hoac, y = modulus */
    const uint64_t *y = ctx->modulus;

    if ((NULL == scl) || (NULL == ctx) || (NULL == input) || (NULL == out) ||
        (NULL == ctx->modulus))
    {
        return (SCL_INVALID_INPUT);
    }

    if ((NULL == scl->bignum_func.is_null) ||
        (NULL == scl->bignum_func.negate) ||
        (NULL == scl->bignum_func.rightshift) ||
        (NULL == scl->bignum_func.add) || (NULL == scl->bignum_func.sub))
    {
        return (SCL_ERROR_API_ENTRY_POINT);
    }

    /* output should be modulus size */
    if (nb_32b_words != ctx->modulus_nb_32b_words)
    {
        return (SCL_INVALID_LENGTH);
    }

    result =
        scl->bignum_func.is_null(scl, (const uint32_t *)input, nb_32b_words);
    if (SCL_OK > result)
    {
        return (result);
    }
    else if ((int32_t) false != result)
    {
        return (SCL_INVALID_INPUT);
    }

    result = scl->bignum_func.is_null(scl, (const uint32_t *)ctx->modulus,
                                      nb_32b_words);
    if (SCL_OK > result)
    {
        return (result);
    }
    else if ((int32_t) false != result)
    {
        return (SCL_INVALID_INPUT);
    }

    /**
     * We use simplification base on the fact that the modulus is odd, so we
     * check it
     */
    if (0 == (y[0] & 1))
    {
        return (SCL_ERR_PARITY);
    }

    /**
     * We use here the Extended Euclidiean algorithm to compute the gcd, and
     * get the modular multiplicative inverse.
     * Please note that we will try to keep Hoac (Handbook of applied
     * Cryptography) notation in order to be accessible for the reader.
     */
    {
        /* Initialisation */
        uint32_t u[nb_32b_words] __attribute__((aligned(8)));
        uint32_t v[nb_32b_words] __attribute__((aligned(8)));
        /* A in Hoac */
        uint32_t a[nb_32b_words + 1] __attribute__((aligned(8)));
        /* C in Hoac */
        uint32_t c[nb_32b_words + 1] __attribute__((aligned(8)));

        /* A = 1 */
        memset(a, 0, sizeof(a));
        a[0] = 1;

        /* C = 0 */
        memset(c, 0, sizeof(c));

        /* u = x */
        memcpy(u, x, sizeof(u));
        /* v = y */
        memcpy(v, y, sizeof(v));

        result = scl->bignum_func.is_null(scl, u, nb_32b_words);
        if (SCL_OK > result)
        {
            return (result);
        }

        while ((int32_t) false == result)
        {
            while (0 == (u[0] & 1))
            {
                result = scl->bignum_func.rightshift(
                    scl, (uint64_t *)u, (uint64_t *)u, 1, nb_32b_words);
                if (SCL_OK > result)
                {
                    return (result);
                }

                if ((0 == (a[0] & 1)))
                {
                    result = scl->bignum_func.rightshift(
                        scl, (uint64_t *)a, (uint64_t *)a, 1, nb_32b_words);
                    if (SCL_OK > result)
                    {
                        return (result);
                    }
                }
                else
                {
                    a[nb_32b_words] = (uint32_t)scl->bignum_func.add(
                        scl, (uint64_t *)a, y, (uint64_t *)a, nb_32b_words);
                    if (0 > (int32_t)a[nb_32b_words])
                    {
                        return ((int32_t)a[nb_32b_words]);
                    }
                    result = scl->bignum_func.rightshift(
                        scl, (uint64_t *)a, (uint64_t *)a, 1, nb_32b_words + 1);
                    if (SCL_OK > result)
                    {
                        return (result);
                    }
                }
            }

            while (0 == (v[0] & 1))
            {
                result = scl->bignum_func.rightshift(
                    scl, (uint64_t *)v, (uint64_t *)v, 1, nb_32b_words);
                if (SCL_OK > result)
                {
                    return (result);
                }

                if ((0 == (c[0] & 1)))
                {
                    result = scl->bignum_func.rightshift(
                        scl, (uint64_t *)c, (uint64_t *)c, 1, nb_32b_words);
                    if (SCL_OK > result)
                    {
                        return (result);
                    }
                }
                else
                {
                    c[nb_32b_words] = (uint32_t)scl->bignum_func.add(
                        scl, (uint64_t *)c, y, (uint64_t *)c, nb_32b_words);
                    if (0 > (int32_t)c[nb_32b_words])
                    {
                        return ((int32_t)c[nb_32b_words]);
                    }
                    result = scl->bignum_func.rightshift(
                        scl, (uint64_t *)c, (uint64_t *)c, 1, nb_32b_words + 1);
                    if (SCL_OK > result)
                    {
                        return (result);
                    }
                }
            }

            if (0 <= scl->bignum_func.compare(scl, (uint64_t *)u, (uint64_t *)v,
                                              nb_32b_words))
            {
                result = scl->bignum_func.sub(scl, (uint64_t *)u, (uint64_t *)v,
                                              (uint64_t *)u, nb_32b_words);
                if (SCL_OK > result)
                {
                    return (result);
                }

                /* This is some kind of modular substraction here */
                result = scl->bignum_func.sub(scl, (uint64_t *)a, (uint64_t *)c,
                                              (uint64_t *)a, nb_32b_words);
                if (SCL_OK > result)
                {
                    return (result);
                }
                else if (1 == result)
                {
                    result = scl->bignum_func.negate(scl, (uint64_t *)a,
                                                     nb_32b_words);
                    if (SCL_OK > result)
                    {
                        return (result);
                    }
                    result = scl->bignum_func.sub(scl, y, (uint64_t *)a,
                                                  (uint64_t *)a, nb_32b_words);
                    if (SCL_OK > result)
                    {
                        return (result);
                    }
                }
            }
            else
            {
                result = scl->bignum_func.sub(scl, (uint64_t *)v, (uint64_t *)u,
                                              (uint64_t *)v, nb_32b_words);
                if (SCL_OK > result)
                {
                    return (result);
                }

                /* This is some kind of modular substraction here */
                result = scl->bignum_func.sub(scl, (uint64_t *)c, (uint64_t *)a,
                                              (uint64_t *)c, nb_32b_words);
                if (1 == result)
                {
                    result = scl->bignum_func.negate(scl, (uint64_t *)c,
                                                     nb_32b_words);
                    if (SCL_OK > result)
                    {
                        return (result);
                    }

                    result = scl->bignum_func.sub(scl, y, (uint64_t *)c,
                                                  (uint64_t *)c, nb_32b_words);
                    if (SCL_OK > result)
                    {
                        return (result);
                    }
                }
            }
            result = scl->bignum_func.is_null(scl, u, nb_32b_words);
            if (SCL_OK > result)
            {
                return (result);
            }
        }

        /* check inversible, v = 1 */
        if (nb_32b_words > 1)
        {
            result = scl->bignum_func.is_null(scl, &v[1], nb_32b_words - 1);
            if (SCL_OK > result)
            {
                return (result);
            }
        }

        if ((v[0] != 1) || ((int32_t) false == result))
        {
            return (SCL_NOT_INVERSIBLE);
        }

        memcpy(out, c, nb_32b_words * sizeof(uint32_t));
    }

    return (SCL_OK);
}
