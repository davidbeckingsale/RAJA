#!/usr/bin/env python

notice= """
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
"""



import sys
from itertools import permutations
from lperm import *



def writeForallPermutations(ndims):
  
  dim_names = getDimNames(ndims)
  
  # Create common strings to all perms
  body_args     = ", ".join(dim_names)  
  next_template = ", ".join(map(lambda a: "typename P%s"%a.upper() , dim_names))
  next_param    = ", ".join(map(lambda a: "P%s const &p%s"%(a.upper(), a) , dim_names))
  
  # Loop over each permutation specialization
  perms = getDimPerms(dim_names)
  for perm in perms:
    # get enumeration name
    enum_name = getEnumName(perm)
    
    # Compute permuted arguments
    func_param   = ", ".join(map(lambda a: "Index_type %s"%a , perm))  
    policy_args   = ", ".join(map(lambda a: "p%s"%a , perm))
    
    # Print header for functor
    print """
template<typename BODY>
struct ForallN_Permute_Functor<%s, BODY>{

  RAJA_INLINE
  constexpr
  explicit ForallN_Permute_Functor(BODY const &b) : body(b) {}

  RAJA_SUPPRESS_HD_WARN  
  RAJA_INLINE
  RAJA_HOST_DEVICE 
  void operator()(%s) const {
    body(%s);
  }
  
  template<typename NextPolicy, typename TAG, %s>
  RAJA_INLINE
  void callNextPolicy(%s) const {
    forallN_policy<NextPolicy>(TAG(), *this, %s);
  }
  
  BODY body;
};

    """ % (enum_name, func_param, body_args, next_template, next_param, policy_args)
    


def main(ndims):
  # ACTUAL SCRIPT ENTRY:
  print """//AUTOGENERATED BY gen_forallN_permute.py
%s
  
#ifndef RAJA_forallN_permute_HXX__
#define RAJA_forallN_permute_HXX__

#include "forallN_permute_lf.hxx"

namespace RAJA {

""" % notice
  ndims_list = range(2,ndims+1)

  # Create the loop interchange specializations for each permutation
  for n in ndims_list:
    writeForallPermutations(n)

  print """

} // namespace RAJA
  
#endif
"""

if __name__ == '__main__':
  main(int(sys.argv[1]))
  
