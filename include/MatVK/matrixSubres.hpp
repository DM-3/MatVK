#pragma once

#include "matvk.hpp"
#include "vulkanBase.hpp"


namespace matvk
{

    class MatrixSubres
    {
    public:
        MatrixSubres(ElemType elemType, Size2D extents);
        void write(void* src, ElemType elemType);
        void read(void* dst, ElemType elemType);

    private:
        ElemType _elemType;
        Size2D _extents;
        vk::Image _image;
        vk::DeviceMemory _memory;
        vk::ImageView _imageView;

        void createImage();
        void createImageView();
        void setImageLayout();
    };

};
