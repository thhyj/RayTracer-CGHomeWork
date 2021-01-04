//
// Created by thhyj on 2020/12/18.
//

#ifndef RAYTRACING_COLLISIBLELIST_H
#define RAYTRACING_COLLISIBLELIST_H
#include <vector>
#include <memory>
#include "Collide.h"
#include "AABB.h"
struct CollisibleList: Collisible {
    std::vector<std::shared_ptr<Collisible>>collisibleList;

    void add(std::shared_ptr<Collisible> x) {
        collisibleList.emplace_back(x);
    }

    bool collide(const Ray &r, double tMin, double tMax, CollideRecord &rec) const override{
        bool flag = false;
        double closest = tMax;
        CollideRecord tempRec;
        for(const auto &i : collisibleList) {
            if(i->collide(r, tMin, closest, tempRec)) {
                rec = tempRec;
                closest = tempRec.t;
                flag = true;
            }
        }
        return flag;
    }
    bool boundingBox(double time0, double time1, aabb& outputBox) const override {
        if(collisibleList.empty()) return false;
        aabb tempBox;
        bool firstBox = true;
        for (const auto& collisibleObject : collisibleList) {
            if(!collisibleObject->boundingBox(time0, time1, tempBox)) return false;
            outputBox = firstBox ? tempBox : surroundingBox(tempBox, outputBox);
            firstBox = false;
        }
        return true;
    }
};
#endif //RAYTRACING_COLLISIBLELIST_H
