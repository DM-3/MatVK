#include "MatVK/matvk.hpp"

int main() 
{
    using namespace matvk;

    printHello();

    Size2D size(32, 32);

    Matrix<float> mA(size), mB(size);
	Matrix<int> mC(64, 64), mD(64, 64);

	Scalar<float> sA;
	Scalar<int> sB;

	Queue Q = {
		mB = mA + sA * mB,
		mC = mD * sB
	};
    Q.endRecording();
    Q.execute();
    Q.waitFinished();

    return 0;
}
