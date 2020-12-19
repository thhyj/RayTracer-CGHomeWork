//
// Created by thhyj on 2020/12/18.
//

#ifndef RAYTRACING_RANDOM_H
#define RAYTRACING_RANDOM_H
#include "Vector.h"
#include "Ray.h"
#include <Random>
/**
 * 生成单位球面上的向量
 * @return
 */
Vector3f getRandomUnit() {
 /*   static std::uniform_real_distribution<double> q(-1,1);
    static std::mt19937 engin;
    while (true) {
        auto p = Vector3f(q(engin),q(engin),q(engin));
        if (p.length2() >= 1) continue;
        return p.normalize();
    }*/
    static Vector3f p;
    static double alpha, beta;
    static const double pi = acos(-1);
    static std::uniform_real_distribution<double> qwq(0.0f, 2 * pi);
    static std::mt19937 engine;
    alpha = qwq(engine), beta = qwq(engine);
    return Vector3f(sin(alpha) * cos(beta), sin(alpha) * sin(beta), cos(beta));
}

double getRandomdouble() {
    static std::uniform_real_distribution<double> qwq(0.0f, 1.0);
    static std::mt19937 engine;
    return qwq(engine);
}
#endif //RAYTRACING_RANDOM_H
