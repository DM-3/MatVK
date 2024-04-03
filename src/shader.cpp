#include "MatVK/shader.hpp"
#include "MatVK/queueBase.hpp"
#include "MatVK/scalarSubres.hpp"
#include "MatVK/matrixSubres.hpp"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <filesystem>


namespace matvk
{

    Shader::Shader(QueueBase& queueBase, const std::shared_ptr<MatrixBase> dst) :
        _queueBase(queueBase), _outputSize({0, 0})
    {
        dst->record(*this);

        int index = 0;
        for (auto scalar : queueBase.scalars())
            _scalars.append(std::string(nameOfType(scalar->type()))
                + " scalar_" + std::to_string(index++) + ";\n");
    }

    void Shader::appendOutput(std::string seq) 
    {
        _output.append(seq);
    }

    void Shader::appendOutput(char c)
    {
        _output += c;
    }

    void Shader::addScalar(std::shared_ptr<ScalarSubres> scalar)
    {
        int index = std::find(_queueBase.scalars().begin(), _queueBase.scalars().end(),
                    scalar) - _queueBase.scalars().begin();
        if (index == _queueBase.scalars().size())
        {
            _queueBase.scalars().push_back(scalar);
            _scalars.append(std::string(nameOfType(scalar->type()))
                + " scalar_" + std::to_string(index) + ";\n");
        }

        _output.append("ubo.scalar_" + std::to_string(index));
    }

    void Shader::addMatrix(std::shared_ptr<MatrixSubres> matrix, 
        Size2D extents, Size2D offset, bool transposed)
    {
        if (_output.empty())
            addOutputMatrix(matrix, extents, offset, transposed);
        else
            addInputMatrix(matrix, extents, offset, transposed);
    }

    std::string Shader::compile()
    {
        std::string str =
            "#version 460\n"
            "#extension GL_KHR_shader_subgroup_shuffle : enable\n"
            "\n"            
            "#define SUB_SIZE " + std::to_string(VKB::subgroupSize()) + "\n"
            "\n"
            "layout(local_size_x = SUB_SIZE) in;\n"
            "\n"
            "layout(binding = 0) buffer ScalarBuffer\n"
            "{\n"
            + _scalars +
            "} ubo;\n"
            "\n"
            + _images +
            "\n"
            "void main()\n"
            "{\n"
            "    ivec2 outputCoord = ivec2(gl_WorkGroupID.xy) * SUB_SIZE;\n"
            "\n"
            + _multiplications + 
            "\n"
            "    outputCoord.x += int(gl_SubgroupInvocationID);\n"
            "    if (outputCoord.x >= " + std::to_string(_outputSize.x) + ")\n"
            "        return;\n"
            "\n"
            "    for(int i = 0; outputCoord.y < " + std::to_string(_outputSize.y) +
            " && i < SUB_SIZE; i++, outputCoord.y++) \n"
            "    {\n"
            + _loads +
            "\n"
            + _output +
            "\n        ));\n"
            "    }\n"
            "}\n";


        std::string name = "shader" + std::to_string(uint64_t(this) % 65536);
        
        std::ofstream compFile(name + ".comp");
        compFile << str;
        compFile.close();

        system(("glslangValidator --target-env vulkan1.3 " 
            + name + ".comp -o " + name + ".spv").c_str());

        std::filesystem::remove(name + ".comp");

        std::ifstream spvFile(name + ".spv");
        std::stringstream buffer;
        buffer << spvFile.rdbuf();
        spvFile.close();

        std::filesystem::remove(name + ".spv");

        return buffer.str();
    }

    void Shader::addOutputMatrix(std::shared_ptr<MatrixSubres> matrix, 
        Size2D extents, Size2D offset, bool transposed)
    {
        Size2D sExt = (transposed ? matrix->extents().yx() : matrix->extents()) - offset;
        _outputSize.x = sExt.x < extents.x ? sExt.x : extents.x;
        _outputSize.y = sExt.y < extents.y ? sExt.y : extents.y;
        

        // find matrix' binding index in the queue the shader is used in
        int bindIndex = std::find(_queueBase.matrices().begin(), _queueBase.matrices().end(),
                        matrix) - _queueBase.matrices().begin() + 1;
        if (bindIndex == _queueBase.matrices().size() + 1)  
            _queueBase.matrices().push_back(matrix);
        
        std::string matName = "mat_" + std::to_string(bindIndex);


        // record image binding
        _images.append("layout (set = 0, binding = " + std::to_string(bindIndex)
            + ", " + qualifierOfType(matrix->type()) + ") uniform "
            + nameOfType(matrix->type(), true) + "image2D "
            + matName + ";\n");


        // record output
        _output.append("\timageStore(" + matName + ", " 
            + matrixAccessPos(extents, offset, transposed) + ", " 
            + nameOfType(matrix->type(), true) + "vec4(\n");
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
                + ", " + qualifierOfType(matrix->type()) + ") uniform readonly "
                + nameOfType(matrix->type(), true) + "image2D "
                + matName + ";\n");


        // record element load
        std::string loadName = matName + "_load_" + std::to_string(extents.x) + "_"
            + std::to_string(extents.y) + "_" + std::to_string(offset.x) + "_"
            + std::to_string(offset.y) + "_" + std::to_string(transposed);
        
        if (_loads.find(loadName) == std::string::npos)
            _loads.append("\t" + std::string(nameOfType(matrix->type())) 
                + " " + loadName + " = imageLoad(" + matName + ", " 
                + matrixAccessPos(extents, offset, transposed) + ")."
                + qualifierOfType(matrix->type(), true) + ";\n");


        // record output
        _output.append(loadName);
    }

    std::string Shader::matrixAccessPos(Size2D extents, Size2D offset, bool transposed)
    {
        std::string str("(outputCoord");

        if (extents.x < _outputSize.x || extents.y < _outputSize.y)
            str += " \% ivec2("
                + std::to_string(extents.x) + ", "
                + std::to_string(extents.y) + ")";

        if (offset.x > 0 || offset.y > 0)
            str += " + ivec2(" 
                + std::to_string(offset.x) + ", "
                + std::to_string(offset.y) + ")";

        str.append(transposed ? ").yx" : ")");

        return str;
    }

    Size2D Shader::dispatchSize()
    {
        return Size2D(
            _outputSize.x / VKB::subgroupSize() + (_outputSize.x % VKB::subgroupSize() ? 1 : 0),
            _outputSize.y / VKB::subgroupSize() + (_outputSize.y % VKB::subgroupSize() ? 1 : 0));
    }

};
