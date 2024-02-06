#include <iostream>
#include "MatVK/matvk.hpp"

#include "MatVK/matrixSubres.hpp"



namespace matvk
{
    void printHello() {
        std::cout << "Hello MatVK" << std::endl;
    }




//---- non-template class definitions

    // accessible classes

    Size2D::Size2D(uint32_t width, uint32_t height) :
        x(width), y(height)
    {}

    Size2D Size2D::operator+(const Size2D other)
    {
        return { x + other.x, y + other.y };
    }

    Size2D Size2D::operator-(const Size2D other)
    {
        return { x - other.x, y - other.y };
    }

};
