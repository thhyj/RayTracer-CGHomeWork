//
// Created by thhyj on 2020/12/18.
//
#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H
#include "Collide.h"
struct Material {
    virtual bool scatter(const Ray &rayIn, const CollideRecord &rec, Vector3f &attenuation, Ray &scattered) const = 0;
    virtual ~Material(){};
};
#endif //RAYTRACING_MATERIAL_H
