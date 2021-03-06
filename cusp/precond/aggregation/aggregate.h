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

#include <cusp/execution_policy.h>

namespace cusp
{
namespace precond
{
namespace aggregation
{

/* \cond */
template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
void standard_aggregation(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                          const MatrixType& C, ArrayType& aggregates);

template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
void standard_aggregation(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                          const MatrixType& C, ArrayType& aggregates, ArrayType& roots);
/* \endcond */

template <typename MatrixType, typename ArrayType>
void standard_aggregation(const MatrixType& C, ArrayType& aggregates);

template <typename MatrixType, typename ArrayType>
void standard_aggregation(const MatrixType& C, ArrayType& aggregates, ArrayType& roots);

/* \cond */
template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
void mis_aggregation(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                     const MatrixType& C, ArrayType& aggregates);

template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
void mis_aggregation(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
                     const MatrixType& C, ArrayType& aggregates, ArrayType& roots);
/* \endcond */

template <typename MatrixType, typename ArrayType>
void mis_aggregation(const MatrixType& C, ArrayType& aggregates);

template <typename MatrixType, typename ArrayType>
void mis_aggregation(const MatrixType& C, ArrayType& aggregates, ArrayType& roots);

/* \cond */
template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
void aggregate(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
               const MatrixType& A, ArrayType& aggregates);
/* \endcond */

template <typename MatrixType, typename ArrayType>
void aggregate(const MatrixType& C, ArrayType& aggregates);

/* \cond */
template <typename DerivedPolicy, typename MatrixType, typename ArrayType>
void aggregate(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
               const MatrixType& A, ArrayType& aggregates, ArrayType& roots);
/* \endcond */

template <typename MatrixType, typename ArrayType>
void aggregate(const MatrixType& C, ArrayType& aggregates, ArrayType& roots);

} // end namespace aggregation
} // end namespace precond
} // end namespace cusp

#include <cusp/precond/aggregation/detail/aggregate.inl>
