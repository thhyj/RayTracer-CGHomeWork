#include "svpng.inc"
#include <cstdio>
#include <vector>
#include <limits>
#include <random>
#include <atomic>
#include <memory>
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
const int WIDTH = 1920;
const int HEIGHT = 1080;

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
std::uniform_real_distribution<double> dis(0.0, 1.0), dis2(-1, 1);
std::mt19937 engine;

Color3f paintRay(const Ray &r, const Collisible &ball, int depth) {
    if(depth <= 0) {
        return Color3f(0,0,0);
    }
    CollideRecord rec;
    if(ball.collide(r, 0.001f, std::numeric_limits<double>::max(), rec)) {
        Ray scattered;
        Vector3f attenuation;
        if(rec.material->scatter(r, rec, attenuation,scattered));
        return attenuation * paintRay(scattered, ball,depth - 1);
    }
    Vector3f dir = r.getDir();
    double t = 0.5f * (dir.y + 1.0f);
    return mix({1.0f, 1.0f, 1.0f}, {0.5f, 0.7f, 1.0f}, t);
}

CollisibleList randomScene() {
    CollisibleList world;
    auto groundMaterial = new Lambertian(Color3f(0.5, 0.5, 0.5));
    world.add(new Sphere(Point3f(0, -1000, 0), 1000.0f, groundMaterial));
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            float chooseMat = getRandomdouble();
            Point3f center(a + 0.9 * getRandomdouble(), 0.2, b + 0.9 * getRandomdouble());
            if ((center - Point3f(4, 0.2, 0)).length() > 0.9) {
                Material* sphereMaterial;
                if (chooseMat < 0.8) {
                    // diffuse
                    auto albedo = getRandomVector() * getRandomVector();
                    sphereMaterial = new Lambertian(albedo);
                    world.add(new Sphere(center, 0.2f, sphereMaterial));
                } else if (chooseMat < 0.95) {
                    // metal
                    auto albedo = getRandomVector(0.5, 1);
                    auto fuzz = getRandomdouble(0, 0.5);
                    sphereMaterial = new Metal(albedo, fuzz);
                    world.add(new Sphere(center, 0.2f, sphereMaterial));
                } else {
                    // glass
                    sphereMaterial = new Glass(1.5f);
                    world.add(new Sphere(center, 0.2f, sphereMaterial));
                }
            }
        }
    }
    auto material1 = new Glass(1.5f);
    world.add(new Sphere(Point3f(0, 1, 0), 1.0f, material1));

    auto material2 = new Lambertian(Color3f(0.4, 0.2, 0.1));
    world.add(new Sphere(Point3f(-4, 1, 0), 1.0f, material2));

    auto material3 = new Metal(Color3f(0.7, 0.6, 0.5), 0.0f);
    world.add(new Sphere(Point3f(4, 1, 0), 1.0f, material3));

    return world;
}


byte d[4 * WIDTH * HEIGHT];
int main() {
    FILE *f = fopen("ch15.png", "wb");
    Point3f lookFrom(13, 2, 3);
    Point3f lookAt(0, 0, 0);
    Vector3f vup(0, 1, 0);
    Camera camera(lookFrom, lookAt, vup, 20, 16.0/9.0, 0.0, 10.0);
    camera.lowerLeftCorner.print();
    camera.horizontal.print();
    camera.vertical.print();
    CollisibleList world = randomScene();

    const int SAMPLE_TIMES = 5000;
    const int MAX_DEPTH = 50;
    long long total = (long long)WIDTH * HEIGHT * SAMPLE_TIMES, threshold;
    std::atomic<long long> tot{0}, percent{0};
    threshold = total / 100;
    {

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; ++y) {
            Color3f tempColor(0, 0, 0);
#pragma omp parallel for
            for (int i = 0; i < SAMPLE_TIMES; ++i) {
                double u = (double) (x + dis(engine)) / WIDTH;
                double v = (double) (y + +dis(engine)) / HEIGHT;
                tempColor += paintRay(camera.getRay(u, v), world, MAX_DEPTH);
                ++tot;
                if (tot == threshold) {
                    percent++;
                    std::cout << percent << '\n';
                    tot = 0;
                }
            }

            tempColor /= SAMPLE_TIMES;
            tempColor = {sqrt(tempColor.r), sqrt(tempColor.g), sqrt(tempColor.b)};
            paint(d, x, y, tempColor, 1.0f);
        }
    }
}
    for(auto iter =  world.collisibleList.begin(); iter != world.collisibleList.end(); ++iter) {
        if(static_cast<Sphere*>((*iter))->material != nullptr)
            delete static_cast<Sphere*>((*iter))->material;
        delete (*iter);
    }
    svpng(f, WIDTH, HEIGHT, d, 1);
    fclose(f);
}
