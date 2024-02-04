#include "MatVK/matvk.hpp"

int main() 
{
    using namespace matvk;

    printHello();

    Matrix<float> mA, mB;
	Matrix<int> mC, mD;

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
