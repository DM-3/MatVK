#include "MatVK/matvk.hpp"

namespace matvk
{

    ConstantBase::ConstantBase() : ExpressionBase({}) {}

    void ConstantBase::record(Shader& shader) const {}

};
