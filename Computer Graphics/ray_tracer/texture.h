#pragma once
#include <string>
#include <cmath>
#include "vector3f.h"
#include "vector2f.h"
#include <iostream>
#include "jpeg.h"
#include <stdlib.h>

namespace fst
{
    class Texture
    {
    public:
        Texture();
        Texture(int width,
                int height,
                unsigned char *image,
                std::string imageName,
                std::string interpolation,
                std::string decalMode,
                std::string appearance);

        math::Vector2f get_ij(math::Vector2f u_v) const;
        fst::math::Vector3f interpolate(const math::Vector2f &i_j ) const;


        int m_width;
        int m_height;
        unsigned char *m_image;
        std::string m_imageName;
        std::string m_interpolation;
        std::string m_decalMode;
        std::string m_appearance;
    };
}