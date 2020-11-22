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
 * @file soft_bignumbers.h
 * @brief arithmetic on bignumber, software implementation
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_SOFT_BIGNUMBERS_H
#define SCL_BACKEND_SOFT_BIGNUMBERS_H

#include <crypto_cfg.h>
#include <stddef.h>
#include <stdint.h>

#include <api/bignumbers/bignumbers.h>
#include <api/scl_api.h>

/**
 * @addtogroup SOFTWARE
 * @addtogroup SOFT_API_BIGNUM
 * @ingroup SOFTWARE
 *  @{
 */

/*@-exportlocal@*/
/**
 * @brief compare two big interger of same length
 *
 * @param[in] scl           metal scl context
 * @param[in] a             first array to compare
 * @param[in] b             second array to compare
 * @param[in] nb_32b_words  number of 32 bits words to compare
 * @return 0            a == b
 * @return 1            a > b
 * @return -1           a < b
 * @warning No check on pointer value
 * @warning Not time constant
 */
CRYPTO_FUNCTION int32_t soft_bignum_compare(const metal_scl_t *const scl,
                                            const uint64_t *const a,
                                            const uint64_t *const b,
                                            size_t nb_32b_words);

/**
 * @brief compare two big interger of different length
 *
 * @param[in] scl               metal scl context
 * @param[in] a                 first array to compare
 * @param[in] a_nb_32b_words    number of 32 bits words in first array
 * @param[in] b                 second array to compare
 * @param[in] b_nb_32b_words    number of 32 bits words in second array
 * @return 0            a == b
 * @return 1            a > b
 * @return -1           a < b
 * @warning No check on pointer value
 */
CRYPTO_FUNCTION int32_t soft_bignum_compare_len_diff(
    const metal_scl_t *const scl, const uint64_t *const a,
    size_t a_nb_32b_words, const uint64_t *const b, size_t b_nb_32b_words);

/**
 * @brief check if the bignumber is null
 *
 * @param[in] scl               metal scl context
 * @param[in] array             array of integers (big integer)
 * @param[in] nb_32b_words      number of 32 bits word in the array
 * @return true (== 1)      if the big integer is null
 * @return false (== 0)     if the big integer is not null
 * @return <0               In case of error
 */
CRYPTO_FUNCTION int32_t soft_bignum_is_null(const metal_scl_t *const scl,
                                            const uint32_t *const array,
                                            size_t nb_32b_words);

/**
 * @brief negate bignumber
 * @details perform array = -array
 *
 * @param[in] scl           metal scl context
 * @param[in,out] array     Input array a
 * @param[in] nb_32b_words  number of 32 bits words to use in calcul
 * @return >= 0 carry from the operation
 * @return < 0 otherwise @ref scl_errors_t
 * @warning This function can be used in case of negative substration result
 */
CRYPTO_FUNCTION int32_t soft_bignum_negate(const metal_scl_t *const scl,
                                           uint64_t *const array,
                                           size_t nb_32b_words);

/**
 * @brief Increment big number by one
 *
 * @param[in] scl           metal scl context
 * @param[in,out] array     Input array a
 * @param[in] nb_32b_words  number of 32 bits words to use in calcul
 * @return >= 0 carry from the operation
 * @return < 0 otherwise @ref scl_errors_t
 * @warning Warning the big number need to be little endian convert if necessary
 * @warning nb_32b_words is limited to 0x3FFFFFFF
 */
CRYPTO_FUNCTION int32_t soft_bignum_inc(const metal_scl_t *const scl,
                                        uint64_t *const array,
                                        size_t nb_32b_words);

/**
 * @brief Do big number addition
 *
 * @param[in] scl           metal scl context
 * @param[in] in_a              Input array a
 * @param[in] in_b              Input array b
 * @param[out] out              Output array (addition result)
 * @param[in] nb_32b_words      number of 32 bits words to use in calcul
 * @return >= 0  carry from the operation
 * @return < 0 otherwise @ref scl_errors_t
 * @warning Warning the big number need to be little endian convert if necessary
 * @warning nb_32b_words is limited to 0x3FFFFFFF
 * @note it is safe to reuse any input buffer as output buffer
 */
CRYPTO_FUNCTION int32_t soft_bignum_add(const metal_scl_t *const scl,
                                        const uint64_t *const in_a,
                                        const uint64_t *const in_b,
                                        uint64_t *const out,
                                        size_t nb_32b_words);

/**
 * @brief Do big number ber subtraction
 *
 * @param[in] scl           metal scl context
 * @param[in] in_a              Input array a
 * @param[in] in_b              Input array b
 * @param[out] out              Output array (subtration result)
 * @param[in] nb_32b_words      number of 32 bits words to use in calcul
 * @return >= 0  borrow from the operation
 * @return < 0 otherwise @ref scl_errors_t
 * @warning Warning the big number need to be little endian convert if necessary
 * @warning nb_32b_words is limited to 0x3FFFFFFF
 * @warning bignumber in input are considered unsigned
 * @warning carry is set when in_a < in_b (in case a positive number is
 * intended, you can do a bitwise not)
 * @note it is safe to reuse any input buffer as output buffer
 */
CRYPTO_FUNCTION int32_t soft_bignum_sub(const metal_scl_t *const scl,
                                        const uint64_t *const in_a,
                                        const uint64_t *const in_b,
                                        uint64_t *const out,
                                        size_t nb_32b_words);

/**
 * @brief Big integer multiplication
 *
 * @param[in] scl           metal scl context
 * @param[in] in_a          Input array a
 * @param[in] in_b          Input array a
 * @param[out] out          Output array, should be twice the size of input
 * array
 * @param[in] nb_32b_words  Number of words, of inputs arrays
 * @return 0 success
 * @return != 0 otherwise @ref scl_errors_t
 * @warning Output should be 2 time the size of Inputs arrays
 */
CRYPTO_FUNCTION int32_t soft_bignum_mult(const metal_scl_t *const scl,
                                         const uint64_t *const in_a,
                                         const uint64_t *const in_b,
                                         uint64_t *const out,
                                         size_t nb_32b_words);

/**
 * @brief bignumber left shift
 *
 * @param[in] scl           metal scl context
 * @param[in] in            big integer array to left shift
 * @param[out] out          output big integer
 * @param[in] shift         number of bits to left shift
 * @param[in] nb_32b_words  size of the big integer in 32bits words
 * @return 0 success
 * @return != 0 otherwise @ref scl_errors_t
 * @note it is safe to reuse any input buffer as output buffer
 */
CRYPTO_FUNCTION int32_t soft_bignum_leftshift(const metal_scl_t *const scl,
                                              const uint64_t *const in,
                                              uint64_t *const out, size_t shift,
                                              size_t nb_32b_words);

/**
 * @brief bignumber right shift
 *
 * @param[in] scl           metal scl context
 * @param[in] in            big integer array to right shift
 * @param[out] out          output big integer
 * @param[in] shift         number of bits to right shift
 * @param[in] nb_32b_words  size of the big integer in 32bits words
 * @return 0 success
 * @return != 0 otherwise @ref scl_errors_t
 * @note it is safe to reuse any input buffer as output buffer
 */
CRYPTO_FUNCTION int32_t soft_bignum_rightshift(const metal_scl_t *const scl,
                                               const uint64_t *const in,
                                               uint64_t *const out,
                                               size_t shift,
                                               size_t nb_32b_words);

/**
 * @brief return most significant bit set in word
 *
 * @param[in] word_64b      64 bits word
 * @return > 0          index of the most significant bit set
 * @note the first bit has index 1, therefore no bit set return 0
 */
CRYPTO_FUNCTION int32_t soft_bignum_msb_set_in_word(uint64_t word_64b);

/**
 * @brief Get msb set in bignumber
 *
 * @param[in] scl           metal scl context
 * @param[in] array         input array (bignumber)
 * @param[in] nb_32b_words  size of the big integer in 32bits words
 * @return >= 0 success, it's the actual index of the most significant bit set
 * @return < 0 in case of errors @ref scl_errors_t
 * @note the first bit has index 1, therefore no bit set return 0
 */
CRYPTO_FUNCTION int32_t soft_bignum_get_msb_set(const metal_scl_t *const scl,
                                                const uint64_t *const array,
                                                size_t nb_32b_words);

/**
 * @brief set one bit in a big integer
 *
 * @param[in] scl           metal scl context
 * @param[in,out] array     input array (bignumber)
 * @param[in] nb_32b_words  size of the big integer in 32bits words
 * @param[in] bit_2_set     index of the bit to set in the big integer
 * @return >= 0 success
 * @return < 0 in case of errors @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t soft_bignum_set_bit(const metal_scl_t *const scl,
                                            uint64_t *const array,
                                            size_t nb_32b_words,
                                            size_t bit_2_set);

/**
 * @brief perform big integer division
 *
 * @param[in] scl                   metal scl context
 * @param[in] dividend              dividend array (big integer)
 * @param[in] dividend_nb_32b_words number of 32 words in dividend array
 * @param[in] divisor               divisor array (big integer)
 * @param[in] divisor_nb_32b_words  number of 32 words in divisor array
 * @param[out] remainder            remainder array (big integer)
 * @param[out] quotient             quotient array (big integer)
 * @return >= 0 success
 * @return < 0 in case of errors @ref scl_errors_t
 * @note remainder should be at least of length equal to divisor_nb_32b_words
 * @note quotient should be at least of length equal to dividend_nb_32b_words
 * @note remainder and quotient are not mandatory
 * @note the implementation is based on Egyptian method (slightly adapted),
 * described here
 * http://compoasso.free.fr/primelistweb/page/prime/euclide_en.php (method 4)
 * @warning This function use internally 2 buffer allocated on stack that can
 * reach dividend_nb_32b_words
 */
CRYPTO_FUNCTION int32_t soft_bignum_div(const metal_scl_t *const scl,
                                        const uint64_t *const dividend,
                                        size_t dividend_nb_32b_words,
                                        const uint64_t *const divisor,
                                        size_t divisor_nb_32b_words,
                                        /*@null@*/ uint64_t *const remainder,
                                        /*@null@*/ uint64_t *const quotient);

/**
 * @brief compute modulus
 * @details perform : remainder = in mod modulus
 *
 * @param[in] scl                   metal scl context
 * @param[in] in                    input big integer (on which the modulus is
 * applied)
 * @param[in] in_nb_32b_words       number of 32 words in input array
 * @param[in] modulus               modulus big integer to apply
 * @param[in] modulus_nb_32b_words  number of 32 words in modulus array
 * @param[out] remainder            remainder array (big integer)
 * @return >= 0 success
 * @return < 0 in case of errors @ref scl_errors_t
 * @note remainder should be at least of length equal to modulus_nb_32b_words
 * @warning This function might call @ref soft_bignum_div depending on scl
 * content and therefore have buffer allocation on stack
 */
CRYPTO_FUNCTION int32_t soft_bignum_mod(const metal_scl_t *const scl,
                                        const uint64_t *const in,
                                        size_t in_nb_32b_words,
                                        const uint64_t *const modulus,
                                        size_t modulus_nb_32b_words,
                                        uint64_t *const remainder);

/**
 * @brief register new modulus array
 * @details register ref of modulus in context
 *
 * @param[in] scl                   metal scl context
 * @param[out] ctx                  bignumber context that will be updated
 * @param[in] modulus               modulus to use for the next modular
 * operations
 * @param[in] modulus_nb_32b_words  size of the modulus array
 * @return >= 0 success
 * @return < 0 in case of errors @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t soft_bignum_set_modulus(const metal_scl_t *const scl,
                                                bignum_ctx_t *const ctx,
                                                const uint64_t *const modulus,
                                                size_t modulus_nb_32b_words);

/**
 * @brief Clear context modulus
 * @details clear ref of modulus in context
 *
 * @param[in] scl                   metal scl context
 * @param[in,out] ctx                  bignumber context that will be updated
 * @return >= 0 success
 * @return < 0 in case of errors @ref scl_errors_t
 */
CRYPTO_FUNCTION int32_t soft_bignum_clear_modulus(const metal_scl_t *const scl,
                                                  bignum_ctx_t *const ctx);

/**
 * @brief Modular negate
 * @details out = -in mod modulus
 *
 * @param[in] scl               metal scl context
 * @param[out] ctx              bignumber context that will be updated
 * @param[in] in                Input array
 * @param[out] out              Output array
 * @param[in] nb_32b_words      number of 32 bits words to use in calcul
 * @return >= 0 success
 * @return < 0 in case of errors @ref scl_errors_t
 * @warning This function allocates internally 1 buffer on stack that can reach
 * nb_32b_words and the ones from @ref soft_bignum_mod
 */
CRYPTO_FUNCTION int32_t soft_bignum_mod_neg(const metal_scl_t *const scl,
                                            const bignum_ctx_t *const ctx,
                                            const uint64_t *const in,
                                            uint64_t *const out,
                                            size_t nb_32b_words);

/**
 * @brief Modular addition
 *
 * @param[in] scl               metal scl context
 * @param[in] ctx               bignumber context (contain modulus info)
 * @param[in] in_a              Input array a
 * @param[in] in_b              Input array b
 * @param[out] out              Output array (addition result)
 * @param[in] nb_32b_words      number of 32 bits words to use in calcul
 * @return >= 0 success
 * @return < 0 in case of errors @ref scl_errors_t
 * @warning the modulus used should be of nb_32b_words size
 * @warning This function allocates internally 1 buffer on stack that can reach
 * nb_32b_words + 1 and the ones from @ref soft_bignum_mod
 */
CRYPTO_FUNCTION int32_t soft_bignum_mod_add(const metal_scl_t *const scl,
                                            const bignum_ctx_t *const ctx,
                                            const uint64_t *const in_a,
                                            const uint64_t *const in_b,
                                            uint64_t *const out,
                                            size_t nb_32b_words);

/**
 * @brief Modular subtraction
 *
 * @param[in] scl               metal scl context
 * @param[in] ctx               bignumber context (contain modulus info)
 * @param[in] in_a              Input array a
 * @param[in] in_b              Input array b
 * @param[out] out              Output array (subtraction result)
 * @param[in] nb_32b_words      number of 32 bits words to use in calcul
 * @return >= 0 success
 * @return < 0 in case of errors @ref scl_errors_t
 * @warning the modulus used should be of nb_32b_words size
 * @warning This function allocates internally 1 buffer on stack that can reach
 * nb_32b_words and the ones from @ref soft_bignum_mod
 */
CRYPTO_FUNCTION int32_t soft_bignum_mod_sub(const metal_scl_t *const scl,
                                            const bignum_ctx_t *const ctx,
                                            const uint64_t *const in_a,
                                            const uint64_t *const in_b,
                                            uint64_t *const out,
                                            size_t nb_32b_words);

/**
 * @brief Modular multiplication
 *
 * @param[in] scl           metal scl context
 * @param[in] ctx           bignumber context (contain modulus info)
 * @param[in] in_a          Input array a
 * @param[in] in_b          Input array b
 * @param[out] out          Output array
 * @param[in] nb_32b_words  Number of words, of inputs arrays and output array
 * @return >= 0 success
 * @return < 0 in case of errors @ref scl_errors_t
 * @warning the modulus used should be of nb_32b_words size
 * @warning This function allocates internally 1 buffer on stack that can reach
 * 2 * nb_32b_words and the ones from @ref soft_bignum_mod
 */
CRYPTO_FUNCTION int32_t soft_bignum_mod_mult(const metal_scl_t *const scl,
                                             const bignum_ctx_t *const ctx,
                                             const uint64_t *const in_a,
                                             const uint64_t *const in_b,
                                             uint64_t *const out,
                                             size_t nb_32b_words);

/**
 * @brief Modular inverse
 *
 * @param[in] scl           metal scl context
 * @param[in] ctx           bignumber context (contain modulus info)
 * @param[in] in            Input array
 * @param[out] out          Output array
 * @param[in] nb_32b_words  Number of words, of inputs arrays and output array
 * @return >= 0 success
 * @return < 0 in case of errors @ref scl_errors_t
 * @warning input should be prime with ctx->modulus
 * @warning This function allocates internally 1 buffer on stack that can reach
 * 4 * nb_32b_words + 2.
 */
CRYPTO_FUNCTION int32_t soft_bignum_mod_inv(const metal_scl_t *const scl,
                                            const bignum_ctx_t *const ctx,
                                            const uint64_t *const in,
                                            uint64_t *const out,
                                            size_t nb_32b_words);

/*@+exportlocal@*/

/** @}*/

#endif /* SCL_BACKEND_SOFT_BIGNUMBERS_H */
