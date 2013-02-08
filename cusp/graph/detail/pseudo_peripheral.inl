/*
 *  Copyright 2008-2009 NVIDIA Corporation
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

/*! \file maximal_independent_set.h
 *  \brief Maximal independent set of a graph
 */

#pragma once

#include <cusp/detail/config.h>

#include <cusp/array1d.h>
#include <cusp/graph/bfs.h>

#include <thrust/copy.h>
#include <thrust/gather.h>
#include <thrust/iterator/counting_iterator.h>

namespace cusp
{
namespace graph
{
namespace detail
{

template<typename MatrixType, typename ArrayType>
typename MatrixType::index_type pseudo_peripheral_vertex(const MatrixType& G, ArrayType& levels)
{
    using namespace thrust::placeholders;

    typedef typename MatrixType::index_type IndexType;
    typedef typename MatrixType::memory_space MemorySpace;

    IndexType delta = 0;
    IndexType x = rand() % G.num_rows;
    IndexType y;

    ArrayType row_lengths(G.num_rows);
    thrust::transform(G.row_offsets.begin() + 1, G.row_offsets.end(), G.row_offsets.begin(), row_lengths.begin(), thrust::minus<IndexType>());

    while(1) {
        cusp::graph::bfs<false>(G, x, levels);

        typename ArrayType::iterator max_level_iter = thrust::max_element(levels.begin(), levels.end());
        int max_level = *max_level_iter;
        int max_count = thrust::count(levels.begin(), levels.end(), max_level);

        if( max_count > 1 ) {
            ArrayType max_level_vertices(max_count);
            ArrayType max_level_valence(max_count);

            thrust::copy_if(thrust::counting_iterator<IndexType>(0),
                            thrust::counting_iterator<IndexType>(G.num_rows),
                            levels.begin(),
                            max_level_vertices.begin(),
                            _1 == max_level);

            thrust::gather(thrust::counting_iterator<IndexType>(0),
                           thrust::counting_iterator<IndexType>(max_count),
                           row_lengths.begin(),
                           max_level_valence.begin());
            int min_valence_pos = thrust::min_element(max_level_valence.begin(), max_level_valence.end()) - max_level_valence.begin();

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

} // end namespace detail

template<typename MatrixType>
typename MatrixType::index_type pseudo_peripheral_vertex(const MatrixType& G)
{
    typedef typename MatrixType::index_type IndexType;
    typedef typename MatrixType::memory_space MemorySpace;
    cusp::array1d<IndexType,MemorySpace> levels(G.num_rows);
    return detail::pseudo_peripheral_vertex(G, levels);
}

} // end namespace graph
} // end namespace cusp
