#ifndef RADIANCE_H
#define RADIANCE_H

#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "material.hpp"

class Renderer {
public:
    virtual void render(Image* image, Camera* camera, Group* group);
};

Vector3f convertPPM(Vector3f r) {
    return Vector3f(std::pow(r.x(), 1./2.2),
                    std::pow(r.y(), 1./2.2),
                    std::pow(r.z(), 1./2.2));
}

class SmallPT: public Renderer {
public:
    void render(Image* image, Camera* camera, Group* group) {
        int samps = 50; // hardcode 采样数
        int w = camera->getWidth(), h = camera->getHeight();
        Vector3f r;
        #pragma omp parallel for schedule(dynamic, 1) private(r)
        // LOOP OVER ALL IMAGE PIXELS
        for(int y = 0; y < h; y++) { // Loop over image rows
            fprintf(stderr, "\rendering (%d spp) %5.2f%%", samps*4, 100.*y/(h-1));
            unsigned short Xi[3] = {0, 0, y*y*y};
            // for(unsigned short x = 0; x < w; x++) // Loop columns
            //     // for each pixel do 2*2 subsamples, and samps samples per subsample
            //     for(int sy = 0; sy < 2; sy++) // 2*2 subpixel rows
            //         for(int sx = 0; sx < 2; sx++, r=Vector3f()) { //2*2 subpixel cols
            //             for(int s = 0; s < samps; s++) {
            //                 // tent filter
            //                 double r1 = 2*erand48(Xi), dx = r1 < 1 ? sqrt(r1)-1: 1-sqrt(2-r1);
            //                 double r2 = 2*erand48(Xi), dy = r2 < 1 ? sqrt(r2)-1: 1-sqrt(2-r2);
            //                 Vector3f dir = cx*((()))
            //             }
                        
            //         }
            for(unsigned short x = 0; x < w; x++) { // Loop columns
                r = Vector3f(); // init
                for(int s = 0; s < samps; s++) {
                    r = r + radiance(camera->generateRay(Vector2f(x, y)), group, 0, Xi) * (1. / samps);
                }
                // clamp already in savePPM.
                image->SetPixel(x, y, convertPPM(r));
            }
        }
    }
protected:
    Vector3f radiance(const Ray &ray, Group *group, int depth, unsigned short *Xi) {
        // depth: ray depth ; Xi : random number seed
        Hit hit;
        if(!group->intersect(ray, hit, 0)) return Vector3f(); // tmin=0 不相交返回空

        if(depth>10) return Vector3f(); // maxDepth hardcode here. 最高递归层数
        
        Vector3f x = ray.getOrigin() + ray.getDirection() * hit.getT();
        Vector3f normal = hit.getNormal();
        Vector3f oriented_normal = Vector3f::dot(normal, ray.getDirection())<0 ? normal : -normal;
        Material* material = hit.getMaterial();
        Vector3f f = material->getColor(); // object_color
        // to be changed if texture added

        // russian roulette: stop the recursion randomly
        double p = f.x()>f.y()&&f.x()>f.z()?f.x():f.y()>f.z()?f.y():f.z(); //max refl
        if(++depth>5||!p) if(erand48(Xi)<p) f = f * (1/p); else return material->getEmit();

        // DIFF
        if(material->getReflect_type() == DIFF) {
            double r1 = 2*M_PI*erand48(Xi); // random angle
            double r2 = erand48(Xi), r2s = sqrt(r2); // random dist from center
            Vector3f w = oriented_normal;
            Vector3f u = Vector3f::cross((std::fabs(w.x())>.1?Vector3f(0,1,0):Vector3f(1,0,0)), w).normalized();
            Vector3f v = Vector3f::cross(w, u);
            Vector3f d = (u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2)).normalized();
            return material->getEmit() + f * radiance(Ray(x, d), group, depth, Xi);
        }
        // SPEC
        else if(material->getReflect_type() == SPEC) {
            return material->getEmit() + f * radiance(Ray(x, ray.getDirection() - 
                    normal * 2 * Vector3f::dot(normal, ray.getDirection())), group, depth, Xi);
        }
        // REFR
        else {
            Ray reflRay(x, ray.getDirection() - normal * 2 * Vector3f::dot(normal, ray.getDirection()));
            bool into = Vector3f::dot(normal, oriented_normal) > 0;
            double nc = 1, nt = 1.5, nnt = into ? nc/nt : nt/nc;
            double ddn = Vector3f::dot(ray.getDirection(), oriented_normal), cos2t;
            if((cos2t = 1 - nnt*nnt*(1-ddn*ddn)) < 0)
                return material->getEmit() + f * radiance(reflRay, group, depth, Xi);
            Vector3f tdir = (ray.getDirection()*nnt - normal*((into?1:-1)*(ddn*nnt+sqrt(cos2t)))).normalized();
            double a = nt - nc, b = nt + nc, R0 = a*a/(b*b), c = 1 - (into?-ddn:Vector3f::dot(tdir, normal));
            double Re = R0+(1-R0)*c*c*c*c*c, Tr = 1-Re, P = .25+.5*Re, RP = Re/P, TP = Tr/(1-P);
            return material->getEmit() + f * (depth > 2 ? (erand48(Xi) < P ?
                radiance(reflRay,group,depth,Xi)*RP:radiance(Ray(x,tdir),group,depth,Xi)*TP) : 
                radiance(reflRay,group,depth,Xi)*Re+radiance(Ray(x,tdir),group,depth,Xi)*Tr); 
        }
    }
};

#endif