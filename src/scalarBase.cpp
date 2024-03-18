#include "MatVK/matvk.hpp"

namespace matvk
{

    ScalarBase::ScalarBase() : ExpressionBase({}) 
    {}

    void ScalarBase::record(Shader& shader) const 
    {}
    
};
