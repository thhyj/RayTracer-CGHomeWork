#include "svpng.inc"
#include <cstdio>
#include <vector>
#include <limits>
#include <random>
#include "Vector.h"
#include "Ray.h"
#include "Collide.h"
#include "Sphere.h"
#include "CollisibleList.h"
#include "Camera.h"
#include "Material.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Glass.h"
const int WIDTH = 640;
const int HEIGHT = 480;
//const int WIDTH = 400;
//const int HEIGHT = 225;
const double pi = acos(-1);
auto viewportHeight = 3.0;
auto viewportWidth = 4.0;
auto focalLength = 1.0;
auto origin = Point3f(0,0,0);
auto horizontal = Vector3f(viewportWidth, 0, 0);
auto vertical = Vector3f(0, viewportHeight, 0);
auto lowerLeftCorner = origin - horizontal/2 - vertical/2 - Vector3f(0, 0, focalLength);
using byte = unsigned char;
/**
 * 以左下角为(0,0)绘制
 * @param d 像素数据
 * @param x 像素x坐标
 * @param y 像素y坐标
 * @param color 颜色向量
 * @param a 透明度
 */
void paint(byte *d, int x, int y, const Color3f &color, double a) {
    d += 4 * ((HEIGHT - y - 1) * WIDTH + x);
    d[0] = color.r * (int)255;
    d[1] = color.g * (int)255;
    d[2] = color.b * (int)255;
    d[3] = a * (int) 255;
   // printf("%d %d %d %d\n", d[0], d[1], d[2], d[3]);
}

void paint(byte *d, int x, int y, byte r, byte g, byte b, byte a) {
    d += 4 * (y * WIDTH + x);
    d[0] = r ;
    d[1] = g;
    d[2] = b;
    d[3] = a;

}

Color3f mix(const Color3f &a, const Color3f &b, double t) { return a * (1.0f - t) + b * t; }
/**
 * 求出光线原点与球心连线的向量 p，求 p 在 r 上的投影 a，
 * 然后计算光线原点，球心，投影点三点形成的直角三角形中表示距球心距离的直角边，判断其与半径的大小。
 * @param r 光线
 * @param o 圆心
 * @param radius 半径
 * @return
 */
double isCollide(const Ray &r, const Point3f &o, double radius) {
    /*Vector3f p = r.getOrigin() - o;
    auto a = dot(r.getDir(), p);

    return p.length2() - a * a < radius * radius;*/
    Vector3f oc = r.getOrigin() - o;
    auto a = dot(r.getDir(), r.getDir());
    auto b = 2.0 * dot(oc, r.getDir());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant) ) / (2.0*a);
    }
}
std::uniform_real_distribution<double> dis(0.0, 1.0), dis2(-1, 1);
std::mt19937 engine;
Color3f paintRay(const Ray &r) {
    double temp = isCollide(r, Point3f(0, 0, -1), 0.5f);
    if(temp > 0.0f) {
        return 0.5*((r.get(temp) - Point3f(0, 0, -1)).normalize()+Point3f(1,1,1));
    }
    Vector3f dir = r.getDir();
    double t = 0.5f * (dir.y + 1.0f);
    return mix({1.0f, 1.0f, 1.0f}, {0.5f, 0.7f, 1.0f}, t);
}

Color3f paintRay(const Ray &r, const Collisible &ball) {
    CollideRecord rec;
    if(ball.collide(r, 0.0f, std::numeric_limits<double>::max(), rec)) {
        return 0.5f * (rec.normal + Vector3f(1,1,1));
    }
    Vector3f dir = r.getDir();
    double t = 0.5f * (dir.y + 1.0f);
    return mix({1.0f, 1.0f, 1.0f}, {0.5f, 0.7f, 1.0f}, t);
}



Color3f paintRayDiffusion(const Ray &r, const Collisible &ball, int depth) {
    if(depth <= 0) {
        return Color3f(0,0,0);
    }
    CollideRecord rec;
    if(ball.collide(r, 0.001f, std::numeric_limits<double>::max(), rec)) {
        Ray scattered;
        Vector3f attenuation;
        if(rec.material->scatter(r, rec, attenuation,scattered));
        return attenuation * paintRayDiffusion(scattered, ball,depth - 1);
    }
    Vector3f dir = r.getDir();
    double t = 0.5f * (dir.y + 1.0f);
    return mix({1.0f, 1.0f, 1.0f}, {0.5f, 0.7f, 1.0f}, t);
}
byte d[4 * WIDTH * HEIGHT];
int main() {
    FILE *f = fopen("ch10.png", "wb");
   lowerLeftCorner.print();
   horizontal.print();
   vertical.print();
    Camera camera(origin, lowerLeftCorner, horizontal, vertical);
   // Camera camera(origin, Vector3f(-1.77778, -1, -1), Vector3f(3.55556, 0, 0), Vector3f(0, 2,  0));

    CollisibleList list;
    Material* material[10];
   // material[0] = new Lambertian({0.8, 0.3, 0.3});
   // material[0] = new Glass(1.5);
   //center
    material[0] = new Lambertian({0.1, 0.2, 0.5});
    //ground
    material[1] = new Lambertian({0.8, 0.8, 0.0});
    //right
    material[2] = new Metal({0.8, 0.6, 0.2}, 0.0);
    //left
   // material[3] = new Metal({0.8, 0.8, 0.8}, 1.0);
    material[3] = new Glass(1.5);
    Sphere* ball[10];
    ball[0] = new Sphere({0, 0, -1}, 0.5, material[0]);
    ball[1] = new Sphere({0, -100.5, -1}, 100, material[1]);
    ball[2] = new Sphere({1, 0, -1}, 0.5, material[2]);
    ball[3] = new Sphere({-1, 0, -1}, 0.5, material[3]);
    ball[4] = new Sphere({-1, 0, -1}, -0.4, material[3]);
    list.add(ball[0]);
    list.add(ball[1]);
    list.add(ball[2]);
    list.add(ball[3]);
    list.add(ball[4]);
    const int SAMPLE_TIMES = 100;
    const int MAX_DEPTH = 50;
    long long total = (long long)WIDTH * HEIGHT * SAMPLE_TIMES, threshold;
    long long tot = 0, percent = 0;
    threshold = total / 100;
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; ++y){
            Color3f tempColor(0,0,0);
            for(int i = 0; i < SAMPLE_TIMES; ++i ) {
                double u = (double )(x + dis(engine)) / WIDTH;
                double v = (double )(y + + dis(engine)) / HEIGHT;
             //   double u = (double )(x) / WIDTH;
            //    double v = (double )(y) / HEIGHT;
           /*     Vector3f temp = lowerLeftCorner + u * horizontal + v * vertical;
                Ray r(origin, temp);*/
                tempColor += paintRayDiffusion(camera.getRay(u, v), list, 50);
                ++tot;
                if(tot > threshold) {
                    percent++;
                    printf("%lld%%\n", percent);
                    tot = 0;
                }
            }

            tempColor /= SAMPLE_TIMES;
            tempColor = {sqrt(tempColor.r), sqrt(tempColor.g), sqrt(tempColor.b)};
            paint(d, x, y, tempColor, 1.0f);
        }
    }
    delete ball[0];
    delete ball[1];
    delete material[0];
    svpng(f, WIDTH, HEIGHT, d, 1);
    fclose(f);
}
