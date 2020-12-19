//
// Created by thhyj on 2020/12/18.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H
#include "Vector.h"
#include "Ray.h"
struct Camera {
    Vector3f horizontal, vertical;
    Point3f origin,lowerLeftCorner;
    Camera(const Point3f &origin, const Point3f &lowerLeftCorner, const Vector3f &horizontal,
           const Vector3f &vertical)
            : origin(origin),
              lowerLeftCorner(lowerLeftCorner),
              horizontal(horizontal),
              vertical(vertical) {}

    Ray getRay(double u, double v) {
        return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
    }
};
#endif //RAYTRACING_CAMERA_H
