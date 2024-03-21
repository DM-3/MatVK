#pragma once

#include <type_traits>
#include <concepts>
#include <memory>
#include <vector>
#include <string>
#include <initializer_list>
#include <inttypes.h>
#include <limits>



namespace matvk 
{

    void hardcodeSubgroupSize(uint32_t size);




//---- types

    template<typename T>
    concept element =
        std::is_same_v<float,   T> ||
        std::is_same_v<double,  T> ||
        std::is_same_v<int,     T>;

    enum class ElemType
    {
        float_32,
        float_64,
        int_32
    };

    template<element E>
    ElemType enumerateType();

    size_t sizeofType(ElemType elemType);




//---- class forward declarations

    // accessible classes

    struct Size2D;
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


    // hidden classes

    class MatrixSubres;
    class ScalarSubres;
    class Shader;




//---- class declarations

    // accessible classes

    struct Size2D
    {
        Size2D(uint32_t width, uint32_t height);
        Size2D operator+(const Size2D other);
        Size2D operator-(const Size2D other);
        Size2D yx();

        uint32_t x;
        uint32_t y;
    };

    template<element E>
    class Matrix : public Expression<E>
    {
    public:
        Matrix(Size2D size);
        Matrix(uint32_t cols, uint32_t rows);
        Matrix(const Matrix<E>& other);
        Matrix(std::shared_ptr<MatrixBase> base);
    
        size_t nElems() const;

        void operator<<(std::vector<E>& src);
        void operator>>(std::vector<E>& dst);

        Matrix operator() (Size2D extents);
        Matrix operator() (uint32_t cols, uint32_t rows);
        Matrix operator[] (Size2D offset);
        Matrix T();

        Assignment operator=(const Expression<E>& src);
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
        Queue& operator<<(std::initializer_list<Assignment> assignments);
        Queue& operator<<(Assignment assignment);
        bool isRecording();
        void endRecording();
        void execute(bool immediateWait = true);
        void waitFinished();

    private:
        bool _recording;
        std::shared_ptr<QueueBase> _base;
    };


    // inaccessible classes

    class ExpressionBase
    {
    public:
        ExpressionBase(std::vector<std::shared_ptr<ExpressionBase>> operands);
        virtual void record(Shader& shader) const = 0;
    
    protected:
        std::vector<std::shared_ptr<ExpressionBase>> _operands;
    };

    template<element E>
    class Expression
    {
        friend class Matrix<E>;
        friend class Scalar<E>;
        friend class Constant<E>;

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
        size_t nElems() const;

        void write(void* src);
        void read(void* dst);

        std::shared_ptr<MatrixBase> resize(Size2D extents);
        std::shared_ptr<MatrixBase> offset(Size2D offset);
        std::shared_ptr<MatrixBase> transpose();

        void record(Shader& shader) const;

    private:
        template<element E> friend class Matrix;
        MatrixBase(ElemType elemType, Size2D size);

        std::shared_ptr<MatrixSubres> _sub;
        Size2D _extents;
        Size2D _offset;
        uint8_t _transpose = 0;
    };

    class ScalarBase : public ExpressionBase
    {
    public:
        void record(Shader& shader) const;

    private:
        template<element E> friend class Scalar;
        ScalarBase(ElemType elemType);

        std::shared_ptr<ScalarSubres> _sub;
    };

    class ConstantBase : public ExpressionBase
    {
    public:
        void record(Shader& shader) const;

    private:
        template<element E> friend class Constant;
        ConstantBase();
    };


    class InfixOperation : public ExpressionBase
    {
    public:
        InfixOperation(std::shared_ptr<ExpressionBase> left, 
            std::shared_ptr<ExpressionBase> right, char opSymbol);
        void record(Shader& shader) const;

    private:
        char _opSymbol;
    };

    class PrefixOperation : public ExpressionBase
    {
    public:
        PrefixOperation(std::vector<std::shared_ptr<ExpressionBase>> operands, std::string name);
        void record(Shader& shader) const;

    private:
        std::string _name;
    };

    class DotOperation : public ExpressionBase
    {
    public:
        DotOperation(std::shared_ptr<ExpressionBase> left, std::shared_ptr<ExpressionBase> right);
        void record(Shader& shader) const;
    };


    class Assignment
    {
    public:
        const std::shared_ptr<ExpressionBase> getDestination();
        const std::shared_ptr<ExpressionBase> getSource();

    private:
        template<element E> friend class Matrix;
        Assignment(std::shared_ptr<ExpressionBase> src, std::shared_ptr<ExpressionBase> dst);

        std::shared_ptr<ExpressionBase> _src;
        std::shared_ptr<ExpressionBase> _dst;
    };




//---- template class definitions

    // accessible classes

    template<element E>
    Matrix<E>::Matrix(Size2D size) : Expression<E>(std::static_pointer_cast<ExpressionBase>
        (std::shared_ptr<MatrixBase>(new MatrixBase(enumerateType<E>(), size))))
    {}

    template<element E>
    Matrix<E>::Matrix(uint32_t cols, uint32_t rows) : Matrix(Size2D(cols, rows))
    {}

    template<element E>
    Matrix<E>::Matrix(const Matrix<E>& other) : Expression<E>(
        std::static_pointer_cast<ExpressionBase>(std::shared_ptr<MatrixBase>
        (new MatrixBase(*std::static_pointer_cast<MatrixBase>(other._base)))))
    {}

    template<element E>
    Matrix<E>::Matrix(std::shared_ptr<MatrixBase> base) : 
        Expression<E>(std::static_pointer_cast<ExpressionBase>(base))
    {}

    template<element E>
    size_t Matrix<E>::nElems() const
    {
        return std::static_pointer_cast<MatrixBase>(this->_base)->nElems();
    }

    template<element E>
    void Matrix<E>::operator<<(std::vector<E>& src) 
    { 
        if (src.size() < nElems())
            src.resize(nElems());
        
        return std::static_pointer_cast<MatrixBase>(this->_base)->
            write(static_cast<void*>(src.data()));
    }

    template<element E>
    void Matrix<E>::operator>>(std::vector<E>& dst) 
    { 
        if (dst.size() < nElems())
            dst.resize(nElems());
    
        return std::static_pointer_cast<MatrixBase>(this->_base)->
            read(static_cast<void*>(dst.data()));
    }

    template<element E>
    Matrix<E> Matrix<E>::operator() (Size2D extents) 
    { 
        return Matrix<E>(std::static_pointer_cast<MatrixBase>(this->_base)->resize(extents));
    }

    template<element E>
    Matrix<E> Matrix<E>::operator() (uint32_t cols, uint32_t rows) 
    { 
        return operator()(Size2D(cols, rows));
    }

    template<element E>
    Matrix<E> Matrix<E>::operator[] (Size2D offset) 
    { 
        return Matrix<E>(std::static_pointer_cast<MatrixBase>(this->_base)->offset(offset));
    }

    template<element E>
    Matrix<E> Matrix<E>::T() 
    { 
        return Matrix<E>(std::static_pointer_cast<MatrixBase>(this->_base)->transpose());
    }

    template<element E>
    Assignment Matrix<E>::operator=(const Expression<E>& src)
    {
        return Assignment(src._base, this->_base);
    }


    template<element E>
    Scalar<E>::Scalar() : Expression<E>(std::static_pointer_cast<ExpressionBase>
        (std::shared_ptr<ScalarBase>(new ScalarBase(enumerateType<E>()))))
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
            std::make_shared<InfixOperation>(left._base, right._base, '+')));
    }

    template<element E>
    Expression<E> operator-(const Expression<E> left, const Expression<E> right) 
    {
        return Expression<E>(std::static_pointer_cast<ExpressionBase>(
            std::make_shared<InfixOperation>(left._base, right._base, '-')));
    }

    template<element E>
    Expression<E> operator*(const Expression<E> left, const Expression<E> right) 
    {
        return Expression<E>(std::static_pointer_cast<ExpressionBase>(
            std::make_shared<InfixOperation>(left._base, right._base, '*')));
    }

    template<element E>
    Expression<E> operator/(const Expression<E> left, const Expression<E> right) 
    {
        return Expression<E>(std::static_pointer_cast<ExpressionBase>(
            std::make_shared<InfixOperation>(left._base, right._base, '/')));
    }

};
