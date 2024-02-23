#include "MatVK/matvk.hpp"
#include <iostream>

int main() 
{
    using namespace matvk;

    printHello();

    Size2D size(32, 32);

    Matrix<float> mA(size), mB(size);
	Matrix<int> mC(64, 64), mD(64, 64);

	Scalar<float> sA;
	Scalar<int> sB;

    std::vector<float> data = { 1.0 };
    mA << data;
    mB << data;

	Queue Q = {
		mB           = mA.T() + sA * mB,
		mC.T()(size) = mD[size](size) * sB
	};
    Q.endRecording();
    Q.execute();
    Q.waitFinished();

    mB >> data;

    return 0;
}
