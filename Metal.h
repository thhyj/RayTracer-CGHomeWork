//
// Created by thhyj on 2020/12/18.
//

#ifndef RAYTRACING_METAL_H
#define RAYTRACING_METAL_H
#include "Material.h"
#include "Random.h"
struct Metal : public Material{
    static Vector3f reflect(const Vector3f &v, const Vector3f &n){
        return v - 2 * n * dot(v, n);
    }
    Vector3f albedo;
    double fuzz;
    Metal(const Vector3f &albedo, double fuzz = 0) : albedo(albedo), fuzz(fuzz){}
    bool scatter(const Ray &rayIn, const CollideRecord &rec, Vector3f &attenuation, Ray &scattered) const override{
        if(fuzz > 1e-6) {
            scattered = Ray(rec.p, reflect(rayIn.getDir(), rec.normal) + fuzz * getRandomUnit());
        } else {
            scattered = Ray(rec.p, reflect(rayIn.getDir(), rec.normal));
        }
        attenuation = albedo;
        return dot(scattered.getDir(), rec.normal) > 0;
    }
};


#endif //RAYTRACING_METAL_H
