#include "hit_record.h"
#include "triangle.h"
#include "ray.h"

namespace fst
{
    Triangle::Triangle(const math::Vector3f& v0, const math::Vector3f& edge1, const math::Vector3f& edge2)
        : m_v0(v0)
        , m_edge1(edge1)
        , m_edge2(edge2)
        , m_normal(math::normalize(math::cross(edge1, edge2)))
    {}

    bool Triangle::intersect(const Ray& ray, HitRecord& hit_record, float max_distance)
    {
        //M�ller-Trumbore algorithm
        auto pvec = math::cross(ray.get_direction(), m_edge2);
        auto inv_det = 1.0f / math::dot(m_edge1, pvec);

        auto tvec = ray.get_origin() - m_v0;
        auto w1 = math::dot(tvec, pvec) * inv_det;

        if (w1 < 0.0f || w1 > 1.0f)
        {
            return false;
        }

        auto qvec = math::cross(tvec, m_edge1);
        auto w2 = math::dot(ray.get_direction(), qvec) * inv_det;

        if (w2 < 0.0f || (w1 + w2) > 1.0f)
        {
            return false;
        }

        auto distance = math::dot(m_edge2, qvec) * inv_det;
        if (distance > 0.0f && distance < max_distance)
        {
            //Fill the intersection record.
            hit_record.normal = m_normal;
            hit_record.distance = distance;

            return true;
        }
        return false;
    }

    bool Triangle::intersectShadowRay(const Ray& ray, float max_distance)
    {
        //M�ller-Trumbore algorithm
        auto pvec = math::cross(ray.get_direction(), m_edge2);
        auto inv_det = 1.0f / math::dot(m_edge1, pvec);

        auto tvec = ray.get_origin() - m_v0;
        auto w1 = math::dot(tvec, pvec) * inv_det;

        if (w1 < 0.0f || w1 > 1.0f)
        {
            return false;
        }

        auto qvec = math::cross(tvec, m_edge1);
        auto w2 = math::dot(ray.get_direction(), qvec) * inv_det;

        if (w2 < 0.0f || (w1 + w2) > 1.0f)
        {
            return false;
        }

        auto distance = math::dot(m_edge2, qvec) * inv_det;
        return distance > 0.0f && distance < max_distance;
    }

    math::Vector2f Triangle::get_barycentric(const math::Vector3f &point){

      
          math::Vector3f m_edge3 = point - m_v0;
        double d1 = math::dot(m_edge1, m_edge1);
        double d2 = math::dot(m_edge1, m_edge2);
        double d3 = math::dot(m_edge2, m_edge2);
        double d4 = math::dot(m_edge3, m_edge1);
        double d5 = math::dot(m_edge3, m_edge2);
        double var = d1 * d3 - d2 * d2;
       
        auto res = math::Vector2f((d3 * d4 - d2 * d5) / var , (d1 * d5 - d2 * d4) / var) ;

        if(res.x < 0) res.x = 0 ;
        if(res.y < 0 ) res.y = 0 ;
        return res ;
    }

    math::Vector2f Triangle::get_uv(const math::Vector2f &barycentric_coord){
        auto res= math::Vector2f(u_v_0.x + barycentric_coord.x *(u_v_1.x - u_v_0.x) + barycentric_coord.y * (u_v_2.x - u_v_0.x) ,
                              u_v_0.y + barycentric_coord.x *(u_v_1.y - u_v_0.y) + barycentric_coord.y * (u_v_2.y - u_v_0.y));

        if(res.x < 0) res.x = 0 ;
        if(res.y < 0 ) res.y = 0 ;

        return res ;
    }
}
