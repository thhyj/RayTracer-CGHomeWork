//
// Created by 27689 on 2021/1/18.
//

#ifndef RAYTRACING_DIFFUSELIGHT_H
#define RAYTRACING_DIFFUSELIGHT_H
#include "Material.h"

#include <Memory>
#include "Texture.h"
class DiffuseLight : public Material {
public:
    DiffuseLight(std::shared_ptr<Texture> emit):emit(emit){}
    DiffuseLight(Color3f c):emit(std::make_shared<SolidColor>(c)){}
    virtual bool scatter(const Ray &rayIn, const CollideRecord &rec, Vector3f &attenuation, Ray &scattered) const override{
        return false;
    }
    virtual Color3f emitted(double u, double v, const Point3f &p) const {
        return emit->value(u, v, p);
    }
private:
    std::shared_ptr<Texture> emit;
};
#endif //RAYTRACING_DIFFUSELIGHT_H
