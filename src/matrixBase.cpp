#include "MatVK/matvk.hpp"
#include "MatVK/matrixSubres.hpp"

namespace matvk
{

    MatrixBase::MatrixBase(Size2D size) : 
        ExpressionBase({}), _extents(size), _offset(Size2D(0, 0)), 
        _sub(std::make_shared<MatrixSubres>(size)) 
    {}

    size_t MatrixBase::nElems() const
    {
        return _extents.x * _extents.y;
    }

    void MatrixBase::write(void* src, size_t elemSize)
    {}

    void MatrixBase::read(void* dst, size_t elemSize)
    {}

    std::shared_ptr<MatrixBase> MatrixBase::resize(Size2D extents)
    {
        auto newMat = std::make_shared<MatrixBase>(*this);
        newMat->_extents = extents;
        return newMat;
    }

    std::shared_ptr<MatrixBase> MatrixBase::offset(Size2D offset)
    {
        auto newMat = std::make_shared<MatrixBase>(*this);
        newMat->_offset = _offset + offset;
        return newMat;
    }

    std::shared_ptr<MatrixBase> MatrixBase::transpose()
    {
        auto newMat = std::make_shared<MatrixBase>(*this);
        newMat->_extents = Size2D(_extents.y, _extents.x);
        newMat->_offset = Size2D(_offset.y, _offset.x);
        newMat->_transpose ^= 1;
        return newMat;
    }

    void MatrixBase::record() 
    {}

};
