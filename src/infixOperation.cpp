#include "MatVK/matvk.hpp"
#include "MatVK/shader.hpp"


namespace matvk
{

    InfixOperation::InfixOperation(std::shared_ptr<ExpressionBase> left, 
        std::shared_ptr<ExpressionBase> right, char opSymbol) : 
        ExpressionBase({ left, right }), _opSymbol(opSymbol) 
    {}

    void InfixOperation::record(Shader& shader) const 
    {
        shader.appendOutput("(");
        _operands[0]->record(shader);
        shader.appendOutput(std::string(" ") + _opSymbol + " ");
        _operands[1]->record(shader);
        shader.appendOutput(")");
    }

};
