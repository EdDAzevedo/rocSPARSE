/*! \file */
/* ************************************************************************
 * Copyright (C) 2018-2023 Advanced Micro Devices, Inc. All rights Reserved.
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

#include "internal/precond/rocsparse_csrilu0.h"
#include "rocsparse_csrilu0.hpp"

#include "internal/level2/rocsparse_csrsv.h"

/*
 * ===========================================================================
 *    C wrapper
 * ===========================================================================
 */

extern "C" rocsparse_status rocsparse_scsrilu0_buffer_size(rocsparse_handle          handle,
                                                           rocsparse_int             m,
                                                           rocsparse_int             nnz,
                                                           const rocsparse_mat_descr descr,
                                                           const float*              csr_val,
                                                           const rocsparse_int*      csr_row_ptr,
                                                           const rocsparse_int*      csr_col_ind,
                                                           rocsparse_mat_info        info,
                                                           size_t*                   buffer_size)
try
{
    return rocsparse_scsrsv_buffer_size(handle,
                                        rocsparse_operation_none,
                                        m,
                                        nnz,
                                        descr,
                                        csr_val,
                                        csr_row_ptr,
                                        csr_col_ind,
                                        info,
                                        buffer_size);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_dcsrilu0_buffer_size(rocsparse_handle          handle,
                                                           rocsparse_int             m,
                                                           rocsparse_int             nnz,
                                                           const rocsparse_mat_descr descr,
                                                           const double*             csr_val,
                                                           const rocsparse_int*      csr_row_ptr,
                                                           const rocsparse_int*      csr_col_ind,
                                                           rocsparse_mat_info        info,
                                                           size_t*                   buffer_size)
try
{
    return rocsparse_dcsrsv_buffer_size(handle,
                                        rocsparse_operation_none,
                                        m,
                                        nnz,
                                        descr,
                                        csr_val,
                                        csr_row_ptr,
                                        csr_col_ind,
                                        info,
                                        buffer_size);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_ccsrilu0_buffer_size(rocsparse_handle               handle,
                                                           rocsparse_int                  m,
                                                           rocsparse_int                  nnz,
                                                           const rocsparse_mat_descr      descr,
                                                           const rocsparse_float_complex* csr_val,
                                                           const rocsparse_int* csr_row_ptr,
                                                           const rocsparse_int* csr_col_ind,
                                                           rocsparse_mat_info   info,
                                                           size_t*              buffer_size)
try
{
    return rocsparse_ccsrsv_buffer_size(handle,
                                        rocsparse_operation_none,
                                        m,
                                        nnz,
                                        descr,
                                        csr_val,
                                        csr_row_ptr,
                                        csr_col_ind,
                                        info,
                                        buffer_size);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_zcsrilu0_buffer_size(rocsparse_handle                handle,
                                                           rocsparse_int                   m,
                                                           rocsparse_int                   nnz,
                                                           const rocsparse_mat_descr       descr,
                                                           const rocsparse_double_complex* csr_val,
                                                           const rocsparse_int* csr_row_ptr,
                                                           const rocsparse_int* csr_col_ind,
                                                           rocsparse_mat_info   info,
                                                           size_t*              buffer_size)
try
{
    return rocsparse_zcsrsv_buffer_size(handle,
                                        rocsparse_operation_none,
                                        m,
                                        nnz,
                                        descr,
                                        csr_val,
                                        csr_row_ptr,
                                        csr_col_ind,
                                        info,
                                        buffer_size);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_scsrilu0_numeric_boost(rocsparse_handle   handle,
                                                             rocsparse_mat_info info,
                                                             int                enable_boost,
                                                             const float*       boost_tol,
                                                             const float*       boost_val)
try
{
    return rocsparse_csrilu0_numeric_boost_template(
        handle, info, enable_boost, boost_tol, boost_val);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_dcsrilu0_numeric_boost(rocsparse_handle   handle,
                                                             rocsparse_mat_info info,
                                                             int                enable_boost,
                                                             const double*      boost_tol,
                                                             const double*      boost_val)
try
{
    return rocsparse_csrilu0_numeric_boost_template(
        handle, info, enable_boost, boost_tol, boost_val);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status
    rocsparse_ccsrilu0_numeric_boost(rocsparse_handle               handle,
                                     rocsparse_mat_info             info,
                                     int                            enable_boost,
                                     const float*                   boost_tol,
                                     const rocsparse_float_complex* boost_val)
try
{
    return rocsparse_csrilu0_numeric_boost_template(
        handle, info, enable_boost, boost_tol, boost_val);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status
    rocsparse_zcsrilu0_numeric_boost(rocsparse_handle                handle,
                                     rocsparse_mat_info              info,
                                     int                             enable_boost,
                                     const double*                   boost_tol,
                                     const rocsparse_double_complex* boost_val)
try
{
    return rocsparse_csrilu0_numeric_boost_template(
        handle, info, enable_boost, boost_tol, boost_val);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_dscsrilu0_numeric_boost(rocsparse_handle   handle,
                                                              rocsparse_mat_info info,
                                                              int                enable_boost,
                                                              const double*      boost_tol,
                                                              const float*       boost_val)
try
{
    return rocsparse_csrilu0_numeric_boost_template(
        handle, info, enable_boost, boost_tol, boost_val);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status
    rocsparse_dccsrilu0_numeric_boost(rocsparse_handle               handle,
                                      rocsparse_mat_info             info,
                                      int                            enable_boost,
                                      const double*                  boost_tol,
                                      const rocsparse_float_complex* boost_val)
try
{
    return rocsparse_csrilu0_numeric_boost_template(
        handle, info, enable_boost, boost_tol, boost_val);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_scsrilu0_analysis(rocsparse_handle          handle,
                                                        rocsparse_int             m,
                                                        rocsparse_int             nnz,
                                                        const rocsparse_mat_descr descr,
                                                        const float*              csr_val,
                                                        const rocsparse_int*      csr_row_ptr,
                                                        const rocsparse_int*      csr_col_ind,
                                                        rocsparse_mat_info        info,
                                                        rocsparse_analysis_policy analysis,
                                                        rocsparse_solve_policy    solve,
                                                        void*                     temp_buffer)
try
{
    return rocsparse_csrilu0_analysis_template(handle,
                                               m,
                                               nnz,
                                               descr,
                                               csr_val,
                                               csr_row_ptr,
                                               csr_col_ind,
                                               info,
                                               analysis,
                                               solve,
                                               temp_buffer);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_dcsrilu0_analysis(rocsparse_handle          handle,
                                                        rocsparse_int             m,
                                                        rocsparse_int             nnz,
                                                        const rocsparse_mat_descr descr,
                                                        const double*             csr_val,
                                                        const rocsparse_int*      csr_row_ptr,
                                                        const rocsparse_int*      csr_col_ind,
                                                        rocsparse_mat_info        info,
                                                        rocsparse_analysis_policy analysis,
                                                        rocsparse_solve_policy    solve,
                                                        void*                     temp_buffer)
try
{
    return rocsparse_csrilu0_analysis_template(handle,
                                               m,
                                               nnz,
                                               descr,
                                               csr_val,
                                               csr_row_ptr,
                                               csr_col_ind,
                                               info,
                                               analysis,
                                               solve,
                                               temp_buffer);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_ccsrilu0_analysis(rocsparse_handle               handle,
                                                        rocsparse_int                  m,
                                                        rocsparse_int                  nnz,
                                                        const rocsparse_mat_descr      descr,
                                                        const rocsparse_float_complex* csr_val,
                                                        const rocsparse_int*           csr_row_ptr,
                                                        const rocsparse_int*           csr_col_ind,
                                                        rocsparse_mat_info             info,
                                                        rocsparse_analysis_policy      analysis,
                                                        rocsparse_solve_policy         solve,
                                                        void*                          temp_buffer)
try
{
    return rocsparse_csrilu0_analysis_template(handle,
                                               m,
                                               nnz,
                                               descr,
                                               csr_val,
                                               csr_row_ptr,
                                               csr_col_ind,
                                               info,
                                               analysis,
                                               solve,
                                               temp_buffer);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_zcsrilu0_analysis(rocsparse_handle                handle,
                                                        rocsparse_int                   m,
                                                        rocsparse_int                   nnz,
                                                        const rocsparse_mat_descr       descr,
                                                        const rocsparse_double_complex* csr_val,
                                                        const rocsparse_int*            csr_row_ptr,
                                                        const rocsparse_int*            csr_col_ind,
                                                        rocsparse_mat_info              info,
                                                        rocsparse_analysis_policy       analysis,
                                                        rocsparse_solve_policy          solve,
                                                        void*                           temp_buffer)
try
{
    return rocsparse_csrilu0_analysis_template(handle,
                                               m,
                                               nnz,
                                               descr,
                                               csr_val,
                                               csr_row_ptr,
                                               csr_col_ind,
                                               info,
                                               analysis,
                                               solve,
                                               temp_buffer);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_csrilu0_clear(rocsparse_handle   handle,
                                                    rocsparse_mat_info info)
try
{
    // Check for valid handle and matrix descriptor
    if(handle == nullptr)
    {
        return rocsparse_status_invalid_handle;
    }
    else if(info == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // Logging
    log_trace(handle, "rocsparse_csrilu0_clear", (const void*&)info);

    // If meta data is not shared, delete it
    if(!rocsparse_check_trm_shared(info, info->csrilu0_info))
    {
        RETURN_IF_ROCSPARSE_ERROR(rocsparse_destroy_trm_info(info->csrilu0_info));
    }

    info->csrilu0_info = nullptr;

    return rocsparse_status_success;
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_scsrilu0(rocsparse_handle          handle,
                                               rocsparse_int             m,
                                               rocsparse_int             nnz,
                                               const rocsparse_mat_descr descr,
                                               float*                    csr_val,
                                               const rocsparse_int*      csr_row_ptr,
                                               const rocsparse_int*      csr_col_ind,
                                               rocsparse_mat_info        info,
                                               rocsparse_solve_policy    policy,
                                               void*                     temp_buffer)
try
{
    if(info != nullptr && info->use_double_prec_tol)
    {
        return rocsparse_csrilu0_template<float, double>(
            handle, m, nnz, descr, csr_val, csr_row_ptr, csr_col_ind, info, policy, temp_buffer);
    }
    else
    {
        return rocsparse_csrilu0_template<float, float>(
            handle, m, nnz, descr, csr_val, csr_row_ptr, csr_col_ind, info, policy, temp_buffer);
    }
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_dcsrilu0(rocsparse_handle          handle,
                                               rocsparse_int             m,
                                               rocsparse_int             nnz,
                                               const rocsparse_mat_descr descr,
                                               double*                   csr_val,
                                               const rocsparse_int*      csr_row_ptr,
                                               const rocsparse_int*      csr_col_ind,
                                               rocsparse_mat_info        info,
                                               rocsparse_solve_policy    policy,
                                               void*                     temp_buffer)
try
{
    return rocsparse_csrilu0_template<double, double>(
        handle, m, nnz, descr, csr_val, csr_row_ptr, csr_col_ind, info, policy, temp_buffer);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_ccsrilu0(rocsparse_handle          handle,
                                               rocsparse_int             m,
                                               rocsparse_int             nnz,
                                               const rocsparse_mat_descr descr,
                                               rocsparse_float_complex*  csr_val,
                                               const rocsparse_int*      csr_row_ptr,
                                               const rocsparse_int*      csr_col_ind,
                                               rocsparse_mat_info        info,
                                               rocsparse_solve_policy    policy,
                                               void*                     temp_buffer)
try
{
    if(info != nullptr && info->use_double_prec_tol)
    {
        return rocsparse_csrilu0_template<rocsparse_float_complex, double>(
            handle, m, nnz, descr, csr_val, csr_row_ptr, csr_col_ind, info, policy, temp_buffer);
    }
    else
    {
        return rocsparse_csrilu0_template<rocsparse_float_complex, float>(
            handle, m, nnz, descr, csr_val, csr_row_ptr, csr_col_ind, info, policy, temp_buffer);
    }
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_zcsrilu0(rocsparse_handle          handle,
                                               rocsparse_int             m,
                                               rocsparse_int             nnz,
                                               const rocsparse_mat_descr descr,
                                               rocsparse_double_complex* csr_val,
                                               const rocsparse_int*      csr_row_ptr,
                                               const rocsparse_int*      csr_col_ind,
                                               rocsparse_mat_info        info,
                                               rocsparse_solve_policy    policy,
                                               void*                     temp_buffer)
try
{
    return rocsparse_csrilu0_template<rocsparse_double_complex, double>(
        handle, m, nnz, descr, csr_val, csr_row_ptr, csr_col_ind, info, policy, temp_buffer);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_csrilu0_zero_pivot(rocsparse_handle   handle,
                                                         rocsparse_mat_info info,
                                                         rocsparse_int*     position)
try
{
    // Check for valid handle and matrix descriptor
    if(handle == nullptr)
    {
        return rocsparse_status_invalid_handle;
    }
    else if(info == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // Logging
    log_trace(handle, "rocsparse_csrilu0_zero_pivot", (const void*&)info, (const void*&)position);

    // Check pointer arguments
    if(position == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // Stream
    hipStream_t stream = handle->stream;

    // If m == 0 || nnz == 0 it can happen, that info structure is not created.
    // In this case, always return -1.
    if(info->csrilu0_info == nullptr)
    {
        if(handle->pointer_mode == rocsparse_pointer_mode_device)
        {
            RETURN_IF_HIP_ERROR(hipMemsetAsync(position, 0xFF, sizeof(rocsparse_int), stream));
        }
        else
        {
            *position = -1;
        }

        return rocsparse_status_success;
    }

    // Differentiate between pointer modes
    if(handle->pointer_mode == rocsparse_pointer_mode_device)
    {
        // rocsparse_pointer_mode_device
        rocsparse_int pivot;

        RETURN_IF_HIP_ERROR(hipMemcpyAsync(
            &pivot, info->zero_pivot, sizeof(rocsparse_int), hipMemcpyDeviceToHost, stream));

        // Wait for host transfer to finish
        RETURN_IF_HIP_ERROR(hipStreamSynchronize(stream));

        if(pivot == std::numeric_limits<rocsparse_int>::max())
        {
            RETURN_IF_HIP_ERROR(hipMemsetAsync(position, 0xFF, sizeof(rocsparse_int), stream));
        }
        else
        {
            RETURN_IF_HIP_ERROR(hipMemcpyAsync(position,
                                               info->zero_pivot,
                                               sizeof(rocsparse_int),
                                               hipMemcpyDeviceToDevice,
                                               stream));

            return rocsparse_status_zero_pivot;
        }
    }
    else
    {
        // rocsparse_pointer_mode_host
        RETURN_IF_HIP_ERROR(hipMemcpyAsync(
            position, info->zero_pivot, sizeof(rocsparse_int), hipMemcpyDeviceToHost, stream));
        RETURN_IF_HIP_ERROR(hipStreamSynchronize(stream));

        // If no zero pivot is found, set -1
        if(*position == std::numeric_limits<rocsparse_int>::max())
        {
            *position = -1;
        }
        else
        {
            return rocsparse_status_zero_pivot;
        }
    }

    return rocsparse_status_success;
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status rocsparse_csrilu0_singular_pivot(rocsparse_handle   handle,
                                                             rocsparse_mat_info info,
                                                             rocsparse_int*     position)
try
{
    // Check for valid handle and matrix descriptor
    if(handle == nullptr)
    {
        return rocsparse_status_invalid_handle;
    }
    else if(info == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // Logging
    log_trace(
        handle, "rocsparse_csrilu0_singular_pivot", (const void*&)info, (const void*&)position);

    // Check pointer arguments
    if(position == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // Stream
    hipStream_t stream = handle->stream;

    // If m == 0 || nnz == 0 it can happen, that info structure is not created.
    // In this case, always return -1.
    if(info->csrilu0_info == nullptr)
    {
        if(handle->pointer_mode == rocsparse_pointer_mode_device)
        {
            // set to 0xFF is assign -1 in signed integer
            RETURN_IF_HIP_ERROR(hipMemsetAsync(position, 0xFF, sizeof(rocsparse_int), stream));
        }
        else
        {
            *position = -1;
        }

        return rocsparse_status_success;
    }

    rocsparse_int const max_int        = std::numeric_limits<rocsparse_int>::max();
    rocsparse_int       zero_pivot     = max_int;
    rocsparse_int       singular_pivot = max_int;

    RETURN_IF_HIP_ERROR(hipMemcpyAsync(
        &zero_pivot, info->zero_pivot, sizeof(rocsparse_int), hipMemcpyDeviceToHost, stream));

    RETURN_IF_HIP_ERROR(hipMemcpyAsync(&singular_pivot,
                                       info->singular_pivot,
                                       sizeof(rocsparse_int),
                                       hipMemcpyDeviceToHost,
                                       stream));

    RETURN_IF_HIP_ERROR(hipStreamSynchronize(stream));

    singular_pivot = std::min(((zero_pivot == -1) ? max_int : zero_pivot),
                              ((singular_pivot == -1) ? max_int : singular_pivot));

    if(singular_pivot == max_int)
    {
        singular_pivot = -1;
    };

    // Differentiate between pointer modes
    if(handle->pointer_mode == rocsparse_pointer_mode_device)
    {

        // rocsparse_pointer_mode_device
        RETURN_IF_HIP_ERROR(hipMemcpyAsync(
            position, &singular_pivot, sizeof(rocsparse_int), hipMemcpyHostToDevice, stream));
    }
    else
    {
        // rocsparse_pointer_mode_host
        *position = singular_pivot;
    };

    return ((singular_pivot == -1) ? rocsparse_status_success : rocsparse_status_singular_pivot);
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status
    rocsparse_csrilu0_set_tolerance(rocsparse_handle handle, rocsparse_mat_info info, double tol)
try
{
    // Check for valid handle and matrix descriptor
    if(handle == nullptr)
    {
        return rocsparse_status_invalid_handle;
    }

    if(info == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    if(tol < 0)
    {
        return rocsparse_status_invalid_value;
    }

    // Logging
    log_trace(handle, "rocsparse_csrilu0_set_tolerance", (const void*&)info, tol);

    info->singular_tol = tol;

    return rocsparse_status_success;
}
catch(...)
{
    return exception_to_rocsparse_status();
}

extern "C" rocsparse_status
    rocsparse_csrilu0_get_tolerance(rocsparse_handle handle, rocsparse_mat_info info, double* tol)
try
{
    // Check for valid handle and matrix descriptor
    if(handle == nullptr)
    {
        return rocsparse_status_invalid_handle;
    }

    if(info == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    if(tol == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // Logging
    log_trace(handle, "rocsparse_csrilu0_get_tolerance", (const void*&)info, tol);

    *tol = info->singular_tol;

    return rocsparse_status_success;
}
catch(...)
{
    return exception_to_rocsparse_status();
}
