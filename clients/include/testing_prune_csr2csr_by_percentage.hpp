/* ************************************************************************
 * Copyright (c) 2020 Advanced Micro Devices, Inc.
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

#pragma once
#ifndef TESTING_PRUNE_CSR2CSR_BY_PERCENTAGE_HPP
#define TESTING_PRUNE_CSR2CSR_BY_PERCENTAGE_HPP

#include <rocsparse.hpp>

#include "gbyte.hpp"
#include "rocsparse_check.hpp"
#include "rocsparse_host.hpp"
#include "rocsparse_init.hpp"
#include "rocsparse_math.hpp"
#include "rocsparse_random.hpp"
#include "rocsparse_test.hpp"
#include "rocsparse_vector.hpp"
#include "utility.hpp"

template <typename T>
void testing_prune_csr2csr_by_percentage_bad_arg(const Arguments& arg)
{
    static constexpr size_t safe_size              = 100;
    static constexpr T      percentage             = static_cast<T>(1);
    static rocsparse_int    nnz_total_dev_host_ptr = 100;
    static size_t           buffer_size            = 100;

    // Create rocsparse handle
    rocsparse_local_handle handle;

    // Allocate memory on device
    device_vector<rocsparse_int> dcsr_row_ptr_A(safe_size);
    device_vector<rocsparse_int> dcsr_col_ind_A(safe_size);
    device_vector<T>             dcsr_val_A(safe_size);
    device_vector<rocsparse_int> dcsr_row_ptr_C(safe_size);
    device_vector<rocsparse_int> dcsr_col_ind_C(safe_size);
    device_vector<T>             dcsr_val_C(safe_size);
    device_vector<T>             dtemp_buffer(safe_size);

    if(!dcsr_row_ptr_A || !dcsr_col_ind_A || !dcsr_val_A || !dcsr_row_ptr_C || !dcsr_col_ind_C
       || !dcsr_val_C || !dtemp_buffer)
    {
        CHECK_HIP_ERROR(hipErrorOutOfMemory);
        return;
    }

    rocsparse_local_mat_descr csr_descr_A;
    rocsparse_local_mat_descr csr_descr_C;

    rocsparse_local_mat_info info;

    rocsparse_set_mat_index_base(csr_descr_A, rocsparse_index_base_zero);
    rocsparse_set_mat_index_base(csr_descr_C, rocsparse_index_base_zero);

    // Test rocsparse_prune_csr2csr_by_percentage_buffer_size
    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage_buffer_size<T>(nullptr,
                                                                                 safe_size,
                                                                                 safe_size,
                                                                                 safe_size,
                                                                                 csr_descr_A,
                                                                                 dcsr_val_A,
                                                                                 dcsr_row_ptr_A,
                                                                                 dcsr_col_ind_A,
                                                                                 percentage,
                                                                                 csr_descr_C,
                                                                                 dcsr_val_C,
                                                                                 dcsr_row_ptr_C,
                                                                                 dcsr_col_ind_C,
                                                                                 info,
                                                                                 &buffer_size),
                            rocsparse_status_invalid_handle);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage_buffer_size<T>(handle,
                                                                                 -1,
                                                                                 safe_size,
                                                                                 safe_size,
                                                                                 csr_descr_A,
                                                                                 dcsr_val_A,
                                                                                 dcsr_row_ptr_A,
                                                                                 dcsr_col_ind_A,
                                                                                 percentage,
                                                                                 csr_descr_C,
                                                                                 dcsr_val_C,
                                                                                 dcsr_row_ptr_C,
                                                                                 dcsr_col_ind_C,
                                                                                 info,
                                                                                 &buffer_size),
                            rocsparse_status_invalid_size);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage_buffer_size<T>(handle,
                                                                                 safe_size,
                                                                                 -1,
                                                                                 safe_size,
                                                                                 csr_descr_A,
                                                                                 dcsr_val_A,
                                                                                 dcsr_row_ptr_A,
                                                                                 dcsr_col_ind_A,
                                                                                 percentage,
                                                                                 csr_descr_C,
                                                                                 dcsr_val_C,
                                                                                 dcsr_row_ptr_C,
                                                                                 dcsr_col_ind_C,
                                                                                 info,
                                                                                 &buffer_size),
                            rocsparse_status_invalid_size);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage_buffer_size<T>(handle,
                                                                                 safe_size,
                                                                                 safe_size,
                                                                                 -1,
                                                                                 csr_descr_A,
                                                                                 dcsr_val_A,
                                                                                 dcsr_row_ptr_A,
                                                                                 dcsr_col_ind_A,
                                                                                 percentage,
                                                                                 csr_descr_C,
                                                                                 dcsr_val_C,
                                                                                 dcsr_row_ptr_C,
                                                                                 dcsr_col_ind_C,
                                                                                 info,
                                                                                 &buffer_size),
                            rocsparse_status_invalid_size);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage_buffer_size<T>(handle,
                                                                                 safe_size,
                                                                                 safe_size,
                                                                                 safe_size,
                                                                                 csr_descr_A,
                                                                                 dcsr_val_A,
                                                                                 dcsr_row_ptr_A,
                                                                                 dcsr_col_ind_A,
                                                                                 percentage,
                                                                                 csr_descr_C,
                                                                                 dcsr_val_C,
                                                                                 dcsr_row_ptr_C,
                                                                                 dcsr_col_ind_C,
                                                                                 info,
                                                                                 nullptr),
                            rocsparse_status_invalid_pointer);

    // Test rocsparse_prune_csr2csr_nnz_by_percentage
    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(nullptr,
                                                                         safe_size,
                                                                         safe_size,
                                                                         safe_size,
                                                                         csr_descr_A,
                                                                         dcsr_val_A,
                                                                         dcsr_row_ptr_A,
                                                                         dcsr_col_ind_A,
                                                                         percentage,
                                                                         csr_descr_C,
                                                                         dcsr_row_ptr_C,
                                                                         &nnz_total_dev_host_ptr,
                                                                         info,
                                                                         dtemp_buffer),
                            rocsparse_status_invalid_handle);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                         -1,
                                                                         safe_size,
                                                                         safe_size,
                                                                         csr_descr_A,
                                                                         dcsr_val_A,
                                                                         dcsr_row_ptr_A,
                                                                         dcsr_col_ind_A,
                                                                         percentage,
                                                                         csr_descr_C,
                                                                         dcsr_row_ptr_C,
                                                                         &nnz_total_dev_host_ptr,
                                                                         info,
                                                                         dtemp_buffer),
                            rocsparse_status_invalid_size);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                         safe_size,
                                                                         -1,
                                                                         safe_size,
                                                                         csr_descr_A,
                                                                         dcsr_val_A,
                                                                         dcsr_row_ptr_A,
                                                                         dcsr_col_ind_A,
                                                                         percentage,
                                                                         csr_descr_C,
                                                                         dcsr_row_ptr_C,
                                                                         &nnz_total_dev_host_ptr,
                                                                         info,
                                                                         dtemp_buffer),
                            rocsparse_status_invalid_size);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                         safe_size,
                                                                         safe_size,
                                                                         -1,
                                                                         csr_descr_A,
                                                                         dcsr_val_A,
                                                                         dcsr_row_ptr_A,
                                                                         dcsr_col_ind_A,
                                                                         percentage,
                                                                         csr_descr_C,
                                                                         dcsr_row_ptr_C,
                                                                         &nnz_total_dev_host_ptr,
                                                                         info,
                                                                         dtemp_buffer),
                            rocsparse_status_invalid_size);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                         safe_size,
                                                                         safe_size,
                                                                         safe_size,
                                                                         csr_descr_A,
                                                                         dcsr_val_A,
                                                                         dcsr_row_ptr_A,
                                                                         dcsr_col_ind_A,
                                                                         -1.0,
                                                                         csr_descr_C,
                                                                         dcsr_row_ptr_C,
                                                                         &nnz_total_dev_host_ptr,
                                                                         info,
                                                                         dtemp_buffer),
                            rocsparse_status_invalid_size);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                         safe_size,
                                                                         safe_size,
                                                                         safe_size,
                                                                         csr_descr_A,
                                                                         dcsr_val_A,
                                                                         dcsr_row_ptr_A,
                                                                         dcsr_col_ind_A,
                                                                         101.0,
                                                                         csr_descr_C,
                                                                         dcsr_row_ptr_C,
                                                                         &nnz_total_dev_host_ptr,
                                                                         info,
                                                                         dtemp_buffer),
                            rocsparse_status_invalid_size);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                         safe_size,
                                                                         safe_size,
                                                                         safe_size,
                                                                         nullptr,
                                                                         dcsr_val_A,
                                                                         dcsr_row_ptr_A,
                                                                         dcsr_col_ind_A,
                                                                         percentage,
                                                                         csr_descr_C,
                                                                         dcsr_row_ptr_C,
                                                                         &nnz_total_dev_host_ptr,
                                                                         info,
                                                                         dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                         safe_size,
                                                                         safe_size,
                                                                         safe_size,
                                                                         csr_descr_A,
                                                                         (const T*)nullptr,
                                                                         dcsr_row_ptr_A,
                                                                         dcsr_col_ind_A,
                                                                         percentage,
                                                                         csr_descr_C,
                                                                         dcsr_row_ptr_C,
                                                                         &nnz_total_dev_host_ptr,
                                                                         info,
                                                                         dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                         safe_size,
                                                                         safe_size,
                                                                         safe_size,
                                                                         csr_descr_A,
                                                                         dcsr_val_A,
                                                                         nullptr,
                                                                         dcsr_col_ind_A,
                                                                         percentage,
                                                                         csr_descr_C,
                                                                         dcsr_row_ptr_C,
                                                                         &nnz_total_dev_host_ptr,
                                                                         info,
                                                                         dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                         safe_size,
                                                                         safe_size,
                                                                         safe_size,
                                                                         csr_descr_A,
                                                                         dcsr_val_A,
                                                                         dcsr_row_ptr_A,
                                                                         nullptr,
                                                                         percentage,
                                                                         csr_descr_C,
                                                                         dcsr_row_ptr_C,
                                                                         &nnz_total_dev_host_ptr,
                                                                         info,
                                                                         dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                         safe_size,
                                                                         safe_size,
                                                                         safe_size,
                                                                         csr_descr_A,
                                                                         dcsr_val_A,
                                                                         dcsr_row_ptr_A,
                                                                         dcsr_col_ind_A,
                                                                         percentage,
                                                                         nullptr,
                                                                         dcsr_row_ptr_C,
                                                                         &nnz_total_dev_host_ptr,
                                                                         info,
                                                                         dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                         safe_size,
                                                                         safe_size,
                                                                         safe_size,
                                                                         csr_descr_A,
                                                                         dcsr_val_A,
                                                                         dcsr_row_ptr_A,
                                                                         dcsr_col_ind_A,
                                                                         percentage,
                                                                         csr_descr_C,
                                                                         nullptr,
                                                                         &nnz_total_dev_host_ptr,
                                                                         info,
                                                                         dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                         safe_size,
                                                                         safe_size,
                                                                         safe_size,
                                                                         csr_descr_A,
                                                                         dcsr_val_A,
                                                                         dcsr_row_ptr_A,
                                                                         dcsr_col_ind_A,
                                                                         percentage,
                                                                         csr_descr_C,
                                                                         dcsr_row_ptr_C,
                                                                         nullptr,
                                                                         info,
                                                                         dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                         safe_size,
                                                                         safe_size,
                                                                         safe_size,
                                                                         csr_descr_A,
                                                                         dcsr_val_A,
                                                                         dcsr_row_ptr_A,
                                                                         dcsr_col_ind_A,
                                                                         percentage,
                                                                         csr_descr_C,
                                                                         dcsr_row_ptr_C,
                                                                         &nnz_total_dev_host_ptr,
                                                                         info,
                                                                         nullptr),
                            rocsparse_status_invalid_pointer);

    // Test rocsparse_prune_csr2csr_by_percentage
    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(nullptr,
                                                                     safe_size,
                                                                     safe_size,
                                                                     safe_size,
                                                                     csr_descr_A,
                                                                     dcsr_val_A,
                                                                     dcsr_row_ptr_A,
                                                                     dcsr_col_ind_A,
                                                                     percentage,
                                                                     csr_descr_C,
                                                                     dcsr_val_C,
                                                                     dcsr_row_ptr_C,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_handle);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     -1,
                                                                     safe_size,
                                                                     safe_size,
                                                                     csr_descr_A,
                                                                     dcsr_val_A,
                                                                     dcsr_row_ptr_A,
                                                                     dcsr_col_ind_A,
                                                                     percentage,
                                                                     csr_descr_C,
                                                                     dcsr_val_C,
                                                                     dcsr_row_ptr_C,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_size);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     safe_size,
                                                                     -1,
                                                                     safe_size,
                                                                     csr_descr_A,
                                                                     dcsr_val_A,
                                                                     dcsr_row_ptr_A,
                                                                     dcsr_col_ind_A,
                                                                     percentage,
                                                                     csr_descr_C,
                                                                     dcsr_val_C,
                                                                     dcsr_row_ptr_C,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_size);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     safe_size,
                                                                     safe_size,
                                                                     -1,
                                                                     csr_descr_A,
                                                                     dcsr_val_A,
                                                                     dcsr_row_ptr_A,
                                                                     dcsr_col_ind_A,
                                                                     percentage,
                                                                     csr_descr_C,
                                                                     dcsr_val_C,
                                                                     dcsr_row_ptr_C,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_size);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     safe_size,
                                                                     safe_size,
                                                                     safe_size,
                                                                     csr_descr_A,
                                                                     dcsr_val_A,
                                                                     dcsr_row_ptr_A,
                                                                     dcsr_col_ind_A,
                                                                     -1.0,
                                                                     csr_descr_C,
                                                                     dcsr_val_C,
                                                                     dcsr_row_ptr_C,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_size);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     safe_size,
                                                                     safe_size,
                                                                     safe_size,
                                                                     csr_descr_A,
                                                                     dcsr_val_A,
                                                                     dcsr_row_ptr_A,
                                                                     dcsr_col_ind_A,
                                                                     101.0,
                                                                     csr_descr_C,
                                                                     dcsr_val_C,
                                                                     dcsr_row_ptr_C,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_size);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     safe_size,
                                                                     safe_size,
                                                                     safe_size,
                                                                     nullptr,
                                                                     dcsr_val_A,
                                                                     dcsr_row_ptr_A,
                                                                     dcsr_col_ind_A,
                                                                     percentage,
                                                                     csr_descr_C,
                                                                     dcsr_val_C,
                                                                     dcsr_row_ptr_C,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     safe_size,
                                                                     safe_size,
                                                                     safe_size,
                                                                     csr_descr_A,
                                                                     (const T*)nullptr,
                                                                     dcsr_row_ptr_A,
                                                                     dcsr_col_ind_A,
                                                                     percentage,
                                                                     csr_descr_C,
                                                                     dcsr_val_C,
                                                                     dcsr_row_ptr_C,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     safe_size,
                                                                     safe_size,
                                                                     safe_size,
                                                                     csr_descr_A,
                                                                     dcsr_val_A,
                                                                     nullptr,
                                                                     dcsr_col_ind_A,
                                                                     percentage,
                                                                     csr_descr_C,
                                                                     dcsr_val_C,
                                                                     dcsr_row_ptr_C,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     safe_size,
                                                                     safe_size,
                                                                     safe_size,
                                                                     csr_descr_A,
                                                                     dcsr_val_A,
                                                                     dcsr_row_ptr_A,
                                                                     nullptr,
                                                                     percentage,
                                                                     csr_descr_C,
                                                                     dcsr_val_C,
                                                                     dcsr_row_ptr_C,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     safe_size,
                                                                     safe_size,
                                                                     safe_size,
                                                                     csr_descr_A,
                                                                     dcsr_val_A,
                                                                     dcsr_row_ptr_A,
                                                                     dcsr_col_ind_A,
                                                                     percentage,
                                                                     nullptr,
                                                                     dcsr_val_C,
                                                                     dcsr_row_ptr_C,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     safe_size,
                                                                     safe_size,
                                                                     safe_size,
                                                                     csr_descr_A,
                                                                     dcsr_val_A,
                                                                     dcsr_row_ptr_A,
                                                                     dcsr_col_ind_A,
                                                                     percentage,
                                                                     csr_descr_C,
                                                                     (T*)nullptr,
                                                                     dcsr_row_ptr_C,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     safe_size,
                                                                     safe_size,
                                                                     safe_size,
                                                                     csr_descr_A,
                                                                     dcsr_val_A,
                                                                     dcsr_row_ptr_A,
                                                                     dcsr_col_ind_A,
                                                                     percentage,
                                                                     csr_descr_C,
                                                                     dcsr_val_C,
                                                                     nullptr,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     safe_size,
                                                                     safe_size,
                                                                     safe_size,
                                                                     csr_descr_A,
                                                                     dcsr_val_A,
                                                                     dcsr_row_ptr_A,
                                                                     dcsr_col_ind_A,
                                                                     percentage,
                                                                     csr_descr_C,
                                                                     dcsr_val_C,
                                                                     dcsr_row_ptr_C,
                                                                     nullptr,
                                                                     info,
                                                                     dtemp_buffer),
                            rocsparse_status_invalid_pointer);

    EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                     safe_size,
                                                                     safe_size,
                                                                     safe_size,
                                                                     csr_descr_A,
                                                                     dcsr_val_A,
                                                                     dcsr_row_ptr_A,
                                                                     dcsr_col_ind_A,
                                                                     percentage,
                                                                     csr_descr_C,
                                                                     dcsr_val_C,
                                                                     dcsr_row_ptr_C,
                                                                     dcsr_col_ind_C,
                                                                     info,
                                                                     nullptr),
                            rocsparse_status_invalid_pointer);
}

template <typename T>
void testing_prune_csr2csr_by_percentage(const Arguments& arg)
{
    rocsparse_int         M          = arg.M;
    rocsparse_int         N          = arg.N;
    rocsparse_int         K          = arg.K;
    T                     percentage = static_cast<T>(arg.percentage);
    rocsparse_int         dim_x      = arg.dimx;
    rocsparse_int         dim_y      = arg.dimy;
    rocsparse_int         dim_z      = arg.dimz;
    rocsparse_index_base  csr_base_A = arg.baseA;
    rocsparse_index_base  csr_base_C = arg.baseB;
    rocsparse_matrix_init mat        = arg.matrix;
    bool                  full_rank  = false;
    std::string           filename
        = arg.timing ? arg.filename : rocsparse_exepath() + "../matrices/" + arg.filename + ".csr";

    // Create rocsparse handle
    rocsparse_local_handle handle;

    // Create matrix descriptors
    rocsparse_local_mat_descr csr_descr_A;
    rocsparse_local_mat_descr csr_descr_C;

    // Create matrix info
    rocsparse_local_mat_info info;

    rocsparse_set_mat_index_base(csr_descr_A, csr_base_A);
    rocsparse_set_mat_index_base(csr_descr_C, csr_base_C);

    // Argument sanity check before allocating invalid memory
    if(M <= 0 || N <= 0 || percentage < static_cast<T>(0) || percentage > static_cast<T>(100))
    {
        static const size_t safe_size = 100;

        EXPECT_ROCSPARSE_STATUS(
            rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                     M,
                                                     N,
                                                     safe_size,
                                                     csr_descr_A,
                                                     nullptr,
                                                     nullptr,
                                                     nullptr,
                                                     percentage,
                                                     csr_descr_C,
                                                     nullptr,
                                                     nullptr,
                                                     nullptr,
                                                     info,
                                                     nullptr),
            (M < 0 || N < 0 || percentage < static_cast<T>(0) || percentage > static_cast<T>(100))
                ? rocsparse_status_invalid_size
                : rocsparse_status_success);

        return;
    }

    // Allocate host memory for output CSR matrix
    host_vector<rocsparse_int> h_csr_row_ptr_A;
    host_vector<rocsparse_int> h_csr_col_ind_A;
    host_vector<T>             h_csr_val_A;

    host_vector<rocsparse_int> h_nnz_total_dev_host_ptr(1);

    rocsparse_seedrand();

    // Generate uncompressed CSR matrix on host (or read from file)
    rocsparse_int nnz_A = 0;
    rocsparse_init_csr_matrix(h_csr_row_ptr_A,
                              h_csr_col_ind_A,
                              h_csr_val_A,
                              M,
                              N,
                              K,
                              dim_x,
                              dim_y,
                              dim_z,
                              nnz_A,
                              csr_base_A,
                              mat,
                              filename.c_str(),
                              false,
                              full_rank);

    // Argument sanity check before allocating invalid memory
    if(nnz_A <= 0)
    {
        EXPECT_ROCSPARSE_STATUS(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                         M,
                                                                         N,
                                                                         nnz_A,
                                                                         csr_descr_A,
                                                                         nullptr,
                                                                         nullptr,
                                                                         nullptr,
                                                                         percentage,
                                                                         csr_descr_C,
                                                                         nullptr,
                                                                         nullptr,
                                                                         nullptr,
                                                                         info,
                                                                         nullptr),
                                (nnz_A < 0) ? rocsparse_status_invalid_size
                                            : rocsparse_status_success);

        return;
    }

    // Allocate device memory for input CSR matrix
    device_vector<rocsparse_int> d_nnz_total_dev_host_ptr(1);
    device_vector<rocsparse_int> d_csr_row_ptr_C(M + 1);
    device_vector<rocsparse_int> d_csr_row_ptr_A(M + 1);
    device_vector<rocsparse_int> d_csr_col_ind_A(nnz_A);
    device_vector<T>             d_csr_val_A(nnz_A);

    if(!d_nnz_total_dev_host_ptr || !d_csr_row_ptr_C || !d_csr_row_ptr_A || !d_csr_col_ind_A
       || !d_csr_val_A)
    {
        CHECK_HIP_ERROR(hipErrorOutOfMemory);
        return;
    }

    // Copy data from CPU to device
    CHECK_HIP_ERROR(hipMemcpy(
        d_csr_row_ptr_A, h_csr_row_ptr_A, sizeof(rocsparse_int) * (M + 1), hipMemcpyHostToDevice));
    CHECK_HIP_ERROR(hipMemcpy(
        d_csr_col_ind_A, h_csr_col_ind_A, sizeof(rocsparse_int) * nnz_A, hipMemcpyHostToDevice));
    CHECK_HIP_ERROR(hipMemcpy(d_csr_val_A, h_csr_val_A, sizeof(T) * nnz_A, hipMemcpyHostToDevice));

    size_t buffer_size = 0;
    CHECK_ROCSPARSE_ERROR(rocsparse_prune_csr2csr_by_percentage_buffer_size<T>(handle,
                                                                               M,
                                                                               N,
                                                                               nnz_A,
                                                                               csr_descr_A,
                                                                               nullptr,
                                                                               d_csr_row_ptr_A,
                                                                               nullptr,
                                                                               percentage,
                                                                               csr_descr_C,
                                                                               nullptr,
                                                                               nullptr,
                                                                               nullptr,
                                                                               info,
                                                                               &buffer_size));

    T* d_temp_buffer = nullptr;
    CHECK_HIP_ERROR(hipMalloc(&d_temp_buffer, buffer_size));

    if(!d_temp_buffer)
    {
        CHECK_HIP_ERROR(hipErrorOutOfMemory);
        return;
    }

    CHECK_ROCSPARSE_ERROR(rocsparse_set_pointer_mode(handle, rocsparse_pointer_mode_host));
    CHECK_ROCSPARSE_ERROR(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                       M,
                                                                       N,
                                                                       nnz_A,
                                                                       csr_descr_A,
                                                                       d_csr_val_A,
                                                                       d_csr_row_ptr_A,
                                                                       d_csr_col_ind_A,
                                                                       percentage,
                                                                       csr_descr_C,
                                                                       d_csr_row_ptr_C,
                                                                       h_nnz_total_dev_host_ptr,
                                                                       info,
                                                                       d_temp_buffer));

    CHECK_ROCSPARSE_ERROR(rocsparse_set_pointer_mode(handle, rocsparse_pointer_mode_device));
    CHECK_ROCSPARSE_ERROR(rocsparse_prune_csr2csr_nnz_by_percentage<T>(handle,
                                                                       M,
                                                                       N,
                                                                       nnz_A,
                                                                       csr_descr_A,
                                                                       d_csr_val_A,
                                                                       d_csr_row_ptr_A,
                                                                       d_csr_col_ind_A,
                                                                       percentage,
                                                                       csr_descr_C,
                                                                       d_csr_row_ptr_C,
                                                                       d_nnz_total_dev_host_ptr,
                                                                       info,
                                                                       d_temp_buffer));

    device_vector<rocsparse_int> d_csr_col_ind_C(h_nnz_total_dev_host_ptr[0]);
    device_vector<T>             d_csr_val_C(h_nnz_total_dev_host_ptr[0]);

    if(arg.unit_check)
    {
        host_vector<rocsparse_int> h_nnz_total_copied_from_device(1);
        CHECK_HIP_ERROR(hipMemcpy(h_nnz_total_copied_from_device,
                                  d_nnz_total_dev_host_ptr,
                                  sizeof(rocsparse_int),
                                  hipMemcpyDeviceToHost));

        unit_check_general<rocsparse_int>(
            1, 1, 1, h_nnz_total_dev_host_ptr, h_nnz_total_copied_from_device);

        CHECK_ROCSPARSE_ERROR(rocsparse_set_pointer_mode(handle, rocsparse_pointer_mode_host));

        CHECK_ROCSPARSE_ERROR(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                       M,
                                                                       N,
                                                                       nnz_A,
                                                                       csr_descr_A,
                                                                       d_csr_val_A,
                                                                       d_csr_row_ptr_A,
                                                                       d_csr_col_ind_A,
                                                                       percentage,
                                                                       csr_descr_C,
                                                                       d_csr_val_C,
                                                                       d_csr_row_ptr_C,
                                                                       d_csr_col_ind_C,
                                                                       info,
                                                                       d_temp_buffer));

        host_vector<rocsparse_int> h_csr_row_ptr_C(M + 1);
        host_vector<rocsparse_int> h_csr_col_ind_C(h_nnz_total_dev_host_ptr[0]);
        host_vector<T>             h_csr_val_C(h_nnz_total_dev_host_ptr[0]);

        // Copy output to host
        CHECK_HIP_ERROR(hipMemcpy(h_csr_row_ptr_C,
                                  d_csr_row_ptr_C,
                                  sizeof(rocsparse_int) * (M + 1),
                                  hipMemcpyDeviceToHost));
        CHECK_HIP_ERROR(hipMemcpy(h_csr_col_ind_C,
                                  d_csr_col_ind_C,
                                  sizeof(rocsparse_int) * h_nnz_total_dev_host_ptr[0],
                                  hipMemcpyDeviceToHost));
        CHECK_HIP_ERROR(hipMemcpy(h_csr_val_C,
                                  d_csr_val_C,
                                  sizeof(T) * h_nnz_total_dev_host_ptr[0],
                                  hipMemcpyDeviceToHost));

        // call host and check results
        host_vector<rocsparse_int> h_csr_row_ptr_C_cpu;
        host_vector<rocsparse_int> h_csr_col_ind_C_cpu;
        host_vector<T>             h_csr_val_C_cpu;
        host_vector<rocsparse_int> h_nnz_C_cpu(1);

        host_prune_csr_to_csr_by_percentage(M,
                                            N,
                                            nnz_A,
                                            h_csr_row_ptr_A,
                                            h_csr_col_ind_A,
                                            h_csr_val_A,
                                            h_nnz_C_cpu[0],
                                            h_csr_row_ptr_C_cpu,
                                            h_csr_col_ind_C_cpu,
                                            h_csr_val_C_cpu,
                                            csr_base_A,
                                            csr_base_C,
                                            percentage);

        unit_check_general<rocsparse_int>(1, 1, 1, h_nnz_C_cpu, h_nnz_total_dev_host_ptr);
        unit_check_general<rocsparse_int>(1, (M + 1), 1, h_csr_row_ptr_C_cpu, h_csr_row_ptr_C);
        unit_check_general<rocsparse_int>(
            1, h_nnz_total_dev_host_ptr[0], 1, h_csr_col_ind_C_cpu, h_csr_col_ind_C);
        unit_check_general<T>(1, h_nnz_total_dev_host_ptr[0], 1, h_csr_val_C_cpu, h_csr_val_C);
    }

    if(arg.timing)
    {
        int number_cold_calls = 2;
        int number_hot_calls  = arg.iters;

        CHECK_ROCSPARSE_ERROR(rocsparse_set_pointer_mode(handle, rocsparse_pointer_mode_host));

        // Warm up
        for(int iter = 0; iter < number_cold_calls; ++iter)
        {
            CHECK_ROCSPARSE_ERROR(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                           M,
                                                                           N,
                                                                           nnz_A,
                                                                           csr_descr_A,
                                                                           d_csr_val_A,
                                                                           d_csr_row_ptr_A,
                                                                           d_csr_col_ind_A,
                                                                           percentage,
                                                                           csr_descr_C,
                                                                           d_csr_val_C,
                                                                           d_csr_row_ptr_C,
                                                                           d_csr_col_ind_C,
                                                                           info,
                                                                           d_temp_buffer));
        }

        double gpu_time_used = get_time_us();

        // Performance run
        for(int iter = 0; iter < number_hot_calls; ++iter)
        {
            CHECK_ROCSPARSE_ERROR(rocsparse_prune_csr2csr_by_percentage<T>(handle,
                                                                           M,
                                                                           N,
                                                                           nnz_A,
                                                                           csr_descr_A,
                                                                           d_csr_val_A,
                                                                           d_csr_row_ptr_A,
                                                                           d_csr_col_ind_A,
                                                                           percentage,
                                                                           csr_descr_C,
                                                                           d_csr_val_C,
                                                                           d_csr_row_ptr_C,
                                                                           d_csr_col_ind_C,
                                                                           info,
                                                                           d_temp_buffer));
        }

        gpu_time_used = (get_time_us() - gpu_time_used) / number_hot_calls;

        double gpu_gbyte = prune_csr2csr_gbyte_count<T>(M, nnz_A, h_nnz_total_dev_host_ptr[0])
                           / gpu_time_used * 1e6;

        std::cout.precision(2);
        std::cout.setf(std::ios::fixed);
        std::cout.setf(std::ios::left);

        std::cout << std::setw(12) << "M" << std::setw(12) << "N" << std::setw(12) << "nnz_A"
                  << std::setw(12) << "nnz_C" << std::setw(12) << "percentage" << std::setw(12)
                  << "GB/s" << std::setw(12) << "msec" << std::setw(12) << "iter" << std::setw(12)
                  << "verified" << std::endl;

        std::cout << std::setw(12) << M << std::setw(12) << N << std::setw(12) << nnz_A
                  << std::setw(12) << h_nnz_total_dev_host_ptr[0] << std::setw(12) << percentage
                  << std::setw(12) << gpu_gbyte << std::setw(12) << gpu_time_used / 1e3
                  << std::setw(12) << number_hot_calls << std::setw(12)
                  << (arg.unit_check ? "yes" : "no") << std::endl;
    }

    CHECK_HIP_ERROR(hipFree(d_temp_buffer));
}

#endif // TESTING_PRUNE_CSR2CSR_BY_PERCENTAGE_HPP
