#include "svpng.inc"
#include <cstdio>
#include <vector>
#include "vector.h"
#include "ray.h"

//const auto aspectRatio = 16.0 / 9.0;
const int WIDTH = 640;
const int HEIGHT = 480;
auto viewportHeight = 3.0;
auto viewportWidth = 4.0;
auto focalLength = 1.0;
auto origin = Point3f(0,0,0);
auto horizontal = Vector3f(viewportWidth, 0, 0);
auto vertical = Vector3f(0, viewportHeight, 0);
auto lowerLeftCorner = origin - horizontal/2 - vertical/2 - Vector3f(0, 0, focalLength);
using byte = unsigned char;

void paint(byte *d, int x, int y, const Color3f &color, float a) {
    d += 4 * (y * WIDTH + x);
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

Color3f mix(const Color3f &a, const Color3f &b, float t) { return a * (1.0f - t) + b * t; }
/**
 * 求出光线原点与球心连线的向量 p，求 p 在 r 上的投影 a，
 * 然后计算光线原点，球心，投影点三点形成的直角三角形中表示距球心距离的直角边，判断其与半径的大小。
 * @param r 光线
 * @param o 圆心
 * @param radius 半径
 * @return
 */
bool isCollide(const Ray &r, const Point3f &o, float radius) {
    Vector3f p = r.getOrigin() - o;
    auto a = dot(r.getDir(), p);

    return p.length2() - a * a < radius * radius;
}

Color3f paintRay(const Ray &r) {
    if(isCollide(r, Point3f(0, 0, -1), 0.5f)) {
        return Color3f(1, 0, 0);
    }
    Vector3f dir = r.getDir();
    float t = 0.5f * (dir.y + 1.0f);
    return mix({1.0f, 1.0f, 1.0f}, {0.5f, 0.7f, 1.0f}, t);
}

int main() {
    FILE *f = fopen("ch4.png", "wb");
//    std::vector<byte> rgba(4 * WIDTH * HEIGHT);;
    byte d[4 * WIDTH * HEIGHT];
   /* for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++){
            paint(rgba.data(), x, y, x * 256 / WIDTH, y * 256 / HEIGHT, 128,
                  (x * 256 / WIDTH + y * 256 / HEIGHT) / 2);
        }
    }*/
   lowerLeftCorner.print();
   horizontal.print();
   vertical.print();
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; x++){
            float u = (float )x / WIDTH;
            float v = (float )y / HEIGHT;
            Vector3f temp = lowerLeftCorner + u * horizontal + v * vertical;
            Ray r(origin, temp);
            paint(d, x, y, paintRay(r), 1.0f);
        //   paint(d, x, y, Color3f(1.0, 0.0, 1.0), 1.0f);
         /*   paint(d, x, y, x * 256 / WIDTH, y * 256 / HEIGHT, 128,
                  (x * 256 / WIDTH + y * 256 / HEIGHT) / 2);*/
        }
    }
    svpng(f, WIDTH, HEIGHT, d, 1);
    fclose(f);
}
