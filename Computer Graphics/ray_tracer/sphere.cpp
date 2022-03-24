#include "sphere.h"
#include "hit_record.h"
#include "ray.h"
#include <cmath>
#include <valarray>
namespace fst
{
    Sphere::Sphere(const math::Vector3f &center, float radius, int material_id , int texture_id , std::string transformations)
        : m_center(center)
        , m_radius(radius)
        , m_material_id(material_id)
        , m_texture_id(texture_id)
        , m_transformations(transformations)
        , m_old_radius(radius)
        , m_old_center(center)
    {}

    bool Sphere::intersect(const Ray& ray, HitRecord& hit_record, float max_distance)
    {
        /* if(length(ray.get_origin() - m_center) < m_radius){
            Ray fakeray(ray.get_origin() , -1 * ray.get_direction());

            auto e = m_center - fakeray.get_origin();
            auto a = math::dot(e, fakeray.get_direction());
            auto x = m_radius * m_radius + a * a - math::dot(e, e);

            auto distance = -1 * (a - sqrtf(x));
            

            if (distance < max_distance)
            {
                //Fill the intersection record.
                hit_record.distance = distance;
                hit_record.normal = math::normalize(m_center - ray.getPoint(hit_record.distance));
                hit_record.material_id = m_material_id;

                return true;
            }
            return false;
        }
        //geometrical approach
        auto e = m_center - ray.get_origin();
        auto a = math::dot(e, ray.get_direction());
        auto x = m_radius * m_radius + a * a - math::dot(e, e);

        if (x < 0.0f)
        {
            return false;
        }

        auto distance = a - sqrtf(x);
        if (distance > 0.0f && distance < max_distance)
        {
            //Fill the intersection record.
            hit_record.distance = distance;
            hit_record.normal = math::normalize(ray.getPoint(hit_record.distance) - m_center);
            hit_record.material_id = m_material_id;


            return true;
        }
        return false; */ 
        
        //geometrical approach
        auto e = m_center - ray.get_origin();
        auto a = math::dot(e, ray.get_direction());
        auto x = m_radius * m_radius + a * a - math::dot(e, e);

        if (x < 0.0f)
        {
            return false;
        }

        auto distance = a - sqrtf(x);
        if (distance < 0.0f) { distance = a + sqrtf(x); } 
        if (distance > 0.0f && distance < max_distance)
        {
            //Fill the intersection record.
            hit_record.distance = distance;
            hit_record.normal = math::normalize(ray.getPoint(hit_record.distance) - m_center);
            hit_record.material_id = m_material_id;

            return true;
        }
        return false;
    }

    bool Sphere::intersectShadowRay(const Ray& ray, float max_distance) const
    {
       /* if(length(ray.get_origin() - m_center) < m_radius){

            auto light_pos = normalize(ray.get_direction()) * max_distance + ray.get_origin();
            auto light_dist_to_center = length(light_pos - m_center);

            return (m_radius < light_dist_to_center) ;
        }
        auto e = m_center - ray.get_origin();
        auto a = math::dot(e, ray.get_direction());
        auto x = m_radius * m_radius + a * a - math::dot(e, e);

        if (x < 0.0f)
        {
            return false;
        }

        auto distance = a - sqrtf(x);
        return distance > 0.0f && distance < max_distance; */

        //geometrical approach
        auto e = m_center - ray.get_origin();
        auto a = math::dot(e, ray.get_direction());
        auto x = m_radius * m_radius + a * a - math::dot(e, e);

        if (x < 0.0f)
        {
            return false;
        }

        auto distance = a - sqrtf(x);
        return distance > 0.0f && distance < max_distance;
    }

    math::Vector2f Sphere::get_uv(const math::Vector3f &intersection_point) const{
        double x_diff = intersection_point.x - m_old_center.x ;    //center m_center a değiştirildi

        if(x_diff == 0) x_diff = 0.00000000001 ;

       
        double argument_of_acos = (intersection_point.y - m_old_center.y) /m_old_radius ;
        if(argument_of_acos > 1) argument_of_acos = 1 ;  //DUE TO FLOATING POINT ERRORS  argument_of_acos can be greater than 1 
        if(argument_of_acos < -1) argument_of_acos = -1;  //or less than -1
        auto res= math::Vector2f(
              (3.14159265359 - std::atan2(intersection_point.z - m_old_center.z , x_diff)) / 6.283185307179586 //center m_center a değiştirildi ayriyetten atan2 argümanları
            , std::acos(argument_of_acos ) / 3.14159265359  //center m_center a değiştirildi
            );
        

        return res;
    }
}
