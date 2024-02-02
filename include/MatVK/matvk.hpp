#pragma once

#include <type_traits>
#include <concepts>
#include <memory>
#include <vector>
#include <initializer_list>

#include "vulkan/vulkan.hpp"



namespace matvk 
{
    void printHello();




//---- types

    template<typename T>
    concept element =
        std::is_same_v<float,   T> ||
        std::is_same_v<double,  T> ||
        std::is_same_v<int,     T>;

    


//---- class forward declarations

    // accessible classes
    template<element E> class Matrix;
    template<element E> class Scalar;
    template<element E> class Constant;
    class Queue;


    // inaccessible classes
    class ExpressionBase;
    template<element E> class Expression;

    class MatrixBase;
    class ScalarBase;
    class ConstantBase;

    class InfixOperation;
    class PrefixOperation;
    class DotOperation;

    class Assignment;
};
