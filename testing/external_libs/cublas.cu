#include <unittest/unittest.h>
#include <cusp/array2d.h>
#include <cusp/gallery/poisson.h>
#include <cusp/blas/cublas/blas.h>
#include <cusp/blas/blas.h>

template<typename ValueType>
void TestCublasAmax(void)
{
    typedef cusp::array1d<ValueType, cusp::device_memory>  Array;
    typedef typename Array::view                           View;

    cublasHandle_t handle;

    if(cublasCreate(&handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasCreate failed");
    }

    cusp::cublas::execution_policy cublas(handle);

    Array x(6);
    View view_x(x);

    x[0] =  7.0f;
    x[1] = -5.0f;
    x[2] =  4.0f;
    x[3] = -3.0f;
    x[4] =  0.0f;
    x[5] =  1.0f;

    ASSERT_EQUAL(cusp::blas::amax(cublas,x), 0);

    ASSERT_EQUAL(cusp::blas::amax(cublas,view_x), 0);

    if(cublasDestroy(handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasDestroy failed");
    }
}
DECLARE_NUMERIC_UNITTEST(TestCublasAmax);

template<typename ValueType>
void TestCublasAsum(void)
{
    typedef cusp::array1d<ValueType, cusp::device_memory>  Array;
    typedef typename Array::view                           View;

    cublasHandle_t handle;

    if(cublasCreate(&handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasCreate failed");
    }

    cusp::cublas::execution_policy cublas(handle);

    Array x(6);
    View view_x(x);

    x[0] =  7.0f;
    x[1] =  5.0f;
    x[2] =  4.0f;
    x[3] = -3.0f;
    x[4] =  0.0f;
    x[5] =  1.0f;

    ASSERT_EQUAL(cusp::blas::asum(cublas,x), 20.0f);

    ASSERT_EQUAL(cusp::blas::asum(cublas,view_x), 20.0f);

    if(cublasDestroy(handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasDestroy failed");
    }
}
DECLARE_NUMERIC_UNITTEST(TestCublasAsum);

template<typename ValueType>
void TestCublasAxpy(void)
{
    typedef cusp::array1d<ValueType, cusp::device_memory>  Array;
    typedef typename Array::view                           View;

    cublasHandle_t handle;

    if(cublasCreate(&handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasCreate failed");
    }

    cusp::cublas::execution_policy cublas(handle);

    Array x(4);
    Array y(4);

    x[0] =  7.0f;
    y[0] =  0.0f;
    x[1] =  5.0f;
    y[1] = -2.0f;
    x[2] =  4.0f;
    y[2] =  0.0f;
    x[3] = -3.0f;
    y[3] =  5.0f;

    cusp::blas::axpy(cublas, x, y, 2.0f);

    ASSERT_EQUAL(y[0],  14.0);
    ASSERT_EQUAL(y[1],   8.0);
    ASSERT_EQUAL(y[2],   8.0);
    ASSERT_EQUAL(y[3],  -1.0);

    View view_x(x);
    View view_y(y);

    cusp::blas::axpy(cublas, view_x, view_y, 2.0f);

    ASSERT_EQUAL(y[0],  28.0);
    ASSERT_EQUAL(y[1],  18.0);
    ASSERT_EQUAL(y[2],  16.0);
    ASSERT_EQUAL(y[3],  -7.0);

    if(cublasDestroy(handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasDestroy failed");
    }
}
DECLARE_NUMERIC_UNITTEST(TestCublasAxpy);

template<typename ValueType>
void TestCublasCopy(void)
{
    typedef cusp::array1d<ValueType, cusp::device_memory>  Array;
    typedef typename Array::view                           View;

    cublasHandle_t handle;

    if(cublasCreate(&handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasCreate failed");
    }

    cusp::cublas::execution_policy cublas(handle);

    Array x(4);

    x[0] =  7.0f;
    x[1] =  5.0f;
    x[2] =  4.0f;
    x[3] = -3.0f;

    {
        Array y(4, -1);
        cusp::blas::copy(cublas, x, y);
        ASSERT_EQUAL(x==y, true);
    }

    {
        Array y(4, -1);
        View view_x(x);
        View view_y(y);
        cusp::blas::copy(cublas, view_x, view_y);
        ASSERT_EQUAL(x==y, true);
    }

    if(cublasDestroy(handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasDestroy failed");
    }
}
DECLARE_NUMERIC_UNITTEST(TestCublasCopy);

template<typename ValueType>
void TestCublasDot(void)
{
    typedef cusp::array1d<ValueType, cusp::device_memory>  Array;
    typedef typename Array::view                           View;

    cublasHandle_t handle;

    if(cublasCreate(&handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasCreate failed");
    }

    cusp::cublas::execution_policy cublas(handle);

    Array x(6);
    Array y(6);

    x[0] =  7.0f;
    y[0] =  0.0f;
    x[1] =  5.0f;
    y[1] = -2.0f;
    x[2] =  4.0f;
    y[2] =  0.0f;
    x[3] = -3.0f;
    y[3] =  5.0f;
    x[4] =  0.0f;
    y[4] =  6.0f;
    x[5] =  4.0f;
    y[5] =  1.0f;

    ASSERT_EQUAL(cusp::blas::dot(cublas, x, y), -21.0f);

    View view_x(x);
    View view_y(y);
    ASSERT_EQUAL(cusp::blas::dot(cublas, view_x, view_y), -21.0f);

    if(cublasDestroy(handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasDestroy failed");
    }
}
DECLARE_REAL_UNITTEST(TestCublasDot);

template<typename ValueType>
void TestCublasNrm2(void)
{
    typedef cusp::array1d<ValueType, cusp::device_memory>  Array;
    typedef typename Array::view                           View;

    cublasHandle_t handle;

    if(cublasCreate(&handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasCreate failed");
    }

    cusp::cublas::execution_policy cublas(handle);

    Array x(6);

    x[0] =  7.0f;
    x[1] =  5.0f;
    x[2] =  4.0f;
    x[3] = -3.0f;
    x[4] =  0.0f;
    x[5] =  1.0f;

    ASSERT_EQUAL(cusp::blas::nrm2(cublas, x), 10.0f);

    ASSERT_EQUAL(cusp::blas::nrm2(cublas, View(x)), 10.0f);
}
DECLARE_NUMERIC_UNITTEST(TestCublasNrm2);

template<typename ValueType>
void TestCublasScal(void)
{
    typedef cusp::array1d<ValueType, cusp::device_memory>      Array;
    typedef typename Array::view                               View;

    cublasHandle_t handle;

    if(cublasCreate(&handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasCreate failed");
    }

    cusp::cublas::execution_policy cublas(handle);

    Array x(6);

    x[0] =  7.0f;
    x[1] =  5.0f;
    x[2] =  4.0f;
    x[3] = -3.0f;
    x[4] =  0.0f;
    x[5] =  4.0f;

    cusp::blas::scal(cublas, x, 4.0f);

    ASSERT_EQUAL(x[0],  28.0);
    ASSERT_EQUAL(x[1],  20.0);
    ASSERT_EQUAL(x[2],  16.0);
    ASSERT_EQUAL(x[3], -12.0);
    ASSERT_EQUAL(x[4],   0.0);
    ASSERT_EQUAL(x[5],  16.0);

    View v(x);
    cusp::blas::scal(cublas, v, 2.0f);

    ASSERT_EQUAL(x[0],  56.0);
    ASSERT_EQUAL(x[1],  40.0);
    ASSERT_EQUAL(x[2],  32.0);
    ASSERT_EQUAL(x[3], -24.0);
    ASSERT_EQUAL(x[4],   0.0);
    ASSERT_EQUAL(x[5],  32.0);

    if(cublasDestroy(handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasDestroy failed");
    }
}
DECLARE_NUMERIC_UNITTEST(TestCublasScal);

template<typename ValueType>
void TestCublasGemv(void)
{
    typedef cusp::array2d<ValueType, cusp::device_memory> Array2d;
    typedef cusp::array1d<ValueType, cusp::device_memory> Array1d;

    cublasHandle_t handle;

    if(cublasCreate(&handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasCreate failed");
    }

    cusp::cublas::execution_policy cublas(handle);

    Array2d A;
    Array1d x(9);
    Array1d y(9);

    cusp::gallery::poisson5pt(A, 3, 3);

    x[0] =  7.0f;
    x[1] =  5.0f;
    x[2] =  4.0f;
    x[3] = -3.0f;
    x[4] =  0.0f;
    x[5] =  4.0f;
    x[6] = -3.0f;
    x[7] =  0.0f;
    x[8] =  4.0f;

    cusp::blas::gemv(cublas, A, x, y);

    ASSERT_EQUAL(y[0],  26.0);
    ASSERT_EQUAL(y[1],   9.0);
    ASSERT_EQUAL(y[2],   7.0);
    ASSERT_EQUAL(y[3], -16.0);
    ASSERT_EQUAL(y[4],  -6.0);
    ASSERT_EQUAL(y[5],   8.0);
    ASSERT_EQUAL(y[6],  -9.0);
    ASSERT_EQUAL(y[7],  -1.0);
    ASSERT_EQUAL(y[8],  12.0);

    if(cublasDestroy(handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasDestroy failed");
    }
}
DECLARE_NUMERIC_UNITTEST(TestCublasGemv);

template<typename ValueType>
void TestCublasSymv(void)
{
    typedef cusp::array2d<ValueType, cusp::device_memory> Array2d;
    typedef cusp::array1d<ValueType, cusp::device_memory> Array1d;

    cublasHandle_t handle;

    if(cublasCreate(&handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasCreate failed");
    }

    cusp::cublas::execution_policy cublas(handle);

    Array2d A;
    Array1d x(9);
    Array1d y(9);

    cusp::gallery::poisson5pt(A, 3, 3);

    x[0] =  7.0f;
    x[1] =  5.0f;
    x[2] =  4.0f;
    x[3] = -3.0f;
    x[4] =  0.0f;
    x[5] =  4.0f;
    x[6] = -3.0f;
    x[7] =  0.0f;
    x[8] =  4.0f;

    cusp::blas::symv(cublas, A, x, y);

    ASSERT_EQUAL(y[0],  26.0);
    ASSERT_EQUAL(y[1],   9.0);
    ASSERT_EQUAL(y[2],   7.0);
    ASSERT_EQUAL(y[3], -16.0);
    ASSERT_EQUAL(y[4],  -6.0);
    ASSERT_EQUAL(y[5],   8.0);
    ASSERT_EQUAL(y[6],  -9.0);
    ASSERT_EQUAL(y[7],  -1.0);
    ASSERT_EQUAL(y[8],  12.0);

    if(cublasDestroy(handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasDestroy failed");
    }
}
DECLARE_REAL_UNITTEST(TestCublasSymv);

template<typename ValueType>
void TestCublasTrmv(void)
{
    typedef cusp::array2d<ValueType, cusp::device_memory> Array2d;
    typedef cusp::array1d<ValueType, cusp::device_memory> Array1d;

    cublasHandle_t handle;

    if(cublasCreate(&handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasCreate failed");
    }

    cusp::cublas::execution_policy cublas(handle);

    Array2d A;
    Array1d x(9);
    Array1d expected(9);

    cusp::gallery::poisson5pt(A, 3, 3);

    // set lower diagonal entries to zero
    for(int j = 0; j < 9; j++)
      for(int i = j + 1; i < 9; i++)
          A(i,j) = ValueType(0);

    x[0] =  7.0f;
    x[1] =  5.0f;
    x[2] =  4.0f;
    x[3] = -3.0f;
    x[4] =  0.0f;
    x[5] =  4.0f;
    x[6] = -3.0f;
    x[7] =  0.0f;
    x[8] =  4.0f;

    cusp::blas::gemv(cublas, A, x, expected);
    cusp::blas::trmv(cublas, A, x);

    ASSERT_ALMOST_EQUAL(x, expected);

    if(cublasDestroy(handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasDestroy failed");
    }
}
DECLARE_NUMERIC_UNITTEST(TestCublasTrmv);

template<typename ValueType>
void TestCublasTrsv(void)
{
    typedef cusp::array2d<ValueType, cusp::device_memory> Array2d;
    typedef cusp::array1d<ValueType, cusp::device_memory> Array1d;

    cublasHandle_t handle;

    if(cublasCreate(&handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasCreate failed");
    }

    cusp::cublas::execution_policy cublas(handle);

    Array2d A;
    Array1d x(9);

    cusp::gallery::poisson5pt(A, 3, 3);

    x[0] =  7.0f;
    x[1] =  5.0f;
    x[2] =  4.0f;
    x[3] = -3.0f;
    x[4] =  0.0f;
    x[5] =  4.0f;
    x[6] = -3.0f;
    x[7] =  0.0f;
    x[8] =  4.0f;

    Array1d b(x);

    cusp::blas::trsv(cublas, A, x);

    // check residual norm
    cusp::array1d<ValueType, cusp::device_memory> residual(x);
    cusp::blas::trmv(cublas, A, residual);
    cusp::blas::axpby(residual, b, residual, -1.0f, 1.0f);

    ASSERT_EQUAL(cusp::blas::nrm2(residual) < 1e-7, true);

    if(cublasDestroy(handle) != CUBLAS_STATUS_SUCCESS)
    {
      throw cusp::runtime_exception("cublasDestroy failed");
    }
}
DECLARE_NUMERIC_UNITTEST(TestCublasTrsv);


