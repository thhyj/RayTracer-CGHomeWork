//
// Created by thhyj on 2020/12/18.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H
#include "Vector.h"
#include "Ray.h"
#include "Random.h"
const double pi = acos(-1);
struct Camera {
    Vector3f horizontal, vertical, u, v, w;
    Point3f origin,lowerLeftCorner;
    double lensRadius;
    Camera(const Point3f &origin, const Point3f &lowerLeftCorner, const Vector3f &horizontal,
           const Vector3f &vertical)
            : origin(origin),
              lowerLeftCorner(lowerLeftCorner),
              horizontal(horizontal),
              vertical(vertical) {}
    Camera(Point3f lookFrom, Point3f lookAt, Vector3f up, float fov,
           float aspectRatio, float aperture, float focusDis) {
        init(lookFrom, lookAt, up, fov, aspectRatio, aperture, focusDis);
    }
    void init(Point3f lookFrom, Point3f lookAt, Vector3f up, float fov,
              float aspectRatio, float aperture, float focusDis) {
        double theta = fov * pi / 180.0;
        double h = tan(theta / 2);
        double viewportHeight = 2.0 * h;
        double viewportWidth = aspectRatio * viewportHeight;
        double focalLength = 1;
        //std::cout << "caonima";
        w = std::move((lookFrom - lookAt).getNormalizedVector());
        u = std::move(cross(up, w).getNormalizedVector());
        v = std::move(cross(w, u));

        origin = lookFrom;
        horizontal = focusDis * viewportWidth * u;
        vertical = focusDis * viewportHeight * v;
        lowerLeftCorner = origin - horizontal / 2.0 - vertical / 2.0 - w * focusDis;
        lensRadius = aperture / 2;
    }
    Ray getRay(double s, double t) const{
        Vector3f r = lensRadius * getRandomUnit();
        Vector3f offset = u * r.x + v * r.y;
        return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
     //   return Ray(origin , lowerLeftCorner + s * horizontal + t * vertical - origin);
    }
};
#endif //RAYTRACING_CAMERA_H
