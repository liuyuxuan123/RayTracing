//
//  ray.h
//  RayTracing
//
//  Created by 刘宇轩 on 2019/5/1.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//
#ifndef RAYH
#define RAYH
#include "vec3.h"

class ray {
    public:
        ray() {
            
        }
        ray(const vec3& a, const vec3& b) {
            this->pos = a;
            this->dir = b;
        }
        vec3 origin() const       {
            return pos;
        }
        vec3 direction() const    {
            return dir;
        }
        vec3 point_at_parameter(float t) const {
            return pos + t * dir;
        }
    
        vec3 pos;
        vec3 dir;
};

#endif 


