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
 * @file hca_sha_miscellaneous.c
 * @brief
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <stdint.h>
#include <stdio.h>

#include <metal/io.h>
#include <metal/machine/platform.h>

#include <scl/scl_retdefs.h>

#include <api/hardware/hca_macro.h>
#include <api/hardware/scl_hca.h>

#if METAL_SIFIVE_HCA_VERSION >= HCA_VERSION(0, 5, 0)
#include <api/hardware/v0.5/hash/hca_sha_miscellaneous.h>
#include <api/hardware/v0.5/sifive_hca-0.5.x.h>

int32_t hca_sha_block(const metal_scl_t *const scl, hash_mode_t hash_mode,
                      uint32_t NbBlocks512, const uint8_t *const data_in)
{
#if __riscv_xlen == 64
    const uint64_t *in64 = (const uint64_t *)data_in;
#elif __riscv_xlen == 32
    const uint32_t *in32 = (const uint32_t *)data_in;
#endif
    size_t i;

    if (0 == METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_SHA_REV))
    {
        // revision of SHA is Zero so the SHA is not present.
        return SCL_ERROR;
    }

    if (NbBlocks512 == 0)
    {
        return SCL_INVALID_INPUT;
    }

    if ((NbBlocks512 & 0x1) && (hash_mode >= SCL_HASH_SHA384))
    {
        // nb block should be even to have 1024bits
        return SCL_INVALID_INPUT;
    }

    for (size_t k = 0; k < NbBlocks512; k++)
    {
        // Put data in the FIFO
        // Wait for IFIFOEMPTY is cleared
        while ((METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_CR) >>
                HCA_REGISTER_CR_IFIFOFULL_OFFSET) &
               HCA_REGISTER_CR_IFIFOFULL_MASK)
        {
        }
#if __riscv_xlen == 64
        if ((uint64_t)data_in & 0x7)
        {
            i = k << 6;
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_64BITS(data_in, i);
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_64BITS(data_in, (i + 8));
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_64BITS(data_in, (i + 16));
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_64BITS(data_in, (i + 24));
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_64BITS(data_in, (i + 32));
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_64BITS(data_in, (i + 40));
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_64BITS(data_in, (i + 48));
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_64BITS(data_in, (i + 56));
        }
        else
        {
            i = k << 3;
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in64[i];
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in64[i + 1];
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in64[i + 2];
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in64[i + 3];
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in64[i + 4];
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in64[i + 5];
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in64[i + 6];
            METAL_REG64(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in64[i + 7];
        }
#elif __riscv_xlen == 32
        if ((uint32_t)data_in & 0x3)
        {
            i = k << 6;
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, i);
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 4));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 8));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 12));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 16));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 20));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 24));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 28));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 32));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 36));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 40));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 44));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 48));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 52));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 56));
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) =
                GET_32BITS(data_in, (i + 60));
        }
        else
        {
            i = k << 4;
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 1];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 2];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 3];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 4];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 5];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 6];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 7];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 8];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 9];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 10];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 11];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 12];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 13];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 14];
            METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_FIFO_IN) = in32[i + 15];
        }
#endif

        if (hash_mode >= SCL_HASH_SHA384)
        {
            // Need to have 1024bits before SHA end performing.
            if (k & 0x1)
            {
                // Wait for SHABUSY is cleared
                while ((METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_SHA_CR) >>
                        HCA_REGISTER_SHA_CR_BUSY_OFFSET) &
                       HCA_REGISTER_SHA_CR_BUSY_MASK)
                {
                }
            }
        }
        else
        {
            // Wait for SHABUSY is cleared
            while ((METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_SHA_CR) >>
                    HCA_REGISTER_SHA_CR_BUSY_OFFSET) &
                   HCA_REGISTER_SHA_CR_BUSY_MASK)
            {
            }
        }
    }
    return SCL_OK;
}
#endif  /* METAL_SIFIVE_HCA_VERSION >= HCA_VERSION(0, 5, 0) */