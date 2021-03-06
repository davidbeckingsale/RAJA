/*!
 ******************************************************************************
 *
 * \file
 *
 * \brief   RAJA header file defining simple class to manage scheduling
 *          of nodes in a task dependency graph.
 *
 ******************************************************************************
 */

#ifndef RAJA_DepGraphNode_HXX
#define RAJA_DepGraphNode_HXX

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

#include "config.hxx"

#include "int_datatypes.hxx"

#include <cstdlib>

#include <iosfwd>

namespace RAJA {

/*!
 ******************************************************************************
 *
 * \brief  Class defining a simple semephore-based data structure for 
 *         managing a node in a dependency graph.
 *
 ******************************************************************************
 */
class DepGraphNode
{
public:

   ///
   /// Constants for total number of allowable dependent tasks 
   /// and alignment of semaphore value (should match cache 
   /// coherence page size?).
   ///
   /// These values may need to be set differently for different
   /// algorithms and platforms.  We haven't figured this out yet!
   ///
   static const int _MaxDepTasks_          = 8;
   static const int _SemaphoreValueAlign_  = 256;

   ///
   /// Default ctor initializes node to default state.
   ///
   DepGraphNode()
      : m_num_dep_tasks(0),
        m_semaphore_reload_value(0),
        m_semaphore_value(0) 
   { 
      posix_memalign((void **)(&m_semaphore_value), 
                     _SemaphoreValueAlign_, sizeof(int)) ;
      *m_semaphore_value = 0 ;       
   }

   ///
   /// Dependency graph node dtor.
   ///
   ~DepGraphNode() { if (m_semaphore_value) free(m_semaphore_value); }

   ///
   /// Get/set semaphore value; i.e., the current number of (unsatisfied)
   /// dependencies that must be satisfied before this task can execute.
   ///
   int& semaphoreValue() {
      return *m_semaphore_value ;
   }

   ///
   /// Get/set semaphore "reload" value; i.e., the total number of external
   /// task dependencies that must be satisfied before this task can execute.
   ///
   int& semaphoreReloadValue() {
      return m_semaphore_reload_value ;
   }

   ///
   /// Get/set the number of "forward-dependencies" for this task; i.e., the
   /// number of external tasks that cannot execute until this task completes. 
   ///
   int& numDepTasks() {
      return m_num_dep_tasks ;
   }

   ///
   /// Get/set the forward dependency task number associated with the given 
   /// index for this task. This is used to notify the appropriate external 
   /// dependencies when this task completes.
   ///
   int& depTaskNum(int tidx) {
      return m_dep_task[tidx] ;
   }


   ///
   /// Print task graph object node data to given output stream.
   ///
   void print(std::ostream& os) const;

private:

   int  m_dep_task[_MaxDepTasks_] ;
   int  m_num_dep_tasks ;
   int  m_semaphore_reload_value ;
   int* m_semaphore_value ;

}; 


}  // closing brace for RAJA namespace

#endif  // closing endif for header file include guard
