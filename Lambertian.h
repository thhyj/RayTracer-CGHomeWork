//
// Created by thhyj on 2020/12/18.
//

#ifndef RAYTRACING_LAMBERTIAN_H
#define RAYTRACING_LAMBERTIAN_H
#include "Material.h"
#include "Random.h"
struct Lambertian : public Material {
    Vector3f albedo;
    Lambertian(Vector3f albedo):albedo(albedo) {}
    bool scatter(const Ray &rayIn, const CollideRecord &rec, Vector3f &attenuation, Ray &scattered) const override{
        scattered = Ray(rec.p, rec.normal + getRandomUnit(), rayIn.time);
        attenuation = albedo;
        return true;
    }
};
#endif //RAYTRACING_LAMBERTIAN_H
