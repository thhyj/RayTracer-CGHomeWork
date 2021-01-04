//
// Created by thhyj on 2020/12/18.
//

#ifndef RAYTRACING_GLASS_H
#define RAYTRACING_GLASS_H
#include "Material.h"
#include "Random.h"
struct Glass : public Material {
    Vector3f albedo;
    double ir;
    Glass(double ir, Vector3f albedo = {1,1,1}):ir(ir),albedo(albedo) {}

    static Vector3f refract(const Vector3f &v, const Vector3f &n, double ratio) {
        double cosTheta = fmin(dot(-v, n), 1.0);
        Vector3f rOutPerpendicular =  ratio * (v + cosTheta*n);
        Vector3f rOutParallel = -sqrt(fabs(1.0 - rOutPerpendicular.length2())) * n;
        return rOutParallel + rOutPerpendicular;
    }
    static Vector3f reflect(const Vector3f &v, const Vector3f &n) {
        return v - 2 * n * dot(v, n);
    }
    bool scatter(const Ray &rayIn, const CollideRecord &rec, Vector3f &attenuation, Ray &scattered) const override{
        double refractionRatio = rec.frontFace ? (1.0/ir) : ir;
        Vector3f &&refracted = refract(rayIn.getDir(), rec.normal, refractionRatio);
        double cosTheta = fmin(dot(-rayIn.getDir(), rec.normal), 1.0);
        double sinTheta = sqrt(1.0 - cosTheta * cosTheta);
        //如果不能折射，那就反射(写的时候这里手抖了打错了一个变量，debug了好久。。。
        if(refractionRatio * sinTheta > 1.0 || reflectance(cosTheta, refractionRatio) > getRandomdouble()) {
            scattered = Ray(rec.p, reflect(rayIn.getDir(), rec.normal), rayIn.time);
        } else {
            scattered = Ray(rec.p, refract(rayIn.getDir(), rec.normal, refractionRatio), rayIn.time);
        }
        attenuation = albedo;
        return true;
    }
    // Use Schlick's approximation
    static double reflectance(double cosine, double ratio) {

        double r0 = (1 - ratio) / (1 + ratio);
        r0 = r0*r0;
        return r0 + (1 - r0) * pow((1 - cosine),5);
    }
};
#endif //RAYTRACING_GLASS_H
