#pragma once

#include "MatVK/matvk.hpp"


namespace matvk
{

    class ScalarSubres;

    class Shader
    {
    public:
        Shader(QueueBase& queue, const std::shared_ptr<MatrixBase> dst);

        void appendOutput(std::string seq);
        void addScalar(std::shared_ptr<ScalarSubres> scalar);
        void addMatrix(std::shared_ptr<MatrixSubres> matrix,
            Size2D extents, Size2D offset, bool transposed);

        std::string assemble();

    private:
        QueueBase& _queueBase;
        std::string _scalars;
        std::string _images;
        std::string _multiplications;
        std::string _loads;
        std::string _output;
        Size2D _outputSize;

        void addOutputMatrix(std::shared_ptr<MatrixSubres> matrix,
            Size2D extents, Size2D offset, bool transposed);
        void addInputMatrix(std::shared_ptr<MatrixSubres> matrix,
            Size2D extents, Size2D offset, bool transposed);
        std::string matrixAccessPos(Size2D extents, Size2D offset, bool transposed);
    };

};
