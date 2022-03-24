#include "material.h"
#include "vector3f.h"

namespace fst
{
    Material::Material(const math::Vector3f& ambient, const math::Vector3f& diffuse, const math::Vector3f& specular, const math::Vector3f& mirror, float phong_exponent)
        : m_ambient(ambient)
        , m_diffuse(diffuse)
        , m_specular(specular)
        , m_mirror(mirror)
        , m_phong_exponent(phong_exponent)
    {}

    math::Vector3f Material::computeBrdf(const math::Vector3f& wi, const math::Vector3f& wo, const math::Vector3f& normal, HitRecord &hitRecord , std::vector<Texture> &textures , math::Vector3f &point , math::Vector3f &rad) const
    {
        math::Vector3f diffuse(0,0,0) ;

        if(hitRecord.mesh != nullptr && hitRecord.mesh -> m_texture_id != -1){
            
            
            auto u_v = hitRecord.triangle->get_uv(hitRecord.triangle->get_barycentric(point));

            auto C = textures[hitRecord.mesh -> m_texture_id - 1].interpolate(textures[hitRecord.mesh -> m_texture_id - 1].get_ij(u_v));
            

            if (textures[hitRecord.mesh -> m_texture_id - 1].m_decalMode == "replace_kd" ){
                diffuse = rad * math::max(math::dot(normal, wi), 0.0f) * C / 255 ;
            }
            else if(textures[hitRecord.mesh -> m_texture_id - 1].m_decalMode == "blend_kd" ){
                diffuse = rad * math::max(math::dot(normal, wi), 0.0f) * (m_diffuse + C / 255) / 2 ;
            }
            else if(textures[hitRecord.mesh -> m_texture_id - 1].m_decalMode == "replace_all" ){
                diffuse = C ;
            } 
        }else if(hitRecord.sphere != nullptr && hitRecord.sphere -> m_texture_id != -1) {
            auto old_point = hitRecord.sphere -> transformation * point ;
            
            auto u_v = hitRecord.sphere -> get_uv(old_point);   

            auto C = textures[hitRecord.sphere -> m_texture_id - 1].interpolate(textures[hitRecord.sphere -> m_texture_id - 1].get_ij(u_v));

            if (textures[hitRecord.sphere -> m_texture_id - 1].m_decalMode == "replace_kd" ){
                diffuse = rad * math::max(math::dot(normal, wi), 0.0f) * C / 255 ;
            }
            else if(textures[hitRecord.sphere -> m_texture_id - 1].m_decalMode == "blend_kd" ){
                diffuse = rad * math::max(math::dot(normal, wi), 0.0f) * (m_diffuse + C / 255) / 2 ;
            }
            else if(textures[hitRecord.sphere -> m_texture_id - 1].m_decalMode == "replace_all" ){
                diffuse = C ;
            }
        }else{ 
            diffuse = rad * math::max(math::dot(normal, wi), 0.0f) * m_diffuse;
        } 

        auto specular = std::pow(math::max(math::dot(math::normalize(wo + wi), normal), 0.0f), m_phong_exponent);

        return (m_specular * specular * rad) + diffuse;
    }
}