#ifndef SCENE_H
#define SCENE_H

#include "sphere.hpp"
#include "radiance.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "mesh.hpp"

double M_PI = 3.1415926535;
double M_1_PI = 1.0 / M_PI;
double erand48(unsigned short xsubi[3]) {
    return (double)rand() / (double) RAND_MAX;
}


struct Scene {
    Renderer* renderer;
    Camera* camera;
    Group* group;
};

Renderer* smallpt = new SmallPT();

std::vector<Object3D*> group00 = {
    new Sphere(Vector3f(1e5+1,  40.8,     81.6    ), 1e5, new Material(Vector3f(),    Vector3f(.75, .25, .25), DIFF)), // LEFT
    new Sphere(Vector3f(-1e5+99,40.8,     81.6    ), 1e5, new Material(Vector3f(),    Vector3f(.25, .25, .75), DIFF)), // RIGHT
    new Sphere(Vector3f(50,     40.8,     1e5     ), 1e5, new Material(Vector3f(),    Vector3f(.75, .75, .75), DIFF)), // BACK
    new Sphere(Vector3f(50,     40.8,     -1e5+170), 1e5, new Material(Vector3f(),    Vector3f(),              DIFF)), // FRONT
    new Sphere(Vector3f(50,     1e5,      81.6    ), 1e5, new Material(Vector3f(),    Vector3f(.75, .75, .75), DIFF)), // BOTTOM
    new Sphere(Vector3f(50,     -1e5+81.6,81.6    ), 1e5, new Material(Vector3f(),    Vector3f(.75, .75, .75), DIFF)), // TOP
    new Sphere(Vector3f(27,      16.5,    47      ), 16.5, new Material(Vector3f(),           Vector3f(.999, .999, .999), SPEC)), //MIRROR
    new Sphere(Vector3f(73,      16.5,    78      ), 16.5, new Material(Vector3f(),           Vector3f(.999, .999, .999), REFR)), //GLASS
    new Sphere(Vector3f(50,      681.6-.27,  81.6    ), 600,  new Material(Vector3f(12, 12, 12), Vector3f(),              DIFF)), //LITE
};

Scene scene00 = {
    smallpt,
    new PerspectiveCamera(Vector3f(50, 52, 295.6), Vector3f(0, 0, -1), Vector3f(0, 1, 0), 1024, 768, 30. * M_PI / 180),
    // 第二行的意思没有完全看懂
    new Group(group00)
};

std::vector<Scene*> scenes = {
    &scene00,
};


#endif