//
//  main.cc
//  RayTracing
//
//  Created by 刘宇轩 on 2019/5/1.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <dispatch/dispatch.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <sys/types.h>
#include <vector>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "colors.h"

#define INITIAL_WIDTH 640
#define INITIAL_HEIGHT 640
#define INITIAL_X_POS 50
#define INITIAL_Y_POS 50
using namespace std;


// Using char to store int value to minize file size
// from 4.8MB -> 1.2MB
char pixels[640 * 640 * 3];
bool isFirstTimeRun = true;
bool is_animation = false;
string file_location = "/Users/liuyuxuan/Documents/RayTracing/frame/";

void load(int frame){
    string fileName = file_location + "frameX" + to_string(frame);
    FILE* pFile;
    if((pFile = fopen(fileName.c_str(), "rb")) == NULL) {
        printf("cant open the file");
        exit(0);
    }
    fread(pixels, sizeof(char), 640 * 640 * 3, pFile);
    fclose(pFile);
    
}

void save(int frame){
    string fileName = file_location + "frameX" + to_string(frame);
    FILE * pFile;
    if((pFile = fopen(fileName.c_str(), "wb")) == NULL) {
        printf("cant open the file");
        exit(0);
    }
    fwrite(pixels, sizeof(char), 640 * 640 * 3, pFile);
    fclose(pFile);
}

vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth+1);
        }
        else {
            return vec3(0,0,0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
        //return vec3(1.0, 1.0, 1.0);
    }
}


hitable *random_scene() {
    int n = 500;
    hitable** list = new hitable*[n+1];
    // Simulate the planet
    int i = 0;
    list[i ++] = new sphere(vec3( 0, -1000, 0), 1000, new lambertian(colors::random_pink_colors[0]));
    list[i ++] = new sphere(vec3(-3, 1,     2), 1.0,  new dielectric(1.5));
    list[i ++] = new sphere(vec3(-8, 1,     0), 1.0,  new lambertian(colors::random_pink_colors[1]));
    list[i ++] = new sphere(vec3(-3, 1,    -2), 1.0,  new metal(colors::random_pink_colors[2], 0.0));
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {  // diffuse
                    list[i++] = new sphere(center, 0.2, new lambertian(colors::random_sphere_color()));
                } else if (choose_mat < 0.95) { // metal
                    list[i++] = new sphere(center, 0.2,
                                           new metal(colors::random_sphere_color(),  0.5*drand48()));
                } else {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    return new hitable_list(list,i);
}


void reshape(int w, int h){
    // float ratio = screenWidth * 1.0 / screenHeight;
    // Reset Matrix
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, INITIAL_WIDTH, INITIAL_HEIGHT);
    // Do something to the engine
    glMatrixMode(GL_MODELVIEW);
}

void loop() {
    // Necessary for MacOS.
    // Has to be different dimensions than in glutInitWindowSize();
    // Or We can delete glutInitWindowSize and do that initialization here
    if(isFirstTimeRun) {
        glutReshapeWindow(INITIAL_WIDTH, INITIAL_HEIGHT);
        isFirstTimeRun = false;
    }
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glClear(GL_COLOR_BUFFER_BIT );
    glLoadIdentity();
    gluOrtho2D(0, INITIAL_WIDTH, 0, INITIAL_HEIGHT);
    glBegin(GL_POINTS);
    for(int height = 0; height < INITIAL_HEIGHT; height ++){
        for(int width = 0; width < INITIAL_WIDTH; width ++){
            int red =   pixels[height * 640 * 3 + width * 3 + 0];
            int green = pixels[height * 640 * 3 + width * 3 + 1];
            int blue =  pixels[height * 640 * 3 + width * 3 + 2];
            red = red < 0 ?     256 + red : red;
            green = green < 0 ? 256 + green : green;
            blue = blue < 0 ?   256 + blue : blue;
            glColor3f(red / 255.0, green / 255.0, blue / 255.0);
            //cout << "(" << red << "," << green << "," << blue << ")" << endl;
            //glColor3f(pixels[height][width][0]/255.0, pixels[height][width][1]/255.0, pixels[height][width][2]/255.0);
            glVertex2i(width, height);
        }
    }
    glEnd();
    glutSwapBuffers();
}


// Update frame every second
clock_t elapse = CLOCKS_PER_SEC;
clock_t lastTime = 0;
int pauseFrame = 0;
int currentFrame = 0;
void update(){
    clock_t currentTime = clock();
    if(lastTime == 0 || currentTime - lastTime > elapse / 20){
        if(is_animation){
            if(pauseFrame < 40){
                pauseFrame += 1;
            }else{
                load(currentFrame);
                currentFrame = min(159, currentFrame + 1);
                glutPostRedisplay();
            }
        }else{
            glutPostRedisplay();
        }
        lastTime = currentTime;
    }
}


vector<int> fallingTime;
double fallingSpeed;
// total frame = 5 * 20 = 100
void set_up_position(hitable_list* hitable_world){
    int size = hitable_world->list_size;
    fallingTime = vector<int>(size, 0);
    for(int i = 0;i < size; i ++){
        fallingTime[i] = rand() % 100;
        if(i > 3){
            dynamic_cast<sphere*>(hitable_world->list[i])->center[1] = 4.2;
        }
    }
}

void real_time_position(hitable_list* hitable_world, int frame){
    int size = hitable_world->list_size;
    for(int i = 4;i < size; i ++){
        if(frame  < fallingTime[i])
            continue;
        sphere* currentSphere = dynamic_cast<sphere*>(hitable_world->list[i]);
        currentSphere->center[1] = max(0.2, 3.0 -  0.5 * pow((frame - fallingTime[i]) / 20.0, 2));
    }
}


void optimized_ray_tracing(){
    int width  = INITIAL_WIDTH;
    int height = INITIAL_HEIGHT;
    int ns = 10;
    is_animation = false;
    dispatch_queue_t executeQueue[] = {
        dispatch_queue_create("raytracing_queue_0", DISPATCH_QUEUE_CONCURRENT),
        dispatch_queue_create("raytracing_queue_1", DISPATCH_QUEUE_CONCURRENT),
        dispatch_queue_create("raytracing_queue_2", DISPATCH_QUEUE_CONCURRENT),
        dispatch_queue_create("raytracing_queue_3", DISPATCH_QUEUE_CONCURRENT),
        dispatch_queue_create("raytracing_queue_4", DISPATCH_QUEUE_CONCURRENT),
        dispatch_queue_create("raytracing_queue_5", DISPATCH_QUEUE_CONCURRENT),
        dispatch_queue_create("raytracing_queue_6", DISPATCH_QUEUE_CONCURRENT),
        dispatch_queue_create("raytracing_queue_7", DISPATCH_QUEUE_CONCURRENT),
        dispatch_queue_create("raytracing_queue_8", DISPATCH_QUEUE_CONCURRENT),
        dispatch_queue_create("raytracing_queue_9", DISPATCH_QUEUE_CONCURRENT),
    };
    dispatch_queue_t* queues = executeQueue;
    dispatch_queue_t dispatch_mission_queue = dispatch_queue_create("raytracing_dispatch_queue", DISPATCH_QUEUE_CONCURRENT);
    
    hitable *world = random_scene();
    hitable_list* hitable_world = dynamic_cast<hitable_list*>(world);
    set_up_position(hitable_world);
    
    vec3 lookfrom(13, 2, 0);
    vec3 lookat(0, 0, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;
    
    __block camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(width) / float(height), aperture, dist_to_focus);
    dispatch_async(dispatch_mission_queue, ^{
        for (int j = height-1; j >= 0; j--) {
            for (int i = 0; i < width; i++) {
                dispatch_queue_t queue = queues[i % 10];
                dispatch_async(queue, ^{
                    vec3 col(0, 0, 0);
                    for (int s=0; s < ns; s++) {
                        float u = float(i + drand48()) / float(width);
                        float v = float(j + drand48()) / float(height);
                        const ray r = cam.get_ray(u, v);
                        //vec3 p = r.point_at_parameter(2.0);
                        col += color(r, world, 0);
                    }
                    col /= float(ns);
                    col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
                    int ir = int(255.99*col[0]);
                    int ig = int(255.99*col[1]);
                    int ib = int(255.99*col[2]);
                    pixels[j * 640 * 3 + i * 3 + 0] = ir;
                    pixels[j * 640 * 3 + i * 3 + 1] = ig;
                    pixels[j * 640 * 3 + i * 3 + 2] = ib;
                    
                });
            }
        }
    });
    
    char *argv [1];
    int argc = 1;
    argv[0] = strdup("RayTracing");
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(INITIAL_X_POS, INITIAL_Y_POS);
    glutCreateWindow("Ray Tracing");
    glutDisplayFunc(loop);
    glutReshapeFunc(reshape);
    glutIdleFunc(update);
    glutMainLoop();
}

void play_ray_tracing_animation(int fromFrame){
    load(0);
    is_animation = true;
    char *argv [1];
    int argc = 1;
    argv[0] = strdup("RayTracing");
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(INITIAL_X_POS, INITIAL_Y_POS);
    glutCreateWindow("Ray Tracing");
    glutDisplayFunc(loop);
    glutReshapeFunc(reshape);
    glutIdleFunc(update);
    glutMainLoop();
}

void store_ray_tracing_animation(){
    int width  = INITIAL_WIDTH;
    int height = INITIAL_HEIGHT;
    int ns = 10;
    is_animation = false;
    hitable *world = random_scene();
    hitable_list* hitable_world = dynamic_cast<hitable_list*>(world);
    set_up_position(hitable_world);
    
    vec3 lookfrom(13, 2, 0);
    vec3 lookat(0, 0, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;
    
    __block camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(width) / float(height), aperture, dist_to_focus);
    
    for(int frame = 120; frame < 20 * 8; frame ++){
        real_time_position(hitable_world, frame);
        
        for (int j = height-1; j >= 0; j --) {
            for (int i = 0; i < width; i ++) {
                vec3 col(0, 0, 0);
                for (int s = 0; s < ns; s ++) {
                    float u = float(i + drand48()) / float(width);
                    float v = float(j + drand48()) / float(height);
                    const ray r = cam.get_ray(u, v);
                    //vec3 p = r.point_at_parameter(2.0);
                    col += color(r, world,0);
                }
                col /= float(ns);
                col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
                
                int ir = int(255.99 * col[0]);
                int ig = int(255.99 * col[1]);
                int ib = int(255.99 * col[2]);
                
                pixels[j * 640 * 3 + i * 3 + 0] = ir;
                pixels[j * 640 * 3 + i * 3 + 1] = ig;
                pixels[j * 640 * 3 + i * 3 + 2] = ib;
                
                
            }
        }
        string fileName = "newframe" + to_string(frame);
        save(frame);
        cout << fileName << " FINISHED!! "<< endl;
    }
}


int main(int argc, char ** argv)  {
    // 1. Using multiprocess to optimize ray tracing
    // !!! Since this project use Grand Central Dispatch(GCD), this project cannot run on Windows System
    // optimized_ray_tracing();
    
    // 2. Play stored ray tracing animation
    play_ray_tracing_animation(0);
    
    // 3. Store New ray tracing animation
    // Need to set file position
    // store_ray_tracing_animation();
    return 0;
}



