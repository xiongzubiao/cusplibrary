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


#include <cusp/array1d.h>
#include <cusp/linear_operator.h>
#include <cusp/multiply.h>
#include <cusp/monitor.h>

#include <cusp/blas/blas.h>

#include <cusp/detail/temporary_array.h>

namespace blas = cusp::blas;

namespace cusp
{
namespace krylov
{
namespace bicg_detail
{

template <typename DerivedPolicy,
          class LinearOperator,
          class Vector,
          class Monitor,
          class Preconditioner>
void bicg(thrust::execution_policy<DerivedPolicy> &exec,
          LinearOperator& A,
          LinearOperator& At,
          Vector& x,
          Vector& b,
          Monitor& monitor,
          Preconditioner& M,
          Preconditioner& Mt)
{
    typedef typename LinearOperator::value_type           ValueType;
    typedef typename cusp::minimum_space<
            typename LinearOperator::memory_space,
            typename Vector::memory_space,
            typename Preconditioner::memory_space>::type  MemorySpace;

    assert(A.num_rows == A.num_cols);        // sanity check

    const size_t N = A.num_rows;

    // allocate workspace
    cusp::detail::temporary_array<ValueType, DerivedPolicy> y(exec, N);

    cusp::detail::temporary_array<ValueType, DerivedPolicy>  p(exec, N);
    cusp::detail::temporary_array<ValueType, DerivedPolicy>  p_star(exec, N);
    cusp::detail::temporary_array<ValueType, DerivedPolicy>  q(exec, N);
    cusp::detail::temporary_array<ValueType, DerivedPolicy>  q_star(exec, N);
    cusp::detail::temporary_array<ValueType, DerivedPolicy>  r(exec, N);
    cusp::detail::temporary_array<ValueType, DerivedPolicy>  r_star(exec, N);
    cusp::detail::temporary_array<ValueType, DerivedPolicy>  z(exec, N);
    cusp::detail::temporary_array<ValueType, DerivedPolicy>  z_star(exec, N);

    // y <- Ax
    cusp::multiply(exec, A, x, y);

    // r <- b - A*x
    blas::axpby(exec, b, y, r, ValueType(1), ValueType(-1));

    if(monitor.finished(r)) {
        return;
    }

    // r_star <- r
    blas::copy(exec, r, r_star);

    // z = M r
    cusp::multiply(exec, M, r, z);

    // z_star = Mt r_star
    cusp::multiply(exec, Mt, r_star, z_star);

    // rho = (z,r_star)
    ValueType rho = blas::dotc(exec, z, r_star);

    // p <- z
    blas::copy(exec, z, p);

    // p_star <- r
    blas::copy(exec, z_star, p_star);

    while (1)
    {
        // q = A p
        cusp::multiply(exec, A, p, q);

        // q_star = At p_star
        cusp::multiply(exec, At, p_star, q_star);

        // alpha = (rho) / (p_star, q)
        ValueType alpha = rho / blas::dotc(exec, p_star, q);

        // x += alpha*p
        blas::axpby(exec, x, p, x, ValueType(1), ValueType(alpha));

        // r -= alpha*q
        blas::axpby(exec, r, q, r, ValueType(1), ValueType(-alpha));

        // r_star -= alpha*q_star
        blas::axpby(exec, r_star, q_star, r_star, ValueType(1), ValueType(-alpha));

        if (monitor.finished(r)) {
            break;
        }

        // z = M r
        cusp::multiply(exec, M, r, z);

        // z_star = Mt r_star
        cusp::multiply(exec, Mt, r_star, z_star);

        ValueType prev_rho = rho;

        // rho = (z,r_star)
        rho = blas::dotc(exec, z, r_star);

        if(rho == ValueType(0)) {
            // Failure!
            // TODO: Make the failure more apparent to the user
            break;
        }

        ValueType beta = rho/prev_rho;

        // p = beta*p + z
        blas::axpby(exec, p, z, p, ValueType(beta), ValueType(1));

        // p_star = beta*p_star + z_star
        blas::axpby(exec, p_star, z_star, p_star, ValueType(beta), ValueType(1));

        ++monitor;
    }
}

template <typename DerivedPolicy,
          class LinearOperator,
          class Vector,
          class Monitor>
void bicg(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
          LinearOperator& A,
          LinearOperator& At,
          Vector& x,
          Vector& b,
          Monitor& monitor)
{
    typedef typename LinearOperator::value_type   ValueType;
    typedef typename LinearOperator::memory_space MemorySpace;

    cusp::identity_operator<ValueType,MemorySpace> M(A.num_rows, A.num_cols);

    cusp::krylov::bicg_detail::bicg(thrust::detail::derived_cast(thrust::detail::strip_const(exec)), A, At, x, b, monitor, M, M);
}

template <typename DerivedPolicy,
          class LinearOperator,
          class Vector>
void bicg(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
          LinearOperator& A,
          LinearOperator& At,
          Vector& x,
          Vector& b)
{
    typedef typename LinearOperator::value_type   ValueType;

    cusp::monitor<ValueType> monitor(b);

    cusp::krylov::bicg_detail::bicg(exec, A, At, x, b, monitor);
}

} // end bicg_detail namespace

template <typename DerivedPolicy,
          class LinearOperator,
          class Vector>
void bicg(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
          LinearOperator& A,
          LinearOperator& At,
          Vector& x,
          Vector& b)
{
    using cusp::krylov::bicg_detail::bicg;

    bicg(thrust::detail::derived_cast(thrust::detail::strip_const(exec)),
         A, At, x, b);
}

template <class LinearOperator,
          class Vector>
void bicg(LinearOperator& A,
          LinearOperator& At,
          Vector& x,
          Vector& b)
{
    using thrust::system::detail::generic::select_system;

    typedef typename LinearOperator::memory_space System1;
    typedef typename Vector::memory_space         System2;

    System1 system1;
    System2 system2;

    cusp::krylov::bicg(select_system(system1,system2), A, At, x, b);
}

template <typename DerivedPolicy,
          class LinearOperator,
          class Vector,
          class Monitor>
void bicg(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
          LinearOperator& A,
          LinearOperator& At,
          Vector& x,
          Vector& b,
          Monitor& monitor)
{
    using cusp::krylov::bicg_detail::bicg;

    bicg(thrust::detail::derived_cast(thrust::detail::strip_const(exec)),
         A, At, x, b, monitor);
}

template <class LinearOperator,
          class Vector,
          class Monitor>
void bicg(LinearOperator& A,
          LinearOperator& At,
          Vector& x,
          Vector& b,
          Monitor& monitor)
{
    using thrust::system::detail::generic::select_system;

    typedef typename LinearOperator::memory_space System1;
    typedef typename Vector::memory_space         System2;

    System1 system1;
    System2 system2;

    cusp::krylov::bicg(select_system(system1,system2), A, At, x, b, monitor);
}

template <typename DerivedPolicy,
          class LinearOperator,
          class Vector,
          class Monitor,
          class Preconditioner>
void bicg(const thrust::detail::execution_policy_base<DerivedPolicy> &exec,
          LinearOperator& A,
          LinearOperator& At,
          Vector& x,
          Vector& b,
          Monitor& monitor,
          Preconditioner& M,
          Preconditioner& Mt)
{
    using cusp::krylov::bicg_detail::bicg;

    bicg(thrust::detail::derived_cast(thrust::detail::strip_const(exec)),
         A, At, x, b, monitor, M, Mt);
}

template <class LinearOperator,
          class Vector,
          class Monitor,
          class Preconditioner>
void bicg(LinearOperator& A,
          LinearOperator& At,
          Vector& x,
          Vector& b,
          Monitor& monitor,
          Preconditioner& M,
          Preconditioner& Mt)
{
    using thrust::system::detail::generic::select_system;

    typedef typename LinearOperator::memory_space System1;
    typedef typename Vector::memory_space         System2;

    System1 system1;
    System2 system2;

    cusp::krylov::bicg(select_system(system1,system2), A, At, x, b, monitor, M, Mt);
}

} // end namespace krylov
} // end namespace cusp

