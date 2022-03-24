#pragma once

#include "scene.h"

namespace fst
{
    class Integrator
    {
    public:
        Integrator(const parser::Scene& parser);

        math::Vector3f renderPixel(const Ray& ray, int depth) ;
        void integrate() ;

        std::vector<std::pair<char , int>> get_transformation_type(const std::string &str) const;
        void apply_transformations();
    private:
        Scene m_scene;
    };
}