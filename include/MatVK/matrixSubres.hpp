#pragma once

#include "matvk.hpp"
#include "vulkanBase.hpp"


namespace matvk
{

    class MatrixSubres
    {
    public:
        MatrixSubres(ElemType elemType, Size2D extents);
        void write(Size2D extents, Size2D offset, void* src);
        void read(Size2D extents, Size2D offset, void* dst);

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
