//
//  colors.h
//  RayTracing
//
//  Created by 刘宇轩 on 2019/5/8.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef colors_h
#define colors_h
#include "vec3.h"

class colors {
public:
    static vec3 random_pink_colors[4];
    static vec3 random_sphere_colors[5];
    
    static vec3 random_pink_color(){
        return random_pink_colors[rand() % sizeof(random_pink_colors) / sizeof(vec3)];
    };
    static vec3 random_sphere_color(){
        return random_sphere_colors[rand() % sizeof(random_sphere_colors) / sizeof(vec3)];
    };

};

vec3 colors::random_pink_colors[4] = {
    vec3(184.0 / 255.0, 107.0 / 255.0, 119.0 / 255.0),
    vec3(234.0 / 255.0, 191.0 / 255.0, 185.0 / 255.0),
    vec3(246.0 / 255.0, 207.0 / 255.0, 202.0 / 255.0),
    vec3(255.0 / 255.0, 232.0 / 255.0, 229.0 / 255.0)
};

vec3 colors::random_sphere_colors[5] = {
    vec3(248.0 / 255.0, 233.0 / 255.0, 161.0 / 255.0),
    vec3(247.0 / 255.0, 108.0 / 255.0, 108.0 / 255.0),
    vec3(168.0 / 255.0, 208.0 / 255.0, 230.0 / 255.0),
    vec3( 55.0 / 255.0,  71.0 / 255.0, 133.0 / 255.0),
    vec3( 36.0 / 255.0,  48.0 / 255.0,  94.0 / 255.0)
};

#endif /* colors_h */
