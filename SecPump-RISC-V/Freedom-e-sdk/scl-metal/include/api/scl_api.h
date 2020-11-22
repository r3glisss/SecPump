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
 * @file scl_api.h
 * @brief Low level API interface description
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#ifndef SCL_BACKEND_SCL_API_H
#define SCL_BACKEND_SCL_API_H

#include <stdint.h>
#include <stdio.h>

#include <scl/scl_defs.h>
#include <scl/scl_retdefs.h>

#include <api/bignumbers/bignumbers.h>
#include <api/blockcipher/aes/aes.h>
#include <api/hash/sha.h>

/**
 * @addtogroup COMMON
 * @addtogroup API
 * @ingroup COMMON
 *  @{
 */

/*! @brief Low level API entry points */
struct _metal_scl_struct;

/*! @see _metal_scl_struct */
typedef struct _metal_scl_struct metal_scl_t;

/*! @brief AESlow level API entry points */
struct __aes_func
{
    /**
     * @brief set AES key
     *
     * @param[in] scl               metal scl context
     * @param[in] type              key type (128, 192, 256)
     * @param[in] key               key value
     * @param[in] aes_process       aes process (encrypt or decrypt)
     * @return 0                    SUCCESS
     * @return != 0                 otherwise @ref scl_errors_t
     */
    int32_t (*setkey)(const metal_scl_t *const scl, scl_aes_key_type_t type,
                      const uint64_t *const key, scl_process_t aes_process);
    /**
     * @brief set IV (Initial Value)
     *
     * @param[in] scl               metal scl context
     * @param[in] iv                iv value
     * @return 0                    SUCCESS
     * @return != 0                 otherwise @ref scl_errors_t
     */
    int32_t (*setiv)(const metal_scl_t *const scl,
                     const uint64_t *const iv);
    /**
     * @brief perform AES cipher operation
     *
     * @param[in] scl               metal scl context
     * @param[in] aes_mode          AES mode
     * @param[in] data_endianness   endianess of the input data
     * @param[in] data_in           data to process
     * @param[in] data_len          length of the data to process (in byte)
     * @param[out] data_out         data output buffer
     * @return 0                    SUCCESS
     * @return != 0                 otherwise @ref scl_errors_t
     */
    int32_t (*cipher)(const metal_scl_t *const scl, scl_aes_mode_t aes_mode,
                      scl_process_t aes_process,
                      scl_endianness_t data_endianness,
                      const uint8_t *const data_in, size_t data_len,
                      uint8_t *const data_out);
    /**
     * @brief initiliaze AES cipher with authentication operation
     *
     * @param[in] scl               metal scl context
     * @param[in,out] ctx           aes authenticate context
     * @param[in] aes_mode          AES mode
     * @param[in] aes_process       aes process (encrypt or decrypt)
     * @param[in] data_endianness   endianess of the input data
     * @param[in] auth_option       option for the ccm mode
     * @param[in] aad               add data
     * @param[in] add_len           length of the add data (in bytes)
     * @param[in] payload_len       length of the payload data (in bytes)
     * @return 0                    SUCCESS
     * @return != 0                 otherwise @ref scl_errors_t
     */
    int32_t (*auth_init)(const metal_scl_t *const scl, aes_auth_ctx_t *const ctx, scl_aes_mode_t aes_mode,
                    scl_process_t aes_process, scl_endianness_t data_endianness,
                    uint32_t auth_option, const uint8_t *const aad,
                    size_t aad_len, uint64_t payload_len);
    /**
     * @brief perform AES cipher with authentication operation
     *
     * @param[in] scl               metal scl context
     * @param[in,out] ctx           aes authenticate context
     * @param[in] payload           data payload to process
     * @param[in] payload_len       length of the current data payload to process (in bytes)
     * @param[out] data_out         data output buffer
     * @param[out] len_out          length of data (in bytes) write into output buffer
     * @return 0                    SUCCESS
     * @return != 0                 otherwise @ref scl_errors_t
     */
    int32_t (*auth_core)(const metal_scl_t *const scl, aes_auth_ctx_t *const ctx,
                    const uint8_t *const payload, uint64_t payload_len, uint8_t *const data_out, size_t *const len_out);
    /**
     * @brief finish AES cipher with authentication operation
     *
     * @param[in] scl               metal scl context
     * @param[in,out] ctx           aes authenticate context
     * @param[out] data_out         data output buffer to complete operation
     * @param[out] tag              tag output buffer (128 bits)
     * @return 0                    SUCCESS
     * @return != 0                 otherwise @ref scl_errors_t
     */
    int32_t (*auth_finish)(const metal_scl_t *const scl,
                           aes_auth_ctx_t *const ctx, uint8_t *const data_out,
                           uint64_t *const tag);
};

/*! @brief Hash low level API entry points */
struct __hash_func
{
    /**
     * @brief Init sha context
     *
     * @param[in] scl               metal scl context
     * @param[out] ctx              sha context
     * @param[in] hash_mode         hash mode
     * @param[in] data_endianness   endianess of the input data
     * @return 0                    SUCCESS
     * @return != 0                 otherwise @ref scl_errors_t
     * @warning only SCL_BIG_ENDIAN_MODE is supported
     */
    int32_t (*sha_init)(const metal_scl_t *const scl, sha_ctx_t *const ctx,
                        hash_mode_t hash_mode, endianness_t data_endianness);
    /**
     * @brief Compute intermediate hash value of the chunk of data in parameter
     *
     * @param[in] scl               metal scl context
     * @param[in,out] ctx           sha context
     * @param[in] data              data to hash
     * @param[in] data_byte_len     data length to hash
     * @return 0                    SUCCESS
     * @return != 0                 otherwise @ref scl_errors_t
     */
    int32_t (*sha_core)(const metal_scl_t *const scl, sha_ctx_t *const ctx,
                        const uint8_t *const data, size_t data_byte_len);
    /**
     * @brief Compute final hash value of the concatenated block pass to
     * sha_core()
     *
     * @param[in] scl               metal scl context
     * @param[in] ctx               sha context
     * @param[out] hash             hash output buffer
     * @param[in,out] hash_len      length of the hash buffer/length of the hash
     * @return 0                    SUCCESS
     * @return != 0                 otherwise @ref scl_errors_t
     */
    int32_t (*sha_finish)(const metal_scl_t *const scl, sha_ctx_t *const ctx,
                          uint8_t *const hash, size_t *const hash_len);
};

/*! @brief True Random Number Generator low level API entry points */
struct __trng_func
{
    /**
     * @brief Initialize TRNG
     *
     * @param[in] scl               metal scl context
     * @return 0                    SUCCESS
     * @return != 0                 otherwise @ref scl_errors_t
     */
    int32_t (*init)(const metal_scl_t *const scl);
    /**
     * @brief get 32bits random value
     *
     * @param[in] scl               metal scl context
     * @return 0                    SUCCESS
     * @return != 0                 otherwise @ref scl_errors_t
     */
    int32_t (*get_data)(const metal_scl_t *const scl, uint32_t *data_out);
};

/*! @brief Big integer arithmetic low level API entry points */
struct __bignum_func
{
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
     */
    int32_t (*compare)(/*@in@*/ const metal_scl_t *const scl,
                       /*@in@*/ const uint64_t *const a,
                       /*@in@*/ const uint64_t *const b, size_t nb_32b_words);

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
    int32_t (*compare_len_diff)(/*@in@*/ const metal_scl_t *const scl,
                                /*@in@*/ const uint64_t *const a,
                                size_t a_nb_32b_words,
                                /*@in@*/ const uint64_t *const b,
                                size_t b_nb_32b_words);

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
    int32_t (*is_null)(/*@in@*/ const metal_scl_t *const scl,
                       /*@in@*/ const uint32_t *const array,
                       size_t nb_32b_words);

    /**
     * @brief negate bignumber
     *
     * @param[in] scl           metal scl context
     * @param[in,out] array     Input array a
     * @param[in] nb_32b_words  number of 32 bits words to use in calcul
     * @return >= 0 carry from the operation
     * @return < 0 otherwise @ref scl_errors_t
     * @warning This function can be used in case of negative substration result
     */
    int32_t (*negate)(/*@in@*/ const metal_scl_t *const scl,
                      /*@in@*/ uint64_t *const array, size_t nb_32b_words);

    /**
     * @brief Increment big number by one
     *
     * @param[in] scl           metal scl context
     * @param[in,out] array     Input array a
     * @param[in] nb_32b_words  number of 32 bits words to use in calcul
     * @return 0 success
     * @return != 0 otherwise @ref scl_errors_t
     * @warning Warning the big number need to be little endian convert if
     * necessary
     * @warning nb_32b_words is limited to 0x3FFFFFFF
     */
    int32_t (*inc)(/*@in@*/ const metal_scl_t *const scl,
                   /*@in@*/ /*@out@*/ uint64_t *const array,
                   size_t nb_32b_words);

    /**
     * @brief Do big number addition
     *
     * @param[in] scl           metal scl context
     * @param[in] in_a              Input array a
     * @param[in] in_b              Input array b
     * @param[out] out              Output array (addition result)
     * @param[in] nb_32b_words      number of 32 bits words to use in calcul
     * @return 0 success
     * @return != 0 otherwise @ref scl_errors_t
     * @warning Warning the big number need to be little endian convert if
     * necessary
     * @warning nb_32b_words is limited to 0x3FFFFFFF
     * @note it is safe to reuse any input buffer as output buffer
     */
    int32_t (*add)(const metal_scl_t *const scl,
                   /*@in@*/ const uint64_t *const in_a,
                   /*@in@*/ const uint64_t *const in_b,
                   /*@out@*/ uint64_t *const out, size_t nb_32b_words);

    /**
     * @brief Do big number ber substraction
     *
     * @param[in] scl           metal scl context
     * @param[in] in_a              Input array a
     * @param[in] in_b              Input array b
     * @param[out] out              Output array (substration result)
     * @param[in] nb_32b_words      number of 32 bits words to use in calcul
     * @return 0 success
     * @return != 0 otherwise @ref scl_errors_t
     * @warning Warning the big number need to be little endian convert if
     * necessary
     * @warning nb_32b_words is limited to 0x3FFFFFFF
     * @warning bignumber in input are considered unsigned
     * @warning carry is set when in_a < in_b (in case a positive number is
     * intended, you can do a bitwise not)
     * @note it is safe to reuse any input buffer as output buffer
     */
    int32_t (*sub)(const metal_scl_t *const scl,
                   /*@in@*/ const uint64_t *const in_a,
                   /*@in@*/ const uint64_t *const in_b,
                   /*@out@*/ uint64_t *const out, size_t nb_32b_words);

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
    int32_t (*mult)(const metal_scl_t *const scl,
                    /*@in@*/ const uint64_t *const in_a,
                    /*@in@*/ const uint64_t *const in_b,
                    /*@out@*/ uint64_t *const out, size_t nb_32b_words);

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
    int32_t (*leftshift)(/*@in@*/ const metal_scl_t *const scl,
                         /*@in@*/ const uint64_t *const in,
                         /*@out@*/ uint64_t *const out, size_t shift,
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
    int32_t (*rightshift)(/*@in@*/ const metal_scl_t *const scl,
                          /*@in@*/ const uint64_t *const in,
                          /*@out@*/ uint64_t *const out, size_t shift,
                          size_t nb_32b_words);

    /**
     * @brief return most significant bit set in word
     *
     * @param[in] word_64b      64 bits word
     * @return > 0          index of the most significant bit set
     * @note the first bit has index 1, therefore no bit set return 0
     */
    int32_t (*msb_set_in_word)(uint64_t word_64b);

    /**
     * @brief Get msb set in bignumber
     *
     * @param[in] scl           metal scl context
     * @param[in] array         input array (bignumber)
     * @param[in] nb_32b_words  size of the big integer in 32bits words
     * @return >= 0 success, it's the actual index of the most significant bit
     * set
     * @return < 0 in case of errors @ref scl_errors_t
     * @note the first bit has index 1, therefore no bit set return 0
     */
    int32_t (*get_msb_set)(/*@in@*/ const metal_scl_t *const scl,
                           /*@in@*/ const uint64_t *const array,
                           size_t nb_32b_words);

    /**
     * @brief set one bit in a big integer
     *
     * @param[in] scl           metal scl context
     * @param[in/out] array     input array (bignumber)
     * @param[in] nb_32b_words  size of the big integer in 32bits words
     * @param[in] bit_2_set     index of the bit to set in the big integer
     * @return >= 0 success
     * @return < 0 in case of errors @ref scl_errors_t
     */
    int32_t (*set_bit)(/*@in@*/ const metal_scl_t *const scl,
                       /*@in@*/ /*@out@*/ uint64_t *const array,
                       size_t nb_32b_words, size_t bit_2_set);

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
     * @note remainder should be at least of length equal to
     * divisor_nb_32b_words
     * @note quotient should be at least of length equal to
     * dividend_nb_32b_words
     * @note remainder and quotient are not mandatory
     */
    int32_t (*div)(const metal_scl_t *const scl,
                   /*@in@*/ const uint64_t *const dividend,
                   size_t dividend_nb_32b_words,
                   /*@in@*/ const uint64_t *const divisor,
                   size_t divisor_nb_32b_words,
                   /*@out@*/ /*@null@*/ uint64_t *const remainder,
                   /*@out@*/ /*@null@*/ uint64_t *const quotient);

    /**
     * @brief compute modulus
     * @details perform : remainder = in mod modulus
     *
     * @param[in] scl                   metal scl context
     * @param[in] in                    input big integer (on which the modulus
     * is applied)
     * @param[in] in_nb_32b_words       number of 32 words in input array
     * @param[in] modulus               modulus big integer to apply
     * @param[in] modulus_nb_32b_words  number of 32 words in modulus array
     * @param[out] remainder            remainder array (big integer)
     * @return >= 0 success
     * @return < 0 in case of errors @ref scl_errors_t
     * @note remainder should be at least of length equal to
     * modulus_nb_32b_words
     */
    int32_t (*mod)(/*@in@*/ const metal_scl_t *const scl,
                   /*@in@*/ const uint64_t *const in, size_t in_nb_32b_words,
                   /*@in@*/ const uint64_t *const modulus,
                   size_t modulus_nb_32b_words,
                   /*@out@*/ uint64_t *const remainder);

    /**
     * @brief register new modulus array
     *
     * @param[in] scl                   metal scl context
     * @param[out] ctx                  bignumber context that will be updated
     * @param[in] modulus               modulus to use for the next modular
     * operations
     * @param[in] modulus_nb_32b_words  size of the modulus array
     * @return >= 0 success
     * @return < 0 in case of errors @ref scl_errors_t
     */
    int32_t (*set_modulus)(/*@in@*/ const metal_scl_t *const scl,
                           /*@out@*/ bignum_ctx_t *const ctx,
                           /*@in@*/ const uint64_t *const modulus,
                           size_t modulus_nb_32b_words);

    /**
     * @brief Modular negate
     * @details out = -in mod modulus
     *
     * @param[in] scl               metal scl context
     * @param[out] ctx              bignumber context that will be updated
     * @param[in] in_a              Input array
     * @param[out] out              Output array
     * @param[in] nb_32b_words      number of 32 bits words to use in calcul
     * @return >= 0 success
     * @return < 0 in case of errors @ref scl_errors_t
     */
    int32_t (*mod_neg)(/*@in@*/ const metal_scl_t *const scl,
                       /*@in@*/ const bignum_ctx_t *const ctx,
                       /*@in@*/ const uint64_t *const in,
                       /*@out@*/ uint64_t *const out, size_t nb_32b_words);

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
     */
    int32_t (*mod_add)(/*@in@*/ const metal_scl_t *const scl,
                       /*@in@*/ const bignum_ctx_t *const ctx,
                       /*@in@*/ const uint64_t *const in_a,
                       /*@in@*/ const uint64_t *const in_b,
                       /*@out@*/ uint64_t *const out, size_t nb_32b_words);

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
     */
    int32_t (*mod_sub)(/*@in@*/ const metal_scl_t *const scl,
                       /*@in@*/ const bignum_ctx_t *const ctx,
                       /*@in@*/ const uint64_t *const in_a,
                       /*@in@*/ const uint64_t *const in_b,
                       /*@out@*/ uint64_t *const out, size_t nb_32b_words);

    /**
     * @brief Modular multiplication
     *
     * @param[in] scl           metal scl context
     * @param[in] ctx           bignumber context (contain modulus info)
     * @param[in] in_a          Input array a
     * @param[in] in_b          Input array b
     * @param[out] out          Output array
     * @param[in] nb_32b_words  Number of words, of inputs arrays and output
     * array
     * @return >= 0 success
     * @return < 0 in case of errors @ref scl_errors_t
     * @warning the modulus used should be of nb_32b_words size
     */
    int32_t (*mod_mult)(/*@in@*/ const metal_scl_t *const scl,
                        /*@in@*/ const bignum_ctx_t *const ctx,
                        /*@in@*/ const uint64_t *const in_a,
                        /*@in@*/ const uint64_t *const in_b,
                        /*@out@*/ uint64_t *const out, size_t nb_32b_words);

    /**
     * @brief Modular inverse
     *
     * @param[in] scl           metal scl context
     * @param[in] ctx           bignumber context (contain modulus info)
     * @param[in] in            Input array
     * @param[out] out          Output array
     * @param[in] nb_32b_words  Number of words, of inputs arrays and output
     * array
     * @return >= 0 success
     * @return < 0 in case of errors @ref scl_errors_t
     * @warning input should be prime with ctx->modulus, otherwise an error is
     * returned
     */
    int32_t (*mod_inv)(/*@in@*/ const metal_scl_t *const scl,
                       /*@in@*/ const bignum_ctx_t *const ctx,
                       /*@in@*/ const uint64_t *const in,
                       /*@out@*/ uint64_t *const out, size_t nb_32b_words);
};

/*! @see _metal_scl_struct */
struct _metal_scl_struct
{
#if __riscv_xlen == 64
    const uint64_t hca_base;
#elif __riscv_xlen == 32
    const uint32_t hca_base;
#endif
    const struct __aes_func aes_func;
    const struct __hash_func hash_func;
    const struct __trng_func trng_func;
    const struct __bignum_func bignum_func;
};

/*@unused@*/ static __inline__ int32_t
default_aes_setkey(const metal_scl_t *const scl, scl_aes_key_type_t type,
                   const uint64_t *const key, scl_process_t aes_process)
{
    /*@-noeffect@*/
    (void)scl;
    (void)type;
    (void)key;
    (void)aes_process;
    /*@+noeffect@*/
    return SCL_ERROR;
}

/*@unused@*/ static __inline__ int32_t 
default_aes_setiv(const metal_scl_t *const scl,
                  const uint64_t *const iv)
{
    /*@-noeffect@*/
    (void)scl;
    (void)iv;
    /*@+noeffect@*/
    return SCL_ERROR;
}

/*@unused@*/ static __inline__ int32_t
default_aes_cipher(const metal_scl_t *const scl, scl_aes_mode_t aes_mode,
                   scl_process_t aes_process,
                   scl_endianness_t data_endianness,
                   const uint8_t *const data_in, size_t data_len,
                   uint8_t *const data_out)
{
    /*@-noeffect@*/
    (void)scl;
    (void)aes_mode;
    (void)aes_process;
    (void)data_endianness;
    (void)data_in;
    (void)data_len;
    (void)data_out;
    /*@+noeffect@*/
    return SCL_ERROR;
}

/*@unused@*/ static __inline__ int32_t 
default_aes_auth_init(const metal_scl_t *const scl, aes_auth_ctx_t *const ctx, scl_aes_mode_t aes_mode,
                      scl_process_t aes_process, scl_endianness_t data_endianness,
                      uint32_t auth_option, const uint8_t *const aad,
                      size_t aad_len, uint64_t payload_len)
{
    /*@-noeffect@*/
    (void)scl;
    (void)ctx;
    (void)aes_mode;
    (void)aes_process;
    (void)data_endianness;
    (void)auth_option;
    (void)aad;
    (void)aad_len;
    (void)payload_len;
    /*@+noeffect@*/
    return SCL_ERROR;
}

/*@unused@*/ static __inline__ int32_t
default_aes_auth_core(const metal_scl_t *const scl, aes_auth_ctx_t *const ctx,
                      const uint8_t *const payload, uint64_t payload_len,
                      uint8_t *const data_out, size_t *const len_out)
{
    /*@-noeffect@*/
    (void)scl;
    (void)ctx;
    (void)payload;
    (void)payload_len;
    (void)data_out;
    (void)len_out;
    /*@+noeffect@*/
    return SCL_ERROR;
}

/*@unused@*/ static __inline__ int32_t
default_aes_auth_finish(const metal_scl_t *const scl, aes_auth_ctx_t *const ctx,
                        uint8_t *const data_out, uint64_t *const tag)
{
    /*@-noeffect@*/
    (void)scl;
    (void)ctx;
    (void)data_out;
    (void)tag;
    /*@+noeffect@*/
    return SCL_ERROR;
}

/*@unused@*/ static __inline__ int32_t
default_sha_init(const metal_scl_t *const scl, sha_ctx_t *const ctx,
                 hash_mode_t hash_mode, endianness_t data_endianness)
{
    /*@-noeffect@*/
    (void)scl;
    (void)ctx;
    (void)hash_mode;
    (void)data_endianness;
    /*@+noeffect@*/
    return SCL_ERROR;
}

/*@unused@*/ static __inline__ int32_t
default_sha_core(const metal_scl_t *const scl, sha_ctx_t *const ctx,
                 const uint8_t *const data, size_t data_byte_len)
{
    /*@-noeffect@*/
    (void)scl;
    (void)ctx;
    (void)data;
    (void)data_byte_len;
    /*@+noeffect@*/
    return SCL_ERROR;
}

/*@unused@*/ static __inline__ int32_t
default_sha_finish(const metal_scl_t *const scl, sha_ctx_t *const ctx,
                   uint8_t *const hash, size_t *const hash_len)
{
    /*@-noeffect@*/
    (void)scl;
    (void)ctx;
    (void)hash;
    (void)hash_len;
    /*@+noeffect@*/
    return SCL_ERROR;
}

/*@unused@*/ static __inline__ int32_t
default_trng_init(const metal_scl_t *const scl)
{
    /*@-noeffect@*/
    (void)scl;
    /*@+noeffect@*/
    return SCL_ERROR;
}

/*@unused@*/ static __inline__ int32_t
default_trng_getdata(const metal_scl_t *const scl, uint32_t *data_out)
{
    /*@-noeffect@*/
    (void)scl;
    (void)data_out;
    /*@+noeffect@*/
    return SCL_ERROR;
}

/** @}*/

#endif /* SCL_BACKEND_SCL_API_H */
