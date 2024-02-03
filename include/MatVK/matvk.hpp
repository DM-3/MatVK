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




//---- class declarations

    // accessible classes

    template<element E>
    class Matrix : public Expression<E>
    {
    public:
        Matrix();
    };

    template<element E>
    class Scalar : public Expression<E>
    {
    public:
        Scalar();
    };

    template<element E>
    class Constant : public Expression<E>
    {
    public:
        Constant();
    };

    class Queue 
    {
    public:
        Queue();
    };


    // inaccessible classes

    class ExpressionBase
    {
    public:
        ExpressionBase();
    };

    template<element E>
    class Expression
    {
    protected:
        Expression();
    };


    class MatrixBase : public ExpressionBase
    {
    private:
        template<element E> friend class Matrix;
        MatrixBase();
    };

    class ScalarBase : public ExpressionBase
    {
    private:
        template<element E> friend class Scalar;
        ScalarBase();
    };

    class ConstantBase : public ExpressionBase
    {
    private:
        template<element E> friend class Constant;
        ConstantBase();
    };


    class InfixOperation : public ExpressionBase
    {
    public:
        InfixOperation();
    };

    class PrefixOperation : public ExpressionBase
    {
    public:
        PrefixOperation();
    };

    class DotOperation : public ExpressionBase
    {
    public:
        DotOperation();
    };


    class Assignment
    {
    private:
        template<element E> friend class Matrix;
        Assignment();
    };




//---- class method definitions

    // accessible classes

    template<element E>
    Matrix<E>::Matrix() {}

    template<element E>
    Scalar<E>::Scalar() {}

    template<element E>
    Constant<E>::Constant() {}

    Queue::Queue() {}


    // inaccessible classes

    ExpressionBase::ExpressionBase() {}

    template<element E>
    Expression<E>::Expression() {}


    MatrixBase::MatrixBase() {}

    ScalarBase::ScalarBase() {}

    ConstantBase::ConstantBase() {}


    InfixOperation::InfixOperation() {}

    PrefixOperation::PrefixOperation() {}

    DotOperation::DotOperation() {}


    Assignment::Assignment() {}
};
