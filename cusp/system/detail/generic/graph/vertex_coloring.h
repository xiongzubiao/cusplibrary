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


#pragma once

#include <cusp/detail/config.h>
#include <cusp/detail/type_traits.h>

#include <thrust/execution_policy.h>

namespace cusp
{
namespace graph
{
template <typename DerivedPolicy, typename MatrixType, typename ArrayType, typename Format>
size_t vertex_coloring(const thrust::detail::execution_policy_base<DerivedPolicy>& exec,
                     const MatrixType& G,
                     ArrayType& colors,
                     Format format);
} // end graph namespace

namespace system
{
namespace detail
{
namespace generic
{

template<typename DerivedPolicy, typename MatrixType, typename ArrayType>
size_t vertex_coloring(thrust::execution_policy<DerivedPolicy>& exec,
                     const MatrixType& G,
                     ArrayType& colors)
{
    typedef typename MatrixType::format Format;

    Format format;

    return cusp::graph::vertex_coloring(exec, G, colors, format);
}

template<typename DerivedPolicy, typename MatrixType, typename ArrayType>
size_t vertex_coloring(thrust::execution_policy<DerivedPolicy>& exec,
                       const MatrixType& G,
                       ArrayType& colors,
                       cusp::known_format)
{
    typedef typename cusp::detail::as_csr_type<MatrixType>::type CsrMatrix;

    CsrMatrix G_csr(G);

    return cusp::graph::vertex_coloring(exec, G_csr, colors);
}

} // end namespace generic
} // end namespace detail
} // end namespace system

namespace graph
{
template <typename DerivedPolicy, typename MatrixType, typename ArrayType, typename Format>
size_t vertex_coloring(const thrust::detail::execution_policy_base<DerivedPolicy>& exec,
                       const MatrixType& G,
                       ArrayType& colors,
                       Format format)
{
    using cusp::system::detail::generic::vertex_coloring;

    return vertex_coloring(thrust::detail::derived_cast(thrust::detail::strip_const(exec)), G, colors, format);
}
} // end graph namespace
} // end namespace cusp
