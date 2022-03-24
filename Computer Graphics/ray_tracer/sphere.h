#pragma once

#include "vector3f.h"
#include <utility>
#include <string>
#include "matrix.h"
#include "vector2f.h"

namespace fst
{
    class Ray;
    struct HitRecord;

    class Sphere
    {
    public:
        Sphere(const math::Vector3f &center, float radius, int material_id , int texture_id , std::string transformations);

        bool intersect(const Ray& ray, HitRecord& hit_record, float max_distance) ;
        bool intersectShadowRay(const Ray& ray, float max_distance) const;

        math::Vector2f get_uv(const math::Vector3f &intersection_point) const;

        math::Vector3f m_center;
        float m_radius;
        float m_old_radius;
        int m_material_id;
        int m_texture_id;
        std::string m_transformations;
        trans_matrix transformation;
        math::Vector3f m_old_center;
    };
}