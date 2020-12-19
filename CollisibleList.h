//
// Created by thhyj on 2020/12/18.
//

#ifndef RAYTRACING_COLLISIBLELIST_H
#define RAYTRACING_COLLISIBLELIST_H

#include <vector>
#include "Collide.h"
struct CollisibleList: Collisible {
    std::vector<Collisible *>collisibleList;

    void add(Collisible *x) {
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

};
#endif //RAYTRACING_COLLISIBLELIST_H
