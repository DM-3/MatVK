#include "MatVK/matvk.hpp"

namespace matvk
{

    DotOperation::DotOperation(std::shared_ptr<ExpressionBase> left, 
        std::shared_ptr<ExpressionBase> right) : 
        ExpressionBase({ left, right }) 
    {}

    void DotOperation::record() {}

};
