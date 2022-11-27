#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>


class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
        this->radius = 1.0;
        this->center = Vector3f(1.0);
    }

    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material) {
        this->center = center;
        this->radius = radius;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        // 射线原点为O，射线方向OD，球的中心为C，垂足为P，交点为R
        Vector3f O = r.getOrigin(), C = this->center;
        Vector3f OD = r.getDirection().normalized(), OC = C - O;
        float disOP = Vector3f::dot(OD, OC);
        if(OC.length() > this->radius && disOP < 0)
            return false;
        float disCP = sqrt(OC.squaredLength() - disOP * disOP);
        if(disCP > this->radius)
            return false;
        float disPR = sqrt(this->radius * this->radius - disCP * disCP);
        float t;
        if(OC.length() > this->radius) t = disOP - disPR;
        else t = disOP + disPR;
        if(t < tmin || t > h.getT()) return false;
        Vector3f R = O + t * OD;
        h.set(t, this->material, (R - C).normalized());
        return true;
    }

protected:
    Vector3f center;
    float radius;
};


#endif
