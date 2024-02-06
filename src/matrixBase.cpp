#include "MatVK/matvk.hpp"
#include "MatVK/matrixSubres.hpp"

namespace matvk
{

    MatrixBase::MatrixBase(Size2D size) : 
        ExpressionBase({}), _extents(size), _offset(Size2D(0, 0)), 
        _sub(std::make_shared<MatrixSubres>(size)) 
    {}

    void MatrixBase::record() {}

};
