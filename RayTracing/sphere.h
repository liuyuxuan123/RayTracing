//
//  sphere.h
//  RayTracing
//
//  Created by 刘宇轩 on 2019/5/2.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//
#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable  {
public:
    sphere() {      }
    sphere(vec3 cen, float r, material *m) : center(cen), radius(r), material_ptr(m) {       };
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    
    vec3 center;
    float radius;
    material* material_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    // t = discriminant
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = material_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = material_ptr;
            return true;
        }
    }
    return false;
}


#endif



