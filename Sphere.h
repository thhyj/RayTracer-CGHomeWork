//
// Created by thhyj on 2020/12/18.
//

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H
#include "Collide.h"
#include "Material.h"

/**
 * 球体光线碰撞
 */
struct Sphere:Collisible {
    std::shared_ptr<Material> material;
    Point3f center;
    double radius;
    Sphere():center(), radius() {}
    Sphere(const Point3f & center, double radius, std::shared_ptr<Material> material):center(center),radius(radius), material(material){}
    /**
     * 求解与球体的碰撞
     * 求解方程(A + tb - C)* (A + tb - C) = r^2;
     * 即t^2*b^2 + 2tb * (A - C) + (A-C)^2 - r^2 = 0;
     * 即A = b^2
     *  B = 2 * b * (A-C)
     *  C = (A-C)^2 -r ^2
     *  由于规定了方向向量为单位向量，因此A始终为1
     * @param r 检测光线
     * @param tMin 当tMin < t < tMax时才记录
     * @param tMax 当tMin < t < tMax时才记录
     * @param rec  碰撞结果记录在rec中
     * @return 返回是否碰撞
     */
    bool collide(const Ray &r, double tMin, double tMax, CollideRecord &rec) const override{
        Vector3f p = r.getOrigin() - center;
        double b = dot(r.getDir(), p);
        double c = p.length2() - radius * radius;
        double delta = b * b - c;
        if (delta > 0) {
            double t = (-b - sqrt(delta));
            if (tMin < t && t < tMax) {
                rec.t = t;
                rec.p = r.get(t);
               // rec.normal = (rec.p - center).normalize();
                rec.material = material;
                rec.setFaceNormal(r, (rec.p - center) / radius);
                return true;
            } else {
                t = (-b + sqrt(delta));
                if (tMin < t && t < tMax) {
                    rec.t = t;
                    rec.p = r.get(t);
                    // rec.normal = (rec.p - center).normalize();
                    rec.material = material;
                    rec.setFaceNormal(r, (rec.p - center) / radius);
                    return true;
                }
            }
        }
        return false;
    }

    bool boundingBox(double time0, double time1, aabb& outputBox) const override {
        outputBox = aabb(
                center - Vector3f(radius, radius, radius),
                center + Vector3f(radius, radius, radius)
                );
        return true;
    }
};
#endif //RAYTRACING_SPHERE_H
