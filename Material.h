//
// Created by thhyj on 2020/12/18.
//
#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H
#include "Collide.h"
struct Material {
    virtual bool scatter(const Ray &rayIn, const CollideRecord &rec, Vector3f &attenuation, Ray &scattered) const = 0;
    virtual Color3f emitted(double u, double v, const Point3f &p) const {
        return Color3f(0, 0, 0);
    };
    virtual ~Material(){};
};
#endif //RAYTRACING_MATERIAL_H
