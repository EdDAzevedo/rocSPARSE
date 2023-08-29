/*! \file */
/* ************************************************************************
 * Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * ************************************************************************ */

#include "internal/level3/rocsparse_gebsrmm.h"
#include "rocsparse_gebsrmm.hpp"

#include "../level2/rocsparse_gebsrmv.hpp"
#include "rocsparse_bsrmm.hpp"

#include "common.h"
#include "utility.h"

template <typename T, typename U>
rocsparse_status rocsparse_gebsrmm_template_small(rocsparse_handle          handle,
                                                  rocsparse_direction       dir,
                                                  rocsparse_operation       trans_A,
                                                  rocsparse_operation       trans_B,
                                                  rocsparse_int             mb,
                                                  rocsparse_int             n,
                                                  rocsparse_int             kb,
                                                  rocsparse_int             nnzb,
                                                  U                         alpha,
                                                  const rocsparse_mat_descr descr,
                                                  const T*                  bsr_val,
                                                  const rocsparse_int*      bsr_row_ptr,
                                                  const rocsparse_int*      bsr_col_ind,
                                                  rocsparse_int             row_block_dim,
                                                  rocsparse_int             col_block_dim,
                                                  const T*                  B,
                                                  int64_t                   ldb,
                                                  U                         beta,
                                                  T*                        C,
                                                  int64_t                   ldc);

template <typename T, typename U>
rocsparse_status rocsparse_gebsrmm_template_large_ext(rocsparse_handle          handle,
                                                      rocsparse_direction       dir,
                                                      rocsparse_operation       trans_A,
                                                      rocsparse_operation       trans_B,
                                                      rocsparse_int             mb,
                                                      rocsparse_int             n,
                                                      rocsparse_int             kb,
                                                      rocsparse_int             nnzb,
                                                      U                         alpha,
                                                      const rocsparse_mat_descr descr,
                                                      const T*                  bsr_val,
                                                      const rocsparse_int*      bsr_row_ptr,
                                                      const rocsparse_int*      bsr_col_ind,
                                                      rocsparse_int             row_block_dim,
                                                      rocsparse_int             col_block_dim,
                                                      const T*                  B,
                                                      int64_t                   ldb,
                                                      U                         beta,
                                                      T*                        C,
                                                      int64_t                   ldc);

template <typename T, typename U>
rocsparse_status rocsparse_gebsrmm_template_general(rocsparse_handle          handle,
                                                    rocsparse_direction       dir,
                                                    rocsparse_operation       trans_A,
                                                    rocsparse_operation       trans_B,
                                                    rocsparse_int             mb,
                                                    rocsparse_int             n,
                                                    rocsparse_int             kb,
                                                    rocsparse_int             nnzb,
                                                    U                         alpha,
                                                    const rocsparse_mat_descr descr,
                                                    const T*                  bsr_val,
                                                    const rocsparse_int*      bsr_row_ptr,
                                                    const rocsparse_int*      bsr_col_ind,
                                                    rocsparse_int             row_block_dim,
                                                    rocsparse_int             col_block_dim,
                                                    const T*                  B,
                                                    int64_t                   ldb,
                                                    U                         beta,
                                                    T*                        C,
                                                    int64_t                   ldc);

template <typename T, typename U>
rocsparse_status rocsparse_gebsrmm_template_dispatch(rocsparse_handle          handle,
                                                     rocsparse_direction       dir,
                                                     rocsparse_operation       trans_A,
                                                     rocsparse_operation       trans_B,
                                                     rocsparse_int             mb,
                                                     rocsparse_int             n,
                                                     rocsparse_int             kb,
                                                     rocsparse_int             nnzb,
                                                     U                         alpha,
                                                     const rocsparse_mat_descr descr,
                                                     const T*                  bsr_val,
                                                     const rocsparse_int*      bsr_row_ptr,
                                                     const rocsparse_int*      bsr_col_ind,
                                                     rocsparse_int             row_block_dim,
                                                     rocsparse_int             col_block_dim,
                                                     const T*                  B,
                                                     int64_t                   ldb,
                                                     U                         beta,
                                                     T*                        C,
                                                     int64_t                   ldc)
{
    const rocsparse_int block_dim = std::max(row_block_dim, col_block_dim);
    if(row_block_dim == col_block_dim)
    {
        return rocsparse_bsrmm_template_dispatch(handle,
                                                 dir,
                                                 trans_A,
                                                 trans_B,
                                                 mb,
                                                 n,
                                                 kb,
                                                 nnzb,
                                                 alpha,
                                                 descr,
                                                 bsr_val,
                                                 bsr_row_ptr,
                                                 bsr_col_ind,
                                                 block_dim,
                                                 B,
                                                 ldb,
                                                 beta,
                                                 C,
                                                 ldc);
    }

    // If n is only 1 and B are non-transposed, then call gebsrmv
    if(n == 1)
    {
        if(trans_B == rocsparse_operation_none)
        {
            return rocsparse_gebsrmv_template_dispatch(handle,
                                                       dir,
                                                       trans_A,
                                                       mb,
                                                       kb,
                                                       nnzb,
                                                       alpha,
                                                       descr,
                                                       bsr_val,
                                                       bsr_row_ptr,
                                                       bsr_col_ind,
                                                       row_block_dim,
                                                       col_block_dim,
                                                       B,
                                                       beta,
                                                       C);
        }
    }

    if(block_dim <= 4)
    {
        return rocsparse_gebsrmm_template_small(handle,
                                                dir,
                                                trans_A,
                                                trans_B,
                                                mb,
                                                n,
                                                kb,
                                                nnzb,
                                                alpha,
                                                descr,
                                                bsr_val,
                                                bsr_row_ptr,
                                                bsr_col_ind,
                                                row_block_dim,
                                                col_block_dim,
                                                B,
                                                ldb,
                                                beta,
                                                C,
                                                ldc);
    }
    else if(block_dim <= 32)
    {
        return rocsparse_gebsrmm_template_large_ext(handle,
                                                    dir,
                                                    trans_A,
                                                    trans_B,
                                                    mb,
                                                    n,
                                                    kb,
                                                    nnzb,
                                                    alpha,
                                                    descr,
                                                    bsr_val,
                                                    bsr_row_ptr,
                                                    bsr_col_ind,
                                                    row_block_dim,
                                                    col_block_dim,
                                                    B,
                                                    ldb,
                                                    beta,
                                                    C,
                                                    ldc);
    }
    else
    {
        return rocsparse_gebsrmm_template_general(handle,
                                                  dir,
                                                  trans_A,
                                                  trans_B,
                                                  mb,
                                                  n,
                                                  kb,
                                                  nnzb,
                                                  alpha,
                                                  descr,
                                                  bsr_val,
                                                  bsr_row_ptr,
                                                  bsr_col_ind,
                                                  row_block_dim,
                                                  col_block_dim,
                                                  B,
                                                  ldb,
                                                  beta,
                                                  C,
                                                  ldc);
    }
}

template <typename T>
rocsparse_status rocsparse_gebsrmm_template(rocsparse_handle          handle,
                                            rocsparse_direction       dir,
                                            rocsparse_operation       trans_A,
                                            rocsparse_operation       trans_B,
                                            rocsparse_int             mb,
                                            rocsparse_int             n,
                                            rocsparse_int             kb,
                                            rocsparse_int             nnzb,
                                            const T*                  alpha,
                                            const rocsparse_mat_descr descr,
                                            const T*                  bsr_val,
                                            const rocsparse_int*      bsr_row_ptr,
                                            const rocsparse_int*      bsr_col_ind,
                                            rocsparse_int             row_block_dim,
                                            rocsparse_int             col_block_dim,
                                            const T*                  B,
                                            int64_t                   ldb,
                                            const T*                  beta,
                                            T*                        C,
                                            int64_t                   ldc)
{
    // Check for valid handle and matrix descriptor
    if(handle == nullptr)
    {
        return rocsparse_status_invalid_handle;
    }
    else if(descr == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // Logging TODO bench logging
    log_trace(handle,
              replaceX<T>("rocsparse_Xgebsrmm"),
              dir,
              trans_A,
              trans_B,
              mb,
              n,
              kb,
              nnzb,
              LOG_TRACE_SCALAR_VALUE(handle, alpha),
              (const void*&)descr,
              (const void*&)bsr_val,
              (const void*&)bsr_row_ptr,
              (const void*&)bsr_col_ind,
              row_block_dim,
              col_block_dim,
              (const void*&)B,
              ldb,
              LOG_TRACE_SCALAR_VALUE(handle, beta),
              (const void*&)C,
              ldc);

    if(rocsparse_enum_utils::is_invalid(dir))
    {
        return rocsparse_status_invalid_value;
    }

    if(rocsparse_enum_utils::is_invalid(trans_A))
    {
        return rocsparse_status_invalid_value;
    }

    if(rocsparse_enum_utils::is_invalid(trans_B))
    {
        return rocsparse_status_invalid_value;
    }

    // Check matrix type
    if(descr->type != rocsparse_matrix_type_general)
    {
        return rocsparse_status_not_implemented;
    }

    // Check matrix sorting mode
    if(descr->storage_mode != rocsparse_storage_mode_sorted)
    {
        return rocsparse_status_requires_sorted_storage;
    }

    // Check operation
    if(trans_A != rocsparse_operation_none)
    {
        return rocsparse_status_not_implemented;
    }
    else if(trans_B != rocsparse_operation_none && trans_B != rocsparse_operation_transpose)
    {
        return rocsparse_status_not_implemented;
    }

    // Check sizes
    if(mb < 0 || n < 0 || kb < 0 || nnzb < 0 || row_block_dim <= 0 || col_block_dim <= 0)
    {
        return rocsparse_status_invalid_size;
    }

    // Quick return if possible
    if(mb == 0 || n == 0 || kb == 0)
    {
        // matrix never accessed however still need to update C matrix
        rocsparse_int m      = row_block_dim * mb;
        rocsparse_int C_size = m * n;
        if(C_size > 0)
        {
            if(C == nullptr && beta == nullptr)
            {
                return rocsparse_status_invalid_pointer;
            }

            if(handle->pointer_mode == rocsparse_pointer_mode_device)
            {
                hipLaunchKernelGGL((scale_array_2d<256>),
                                   dim3((C_size - 1) / 256 + 1),
                                   dim3(256),
                                   0,
                                   handle->stream,
                                   m,
                                   n,
                                   ldc,
                                   0,
                                   C,
                                   beta,
                                   rocsparse_order_column);
            }
            else
            {
                hipLaunchKernelGGL((scale_array_2d<256>),
                                   dim3((C_size - 1) / 256 + 1),
                                   dim3(256),
                                   0,
                                   handle->stream,
                                   m,
                                   n,
                                   ldc,
                                   0,
                                   C,
                                   *beta,
                                   rocsparse_order_column);
            }
        }

        return rocsparse_status_success;
    }

    // Check pointer arguments
    if(bsr_row_ptr == nullptr || B == nullptr || C == nullptr || alpha == nullptr
       || beta == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // value arrays and column indices arrays must both be null (zero matrix) or both not null
    if((bsr_val == nullptr && bsr_col_ind != nullptr)
       || (bsr_val != nullptr && bsr_col_ind == nullptr))
    {
        return rocsparse_status_invalid_pointer;
    }

    if(nnzb != 0 && (bsr_val == nullptr && bsr_col_ind == nullptr))
    {
        return rocsparse_status_invalid_pointer;
    }

    static constexpr rocsparse_int s_one = static_cast<rocsparse_int>(1);
    switch(trans_A)
    {
    case rocsparse_operation_none:
    {
        // Check leading dimension of C
        if(ldc < std::max(s_one, mb * row_block_dim))
        {
            return rocsparse_status_invalid_size;
        }

        // Check leading dimension of B
        switch(trans_B)
        {
        case rocsparse_operation_none:
        {
            if(ldb < std::max(s_one, kb * col_block_dim))
            {
                return rocsparse_status_invalid_size;
            }
            break;
        }
        case rocsparse_operation_transpose:
        case rocsparse_operation_conjugate_transpose:
        {
            if(ldb < std::max(s_one, n))
            {
                return rocsparse_status_invalid_size;
            }
            break;
        }
        }
        break;
    }
    case rocsparse_operation_transpose:
    case rocsparse_operation_conjugate_transpose:
    {
        // Check leading dimension of C
        if(ldc < std::max(s_one, kb * col_block_dim))
        {
            return rocsparse_status_invalid_size;
        }

        switch(trans_B)
        {
        case rocsparse_operation_none:
        {
            if(ldb < std::max(s_one, mb * row_block_dim))
            {
                return rocsparse_status_invalid_size;
            }
            break;
        }
        case rocsparse_operation_transpose:
        case rocsparse_operation_conjugate_transpose:
        {
            if(ldb < std::max(s_one, n))
            {
                return rocsparse_status_invalid_size;
            }
            break;
        }
        }
        break;
    }
    }

    if(handle->pointer_mode == rocsparse_pointer_mode_device)
    {
        return rocsparse_gebsrmm_template_dispatch(handle,
                                                   dir,
                                                   trans_A,
                                                   trans_B,
                                                   mb,
                                                   n,
                                                   kb,
                                                   nnzb,
                                                   alpha,
                                                   descr,
                                                   bsr_val,
                                                   bsr_row_ptr,
                                                   bsr_col_ind,
                                                   row_block_dim,
                                                   col_block_dim,
                                                   B,
                                                   ldb,
                                                   beta,
                                                   C,
                                                   ldc);
    }
    else
    {
        return rocsparse_gebsrmm_template_dispatch(handle,
                                                   dir,
                                                   trans_A,
                                                   trans_B,
                                                   mb,
                                                   n,
                                                   kb,
                                                   nnzb,
                                                   *alpha,
                                                   descr,
                                                   bsr_val,
                                                   bsr_row_ptr,
                                                   bsr_col_ind,
                                                   row_block_dim,
                                                   col_block_dim,
                                                   B,
                                                   ldb,
                                                   *beta,
                                                   C,
                                                   ldc);
    }
}

/*
 * ===========================================================================
 *    C wrapper
 * ===========================================================================
 */
#define C_IMPL(NAME, TYPE)                                                    \
    extern "C" rocsparse_status NAME(rocsparse_handle          handle,        \
                                     rocsparse_direction       dir,           \
                                     rocsparse_operation       trans_A,       \
                                     rocsparse_operation       trans_B,       \
                                     rocsparse_int             mb,            \
                                     rocsparse_int             n,             \
                                     rocsparse_int             kb,            \
                                     rocsparse_int             nnzb,          \
                                     const TYPE*               alpha,         \
                                     const rocsparse_mat_descr descr,         \
                                     const TYPE*               bsr_val,       \
                                     const rocsparse_int*      bsr_row_ptr,   \
                                     const rocsparse_int*      bsr_col_ind,   \
                                     rocsparse_int             row_block_dim, \
                                     rocsparse_int             col_block_dim, \
                                     const TYPE*               B,             \
                                     rocsparse_int             ldb,           \
                                     const TYPE*               beta,          \
                                     TYPE*                     C,             \
                                     rocsparse_int             ldc)           \
    try                                                                       \
    {                                                                         \
        return rocsparse_gebsrmm_template(handle,                             \
                                          dir,                                \
                                          trans_A,                            \
                                          trans_B,                            \
                                          mb,                                 \
                                          n,                                  \
                                          kb,                                 \
                                          nnzb,                               \
                                          alpha,                              \
                                          descr,                              \
                                          bsr_val,                            \
                                          bsr_row_ptr,                        \
                                          bsr_col_ind,                        \
                                          row_block_dim,                      \
                                          col_block_dim,                      \
                                          B,                                  \
                                          ldb,                                \
                                          beta,                               \
                                          C,                                  \
                                          ldc);                               \
    }                                                                         \
    catch(...)                                                                \
    {                                                                         \
        return exception_to_rocsparse_status();                               \
    }

C_IMPL(rocsparse_sgebsrmm, float);
C_IMPL(rocsparse_dgebsrmm, double);
C_IMPL(rocsparse_cgebsrmm, rocsparse_float_complex);
C_IMPL(rocsparse_zgebsrmm, rocsparse_double_complex);
