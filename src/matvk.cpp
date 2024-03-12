#include <iostream>
#include "MatVK/matvk.hpp"

#include "MatVK/matrixSubres.hpp"



namespace matvk
{

//---- types

    template<>
    ElemType enumerateType<float>() { return ElemType::float_32; }

    template<>
    ElemType enumerateType<double>() { return ElemType::float_64; }

    template<>
    ElemType enumerateType<int>() { return ElemType::int_32; }

    size_t sizeofType(ElemType elemType)
    {
        switch(elemType) {
            case ElemType::float_32:    return 4;
            case ElemType::float_64:    return 8;
            case ElemType::int_32:      return 4;
            default: return 0;
        }
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
