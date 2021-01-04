//
// Created by thhyj on 2020/12/18.
//

#ifndef RAYTRACING_RANDOM_H
#define RAYTRACING_RANDOM_H
#include "Vector.h"
#include "Ray.h"
#include <random>
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

double getRandomdouble(double min, double max) {
    return min + (max - min) * getRandomdouble();
}

uint64_t shuffle_table[4];
// The actual algorithm
uint64_t next(void) {
    uint64_t s1 = shuffle_table[0];
    uint64_t s0 = shuffle_table[1];
    uint64_t result = s0 + s1;
    shuffle_table[0] = s0;
    s1 ^= s1 << 23;
    shuffle_table[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
    return result;
}
int getRandomInt(int min, int max) {
    return next() % (max - min + 1) + min;
}

Vector3f getRandomVector() {
    return Vector3f(getRandomdouble(), getRandomdouble(), getRandomdouble());
}

Vector3f getRandomVector(double min, double max) {
    return Vector3f(getRandomdouble(min, max), getRandomdouble(min, max), getRandomdouble(min, max));
}


#endif //RAYTRACING_RANDOM_H
