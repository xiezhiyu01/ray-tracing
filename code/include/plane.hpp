#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D {
public:
    Plane() {

    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        this->normal = normal;
        this->d = d; // d的正负？
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float dot_n_Rd = Vector3f::dot(this->normal, r.getDirection());
        float t = -(this->d + Vector3f::dot(this->normal, r.getOrigin())) / dot_n_Rd;
        if(t < 0 || t < tmin || t > h.getT()) return false;
        if(dot_n_Rd > 0) h.set(t, this->material, -this->normal);
        else h.set(t, this->material, this->normal);
        return true;
    }

protected:
    Vector3f normal;
    float d;
};

#endif //PLANE_H
		

