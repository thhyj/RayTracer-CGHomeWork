#include "svpng.inc"
//#include "stb_image.h"
#include <cstdio>
#include <vector>
#include <limits>
#include <random>
#include <atomic>
#include <memory>
#include <ctime>
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
#include "BVH.h"
#include "MovingSphere.h"
#include "DiffuseLight.h"
const int WIDTH = 640;
const int HEIGHT = 360;

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

Color3f paintRay(const Ray &r, const Color3f& background, const Collisible &ball, int depth) {
    if(depth <= 0) {
        return Color3f(0,0,0);
    }
    CollideRecord rec;
    if(ball.collide(r, 0.001f, std::numeric_limits<double>::max(), rec)) {
        Ray scattered;
        Vector3f attenuation;
        Color3f emitted = rec.material->emitted(rec.u, rec.v, rec.p);
        if(rec.material->scatter(r, rec, attenuation,scattered))
            return emitted+ attenuation * paintRay(scattered, background, ball,depth - 1);
        return emitted;
    }
   /* Vector3f dir = r.getDir();
    double t = 0.5f * (dir.y + 1.0f);
    return mix({1.0f, 1.0f, 1.0f}, {0.5f, 0.7f, 1.0f}, t);*/
    return background;
}

BVH randomScene() {
    CollisibleList world;
   // auto checker = std::make_shared<checkerTexture>(Color3f(0.2, 0.3, 0.1), Color3f(0.9, 0.9, 0.9));
   // auto ground_material = std::make_shared<Lambertian>(Color3f(0.2, 0.3, 0.1));
    auto ground_material = std::make_shared<Lambertian>(std::make_shared<NoiseTexture>(4));
    world.add(std::make_shared<Sphere>(Point3f(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = getRandomdouble();
            Point3f center(a + 0.9*getRandomdouble(), 0.2, b + 0.9*getRandomdouble());

            if ((center - Point3f(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color3f(getRandomdouble(), getRandomdouble(), getRandomdouble()) *
                            Color3f(getRandomdouble(), getRandomdouble(), getRandomdouble());
             //       sphere_material = std::make_shared<Lambertian>(albedo);
                    sphere_material = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("yileina.png"));

                 /*   world.add(std::make_shared<MovingSphere>(center,center +
                    Vector3f(0, getRandomdouble(0, 0.5), 0) ,0, 1, 0.2, sphere_material));*/
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color3f(getRandomdouble(0, 0.5), getRandomdouble(0, 0.5), getRandomdouble(0, 0.5));
                    auto fuzz = getRandomdouble(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Glass>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Glass>(1.5);
    world.add(std::make_shared<Sphere>(Point3f(0, 1, 0), 1.0, material1));

    //auto material2 = std::make_shared<Lambertian>(Color3f(0.4, 0.2, 0.1));
    auto material2 = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("yileina.png"));

    world.add(std::make_shared<Sphere>(Point3f(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color3f(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Point3f(4, 1, 0), 1.0, material3));
    return BVH(world, 0, 1);
   // return world;
}

BVH twoSphere() {
    CollisibleList world;
    auto checker = std::make_shared<checkerTexture>(Color3f(0.2, 0.3, 0.1), Color3f(0.9, 0.9, 0.9));
    world.add(std::make_shared<Sphere>(Point3f(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
    world.add(std::make_shared<Sphere>(Point3f(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));
    return BVH(world, 0, 1);
}

BVH twoPerlinSphere() {
    CollisibleList world;
    auto perlintex = std::make_shared<NoiseTexture>(4);
    world.add(std::make_shared<Sphere>(Point3f(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlintex)));
    world.add(std::make_shared<Sphere>(Point3f(0, 2, 0), 2, std::make_shared<Lambertian>(perlintex)));
    return BVH(world, 0, 1);
}


BVH earth() {
    CollisibleList world;
    auto earthTexture = std::make_shared<ImageTexture>("yileina.png");
    auto earthMaterial = std::make_shared<Lambertian>(earthTexture);
    auto globe = std::make_shared<Sphere>(Point3f(0, 0, 0), 2, earthMaterial);
    world.add(globe);
    return BVH(world, 0, 1);
}

BVH black(Color3f &background) {
    background = Color3f(0, 0, 0);
    CollisibleList world;
    auto earthTexture = std::make_shared<ImageTexture>("yileina.png");
    auto earthMaterial = std::make_shared<Lambertian>(earthTexture);
    auto globe = std::make_shared<Sphere>(Point3f(0, 0, 0), 2, earthMaterial);
    world.add(globe);
    return BVH(world, 0, 1);
}

BVH blackWorld(Color3f &background) {
    background = Color3f(0, 0, 0);
    CollisibleList world;
    // auto checker = std::make_shared<checkerTexture>(Color3f(0.2, 0.3, 0.1), Color3f(0.9, 0.9, 0.9));
    // auto ground_material = std::make_shared<Lambertian>(Color3f(0.2, 0.3, 0.1));
    auto ground_material = std::make_shared<Lambertian>(std::make_shared<NoiseTexture>(4));
    world.add(std::make_shared<Sphere>(Point3f(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = getRandomdouble();
            Point3f center(a + 0.9*getRandomdouble(), 0.2, b + 0.9*getRandomdouble());

            if ((center - Point3f(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.5) {
                    // diffuse
                    auto albedo = Color3f(getRandomdouble(), getRandomdouble(), getRandomdouble()) *
                                  Color3f(getRandomdouble(), getRandomdouble(), getRandomdouble());
                    //       sphere_material = std::make_shared<Lambertian>(albedo);
                    sphere_material = std::make_shared<DiffuseLight>(std::make_shared<ImageTexture>("yileina.png"));

                    /*   world.add(std::make_shared<MovingSphere>(center,center +
                       Vector3f(0, getRandomdouble(0, 0.5), 0) ,0, 1, 0.2, sphere_material));*/
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material,
                                                       getRandomdouble(-60, 60),
                                                       getRandomdouble(-30, 30)));
                } else if (choose_mat < 0.75) {
                    // metal
                    auto albedo = Color3f(getRandomdouble(0, 0.5), getRandomdouble(0, 0.5), getRandomdouble(0, 0.5));
                    auto fuzz = getRandomdouble(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Glass>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Glass>(1.5);
    world.add(std::make_shared<Sphere>(Point3f(0, 1, 0), 1.0, material1));

    //auto material2 = std::make_shared<Lambertian>(Color3f(0.4, 0.2, 0.1));
    auto material2 = std::make_shared<DiffuseLight>(std::make_shared<ImageTexture>("yileina.png"));

    world.add(std::make_shared<Sphere>(Point3f(-4, 1, -2), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color3f(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Point3f(4, 1, 0), 1.0, material3));
    return BVH(world, 0, 1);
    return BVH(world, 0, 1);
}

byte d[4 * WIDTH * HEIGHT];
int main() {
    randomInit();
    FILE *f = fopen("ch37.png", "wb");
    Point3f lookFrom(13, 7, 3);
    Point3f lookAt(0, 0, 0);
    Vector3f vup(0, 1, 0);
    Camera camera(lookFrom, lookAt, vup, 20, 16.0/9.0, 0.0, 10.0, 0.0, 1.0);
    camera.lowerLeftCorner.print();
    camera.horizontal.print();
    camera.vertical.print();
    Color3f background = Color3f(0, 0, 0);
    background = Color3f(0.7, 0.8, 1.0);
  //  CollisibleList world = randomScene();
   // BVH world = randomScene();
 //  BVH world = twoSphere();
  //  BVH world = twoPerlinSphere();
    //BVH world = earth();
   // BVH world = black(background);
    BVH world = blackWorld(background);
    const int SAMPLE_TIMES = 100;
    const int MAX_DEPTH = 50;
    long long total = (long long)WIDTH * HEIGHT * SAMPLE_TIMES, threshold;

    std::atomic<long long> tot{0}, percent{0};
    threshold = total / 100;
    double st = clock();
    {

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; ++y) {
            Color3f tempColor(0, 0, 0);
#pragma omp parallel for
            for (int i = 0; i < SAMPLE_TIMES; ++i) {
                double u = (double) (x + getRandomdouble()) / WIDTH;
                double v = (double) (y + getRandomdouble()) / HEIGHT;
                tempColor += paintRay(camera.getRay(u, v),background, world, MAX_DEPTH);
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

    svpng(f, WIDTH, HEIGHT, d, 1);
    fclose(f);
    std::cout <<"time = " << clock() - st;
}
