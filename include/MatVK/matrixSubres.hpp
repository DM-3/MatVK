#pragma once

#include "matvk.hpp"
#include "vulkan/vulkan.hpp"


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
