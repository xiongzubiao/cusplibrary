/*
 *  Copyright 2008-2013 NVIDIA Corporation
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

#include <cusp/graph/pseudo_peripheral.h>

#include <thrust/execution_policy.h>
#include <thrust/sequence.h>
#include <thrust/scatter.h>
#include <thrust/sort.h>

namespace cusp
{
namespace graph
{
template <typename DerivedPolicy, typename MatrixType, typename PermutationType, typename Format>
void symmetric_rcm(const thrust::detail::execution_policy_base<DerivedPolicy>& exec,
                   const MatrixType& G,
                   PermutationType& P,
                   Format format);
} // end graph namespace

namespace system
{
namespace detail
{
namespace generic
{

template<typename DerivedPolicy, typename MatrixType, typename PermutationType>
void symmetric_rcm(thrust::execution_policy<DerivedPolicy>& exec,
                   const MatrixType& G,
                   PermutationType& P,
                   cusp::csr_format)
{
    typedef typename MatrixType::index_type IndexType;
    typedef typename MatrixType::value_type ValueType;
    typedef typename MatrixType::memory_space MemorySpace;

    // find peripheral vertex and return BFS levels from vertex
    cusp::graph::pseudo_peripheral_vertex(exec, G, P.permutation);

    // sort vertices by level in BFS traversal
    cusp::detail::temporary_array<IndexType,DerivedPolicy> levels(exec, G.num_rows);
    thrust::sequence(exec, levels.begin(), levels.end());
    thrust::sort_by_key(exec, P.permutation.begin(), P.permutation.end(), levels.begin());

    // form RCM permutation matrix
    thrust::scatter(exec,
                    thrust::counting_iterator<IndexType>(0),
                    thrust::counting_iterator<IndexType>(G.num_rows),
                    levels.begin(), P.permutation.begin());
}

template <typename DerivedPolicy, typename MatrixType, typename PermutationType>
void symmetric_rcm(const thrust::detail::execution_policy_base<DerivedPolicy>& exec,
                   const MatrixType& G,
                   PermutationType& P)
{
    typedef typename MatrixType::format Format;

    Format format;

    cusp::graph::symmetric_rcm(exec, G, P, format);
}

template <typename DerivedPolicy, typename MatrixType, typename PermutationType>
void symmetric_rcm(const thrust::detail::execution_policy_base<DerivedPolicy>& exec,
                   const MatrixType& G,
                   PermutationType& P,
                   cusp::known_format)
{
    typedef typename cusp::detail::as_csr_type<MatrixType>::type CsrMatrix;

    CsrMatrix G_csr(G);

    cusp::graph::symmetric_rcm(exec, G_csr, P);
}

} // end namespace generic
} // end namespace detail
} // end namespace system

namespace graph
{
template <typename DerivedPolicy, typename MatrixType, typename PermutationType, typename Format>
void symmetric_rcm(const thrust::detail::execution_policy_base<DerivedPolicy>& exec,
                   const MatrixType& G,
                   PermutationType& P,
                   Format format)
{
    using cusp::system::detail::generic::symmetric_rcm;

    symmetric_rcm(thrust::detail::derived_cast(thrust::detail::strip_const(exec)), G, P, format);
}
} // end graph namespace
} // end namespace cusp
