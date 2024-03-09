#pragma once

#include "matvk.hpp"
#include "vulkanBase.hpp"


namespace matvk
{

    class MatrixSubres
    {
    public:
        MatrixSubres(Size2D extents);

    private:
        Size2D _extents;
        vk::Image _image;
        vk::DeviceMemory _memory;
        vk::ImageView _imageView;

        void createImage();
        void createImageView();
        void setImageLayout();
    };

};
