#include "integrator.h"
#include "image.h"
#include "ctpl_stl.h"

namespace fst
{
    Integrator::Integrator(const parser::Scene& parser)
    {
        m_scene.loadFromParser(parser);
    }

    math::Vector3f Integrator::renderPixel(const Ray& ray, int depth)
    {
        if (depth > m_scene.max_recursion_depth)
        {
            return math::Vector3f(0.0f, 0.0f, 0.0f);
        }

        HitRecord hit_record;
        auto result = m_scene.intersect(ray, hit_record, std::numeric_limits<float>::max());

        if (!result)
        {
            return m_scene.background_color;
        }

        auto& material = m_scene.materials[hit_record.material_id - 1];
        auto color = material.get_ambient() * m_scene.ambient_light;
        auto intersection_point = ray.getPoint(hit_record.distance);

        for (auto& light : m_scene.point_lights)
        {
            auto to_light = light.get_position() - intersection_point;
            auto light_pos_distance = math::length(to_light);
            to_light = to_light / light_pos_distance;

            Ray shadow_ray(intersection_point + m_scene.shadow_ray_epsilon * to_light, to_light);

            if (!m_scene.intersectShadowRay(shadow_ray, light_pos_distance))
            {
                auto rad = light.computeRadiance(light_pos_distance);
                fst::math::Vector3f az =  ray.get_origin() + ray.get_direction() * hit_record.distance ;

                color = color + material.computeBrdf(to_light, -ray.get_direction(), hit_record.normal , hit_record , m_scene.textures , az , rad);
            }
        }

        auto& mirror = material.get_mirror();
        if (mirror.x + mirror.y + mirror.z > 0.0f)
        {
            auto new_direction = math::reflect(ray.get_direction(), hit_record.normal);
            Ray secondary_ray(intersection_point + m_scene.shadow_ray_epsilon * new_direction, new_direction);

            return color + mirror * renderPixel(secondary_ray, depth + 1);
        }
        else
        {
            return color;
        }
    }

    void Integrator::integrate()
    {
        for (auto& camera : m_scene.cameras)
        {
            auto& resolution = camera.get_screen_resolution();
            Image image(resolution.x, resolution.y);

             ctpl::thread_pool pool(128);
            for (int i = 0; i < resolution.x; ++i)
            {
                pool.push([i, &resolution, &camera, &image, this](int id) {
                    for (int j = 0; j < resolution.y; ++j)
                    {
                        auto ray = camera.castPrimayRay(static_cast<float>(i), static_cast<float>(j));
                        auto color = renderPixel(ray, 0);
                        image.setPixel(i, j, math::clamp(color, 0.0f, 255.0f));
                    }
                });
            }
            pool.stop(true);

            image.save(camera.get_image_name());
        }
    }
    std::vector<std::pair<char , int>> Integrator::get_transformation_type(const std::string &str) const
    {
        std::vector<std::pair<char , int>> result;
        for(int i = 0 ; i < str.size() ; i++){
            if(str[i] == 'r' || str[i] == 's' || str[i] == 't' ){
                result.push_back(std::pair<char,int>(str[i],0));
                int num = 0;
                for(int j = 1 ;  ; j++){
                    if(str[i+j]< '0' || str[i+j] > '9' ){
                        result[result.size()-1].second = num;
                        i += j ;
                        break;
                    }
                    else{
                        num = 10*num + str[i+j] - '0' ;
                    }
                }
            }

        }
        return result;
    }



    void Integrator::apply_transformations(){
        std::vector<trans_matrix> rotation_matrices(0);
        std::vector<trans_matrix> scaling_matrices(0);
        std::vector<trans_matrix> translation_matrices(0);

        std::vector<trans_matrix> rotation_matrices_inv(0);
        std::vector<trans_matrix> scaling_matrices_inv(0);
        std::vector<trans_matrix> translation_matrices_inv(0);

        std::vector<trans_matrix> rotation_matrices_inv_transpose(0);
        std::vector<trans_matrix> scaling_matrices_inv_transpose(0);

        for(const auto& i : m_scene.rotations){
            rotation_matrices.push_back(trans_matrix(i));

            auto inv_transformation = i;
            inv_transformation.angle *= -1 ;

            rotation_matrices_inv.push_back(trans_matrix(inv_transformation));
            rotation_matrices_inv_transpose.push_back(rotation_matrices_inv.back().transpose());
        }
        for(const auto& i : m_scene.scalings){
            scaling_matrices.push_back(trans_matrix(i));
            
            auto inv_transformation = i;

            if(inv_transformation.x != 0) inv_transformation.x = 1 / inv_transformation.x;
            if(inv_transformation.y != 0) inv_transformation.y = 1 / inv_transformation.y;
            if(inv_transformation.z != 0) inv_transformation.z = 1 / inv_transformation.z;

            scaling_matrices_inv.push_back(trans_matrix(inv_transformation));
            scaling_matrices_inv_transpose.push_back(scaling_matrices_inv.back().transpose());
        }
        for(const auto& i : m_scene.translations){
            translation_matrices.push_back(trans_matrix(i));

            auto inv_transformation = i;

            inv_transformation.x *= -1;
            inv_transformation.y *= -1;
            inv_transformation.z *= -1;

            translation_matrices_inv.push_back(trans_matrix(inv_transformation));
        }
        
        for(auto &i : m_scene.spheres){
            auto parsed_str = get_transformation_type(i.m_transformations) ;
            trans_matrix transformation ;
            
            transformation.data[0][0] = 1 ;
            transformation.data[1][1] = 1 ;
            transformation.data[2][2] = 1 ;
            transformation.data[3][3] = 1 ;
            
            for(const auto &j : parsed_str) {
                if (j.first == 'r') {
                    i.m_center = rotation_matrices[j.second - 1] * i.m_center;

                    transformation = transformation * rotation_matrices_inv[j.second - 1];
                }
                else if (j.first == 't') {
                    i.m_center = translation_matrices[j.second - 1] * i.m_center;

                    transformation =  transformation * translation_matrices_inv[j.second - 1];
                }
                else if (j.first == 's') {
                    i.m_center = scaling_matrices[j.second - 1] * i.m_center;
                        
                    i.m_radius = std::abs( i.m_radius * scaling_matrices[j.second - 1].data[0][0]);
                        
                    transformation = transformation * scaling_matrices_inv[j.second - 1] ;
                }
            }
            i.transformation = transformation ;
        }

        for(auto &i : m_scene.meshes){
            auto parsed_str = get_transformation_type(i.m_transformations) ;

            //TODO: j üzerinden dönen loop taki matrisi önceden hesapla tek tek çarpmak yerine.
            for(const auto &j : parsed_str){
                for(auto &tri : i.m_triangles){

                    if      (j.first == 'r') {
                        const auto m_v0 = tri.m_v0;

                        tri.m_v0 = rotation_matrices[j.second - 1] * tri.m_v0;
                        tri.m_edge1 = rotation_matrices[j.second - 1] * (m_v0 + tri.m_edge1) - tri.m_v0;
                        tri.m_edge2 = rotation_matrices[j.second - 1] * (m_v0 + tri.m_edge2) - tri.m_v0;

                        tri.m_normal = rotation_matrices_inv_transpose[j.second - 1] * tri.m_normal;
                    }
                    else if (j.first == 't'){
                        tri.m_v0 = translation_matrices[j.second - 1] * tri.m_v0 ;
                    }
                    else if (j.first == 's') {
                        const auto m_v0 = tri.m_v0;

                        tri.m_v0 = scaling_matrices[j.second - 1] * tri.m_v0;
                        tri.m_edge1 = scaling_matrices[j.second - 1] * (tri.m_edge1 + m_v0) - tri.m_v0;
                        tri.m_edge2 = scaling_matrices[j.second - 1] * (tri.m_edge2 + m_v0) - tri.m_v0;


                        tri.m_normal = scaling_matrices_inv_transpose[j.second - 1] * tri.m_normal;
                        tri.m_normal = normalize(tri.m_normal);
                    }
                }
            }
        }
    }
}
