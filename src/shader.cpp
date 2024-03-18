#include "MatVK/shader.hpp"


namespace matvk
{

    Shader::Shader(QueueBase& queueBase, const std::shared_ptr<MatrixBase> dst) :
        _queueBase(queueBase), _outputSize({0, 0})
    {
        dst->record(*this);
    }

    void Shader::appendOutput(std::string seq) 
    {}

    void Shader::addScalar(std::shared_ptr<ScalarSubres> scalar)
    {}

    void Shader::addMatrix(std::shared_ptr<MatrixSubres> matrix, 
        Size2D extents, Size2D offset, bool transposed)
    {
        if (_output.empty())
            addOutputMatrix(matrix, extents, offset, transposed);
        else
            addInputMatrix(matrix, extents, offset, transposed);
    }

    std::string Shader::assemble()
    {
        return std::string();
    }

    void Shader::addOutputMatrix(std::shared_ptr<MatrixSubres> matrix, 
        Size2D extents, Size2D offset, bool transposed)
    {}

    void Shader::addInputMatrix(std::shared_ptr<MatrixSubres> matrix, 
        Size2D extents, Size2D offset, bool transposed)
    {}

};
