//
// Created by thhyj on 2021/1/7.
//

#ifndef RAYTRACING_TEXTURE_H
#define RAYTRACING_TEXTURE_H
#include "Vector.h"
class Texture {
public:
    virtual Color3f value(double u, double v, const Point3f& p) const = 0;
};
class SolidColor : public Texture {
private:
    Color3f color;
public:
    SolidColor(){}
    SolidColor(Color3f color):color(color){}
    SolidColor(double r, double g, double b):SolidColor(Color3f(r,g,b)){}
    virtual Color3f value(double u, double v, const Point3f& p) const override {
        return color;
    }

};

class checkerTexture : public Texture {
public:
    checkerTexture() {}

    checkerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
            : even(even), odd(odd) {}

    checkerTexture(Color3f c1, Color3f c2)
            : even(std::make_shared<SolidColor>(c1)) , odd(std::make_shared<SolidColor>(c2)) {}

    virtual Color3f value(double u, double v, const Point3f& p) const override {
        auto sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

public:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;
};

#endif //RAYTRACING_TEXTURE_H
