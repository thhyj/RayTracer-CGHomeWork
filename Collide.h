//
// Created by thhyj on 2020/12/18.
//

#ifndef RAYTRACING_COLLIDE_H
#define RAYTRACING_COLLIDE_H
#include "Vector.h"
#include "Ray.h"
//#include "Material.h"
struct Material;
/**
 * 记录光线碰撞的点，法向量和t
 */
struct CollideRecord {
    double t;
    Point3f p;
    Vector3f normal;
    bool frontFace;
    Material *material;
    inline void setFaceNormal(const Ray& r, const Vector3f& outwardNormal) {
        frontFace = dot(r.getDir(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal :-outwardNormal;
    }
};
/**
 * 光线碰撞基类
 *
 */
struct Collisible {
    /**
     *
     * @param r 检测光线
     * @param tMin 当tMin < t < tMax时才记录
     * @param tMax 当tMin < t < tMax时才记录
     * @param rec  碰撞结果记录在rec中
     * @return 返回是否碰撞
     */
    virtual bool collide(const Ray &r, double tMin, double tMax, CollideRecord &rec) const = 0;
    virtual ~Collisible(){}
};
#endif //RAYTRACING_COLLIDE_H
