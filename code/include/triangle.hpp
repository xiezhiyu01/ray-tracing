#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;

// TODO: implement this class and add more fields as necessary,
class Triangle: public Object3D {

public:
	Triangle() = delete;

    // a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->normal = Vector3f::cross(b-a, c-a).normalized();
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
        // return false;
		Vector3f E1 = a - b, E2 = a - c, S = a - ray.getOrigin();
		float div = Matrix3f(ray.getDirection(), E1, E2).determinant();
		if(div == 0) return false;
		float t = Matrix3f(S, E1, E2).determinant() / div;
		float beta = Matrix3f(ray.getDirection(), S, E2).determinant() / div;
		float gamma = Matrix3f(ray.getDirection(), E1, S).determinant() / div;
		float alpha = 1.0 - beta - gamma;
		if(0 <= alpha <= 1 && 0 <= beta <= 1 && 0 <= gamma <= 1) {
			if(t < 0 || t < tmin || t > hit.getT()) return false;
			if(Vector3f::dot(this->normal, ray.getDirection()) > 0)
				this->normal = -this->normal;
			hit.set(t, this->material, this->normal);
			return true;
		}
		return false;
	}
	Vector3f normal;
	Vector3f vertices[3]; // 不需要？？
protected:
	Vector3f a, b, c;
};

#endif //TRIANGLE_H
