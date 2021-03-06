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
    static double pi;
    std::shared_ptr<Material> material;
    Point3f center;
    double radius;
    double rotationHorizontal, rotationVertical;
    Sphere():center(), radius() {}
    Sphere(const Point3f & center, double radius, std::shared_ptr<Material> material,
           double rotationHorizontal = 0, double rotationVertical = 0):
           center(center),radius(radius), material(material),
           rotationHorizontal(rotationHorizontal),
           rotationVertical(rotationVertical){}
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

    inline bool getCollideResult(const Ray &r, double tMin, double tMax, CollideRecord &rec,const Point3f center) const {
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
                Vector3f &&outwardNormal = (rec.p - center) / radius;
                rec.setFaceNormal(r, outwardNormal);
                getSphereUV(outwardNormal, rec.u, rec.v);
                return true;
            } else {
                t = (-b + sqrt(delta));
                if (tMin < t && t < tMax) {
                    rec.t = t;
                    rec.p = r.get(t);
                    // rec.normal = (rec.p - center).normalize();
                    rec.material = material;
                    Vector3f &&outwardNormal = (rec.p - center) / radius;
                    rec.setFaceNormal(r, outwardNormal);
                    getSphereUV(outwardNormal, rec.u, rec.v);
                    return true;
                }
            }
        }
        return false;
    }

    bool collide(const Ray &r, double tMin, double tMax, CollideRecord &rec) const override{
        return getCollideResult(r, tMin, tMax, rec,center);
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
                Vector3f &&outwardNormal = (rec.p - center) / radius;
                rec.setFaceNormal(r, outwardNormal);
                getSphereUV(outwardNormal, rec.u, rec.v);
                return true;
            } else {
                t = (-b + sqrt(delta));
                if (tMin < t && t < tMax) {
                    rec.t = t;
                    rec.p = r.get(t);
                    // rec.normal = (rec.p - center).normalize();
                    rec.material = material;
                    Vector3f &&outwardNormal = (rec.p - center) / radius;
                    rec.setFaceNormal(r, outwardNormal);
                    getSphereUV(outwardNormal, rec.u, rec.v);
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

    void getSphereUV(const Point3f &p, double &u, double &v) const {
        double theta = acos(-p.y);
        double phi = atan2(-p.z, p.x) + pi;
      //  std::cout << "before phi = " << phi << '\n';
     //   std::cout <<"rotationHorizontal = " << rotationHorizontal << '\n';
        phi += rotationHorizontal / (180.0) * pi;
     //   std::cout << "after phi = " << phi << '\n';
        if(phi > 2 * pi) {
            phi -= 2 * pi;
        }
        if(phi < 0) {
            phi += 2 * pi;
        }
  //      std::cout << "before theta = " << theta << '\n';
   //     std::cout <<"rotationVertical = " << rotationVertical << '\n';
        theta += rotationVertical / (180.0) * pi;
   //    std::cout << "after theta = " << theta << '\n';
        if(theta > pi) {
            theta -= pi;
        }
        if(theta < 0) {
            theta+=pi;
        }
        u = phi / (2 * pi);
        v = theta / pi;
    }

};
double Sphere::pi = acos(-1);
#endif //RAYTRACING_SPHERE_H