#include "MatVK/matvk.hpp"

namespace matvk
{

    Assignment::Assignment(std::shared_ptr<ExpressionBase> src, std::shared_ptr<ExpressionBase> dst) :
        _src(src), _dst(dst)
    {}

};
