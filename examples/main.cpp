#include "MatVK/matvk.hpp"
#include <iostream>


int main() 
{
    using namespace matvk;

    //hardcodeSubgroupSize(16);

    Size2D size(8, 8);

    Matrix<float> mA(size), mB(size);
	Matrix<int> mC(64, 64), mD(64, 64);

	Scalar<float> sA;
	Scalar<int> sB;

    std::vector<float> data(mA.nElems(), 1.0f);
    mA << data;
    mB << data;

	Queue Q = {
		mB           = mA.T() + sA * mB,
		mC.T()(size) = mD[size](size) * sB
	};
    Q.execute(true);

    mB >> data;

    return 0;
}
