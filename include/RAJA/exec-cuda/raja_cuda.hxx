/*!
 ******************************************************************************
 *
 * \file
 *
 * \brief   Header file containing RAJA headers for NVCC CUDA execution.
 *
 *          These methods work only on platforms that support CUDA. 
 *
 ******************************************************************************
 */

#ifndef RAJA_cuda_HXX
#define RAJA_cuda_HXX

#if defined(RAJA_USE_CUDA)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016, Lawrence Livermore National Security, LLC.
//
// Produced at the Lawrence Livermore National Laboratory
//
// LLNL-CODE-689114
//
// All rights reserved.
//
// This file is part of RAJA.
//
// For additional details, please also read raja/README-license.txt.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the disclaimer below.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the disclaimer (as noted below) in the
//   documentation and/or other materials provided with the distribution.
//
// * Neither the name of the LLNS/LLNL nor the names of its contributors may
//   be used to endorse or promote products derived from this software without
//   specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL LAWRENCE LIVERMORE NATIONAL SECURITY,
// LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <cuda.h>
#include <cuda_runtime.h>


namespace RAJA {


//
/////////////////////////////////////////////////////////////////////
//
// Generalization of CUDA dim3 x, y and z used to describe
// sizes and indices for threads and blocks.
//
/////////////////////////////////////////////////////////////////////
//

struct Dim3x {
  __host__ __device__ inline unsigned int &operator()(dim3 &dim){
    return dim.x;
  }

  __host__ __device__ inline unsigned int operator()(dim3 const &dim){
    return dim.x;
  }
};

struct Dim3y {
  __host__ __device__ inline unsigned int &operator()(dim3 &dim){
    return dim.y;
  }

  __host__ __device__ inline unsigned int operator()(dim3 const &dim){
    return dim.y;
  }
};

struct Dim3z {
  __host__ __device__ inline unsigned int &operator()(dim3 &dim){
    return dim.z;
  }

  __host__ __device__ inline unsigned int operator()(dim3 const &dim){
    return dim.z;
  }
};

//
/////////////////////////////////////////////////////////////////////
//
// Execution policies
//
/////////////////////////////////////////////////////////////////////
//

///
/// Segment execution policies
///
template <size_t BLOCK_SIZE>
struct cuda_exec {};
///
template <size_t BLOCK_SIZE>
struct cuda_exec_async {};


//
//
//

//
// NOTE: There is no Index set segment iteration policy for CUDA
//

///
///////////////////////////////////////////////////////////////////////
///
/// Reduction execution policies
///
///////////////////////////////////////////////////////////////////////
///
template <size_t BLOCK_SIZE>
struct cuda_reduce {};

//
// Operations in the included files are parametrized using the following
// values. 
//
const int WARP_SIZE = 32;


}  // closing brace for RAJA namespace


//
// Headers containing traversal and reduction templates 
//
#include "reduce_cuda.hxx"
#include "forall_cuda.hxx"

#if defined(RAJA_USE_NESTED)
#include "forallN_cuda.hxx"
#endif


#endif  // closing endif for if defined(RAJA_USE_CUDA)

#endif  // closing endif for header file include guard

