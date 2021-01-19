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
std::atomic_int id{0};
const int threadNumber = 16;
double alpha[threadNumber], bet[threadNumber];
std::uniform_real_distribution<double> qwq[threadNumber], qaq[threadNumber];
std::mt19937 engine[threadNumber];


void randomInit() {
    static const double pi = acos(-1);
    for(int i = 0; i < threadNumber; ++i) {
        qwq[i] = std::uniform_real_distribution<double>(0.0, 2 * pi);
        engine[i] = std::mt19937(i);
        qaq[i] = std::uniform_real_distribution<double>(0.0, 1.0);
    }
}
Vector3f getRandomUnit() {

    ++id;
    int nowId = id % threadNumber;
    if(id > 1000000000) {
        id -= 1000000000;
    }

    alpha[nowId] = qwq[nowId](engine[nowId]), bet[nowId] = qwq[nowId](engine[nowId]);
    return Vector3f(sin(alpha[nowId]) * cos(bet[nowId]), sin(alpha[nowId]) * sin(bet[nowId]), cos(bet[nowId]));
}
double getRandomdouble() {
    ++id;
    int nowId = id % threadNumber;
    if(id > 1000000000) {
        id -= 1000000000;
    }
    return qaq[nowId](engine[nowId]);
}

double getRandomdouble(double min, double max) {
    return min + (max - min) * getRandomdouble();
}

uint64_t shuffle_table[threadNumber][4];
// The actual algorithm
uint64_t next(void) {
    ++id;
    int nowId = id % threadNumber;
    if(id > 1000000000) {
        id -= 1000000000;
    }
    uint64_t s1 = shuffle_table[nowId][0];
    uint64_t s0 = shuffle_table[nowId][1];
    uint64_t result = s0 + s1;
    shuffle_table[nowId][0] = s0;
    s1 ^= s1 << 23;
    shuffle_table[nowId][1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
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
