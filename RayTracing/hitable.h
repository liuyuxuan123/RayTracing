//
//  hitable.h
//  RayTracing
//
//  Created by 刘宇轩 on 2019/5/1.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//
#ifndef HITABLEH
#define HITABLEH 

#include "ray.h"

class material;
struct hit_record {
    float t;  
    vec3 p;
    vec3 normal; 
    material *mat_ptr;
};

class hitable  {
    public:
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif




