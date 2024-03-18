#include "MatVK/matvk.hpp"

namespace matvk
{

    InfixOperation::InfixOperation(std::shared_ptr<ExpressionBase> left, 
        std::shared_ptr<ExpressionBase> right, char opSymbol) : 
        ExpressionBase({ left, right }), _opSymbol(opSymbol) 
    {}

    void InfixOperation::record(Shader& shader) const {}

};
