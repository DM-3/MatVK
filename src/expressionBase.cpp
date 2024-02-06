#include "MatVK/matvk.hpp"

namespace matvk
{

    ExpressionBase::ExpressionBase(std::vector<std::shared_ptr<ExpressionBase>> operands) :
        _operands(operands)
    {}

};
