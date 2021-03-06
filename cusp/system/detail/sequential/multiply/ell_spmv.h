/*
 *  Copyright 2008-2014 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once

#include <cusp/detail/config.h>
#include <cusp/detail/format.h>

#include <cusp/functional.h>
#include <cusp/system/detail/sequential/execution_policy.h>

namespace cusp
{
namespace system
{
namespace detail
{
namespace sequential
{

template <typename DerivedPolicy,
         typename MatrixType,
         typename VectorType1,
         typename VectorType2,
         typename UnaryFunction,
         typename BinaryFunction1,
         typename BinaryFunction2>
void multiply(sequential::execution_policy<DerivedPolicy>& exec,
              const MatrixType& A,
              const VectorType1& x,
              VectorType2& y,
              UnaryFunction   initialize,
              BinaryFunction1 combine,
              BinaryFunction2 reduce,
              cusp::ell_format,
              cusp::array1d_format,
              cusp::array1d_format)
{
    typedef typename MatrixType::index_type  IndexType;
    typedef typename VectorType2::value_type ValueType;

    const size_t& num_entries_per_row = A.column_indices.num_cols;

    const IndexType invalid_index = MatrixType::invalid_index;

    for(size_t i = 0; i < A.num_rows; i++)
        y[i] = initialize(y[i]);

    for(size_t n = 0; n < num_entries_per_row; n++)
    {
        for(size_t i = 0; i < A.num_rows; i++)
        {
            const IndexType& j   = A.column_indices(i, n);
            const ValueType& Aij = A.values(i,n);

            if (j != invalid_index)
            {
                const ValueType& xj = x[j];
                y[i] = reduce(y[i], combine(Aij, xj));
            }
        }
    }
}

} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace cusp

