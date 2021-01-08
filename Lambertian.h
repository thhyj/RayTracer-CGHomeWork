//
// Created by thhyj on 2020/12/18.
//

#ifndef RAYTRACING_LAMBERTIAN_H
#define RAYTRACING_LAMBERTIAN_H
#include "Material.h"
#include "Random.h"
#include "Texture.h"
#include <memory>
struct Lambertian : public Material {
    std::shared_ptr<Texture> albedo;
    Lambertian(const Color3f& color):albedo(std::make_shared<SolidColor>(color)) {}
    Lambertian(std::shared_ptr<Texture> a):albedo(a){}
    bool scatter(const Ray &rayIn, const CollideRecord &rec, Vector3f &attenuation, Ray &scattered) const override{
        scattered = Ray(rec.p, rec.normal + getRandomUnit(), rayIn.time);
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
};
#endif //RAYTRACING_LAMBERTIAN_H
