//
// Created by thhyj on 2021/1/7.
//

#ifndef RAYTRACING_TEXTURE_H
#define RAYTRACING_TEXTURE_H
#define STB_IMAGE_IMPLEMENTATION
#include "Vector.h"
#include "stb_image.h"
#include <algorithm>
#include <random>
class Texture {
public:
    virtual Color3f value(double u, double v, const Point3f& p) const = 0;
};
class SolidColor : public Texture {
private:
    Color3f color;
public:
    SolidColor(){}
    SolidColor(Color3f color):color(color){}
    SolidColor(double r, double g, double b):SolidColor(Color3f(r,g,b)){}
    virtual Color3f value(double u, double v, const Point3f& p) const override {
        return color;
    }

};

class checkerTexture : public Texture {
public:
    checkerTexture() {}

    checkerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
            : even(even), odd(odd) {}

    checkerTexture(Color3f c1, Color3f c2)
            : even(std::make_shared<SolidColor>(c1)) , odd(std::make_shared<SolidColor>(c2)) {}

    virtual Color3f value(double u, double v, const Point3f& p) const override {
        auto sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

public:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;
};

class Perlin {
public:
    Perlin() {
        randomVector = new Vector3f[pointCount];
        for(int i = 0; i < pointCount; ++i) {
            randomVector[i] = getRandomVector(-1, 1);
        }
        permX = perlinGeneratePerm();
        permY = perlinGeneratePerm();
        permZ = perlinGeneratePerm();
    }
    ~Perlin() {
        delete [] randomVector;
        delete [] permX;
        delete [] permY;
        delete [] permZ;
    }

    double noise(const Point3f &p)  const{
     /*   auto i = static_cast<int>(4 * p.x) & 255;
        auto j = static_cast<int>(4 * p.y) & 255;
        auto k = static_cast<int>(4 * p.z) & 255;

        return randomDouble[permX[i] ^ permY[j] ^ permZ[k]];*/
        auto u = p.x - floor(p.x);
        auto v = p.y - floor(p.y);
        auto w = p.z - floor(p.z);
        //Ease Curve
        u = u * u * (3 - 2 * u);
        v = v * v * (3 - 2 * v);
        w = w * w * (3 - 2 * w);

        auto i = static_cast<int>(floor(p.x));
        auto j = static_cast<int>(floor(p.y));
        auto k = static_cast<int>(floor(p.z));
        Vector3f c[2][2][2];

        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = randomVector[
                            permX[(i+di) & 255] ^
                            permY[(j+dj) & 255] ^
                            permZ[(k+dk) & 255]
                    ];

        return trilinearInterp(c, u, v, w);

    }

    double turb (const Point3f &p, int depth = 7) const {
        auto accum = 0.0;
        auto tempP = p;
        auto weight = 1.0;
        for(int i = 0; i < depth; ++i) {
            accum += weight * noise(tempP);
            weight *= 0.5;
            tempP *= 2;
        }
        return fabs(accum);
    }

private:
    static const int pointCount = 256;
    Vector3f *randomVector;
    int *permX, *permY, *permZ;
    static int* perlinGeneratePerm() {
        auto p = new int[pointCount];
        for(int i = 0; i < pointCount; ++i) {
            p[i] = i;
        }
        std::shuffle(p,p + pointCount, std::mt19937(std::random_device()()));
        return p;
    }

    /**
     * 三线性插值
     * @param c
     * @param u
     * @param v
     * @param w
     * @return
     */
    static double trilinearInterp(Vector3f c[2][2][2], double u, double v, double w) {
        double accum = 0.0;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++) {
                    Vector3f weight(u - i, v - j, w - k);
                    accum += (i * u + (1 - i)*(1 - u)) *
                             (j * v + (1 - j)*(1 - v)) *
                             (k * w + (1 - k)*(1 - w)) * dot(c[i][j][k], weight);
                }

        return accum;
    }

};

class NoiseTexture : public Texture {
public:
    NoiseTexture():scale(1.0){
    }
    NoiseTexture(double scale):scale(scale){}
    virtual Color3f value(double u, double v, const Point3f &p) const override {
      //  return Color3f(1, 1, 1) * 0.5 * ( 1.0 +  noise.noise(p * scale));
        return Color3f(1,1,1) * 0.5 * (1.0 + sin(scale*p.z + 10 * noise.turb(p)));
    }

public:
    Perlin noise;
    double scale;
};

class ImageTexture : public Texture {
public:
    const static int bytesPerPixel = 3;
    ImageTexture():data(nullptr), width(0), height(0), bytesPerScanline(0){}
    ImageTexture(const char* fileName) {
        int componentsPerPixel = bytesPerPixel;
        data = stbi_load(fileName, &width, &height, &componentsPerPixel, componentsPerPixel);
        if(!data) {
            std::cerr << "could not load Texture image file " << fileName << '\n';
            width = height = 0;
        }

        bytesPerScanline = bytesPerPixel * width;
    }
    ~ImageTexture() {
        delete data;
    }

    static double clamp(double x, double min, double max) {
        if(x < min) return min;
        if(x > max) return max;
        return x;
    }

    virtual Color3f value(double u, double v, const Vector3f& p) const override {
        if(data == nullptr) return Color3f(0, 1, 1);
        u = clamp(u, 0, 1);
        v = 1.0 - clamp(v, 0, 1);

        int i = static_cast<int>(u * width);
        int j = static_cast<int>(v * height);

        if(i >= width) i = width - 1;
        if(j >= height) j = height - 1;

        const double colorScale = 1.0 / 255.0;

        auto pixel = data + j * bytesPerScanline + i * bytesPerPixel;
        return Color3f(colorScale * pixel[0], colorScale * pixel[1],colorScale * pixel[2]);
    }

private:
    unsigned char *data;
    int width, height;
    int bytesPerScanline;
};

#endif //RAYTRACING_TEXTURE_H
