#pragma once
#include "vector3f.h"
#include <vector>
#include "matrixInverse.h"
#include "rotation.h"
#include "translation.h"
#include "scaling.h"
#include <iostream>

namespace fst
{
    class trans_matrix
    {
	    public:
	        double data[4][4];
	        

	        trans_matrix() ;

	        trans_matrix(const Rotation    &rotate   );
			trans_matrix(const Translation &translate);
			trans_matrix(const Scaling     &scale    );

	        trans_matrix operator * (const trans_matrix &RHS) const ;
	        math::Vector3f operator *  (const math::Vector3f  &RHS) ;

	        trans_matrix transpose();

	        trans_matrix inv();

            void print() const ;
        };
} // namespace fst
