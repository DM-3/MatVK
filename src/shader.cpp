#include "MatVK/shader.hpp"
#include "MatVK/queueBase.hpp"
#include "MatVK/matrixSubres.hpp"


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
    {
        _outputSize = extents;


        // find matrix' binding index in the queue the shader is used in
        int bindIndex = std::find(_queueBase.matrices().begin(), _queueBase.matrices().end(),
                        matrix) - _queueBase.matrices().begin() + 1;
        if (bindIndex == _queueBase.matrices().size() + 1)  
            _queueBase.matrices().push_back(matrix);
        
        std::string matName = "mat_" + std::to_string(bindIndex);


        // record image binding
        _images.append("layout (set = 0, binding = " + std::to_string(bindIndex)
            + ", " + qualifierOfType(matrix->type()) + ") uniform image2D"
            + matName + ";\n");


        // record output
        _output.append("imageStore(" + matName + ", " + matrixAccessPos(
            extents, offset, transposed) + ", vec4(\n");
    }

    void Shader::addInputMatrix(std::shared_ptr<MatrixSubres> matrix, 
        Size2D extents, Size2D offset, bool transposed)
    {
        // find matrix' binding index in the queue the shader is used in
        int bindIndex = std::find(_queueBase.matrices().begin(), _queueBase.matrices().end(),
                        matrix) - _queueBase.matrices().begin() + 1;
        if (bindIndex == _queueBase.matrices().size() + 1)
            _queueBase.matrices().push_back(matrix);
        
        std::string matName = "mat_" + std::to_string(bindIndex);


        // record image binding
        if (_images.find(matName) == std::string::npos)
            _images.append("layout (set = 0, binding = " + std::to_string(bindIndex)
                + ", " + qualifierOfType(matrix->type()) + ") uniform readonly image2D"
                + matName + ";\n");


        // record element load
        std::string loadName = matName + "_load_" + std::to_string(extents.x) + "_"
            + std::to_string(extents.y) + "_" + std::to_string(offset.x) + "_"
            + std::to_string(offset.y) + "_" + std::to_string(transposed);
        
        if (_loads.find(loadName) == std::string::npos)
            _loads.append(std::string(nameOfType(matrix->type())) + " "
                + loadName + " = imageLoad(" + matName + ", " 
                + matrixAccessPos(extents, offset, transposed) + ")."
                + qualifierOfType(matrix->type(), true) + ";\n");


        // record output
        _output.append(loadName);
    }

    std::string Shader::matrixAccessPos(Size2D extents, Size2D offset, bool transposed)
    {
        return "vec2()";
    }

};
