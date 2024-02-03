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
    class QueueBase;




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
        Queue(std::initializer_list<Assignment> assignments);
        Queue& operator<<(Assignment assignment);
        Queue& operator<<(std::initializer_list<Assignment> assignments);
        void endRecording();
        void execute(bool immediateWait = true);
        void waitFinished();

    private:
        std::shared_ptr<QueueBase> _base;
    };


    // inaccessible classes

    class ExpressionBase
    {
    public:
        ExpressionBase(std::vector<std::shared_ptr<ExpressionBase>> operands);
        virtual void record() = 0;
    
    protected:
        std::vector<std::shared_ptr<ExpressionBase>> _operands;
    };

    template<element E>
    class Expression
    {
    public:
        friend Expression<E> operator+ <>(const Expression<E> left, const Expression<E> right);
        friend Expression<E> operator- <>(const Expression<E> left, const Expression<E> right);
        friend Expression<E> operator* <>(const Expression<E> left, const Expression<E> right);
        friend Expression<E> operator/ <>(const Expression<E> left, const Expression<E> right);

    protected:
        Expression(std::shared_ptr<ExpressionBase> base);
        std::shared_ptr<ExpressionBase> _base;
    };


    class MatrixBase : public ExpressionBase
    {
    public:
        void record();

    private:
        template<element E> friend class Matrix;
        MatrixBase();
    };

    class ScalarBase : public ExpressionBase
    {
    public:
        void record();

    private:
        template<element E> friend class Scalar;
        ScalarBase();
    };

    class ConstantBase : public ExpressionBase
    {
    public:
        void record();

    private:
        template<element E> friend class Constant;
        ConstantBase();
    };


    class InfixOperation : public ExpressionBase
    {
    public:
        InfixOperation(std::shared_ptr<ExpressionBase> left, 
            std::shared_ptr<ExpressionBase> right, char opSymbol);
        void record();

    private:
        char _opSymbol;
    };

    class PrefixOperation : public ExpressionBase
    {
    public:
        PrefixOperation(std::vector<std::shared_ptr<ExpressionBase>> operands, std::string name);
        void record();

    private:
        std::string _name;
    };

    class DotOperation : public ExpressionBase
    {
    public:
        DotOperation(std::shared_ptr<ExpressionBase> left, std::shared_ptr<ExpressionBase> right);
        void record();
    };


    class Assignment
    {
    private:
        template<element E> friend class Matrix;
        Assignment(std::shared_ptr<ExpressionBase> src, std::shared_ptr<ExpressionBase> dst);

        std::shared_ptr<ExpressionBase> _src;
        std::shared_ptr<ExpressionBase> _dst;
    };




//---- template class definitions

    // accessible classes

    template<element E>
    Matrix<E>::Matrix() : Expression<E>(std::static_pointer_cast<ExpressionBase>
        (std::shared_ptr<MatrixBase>(new MatrixBase())))
    {}


    template<element E>
    Scalar<E>::Scalar() : Expression<E>(std::static_pointer_cast<ExpressionBase>
        (std::shared_ptr<ScalarBase>(new ScalarBase())))
    {}


    template<element E>
    Constant<E>::Constant() : Expression<E>(std::static_pointer_cast<ExpressionBase>
        (std::shared_ptr<ConstantBase>(new ConstantBase())))
    {}



    // inaccessible classes

    template<element E>
    Expression<E>::Expression(std::shared_ptr<ExpressionBase> base) :
        _base(base)
    {}




//---- template function definitions

    template<element E>
    Expression<E> operator+(const Expression<E> left, const Expression<E> right) 
    {
        return Expression<E>(std::static_pointer_cast<ExpressionBase>(
            std::make_shared<InfixOperation>(left.base, right.base, '+')));
    }

    template<element E>
    Expression<E> operator-(const Expression<E> left, const Expression<E> right) 
    {
        return Expression<E>(std::static_pointer_cast<ExpressionBase>(
            std::make_shared<InfixOperation>(left.base, right.base, '-')));
    }

    template<element E>
    Expression<E> operator*(const Expression<E> left, const Expression<E> right) 
    {
        return Expression<E>(std::static_pointer_cast<ExpressionBase>(
            std::make_shared<InfixOperation>(left.base, right.base, '*')));
    }

    template<element E>
    Expression<E> operator/(const Expression<E> left, const Expression<E> right) 
    {
        return Expression<E>(std::static_pointer_cast<ExpressionBase>(
            std::make_shared<InfixOperation>(left.base, right.base, '/')));
    }

};
