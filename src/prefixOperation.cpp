#include "MatVK/matvk.hpp"

namespace matvk
{

    PrefixOperation::PrefixOperation(
        std::vector<std::shared_ptr<ExpressionBase>> operands, std::string name) : 
        ExpressionBase(operands), _name(name) 
    {}

    void PrefixOperation::record(Shader& shader) const {}
    
};
