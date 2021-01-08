//
// Created by thhyj on 2021/1/1.
//

#ifndef RAYTRACING_MOVINGSPHERE_H
#define RAYTRACING_MOVINGSPHERE_H
#include "Collide.h"

class MovingSphere : public Sphere {
private:
    Point3f center0, center1;
    double time0, time1;

public:
    MovingSphere(Point3f center0, Point3f center1, double time0, double time1, double radius,std::shared_ptr<Material> material):
    center0(center0), center1(center1), time0(time0), time1(time1){
        Sphere::radius = radius;
        Sphere::material = material;
    }

    Point3f getCenter(double time) const{
        return center0 + (time - time0) / (time1 - time0) * (center1 - center0);
    }

    bool collide(const Ray &r, double tMin, double tMax, CollideRecord &rec) const override{
        return getCollideResult(r, tMin, tMax, rec,getCenter(r.time));
        Vector3f p = r.getOrigin() - getCenter(r.time);
        double b = dot(r.getDir(), p);
        double c = p.length2() - radius * radius;
        double delta = b * b - c;
      //  return getCollideResult(b, delta, r, tMin, tMax, rec);
        if (delta > 0) {
            double t = (-b - sqrt(delta));
            if (tMin < t && t < tMax) {
                rec.t = t;
                rec.p = r.get(t);
                // rec.normal = (rec.p - center).normalize();
                rec.material = material;
                rec.setFaceNormal(r, (rec.p - getCenter(r.time)) / radius);
                return true;
            } else {
                t = (-b + sqrt(delta));
                if (tMin < t && t < tMax) {
                    rec.t = t;
                    rec.p = r.get(t);
                    // rec.normal = (rec.p - center).normalize();
                    rec.material = material;
                    rec.setFaceNormal(r, (rec.p - getCenter(r.time)) / radius);
                    return true;
                }
            }
        }
        return false;
    }
    bool boundingBox(double time0, double time1, aabb& outputBox) const override {
        aabb box0 = aabb(
                getCenter(time0) - Vector3f(radius, radius, radius),
                getCenter(time1) + Vector3f(radius,radius,radius)
                );
        aabb box1 = aabb(
                getCenter(time1) - Vector3f(radius, radius, radius),
                getCenter(time1) + Vector3f(radius, radius, radius)
                );
        outputBox = surroundingBox(box0, box1);
        return true;
    }
};

#endif //RAYTRACING_MOVINGSPHERE_H
