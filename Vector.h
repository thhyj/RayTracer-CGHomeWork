//
// Created by thhyj on 2020/12/8.
//

#ifndef RAYTRACING_VECTOR_H
#define RAYTRACING_VECTOR_H



#include <cmath>
#include <iostream>
/**
 * 三维向量类
 * 因为只是光线追踪的demo，不需要四维齐次坐标，因此只写了三维形式提高程序运行效率
 */
struct Vector3f {
    union {
        double a[3];
        struct {
            double r, g, b;
        };
        struct {
            double x, y, z;
        };
    };
    Vector3f():a(){}
    Vector3f(double x, double y, double z):x(x), y(y), z(z) {
    }
    Vector3f(double *a) {
        this->a[0] = a[0];
        this->a[1] = a[1];
        this->a[2] = a[2];
    }
    const Vector3f &operator+()const{
        return *this;
    }
    Vector3f operator - () const {
        return Vector3f(-x, -y, -z);
    }
    double operator[](int i) const{
        return a[i];
    }
    double &operator[](int i){
        return a[i];
    }

    Vector3f operator + (const Vector3f &rhs) const{
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    Vector3f operator - (const Vector3f &rhs) const{
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    Vector3f &operator +=(const Vector3f &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    Vector3f &operator -=(const Vector3f &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vector3f operator*(const Vector3f &rhs) const{
        return Vector3f{x * rhs.x, y * rhs.y, z * rhs.z};
    }

    Vector3f &operator*=(const Vector3f &rhs) {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }

    Vector3f operator*(double t) const {
        return Vector3f{x * t, y * t, z * t};
    }

    friend Vector3f operator*(double t, const Vector3f& rhs) {
        return Vector3f{t * rhs.x, t * rhs.y, t * rhs.z};
    }

    Vector3f &operator*=(double t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    Vector3f operator/(const Vector3f &rhs) const{
        return Vector3f{x / rhs.x, y / rhs.y, z / rhs.z};
    }

    Vector3f &operator/=(const Vector3f &rhs) {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }

    Vector3f operator/(double t) const{
        return Vector3f{x / t, y / t, z / t};
    }

    Vector3f &operator/=(double t) {
        x /= t;
        y /= t;
        z /= t;
        return *this;
    }



    double length() const {
        return sqrt(x * x + y * y + z * z);
    }
    double length2() const {
        return x * x + y * y + z * z;
    }

    Vector3f &normalize() {
        double inv = 1.0f / length();
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    Vector3f getNormalizedVector() const {
        return Vector3f(*this).normalize();
    }

    friend std::istream &operator>>(std::istream &is, Vector3f &vec) {
        is>>vec.x>>vec.y>>vec.z;
        return is;
    }

    double dot(const Vector3f &rhs) const{
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    Vector3f cross(const Vector3f &b) const{
        return {y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x};
    }
    void print() const{
        std:: cout <<x<< " "<<y<< " "<<z<< "\n";
    }
};
double dot(const Vector3f &a, const Vector3f &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

Vector3f cross(const Vector3f &a, const Vector3f &b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

using Color3f = Vector3f;
using Point3f = Vector3f;
#endif //RAYTRACING_VECTOR_H