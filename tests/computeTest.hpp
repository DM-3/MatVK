#include "MatVK/matvk.hpp"
#include "gtest/gtest.h"
#include "Eigen/Dense"
#include <random>
#include <tuple>


namespace matvk
{
    namespace tests
    {

        template<element E>
        void populateRandom(std::vector<E>& data, size_t size);

        template<>
        void populateRandom(std::vector<float>& data, size_t size)
        {
            if (data.size() != size) data.resize(size);
            for (auto& e : data)
                e = float(rand()) / RAND_MAX * 100.f - 50.f;
        }

        template<>
        void populateRandom(std::vector<double>& data, size_t size)
        {
            if (data.size() != size) data.resize(size);
            for (auto& e : data)
                e = double(rand()) / RAND_MAX * 1000. - 500.;
        }

        template<>
        void populateRandom(std::vector<int>& data, size_t size)
        {
            if (data.size() != size) data.resize(size);
            for (auto& e : data)
            {
                e = rand() % 1000 - 500;
                e += e ? 0 : 1;             // avoid integer division by zero
            }
        }


        template<element E>
        void compareMatrices(Matrix<E> v, Eigen::Matrix<E, -1, -1> e, double accuracy)
        {
            // compare sizes
            ASSERT_EQ(v.size().y, e.rows()) << " number of rows";
            ASSERT_EQ(v.size().x, e.cols()) << " number of columns";

            // read matvk matrix
            std::vector<E> vec;
            v >> vec;

            // iteratively compare elements
            for (int y = 0; y < v.size().y; y++)
                for (int x = 0; x < v.size().x; x++)
                {
                    E actual = vec[y * v.size().x + x];
                    E expected = e(y, x);
                    EXPECT_NEAR(actual, expected, abs(expected * accuracy))
                        << "...at coordinate (" << x << ", " << y << ")\n";
                }
        }


        template<element E>
        class ComputeTest
        {
        protected:
            using EigMat = Eigen::Matrix<E, Eigen::Dynamic, Eigen::Dynamic>;

            ComputeTest(Size2D size) :
                _size(size), _vA(size), _vB(size), _vC(size)
            {
                std::vector<E> data;

                populateRandom(data, _vA.nElems());
                _eA = Eigen::Map<EigMat>(data.data(), size.x, size.y);
                _eA.transposeInPlace();
                _vA << data;

                populateRandom(data, _vA.nElems());
                _eB = Eigen::Map<EigMat>(data.data(), size.x, size.y);
                _eB.transposeInPlace();
                _vB << data;
            }

            Size2D      _size;
            Queue       _queue;
            EigMat      _eA;
            Matrix<E>   _vA;
            EigMat      _eB;
            Matrix<E>   _vB;
            EigMat      _eC;
            Matrix<E>   _vC;
        };


        template<element E>
        class ComputeTestT : 
            public testing::Test, 
            public ComputeTest<E>
        {
        protected:
            ComputeTestT() :
                ComputeTest<E>(Size2D(32, 32))
            {}
        };


        class ComputeTestP : 
            public testing::TestWithParam<std::tuple<size_t, size_t>>,
            public ComputeTest<float>
        {
        protected:
            ComputeTestP() :
                ComputeTest(Size2D(
                    std::get<0>(GetParam()), 
                    std::get<1>(GetParam())
                ))
            {}
        };

    };
};
