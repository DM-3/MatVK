#include <iostream>
#include "MatVK/matvk.hpp"



namespace matvk
{
    void printHello() {
        std::cout << "Hello MatVK" << std::endl;
    }




//---- non-template class definitions

    // accessible classes

    Queue::Queue() {}

    Queue::Queue(std::initializer_list<Assignment> assignments) {}

    Queue& Queue::operator<<(Assignment assignment) { return *this; }

    Queue& Queue::operator<<(std::initializer_list<Assignment> assignments) { return *this; }

    void Queue::endRecording() {}

    void Queue::execute(bool immediateWait) {}

    void Queue::waitFinished() {}



    // inaccessible classes

    ExpressionBase::ExpressionBase(std::vector<std::shared_ptr<ExpressionBase>> operands) :
        _operands(operands)
    {}


    MatrixBase::MatrixBase() : ExpressionBase({}) {}

    void MatrixBase::record() {}


    ScalarBase::ScalarBase() : ExpressionBase({}) {}

    void ScalarBase::record() {}
    

    ConstantBase::ConstantBase() : ExpressionBase({}) {}

    void ConstantBase::record() {}
    

    InfixOperation::InfixOperation(std::shared_ptr<ExpressionBase> left, 
        std::shared_ptr<ExpressionBase> right, char opSymbol) : 
        ExpressionBase({ left, right }), _opSymbol(opSymbol) 
    {}

    void InfixOperation::record() {}
    

    PrefixOperation::PrefixOperation(
        std::vector<std::shared_ptr<ExpressionBase>> operands, std::string name) : 
        ExpressionBase(operands), _name(name) 
    {}

    void PrefixOperation::record() {}
    

    DotOperation::DotOperation(std::shared_ptr<ExpressionBase> left, 
        std::shared_ptr<ExpressionBase> right) : 
        ExpressionBase({ left, right }) 
    {}

    void DotOperation::record() {}
    

    Assignment::Assignment(std::shared_ptr<ExpressionBase> src, std::shared_ptr<ExpressionBase> dst) :
        _src(src), _dst(dst)
    {}
};
