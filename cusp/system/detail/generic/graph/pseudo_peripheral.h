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
#include <cusp/detail/temporary_array.h>

#include <cusp/graph/breadth_first_search.h>

#include <thrust/execution_policy.h>

namespace cusp
{
namespace graph
{
template<typename DerivedPolicy, typename MatrixType, typename ArrayType, typename Format>
typename MatrixType::index_type
pseudo_peripheral_vertex(const thrust::detail::execution_policy_base<DerivedPolicy>& exec,
                         const MatrixType& G,
                         ArrayType& levels,
                         Format format);
} // end graph namespace

namespace system
{
namespace detail
{
namespace generic
{

template<typename DerivedPolicy, typename MatrixType>
typename MatrixType::index_type
pseudo_peripheral_vertex(thrust::execution_policy<DerivedPolicy>& exec,
                         const MatrixType& G)
{
    typedef typename MatrixType::index_type IndexType;

    cusp::detail::temporary_array<IndexType, DerivedPolicy> levels(exec, G.num_rows);

    return cusp::graph::pseudo_peripheral_vertex(exec, G, levels);
}

template<typename DerivedPolicy, typename MatrixType, typename ArrayType>
typename MatrixType::index_type
pseudo_peripheral_vertex(thrust::execution_policy<DerivedPolicy>& exec,
                         const MatrixType& G,
                         ArrayType& levels)
{
    typedef typename MatrixType::format Format;

    Format format;

    return cusp::graph::pseudo_peripheral_vertex(exec, G, levels, format);
}

template<typename DerivedPolicy, typename MatrixType, typename ArrayType>
typename MatrixType::index_type
pseudo_peripheral_vertex(thrust::execution_policy<DerivedPolicy>& exec,
                         const MatrixType& G,
                         ArrayType& levels,
                         cusp::csr_format)
{
    typedef typename MatrixType::index_type IndexType;

    IndexType delta = 0;
    IndexType x = rand() % G.num_rows;
    IndexType y;

    cusp::detail::temporary_array<IndexType, DerivedPolicy> row_lengths(exec, G.num_rows);
    thrust::transform(exec,
                      G.row_offsets.begin() + 1, G.row_offsets.end(),
                      G.row_offsets.begin(), row_lengths.begin(),
                      thrust::minus<IndexType>());

    while(1)
    {
        cusp::graph::breadth_first_search(exec, G, x, levels);

        typename ArrayType::iterator max_level_iter = thrust::max_element(exec, levels.begin(), levels.end());
        int max_level = *max_level_iter;
        int max_count = thrust::count(exec, levels.begin(), levels.end(), max_level);

        if( max_count > 1 )
        {
            cusp::detail::temporary_array<IndexType, DerivedPolicy> max_level_vertices(exec, max_count);
            cusp::detail::temporary_array<IndexType, DerivedPolicy> max_level_valence(exec, max_count);

            thrust::copy_if(exec,
                            thrust::counting_iterator<IndexType>(0),
                            thrust::counting_iterator<IndexType>(G.num_rows),
                            levels.begin(),
                            max_level_vertices.begin(),
                            thrust::placeholders::_1 == max_level);

            thrust::gather(exec,
                           thrust::counting_iterator<IndexType>(0),
                           thrust::counting_iterator<IndexType>(max_count),
                           row_lengths.begin(),
                           max_level_valence.begin());

            int min_valence_pos = thrust::min_element(exec, max_level_valence.begin(), max_level_valence.end()) - max_level_valence.begin();

            y = max_level_vertices[min_valence_pos];
        }
        else
        {
            y = max_level_iter - levels.begin();
        }

        if( levels[y] <= delta ) break;

        x = y;
        delta = levels[y];
    }

    return y;
}

} // end namespace generic
} // end namespace detail
} // end namespace system

namespace graph
{
template <typename DerivedPolicy, typename MatrixType, typename ArrayType, typename Format>
typename MatrixType::index_type
pseudo_peripheral_vertex(const thrust::detail::execution_policy_base<DerivedPolicy>& exec,
                         const MatrixType& G,
                         ArrayType& levels,
                         Format format)
{
    using cusp::system::detail::generic::pseudo_peripheral_vertex;

    return pseudo_peripheral_vertex(thrust::detail::derived_cast(thrust::detail::strip_const(exec)), G, levels, format);
}
} // end graph namespace
} // end namespace cusp
