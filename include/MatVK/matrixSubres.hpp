#pragma once

#include "matvk.hpp"
#include "vulkanBase.hpp"


namespace matvk
{

    class MatrixSubres
    {
    public:
        MatrixSubres(Size2D size);

    private:
        Size2D _size;
    };

};
