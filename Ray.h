//
// Created by thhyj on 2020/12/8.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H
#include "Vector.h"
/**
 * 光线类
 * 由一个起点和方向向量表示一条光线
 */
struct Ray {
    Point3f o;
    Vector3f dir;
    double time;
    Ray() = default;
    Ray(const Point3f &o, const Vector3f &dir, double time = 0.0):o(o), dir(dir), time(time){
        this->dir.normalize();
    }

    const Point3f &getOrigin() const{
        return o;
    }

    const Vector3f &getDir() const{
        return dir;
    }

    Point3f get(double t) const {
        return o + dir * t;
    }
    void print() const{
        std::cout << "origin: ";
        o.print();
        std::cout<<"dir: ";
        dir.print();
    }
};
#endif //RAYTRACING_RAY_H
