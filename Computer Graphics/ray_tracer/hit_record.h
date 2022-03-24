#pragma once

#include "triangle.h"
#include "mesh.h"
#include "sphere.h"
#include "vector3f.h"

namespace fst
{
    struct HitRecord
    {
        math::Vector3f normal;
        float distance;
        int material_id;

        Triangle *triangle;
        Mesh *mesh;
        Sphere *sphere;
    };
}