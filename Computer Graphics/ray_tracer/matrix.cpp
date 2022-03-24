//#pragma once
#include"matrix.h"

const double pi = 3.14159265359 ;

namespace fst
{
    trans_matrix trans_matrix::operator * (const trans_matrix &RHS) const{
        trans_matrix res;
/*
        res.data[0][0] = this->data[0][0] * RHS.data[0][0] + this->data[0][1] * RHS.data[1][0] + this->data[0][2] * RHS.data[2][0] + this->data[0][3] * RHS.data[3][0] ;
        res.data[0][1] = this->data[0][0] * RHS.data[0][1] + this->data[0][1] * RHS.data[1][1] + this->data[0][2] * RHS.data[2][1] + this->data[0][3] * RHS.data[3][1] ;
        res.data[0][2] = this->data[0][0] * RHS.data[0][2] + this->data[0][1] * RHS.data[1][2] + this->data[0][2] * RHS.data[2][2] + this->data[0][3] * RHS.data[3][2] ;
        res.data[0][3] = this->data[0][0] * RHS.data[0][3] + this->data[0][1] * RHS.data[1][3] + this->data[0][2] * RHS.data[2][3] + this->data[0][3] * RHS.data[3][3] ;
        res.data[1][0] = this->data[1][0] * RHS.data[0][0] + this->data[1][1] * RHS.data[1][0] + this->data[1][2] * RHS.data[2][0] + this->data[1][3] * RHS.data[3][0] ;
        res.data[1][1] = this->data[1][0] * RHS.data[0][1] + this->data[1][1] * RHS.data[1][1] + this->data[1][2] * RHS.data[2][1] + this->data[1][3] * RHS.data[3][1] ;
        res.data[1][2] = this->data[1][0] * RHS.data[0][2] + this->data[1][1] * RHS.data[1][2] + this->data[1][2] * RHS.data[2][2] + this->data[1][3] * RHS.data[3][2] ;
        res.data[1][3] = this->data[1][0] * RHS.data[0][3] + this->data[1][1] * RHS.data[1][3] + this->data[1][2] * RHS.data[2][3] + this->data[1][3] * RHS.data[3][3] ;
        res.data[2][0] = this->data[2][0] * RHS.data[0][0] + this->data[2][1] * RHS.data[1][0] + this->data[2][2] * RHS.data[2][0] + this->data[2][3] * RHS.data[3][0] ;
        res.data[2][1] = this->data[2][0] * RHS.data[0][1] + this->data[2][1] * RHS.data[1][1] + this->data[2][2] * RHS.data[2][1] + this->data[2][3] * RHS.data[3][1] ;
        res.data[2][2] = this->data[2][0] * RHS.data[0][2] + this->data[2][1] * RHS.data[1][2] + this->data[2][2] * RHS.data[2][2] + this->data[2][3] * RHS.data[3][2] ;
        res.data[2][3] = this->data[2][0] * RHS.data[0][3] + this->data[2][1] * RHS.data[1][3] + this->data[2][2] * RHS.data[2][3] + this->data[2][3] * RHS.data[3][3] ;
        res.data[3][0] = this->data[3][0] * RHS.data[0][0] + this->data[3][1] * RHS.data[1][0] + this->data[3][2] * RHS.data[2][0] + this->data[3][3] * RHS.data[3][0] ;
        res.data[3][1] = this->data[3][0] * RHS.data[0][1] + this->data[3][1] * RHS.data[1][1] + this->data[3][2] * RHS.data[2][1] + this->data[3][3] * RHS.data[3][1] ;
        res.data[3][2] = this->data[3][0] * RHS.data[0][2] + this->data[3][1] * RHS.data[1][2] + this->data[3][2] * RHS.data[2][2] + this->data[3][3] * RHS.data[3][2] ;
        res.data[3][3] = this->data[3][0] * RHS.data[0][3] + this->data[3][1] * RHS.data[1][3] + this->data[3][2] * RHS.data[2][3] + this->data[3][3] * RHS.data[3][3] ;
 */
        for(int i = 0 ; i <=3 ; ++i){
            for(int j = 0 ; j <=3 ; ++j){
                for(int k = 0; k<=3; ++k){
                    res.data[i][j] += data[i][k] * RHS.data[k][j];
                }
            }
        }
        return  res ;
    }
    
    math::Vector3f trans_matrix::operator * (const math::Vector3f &RHS){
        return  math::Vector3f(data[0][0] , data[1][0] , data[2][0]) * RHS.x +
                math::Vector3f(data[0][1] , data[1][1] , data[2][1]) * RHS.y +
                math::Vector3f(data[0][2] , data[1][2] , data[2][2]) * RHS.z +
                math::Vector3f(data[0][3] , data[1][3] , data[2][3]);
    }
    
    trans_matrix::trans_matrix(){
        data[0][0] = 0 ;
        data[0][1] = 0 ;
        data[0][2] = 0 ;
        data[0][3] = 0 ;
        data[1][0] = 0 ;
        data[1][1] = 0 ;
        data[1][2] = 0 ;
        data[1][3] = 0 ;
        data[2][0] = 0 ;
        data[2][1] = 0 ;
        data[2][2] = 0 ;
        data[2][3] = 0 ;
        data[3][0] = 0 ;
        data[3][1] = 0 ;
        data[3][2] = 0 ;
        data[3][3] = 0 ;
    }

    trans_matrix::trans_matrix(const Rotation &rotate){
        auto u = math::normalize(math::Vector3f(rotate.x , rotate.y , rotate.z)) ;

        auto d = std::sqrt(u.y * u.y + u.z * u.z) ;
        trans_matrix rotate_tox;
        trans_matrix rotate_toy;

        if(d != 0 ){
            rotate_tox.data[0][0] = 1 ;
            rotate_tox.data[1][1] =  u.z / d ;
            rotate_tox.data[1][2] = -u.y / d ;
            rotate_tox.data[2][1] =  u.y / d ;
            rotate_tox.data[2][2] =  u.z / d ;
            rotate_tox.data[3][3] = 1 ;

        }
        else {
            rotate_tox.data[0][0] = 1 ;   
            rotate_tox.data[1][1] = 1 ;
            rotate_tox.data[2][2] = 1 ;
            rotate_tox.data[3][3] = 1 ; 
        }

        auto u_length = std::sqrt(u.x * u.x + u.y * u.y + u.z * u.z) ;

        rotate_toy.data[0][0] = d / u_length;
        rotate_toy.data[0][2] = -u.x / u_length ;
        rotate_toy.data[1][1] = 1;
        rotate_toy.data[2][0] = u.x / u_length ;
        rotate_toy.data[2][2] = d / u_length;
        rotate_toy.data[3][3] = 1 ;

        double kosinus = std::cos(rotate.angle*pi/180);
        double sinus = std::sin(rotate.angle*pi/180);

        trans_matrix the_wanted;
        the_wanted.data[0][0] = kosinus;
        the_wanted.data[0][1] = -sinus ;
        the_wanted.data[1][0] = sinus;
        the_wanted.data[1][1] = kosinus ;
        the_wanted.data[2][2] = 1 ;
        the_wanted.data[3][3] = 1 ;

        trans_matrix anti_rotate_tox = rotate_tox.transpose();
        trans_matrix anti_rotate_toy = rotate_toy.transpose();

        *this  =  anti_rotate_tox * anti_rotate_toy  * the_wanted * rotate_toy * rotate_tox;
    }

    trans_matrix::trans_matrix(const Translation &translate){
        data[0][0] = 1 ;
        data[0][1] = 0 ;
        data[0][2] = 0 ;
        data[0][3] = translate.x ;
        data[1][0] = 0 ;
        data[1][1] = 1 ;
        data[1][2] = 0 ;
        data[1][3] = translate.y ;
        data[2][0] = 0 ;
        data[2][1] = 0 ;
        data[2][2] = 1 ;
        data[2][3] = translate.z ;
        data[3][0] = 0 ;
        data[3][1] = 0 ;
        data[3][2] = 0 ;
        data[3][3] = 1 ;
    }

    trans_matrix::trans_matrix(const Scaling &scale){
        data[0][0] = scale.x ;
        data[0][1] = 0 ;
        data[0][2] = 0 ;
        data[0][3] = 0 ;
        data[1][0] = 0 ;
        data[1][1] = scale.y ;
        data[1][2] = 0 ;
        data[1][3] = 0 ;
        data[2][0] = 0 ;
        data[2][1] = 0 ;
        data[2][2] = scale.z ;
        data[2][3] = 0 ;
        data[3][0] = 0 ;
        data[3][1] = 0 ;
        data[3][2] = 0 ;
        data[3][3] = 1 ;
    }

    trans_matrix trans_matrix::transpose(){
        trans_matrix res ;
        res.data[0][0] = data[0][0] ;
        res.data[0][1] = data[1][0] ;
        res.data[0][2] = data[2][0] ;
        res.data[0][3] = data[3][0] ;
        res.data[1][0] = data[0][1] ;
        res.data[1][1] = data[1][1] ;
        res.data[1][2] = data[2][1] ;
        res.data[1][3] = data[3][1] ;
        res.data[2][0] = data[0][2] ;
        res.data[2][1] = data[1][2] ;
        res.data[2][2] = data[2][2] ;
        res.data[2][3] = data[3][2] ;
        res.data[3][0] = data[0][3] ;
        res.data[3][1] = data[1][3] ;
        res.data[3][2] = data[2][3] ;
        res.data[3][3] = data[3][3] ;

        return res ;
    }

    trans_matrix trans_matrix::inv(){
        double temp[16] ;
        double res[16] ;

        trans_matrix result ;

        temp[0] = data[0][0] ;
        temp[1] = data[0][1] ;
        temp[2] = data[0][2] ;
        temp[3] = 0 ;
        temp[4] = data[1][0] ;
        temp[5] = data[1][1] ;
        temp[6] = data[1][2] ;
        temp[7] = 0 ;
        temp[8] = data[2][0] ;
        temp[9] = data[2][1] ;
        temp[10] = data[2][2] ;
        temp[11] = 0 ;
        temp[12] = 0 ;
        temp[13] = 0 ;
        temp[14] = 0 ;
        temp[15] = 1 ;

        invert(temp , res) ;  //resin yerinde temp 1 vardı

        result.data[0][0] = res[0] ;
        result.data[0][1] = res[1] ;
        result.data[0][2] = res[2] ;
        result.data[0][3] = res[3] ;
        result.data[1][0] = res[4] ;
        result.data[1][1] = res[5] ;
        result.data[1][2] = res[6] ;
        result.data[1][3] = res[7] ;
        result.data[2][0] = res[8] ;
        result.data[2][1] = res[9] ;
        result.data[2][2] = res[10] ;
        result.data[2][3] = res[11] ;
        result.data[3][0] = res[12] ;
        result.data[3][1] = res[13] ;
        result.data[3][2] = res[14] ;
        result.data[3][3] = res[15] ;

        return result ;
    }

    void trans_matrix::print() const {
        for(auto i = 0  ; i < 4 ; ++i){
            for (int j = 0; j < 4; ++j) {
                std::cout << data[i][j] << " " ;
            }
            std::cout << std::endl ;
        }
    }
} // namespace fst
