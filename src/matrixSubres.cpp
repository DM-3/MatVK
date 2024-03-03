#include "MatVK/matrixSubres.hpp"
#include <iostream>

namespace matvk
{

    MatrixSubres::MatrixSubres(Size2D size) :
        _size(size)
    {
        std::cout << VKB::device() << std::endl;
    }

};
