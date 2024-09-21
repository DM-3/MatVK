#include "MatVK/matvk.hpp"
#include <iostream>


template <typename T>
void printMat(std::vector<T> data, uint32_t cols, uint32_t rows);

int main() 
{
    using namespace matvk;

    Size2D size(8, 8);

    Matrix<float> mA(1, 8), mB(size);
	Matrix<int> mC(64, 64), mD(64, 64);

	Scalar<float> sA = 0.0f;
	Scalar<int> sB = 5;

    std::vector<float> data(mA.nElems(), 1.0f);
    mA << data;
    mB << data;

	Queue Q = {
		//mB(6, 6) = mA.T() + sA * mB,
        mB = mB * sA,
        mB(7, 5) = mA + mB,
        mC.T()   = mD[size](size) * sB
	};
    Q.execute(true);

    mB >> data;
    printMat(data, size.x, size.y);

    return 0;
}

template <typename T>
void printMat(std::vector<T> data, uint32_t cols, uint32_t rows)
{
    std::cout.precision(3);
    std::cout << "\n" << std::fixed;

    for (uint32_t m = 0; m < rows; m++)
    {
        std::cout << "[ ";
        for (uint32_t n = 0; n < cols; n++)
        {
            if (data[cols * m + n] != 0)
                std::cout << data[cols * m + n];
            else
                std::cout << "0";
            std::cout << "\t";
        }
        std::cout << "]\n";
    }

    std::cout << std::endl;
}
