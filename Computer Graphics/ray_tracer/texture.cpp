#include "texture.h"
#include <string>

namespace fst
{
    Texture::Texture() {}
    Texture::Texture(int width,
                     int height,
                     unsigned char *image,
                     std::string imageName,
                     std::string interpolation,
                     std::string decalMode,
                     std::string appearance)
            : m_width(width),
              m_height(height),
              m_image(image),
              m_imageName(imageName),
              m_interpolation(interpolation),
              m_decalMode(decalMode),
              m_appearance(appearance) {

        char *cstr = (char *) malloc((imageName.size() + 1) * sizeof(char)) ;

        cstr[imageName.size()] = 0 ;
    
        for(int i = 0 ; i < imageName.size()  ; ++i){
            cstr[i] = imageName[i] ;
        }
        
        read_jpeg_header(cstr , m_width , m_height);
        
        m_image = new unsigned char [m_width * m_height * 3];
        

        read_jpeg(cstr, m_image, m_width , m_height);
        

        free(cstr);
    }

    math::Vector2f Texture::get_ij(math::Vector2f u_v) const{
          
        switch (m_appearance[0]) {
            case 'c':{
                if (u_v.x > 1) u_v.x = 1;
                if (u_v.x < 0) u_v.x = 0 ;
                if (u_v.y > 1) u_v.y = 1 ;
                if (u_v.y < 0) u_v.y = 0 ;
            } break ;
            case 'r':{
                u_v.x = u_v.x - std::floor(u_v.x);
                u_v.y = u_v.y - std::floor(u_v.y);
            }
        }
          
        math::Vector2f i_j(u_v.x * (m_width  - 1) , u_v.y * (m_height  - 1)) ;
              

        if (i_j.x == m_width  - 1) i_j.x = i_j.x - 0.0001 ;  //m_width ye çevrildi
        if (i_j.y == m_height  - 1) i_j.y = i_j.y - 0.0001 ; //m_Height e çevrildi ayrıca baştaki i_j.height idi
       

        if(i_j.x < 0) i_j.x = 0;
        if(i_j.y < 0) i_j.y = 0;
        return i_j;
    }

    math::Vector3f Texture::interpolate(const math::Vector2f &i_j) const{
        
        if(m_interpolation == "nearest"){
            
            int i_int = std::round(i_j.x);
            int j_int = std::round(i_j.y);
            
            
           


            std::size_t pix_loc = i_int * 3 + (m_width ) * 3 * j_int;
            
            float asi = m_image[pix_loc];
            float basi = m_image[pix_loc+1];
            float casi = m_image[pix_loc+2];
            
            
            
            
            
            auto res =  math::Vector3f(m_image[pix_loc] , m_image[pix_loc + 1] , m_image[pix_loc + 2]);  //m_image yaptırıldı ;:
        
            

            return res;
        }
        else{  
           
            
            int p = std::floor(i_j.x);
         
            int q = std::floor(i_j.y);
            

            double dx = i_j.x - p;
            

            double dy = i_j.y  - q;
                        

            if (p >= m_width - 1 || m_height - 1 <= q) return math::Vector3f(0,0,0);
            std::size_t  pix_loc_1 =  p       * 3 + m_width * 3 * q;
            std::size_t  pix_loc_2 =  (p + 1) * 3 + m_width * 3 * q;
            std::size_t  pix_loc_3 =  p       * 3 + m_width * 3 * (q + 1);
            std::size_t  pix_loc_4 =  (p + 1) * 3 + m_width * 3 * (q + 1);   //bu 4 lü kontrol edilcek
         
           
            math::Vector3f first_contribution (m_image[pix_loc_1],m_image[pix_loc_1 + 1],m_image[pix_loc_1 + 2]);
            math::Vector3f second_contribution(m_image[pix_loc_2],m_image[pix_loc_2 + 1],m_image[pix_loc_2 + 2]);
            math::Vector3f third_contribution (m_image[pix_loc_3],m_image[pix_loc_3 + 1],m_image[pix_loc_3 + 2]);   //bu 4 lüdeki imageler hep m_image a çevrildi
            math::Vector3f fourth_contribution(m_image[pix_loc_4],m_image[pix_loc_4 + 1],m_image[pix_loc_4 + 2]); //bu 4lü de kontrol edilecek
            
            
            first_contribution  = first_contribution  * (1 - dx) * (1 - dy);
            second_contribution = second_contribution * (dx) * (1 - dy);
            third_contribution  = third_contribution  * (1 - dx) * (dy);
            fourth_contribution = fourth_contribution * (dx) * (dy);

            return first_contribution + second_contribution + third_contribution + fourth_contribution;
        }
    }
} // namespace fst