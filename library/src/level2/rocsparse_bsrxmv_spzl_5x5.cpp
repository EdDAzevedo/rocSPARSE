/*! \file */
/* ************************************************************************
 * Copyright (c) 2021-2022 Advanced Micro Devices, Inc.
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

#include "rocsparse_bsrxmv_spzl.hpp"

template <unsigned int BLOCKSIZE, rocsparse_direction DIR>
__device__ void sbsrxmvn_5x5_device(rocsparse_int mb,
                                    float         alpha,
                                    rocsparse_int size_of_mask,
                                    const rocsparse_int* __restrict__ bsr_mask_ptr,
                                    const rocsparse_int* __restrict__ bsr_row_ptr,
                                    const rocsparse_int* __restrict__ bsr_end_ptr,
                                    const rocsparse_int* __restrict__ bsr_col_ind,
                                    const float* __restrict__ bsr_val,
                                    const float* __restrict__ x,
                                    float beta,
                                    float* __restrict__ y,
                                    rocsparse_index_base idx_base)

{
    static constexpr int block_size = 5;

#define A_OFFROW(i, j, k) bsr_val[(j) + ((i)*block_size) + ((uint64_t)(k)*block_size * block_size)]
#define A_OFFCOL(i, j, k) bsr_val[(i) + ((j)*block_size) + ((uint64_t)(k)*block_size * block_size)]

    int const local_i = threadIdx.x % block_size;
    int const local_j = threadIdx.x / block_size;

    int row = blockIdx.x * blockDim.y + threadIdx.y;
    if(bsr_mask_ptr != nullptr)
    {
        row = bsr_mask_ptr[row] - idx_base;
    }

    if((row < mb) && (local_j < block_size))
    {
        const int global_row = local_i + row * block_size;

        const rocsparse_int offset_begin = bsr_row_ptr[row] - idx_base;
        const rocsparse_int offset_end   = (bsr_end_ptr == nullptr)
                                               ? (bsr_row_ptr[row + 1] - idx_base)
                                               : (bsr_end_ptr[row] - idx_base);

        float fk, f1;
        fk = static_cast<float>(0);
        if(DIR == rocsparse_direction_row)
        {
#pragma unroll 4
            for(rocsparse_int offset = offset_begin; offset < offset_end; offset++)
            {
                const rocsparse_int jam0 = bsr_col_ind[offset] - idx_base;
                fk                       = rocsparse_fma(
                    A_OFFROW(local_i, local_j, offset), x[local_j + jam0 * block_size], fk);
            }
        }
        else
        {
#pragma unroll 4
            for(rocsparse_int offset = offset_begin; offset < offset_end; offset++)
            {
                const rocsparse_int jam0 = bsr_col_ind[offset] - idx_base;
                fk                       = rocsparse_fma(
                    A_OFFCOL(local_i, local_j, offset), x[local_j + jam0 * block_size], fk);
            }
        }

        // Reduction along the subcolumns, threads with l=0 hold the complete sum
        f1 = fk;
#pragma unroll 4
        for(int s = 1; s < block_size; ++s)
            f1 = f1 + __shfl(fk, local_i + s * block_size, 32);

        f1 = beta * y[global_row] + alpha * f1;
        if(local_j == 0)
        {
            y[global_row] = f1;
        }
    }
}

// BSRXMV kernel for BSR block dimension of 5
template <unsigned int BLOCKSIZE, typename T>
__device__ void bsrxmvn_5x5_device(rocsparse_int       mb,
                                   rocsparse_direction dir,
                                   T                   alpha,
                                   rocsparse_int       size_of_mask,
                                   const rocsparse_int* __restrict__ bsr_mask_ptr,
                                   const rocsparse_int* __restrict__ bsr_row_ptr,
                                   const rocsparse_int* __restrict__ bsr_end_ptr,
                                   const rocsparse_int* __restrict__ bsr_col_ind,
                                   const T* __restrict__ bsr_val,
                                   const T* __restrict__ x,
                                   T beta,
                                   T* __restrict__ y,
                                   rocsparse_index_base idx_base)
{
    // BSR block dimension
    static constexpr int BSRDIM = 5;

    // BSR block lane id
    rocsparse_int lid = hipThreadIdx_x % BSRDIM;

    // Number of BSR blocks processed at the same time
    const unsigned int NBLOCKS = BLOCKSIZE / (BSRDIM * BSRDIM);

    // Offset into x vector
    rocsparse_int idx
        = (dir == rocsparse_direction_column) ? ((hipThreadIdx_x / BSRDIM) % BSRDIM) : lid;

    // Each thread block processes a single BSR row
    rocsparse_int row = hipBlockIdx_x;
    if(bsr_mask_ptr != nullptr)
    {
        row = bsr_mask_ptr[row] - idx_base;
    }

    // BSR row entry and exit point
    rocsparse_int row_begin = bsr_row_ptr[row] - idx_base;
    rocsparse_int row_end   = (bsr_end_ptr == nullptr) ? (bsr_row_ptr[row + 1] - idx_base)
                                                       : (bsr_end_ptr[row] - idx_base);

#if 0
    // Each thread block processes a single BSR row
    rocsparse_int row = bsr_mask_ptr[hipBlockIdx_x] - idx_base;



    // BSR row entry and exit point
    rocsparse_int row_begin = bsr_row_ptr[row] - idx_base;
    rocsparse_int row_end   = bsr_end_ptr[row] - idx_base;
#endif

    // BSR block row accumulator
    T sum = static_cast<T>(0);

    // Loop over all BSR blocks in the current row where each lane
    // processes a BSR block value
    for(rocsparse_int j = row_begin; j < row_end; j += NBLOCKS)
    {
        rocsparse_int k = j + hipThreadIdx_x / (BSRDIM * BSRDIM);

        // Do not exceed the row
        if(k < row_end)
        {
            // Column index into x vector
            rocsparse_int col = (bsr_col_ind[k] - idx_base) * BSRDIM;

            // Compute the sum of the two rows within the BSR blocks of the current
            // BSR row
            sum = rocsparse_fma(bsr_val[j * BSRDIM * BSRDIM + hipThreadIdx_x], x[col + idx], sum);
        }
    }

    // Accumulate each row sum of the BSR block
    __shared__ T sdata[BSRDIM * BSRDIM * NBLOCKS];

    sdata[hipThreadIdx_x] = sum;

    __threadfence_block();

    if(dir == rocsparse_direction_column)
    {
        if(hipThreadIdx_x < BLOCKSIZE - BSRDIM * 8)
            sdata[hipThreadIdx_x] += sdata[hipThreadIdx_x + BSRDIM * 8];
        __threadfence_block();
        if(hipThreadIdx_x < BSRDIM * 4)
            sdata[hipThreadIdx_x] += sdata[hipThreadIdx_x + BSRDIM * 4];
        __threadfence_block();
        if(hipThreadIdx_x < BSRDIM * 2)
            sdata[hipThreadIdx_x] += sdata[hipThreadIdx_x + BSRDIM * 2];
        __threadfence_block();
        if(hipThreadIdx_x < BSRDIM * 1)
            sum = sdata[hipThreadIdx_x] + sdata[hipThreadIdx_x + BSRDIM * 1];
    }
    else
    {
        // Accumulate the row sum for different blocks
        if(hipThreadIdx_x < BSRDIM * BSRDIM)
            sdata[hipThreadIdx_x] += sdata[hipThreadIdx_x + BSRDIM * BSRDIM];
        __threadfence_block();

        // Reduce the intra block row sum
        if(lid < 1)
            sdata[hipThreadIdx_x] += sdata[hipThreadIdx_x + 4];
        __threadfence_block();
        if(lid < 2)
            sdata[hipThreadIdx_x] += sdata[hipThreadIdx_x + 2];
        __threadfence_block();

        // Final reduction
        if(hipThreadIdx_x < BSRDIM)
            sum = sdata[hipThreadIdx_x * BSRDIM] + sdata[hipThreadIdx_x * BSRDIM + 1];
    }

    // First 5 threads write row sums to global memory
    if(hipThreadIdx_x < BSRDIM)
    {
        if(beta != static_cast<T>(0))
        {
            y[row * BSRDIM + hipThreadIdx_x]
                = rocsparse_fma(beta, y[row * BSRDIM + hipThreadIdx_x], alpha * sum);
        }
        else
        {
            y[row * BSRDIM + hipThreadIdx_x] = alpha * sum;
        }
    }
}

template <unsigned int BLOCKSIZE, typename T, typename U>
__launch_bounds__(BLOCKSIZE) ROCSPARSE_KERNEL
    void bsrxmvn_5x5_kernel(rocsparse_int       mb,
                            rocsparse_direction dir,
                            U                   alpha_device_host,
                            rocsparse_int       size_of_mask,
                            const rocsparse_int* __restrict__ bsr_mask_ptr,
                            const rocsparse_int* __restrict__ bsr_row_ptr,
                            const rocsparse_int* __restrict__ bsr_end_ptr,
                            const rocsparse_int* __restrict__ bsr_col_ind,
                            const T* __restrict__ bsr_val,
                            const T* __restrict__ x,
                            U beta_device_host,
                            T* __restrict__ y,
                            rocsparse_index_base idx_base)
{
    auto alpha = load_scalar_device_host(alpha_device_host);
    auto beta  = load_scalar_device_host(beta_device_host);
    if(alpha != static_cast<T>(0) || beta != static_cast<T>(1))
    {
        bsrxmvn_5x5_device<BLOCKSIZE>(mb,
                                      dir,
                                      alpha,
                                      size_of_mask,
                                      bsr_mask_ptr,
                                      bsr_row_ptr,
                                      bsr_end_ptr,
                                      bsr_col_ind,
                                      bsr_val,
                                      x,
                                      beta,
                                      y,
                                      idx_base);
    }
}

template <unsigned int BLOCKSIZE, rocsparse_direction DIR, typename U>
__launch_bounds__(BLOCKSIZE) ROCSPARSE_KERNEL
    void sbsrxmvn_5x5_kernel(rocsparse_int mb,
                             U             alpha_device_host,
                             rocsparse_int size_of_mask,
                             const rocsparse_int* __restrict__ bsr_mask_ptr,
                             const rocsparse_int* __restrict__ bsr_row_ptr,
                             const rocsparse_int* __restrict__ bsr_end_ptr,
                             const rocsparse_int* __restrict__ bsr_col_ind,
                             const float* __restrict__ bsr_val,
                             const float* __restrict__ x,
                             U beta_device_host,
                             float* __restrict__ y,
                             rocsparse_index_base idx_base)
{
    auto alpha = load_scalar_device_host(alpha_device_host);
    auto beta  = load_scalar_device_host(beta_device_host);
    if(alpha != static_cast<float>(0) || beta != static_cast<float>(1))
    {
        sbsrxmvn_5x5_device<BLOCKSIZE, DIR>(mb,
                                            alpha,
                                            size_of_mask,
                                            bsr_mask_ptr,
                                            bsr_row_ptr,
                                            bsr_end_ptr,
                                            bsr_col_ind,
                                            bsr_val,
                                            x,
                                            beta,
                                            y,
                                            idx_base);
    }
}

template <typename T>
struct kernels_type_dispatch
{
    template <typename U>
    static void bsrxmvn_5x5(rocsparse_handle     handle,
                            rocsparse_direction  dir,
                            rocsparse_int        mb,
                            rocsparse_int        nnzb,
                            U                    alpha_device_host,
                            rocsparse_int        size_of_mask,
                            const rocsparse_int* bsr_mask_ptr,
                            const rocsparse_int* bsr_row_ptr,
                            const rocsparse_int* bsr_end_ptr,
                            const rocsparse_int* bsr_col_ind,
                            const T*             bsr_val,
                            const T*             x,
                            U                    beta_device_host,
                            T*                   y,
                            rocsparse_index_base base)
    {
        const rocsparse_int size = (bsr_mask_ptr == nullptr) ? mb : size_of_mask;
        hipLaunchKernelGGL((bsrxmvn_5x5_kernel<50>),
                           dim3(size),
                           dim3(50),
                           0,
                           handle->stream,
                           mb,
                           dir,
                           alpha_device_host,
                           size_of_mask,
                           bsr_mask_ptr,
                           bsr_row_ptr,
                           bsr_end_ptr,
                           bsr_col_ind,
                           bsr_val,
                           x,
                           beta_device_host,
                           y,
                           base);
    }
};

//
// Specialization for floats.
//
template <>
struct kernels_type_dispatch<float>
{
    template <typename U>
    static void bsrxmvn_5x5(rocsparse_handle     handle,
                            rocsparse_direction  dir,
                            rocsparse_int        mb,
                            rocsparse_int        nnzb,
                            U                    alpha_device_host,
                            rocsparse_int        size_of_mask,
                            const rocsparse_int* bsr_mask_ptr,
                            const rocsparse_int* bsr_row_ptr,
                            const rocsparse_int* bsr_end_ptr,
                            const rocsparse_int* bsr_col_ind,
                            const float*         bsr_val,
                            const float*         x,
                            U                    beta_device_host,
                            float*               y,
                            rocsparse_index_base base)
    {
        const int wsize = handle->wavefront_size;
        if(wsize == 32)
        {
            const rocsparse_int size = (bsr_mask_ptr == nullptr) ? mb : size_of_mask;
            hipLaunchKernelGGL((bsrxmvn_5x5_kernel<50>),
                               dim3(size),
                               dim3(50),
                               0,
                               handle->stream,
                               mb,
                               dir,
                               alpha_device_host,
                               size_of_mask,
                               bsr_mask_ptr,
                               bsr_row_ptr,
                               bsr_end_ptr,
                               bsr_col_ind,
                               bsr_val,
                               x,
                               beta_device_host,
                               y,
                               base);
        }
        else
        {
            static constexpr int nhalfwarps_per_block  = 8;
            static constexpr int nthreads_per_halfwarp = 32;
            const rocsparse_int  size = (bsr_mask_ptr == nullptr) ? mb : size_of_mask;
            dim3 const           nThreads_solver(nthreads_per_halfwarp, nhalfwarps_per_block, 1);
            dim3 const           nBlocks_solver((size - 1) / nhalfwarps_per_block + 1, 1, 1);

            if(rocsparse_direction_row == dir)
            {
                hipLaunchKernelGGL(
                    (sbsrxmvn_5x5_kernel<nthreads_per_halfwarp * nhalfwarps_per_block,
                                         rocsparse_direction_row>),
                    nBlocks_solver,
                    nThreads_solver,
                    0,
                    handle->stream,
                    mb,
                    alpha_device_host,
                    size_of_mask,
                    bsr_mask_ptr,
                    bsr_row_ptr,
                    bsr_end_ptr,
                    bsr_col_ind,
                    bsr_val,
                    x,
                    beta_device_host,
                    y,
                    base);
            }
            else
            {
                hipLaunchKernelGGL(
                    (sbsrxmvn_5x5_kernel<nthreads_per_halfwarp * nhalfwarps_per_block,
                                         rocsparse_direction_column>),
                    nBlocks_solver,
                    nThreads_solver,
                    0,
                    handle->stream,
                    mb,
                    alpha_device_host,
                    size_of_mask,
                    bsr_mask_ptr,
                    bsr_row_ptr,
                    bsr_end_ptr,
                    bsr_col_ind,
                    bsr_val,
                    x,
                    beta_device_host,
                    y,
                    base);
            }
        }
    }
};

template <typename T, typename U>
void bsrxmvn_5x5(rocsparse_handle     handle,
                 rocsparse_direction  dir,
                 rocsparse_int        mb,
                 rocsparse_int        nnzb,
                 U                    alpha_device_host,
                 rocsparse_int        size_of_mask,
                 const rocsparse_int* bsr_mask_ptr,
                 const rocsparse_int* bsr_row_ptr,
                 const rocsparse_int* bsr_end_ptr,
                 const rocsparse_int* bsr_col_ind,
                 const T*             bsr_val,
                 const T*             x,
                 U                    beta_device_host,
                 T*                   y,
                 rocsparse_index_base base)
{
    kernels_type_dispatch<T>::template bsrxmvn_5x5<U>(handle,
                                                      dir,
                                                      mb,
                                                      nnzb,
                                                      alpha_device_host,
                                                      size_of_mask,
                                                      bsr_mask_ptr,
                                                      bsr_row_ptr,
                                                      bsr_end_ptr,
                                                      bsr_col_ind,
                                                      bsr_val,
                                                      x,
                                                      beta_device_host,
                                                      y,
                                                      base);
}

//
// INSTANTIATE.
//
#define INSTANTIATE(TYPE)                                             \
    template void bsrxmvn_5x5(rocsparse_handle     handle,            \
                              rocsparse_direction  dir,               \
                              rocsparse_int        mb,                \
                              rocsparse_int        nnzb,              \
                              const TYPE*          alpha_device_host, \
                              rocsparse_int        size_of_mask,      \
                              const rocsparse_int* bsr_mask_ptr,      \
                              const rocsparse_int* bsr_row_ptr,       \
                              const rocsparse_int* bsr_end_ptr,       \
                              const rocsparse_int* bsr_col_ind,       \
                              const TYPE*          bsr_val,           \
                              const TYPE*          x,                 \
                              const TYPE*          beta_device_host,  \
                              TYPE*                y,                 \
                              rocsparse_index_base base);             \
    template void bsrxmvn_5x5(rocsparse_handle     handle,            \
                              rocsparse_direction  dir,               \
                              rocsparse_int        mb,                \
                              rocsparse_int        nnzb,              \
                              TYPE                 alpha_device_host, \
                              rocsparse_int        size_of_mask,      \
                              const rocsparse_int* bsr_mask_ptr,      \
                              const rocsparse_int* bsr_row_ptr,       \
                              const rocsparse_int* bsr_end_ptr,       \
                              const rocsparse_int* bsr_col_ind,       \
                              const TYPE*          bsr_val,           \
                              const TYPE*          x,                 \
                              TYPE                 beta_device_host,  \
                              TYPE*                y,                 \
                              rocsparse_index_base base)

INSTANTIATE(float);
INSTANTIATE(double);
INSTANTIATE(rocsparse_float_complex);
INSTANTIATE(rocsparse_double_complex);

#undef INSTANTIATE
