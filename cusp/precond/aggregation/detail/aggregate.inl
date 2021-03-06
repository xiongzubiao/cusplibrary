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

#include <cusp/detail/config.h>

#include <cusp/precond/aggregation/system/detail/generic/standard_aggregate.h>
#include <cusp/precond/aggregation/system/detail/generic/mis_aggregate.h>

namespace cusp
{
namespace precond
{
namespace aggregation
{

template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
void standard_aggregate(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                        const MatrixType& A, ArrayType& aggregates, ArrayType& roots)
{
    using cusp::precond::aggregation::detail::standard_aggregate;

    standard_aggregate(thrust::detail::derived_cast(thrust::detail::strip_const(exec)), A, aggregates, roots);
}

template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
void standard_aggregate(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                        const MatrixType& A, ArrayType& aggregates)
{
    ArrayType roots(A.num_rows);

    standard_aggregate(exec, A, aggregates, roots);
}

template <typename MatrixType, typename ArrayType>
void standard_aggregate(const MatrixType& A, ArrayType& aggregates, ArrayType& roots)
{
    using thrust::system::detail::generic::select_system;

    typedef typename MatrixType::memory_space System;

    System system;

    cusp::precond::aggregation::standard_aggregate(select_system(system), A, aggregates, roots);
}

template <typename MatrixType, typename ArrayType>
void standard_aggregate(const MatrixType& A, ArrayType& aggregates)
{
    ArrayType roots(A.num_rows);

    standard_aggregate(A, aggregates, roots);
}

template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
void mis_aggregate(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                   const MatrixType& A, ArrayType& aggregates, ArrayType& roots)
{
    using cusp::precond::aggregation::detail::mis_aggregate;

    mis_aggregate(thrust::detail::derived_cast(thrust::detail::strip_const(exec)), A, aggregates, roots);
}

template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
void mis_aggregate(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                   const MatrixType& A, ArrayType& aggregates)
{
    ArrayType roots(A.num_rows);

    mis_aggregate(exec, A, aggregates, roots);
}

template <typename MatrixType, typename ArrayType>
void mis_aggregate(const MatrixType& A, ArrayType& aggregates, ArrayType& roots)
{
    using thrust::system::detail::generic::select_system;

    typedef typename MatrixType::memory_space System;

    System system;

    mis_aggregate(select_system(system), A, aggregates, roots);
}

template <typename MatrixType, typename ArrayType>
void mis_aggregate(const MatrixType& A, ArrayType& aggregates)
{
    ArrayType roots(A.num_rows);

    mis_aggregate(A, aggregates, roots);
}

template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
typename thrust::detail::enable_if_convertible<typename MatrixType::memory_space,cusp::host_memory>::type
aggregate(thrust::execution_policy<DerivedPolicy> &exec,
          const MatrixType& A, ArrayType& aggregates, ArrayType& roots)
{
    standard_aggregate(exec, A, aggregates, roots);
}

template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
typename thrust::detail::disable_if_convertible<typename MatrixType::memory_space,cusp::host_memory>::type
aggregate(thrust::execution_policy<DerivedPolicy> &exec,
          const MatrixType& A, ArrayType& aggregates, ArrayType& roots)
{
    mis_aggregate(exec, A, aggregates, roots);
}

template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
void aggregate(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
               const MatrixType& A, ArrayType& aggregates, ArrayType& roots)
{
    aggregate(thrust::detail::derived_cast(thrust::detail::strip_const(exec)), A, aggregates, roots);
}

template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
void aggregate(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
               const MatrixType& A, ArrayType& aggregates)
{
    ArrayType roots(A.num_rows);

    aggregate(thrust::detail::derived_cast(thrust::detail::strip_const(exec)), A, aggregates, roots);
}

template <typename MatrixType, typename ArrayType>
void aggregate(const MatrixType& A, ArrayType& aggregates, ArrayType& roots)
{
    using thrust::system::detail::generic::select_system;

    typedef typename MatrixType::memory_space System;

    System system;

    cusp::precond::aggregation::aggregate(select_system(system), A, aggregates, roots);
}

template <typename MatrixType, typename ArrayType>
void aggregate(const MatrixType& A, ArrayType& aggregates)
{
    ArrayType roots(A.num_rows);

    aggregate(A, aggregates, roots);
}

} // end namespace aggregation
} // end namespace precond
} // end namespace cusp

