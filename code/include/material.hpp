#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>

enum Reflect_type {DIFF, SPEC, REFR}; // reflection type: diffuse, specular, refractive

class Material {
public:

    explicit Material(Vector3f _emit, Vector3f _color, Reflect_type _reflect):
        emit(_emit), color(_color), reflect_type(_reflect) {}

    virtual ~Material() = default;
    
    Reflect_type getReflect_type() {return reflect_type;}
    Vector3f getEmit() {return emit;}
    Vector3f getColor() {return color;}


protected:
    Vector3f emit;
    Vector3f color;
    Reflect_type reflect_type;
};


#endif // MATERIAL_H
