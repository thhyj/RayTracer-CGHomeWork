//
// Created by thhyj on 2021/1/4.
//

#ifndef RAYTRACING_AABB_H
#define RAYTRACING_AABB_H
#include "Ray.h"
class aabb {
public:
    aabb() {}
    aabb(const Point3f &a, const Point3f &b) {
        maximum = b;
        minimum = a;
    }
    Point3f max() const {return maximum;}
    Point3f min() const {return minimum;}
    bool hit(const Ray &r, double tMin, double tMax) const {
        for(int a = 0; a < 3; ++a) {
            double inv = 1.0 / r.getDir()[a];
            double t0 = (min()[a] - r.getOrigin()[a]) * inv;
            double t1 = (max()[a] - r.getOrigin()[a]) * inv;
            if(inv < 0.0) {
                std::swap(t0, t1);
            }
            tMin = std::fmax(t0, tMin);
            tMax = std::fmin(t1, tMax);
            if(tMax <= tMin) return false;
        }
        return true;
    }

private:
    Point3f maximum, minimum;
};
aabb surroundingBox(const aabb &box0, const aabb &box1) {
    Point3f left(fmin(box0.min().x, box1.min().x),
                 fmin(box0.min().y, box1.min().y),
                 fmin(box0.min().z, box1.min().z)
    );
    Point3f right(fmax(box0.max().x, box1.max().x),
                  fmax(box0.max().y, box1.max().y),
                  fmax(box0.max().z, box1.max().z)
    );
    return aabb(left, right);
}


#endif //RAYTRACING_AABB_H
