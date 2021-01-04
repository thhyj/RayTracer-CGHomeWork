//
// Created by thhyj on 2021/1/4.
//

#ifndef RAYTRACING_BVH_H
#define RAYTRACING_BVH_H
#include <functional>
#include <algorithm>
#include "Collide.h"
#include "CollisibleList.h"
#include "AABB.h"
#include "Random.h"
using std::placeholders::_1,std::placeholders::_2;
bool boxCompare(const std::shared_ptr<Collisible> a, const std::shared_ptr<Collisible> b, int axis) {
    aabb boxA;
    aabb boxB;

    if (!a->boundingBox(0,0, boxA) || !b->boundingBox(0,0, boxB))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return boxA.min().a[axis] < boxB.min().a[axis];
}

class BVH : public Collisible {
public:
    BVH();
    BVH(const CollisibleList & list, double time0, double time1)
        :BVH(list.collisibleList, 0, list.collisibleList.size(), time0, time1) {}
    BVH(const std::vector<std::shared_ptr<Collisible>>&srcObjects, size_t start, size_t end, double time0, double time1) {
        auto objects = srcObjects;
        int axis = getRandomInt(0, 2);
        auto comparator = [axis](auto && PH1, auto && PH2) { return boxCompare(PH1, PH2, axis); };
        size_t objectSpan = end - start;
        if(objectSpan == 1) {
            left = right = objects[start];
        } else {
            if (objectSpan == 2) {
                if(comparator(objects[start], objects[start + 1])) {
                    left = objects[start];
                    right = objects[start + 1];
                } else {
                    left = objects[start + 1];
                    right = objects[start];
                }
            } else {
                std::sort(objects.begin() + start, objects.begin() + end, comparator);
                auto mid = start + objectSpan / 2;
                left = std::make_shared<BVH>(objects, start, mid,time0, time1);
                right = std::make_shared<BVH>(objects, mid, end, time0, time1);
            }
        }
        aabb boxLeft, boxRight;
        if(!left->boundingBox(time0, time1, boxLeft) || !right->boundingBox(time0, time1, boxRight)) {
            std::cerr << "No bounding box in bvh_node constructor.\n";
        }
        box = surroundingBox(boxLeft, boxRight);
    }
    virtual bool collide(const Ray &r, double tMin, double tMax, CollideRecord& rec) const override{
        if(!box.hit(r, tMin, tMax)) {
            return false;
        }
        bool hitLeft = left->collide(r, tMin, tMax, rec);
        bool hitRight = right->collide(r, tMin, hitLeft ? rec.t :tMax, rec);
        return hitLeft||hitRight;
    }

    virtual bool boundingBox(double time0, double time1, aabb& outputBox) const override {
        outputBox = box;
        return true;
    }
public:
    std::shared_ptr<Collisible> left;
    std::shared_ptr<Collisible> right;
    aabb box;

};
#endif //RAYTRACING_BVH_H
