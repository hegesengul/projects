#pragma once

#include "triangle.h"
#include "vector3f.h"
#include <vector>
#include <string>

namespace fst
{
    class Mesh : public Triangular
    {
    public:
        Mesh(const std::vector<Triangle>& triangles, int material_id , int texture_id , std::string transformations);
        Mesh(std::vector<Triangle>&& triangles, int material_id ,  int texture_id , std::string transformations);

        bool intersect(const Ray& ray, HitRecord& hit_record, float max_distance)   ;
        bool intersectShadowRay(const Ray& ray, float max_distance)  ;


	    std::vector<Triangle> m_triangles;
        int m_material_id;
        int m_texture_id;
        std::string m_transformations;
    };
}
