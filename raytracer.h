#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>

typedef struct {
    unsigned char* image;
    int width;
    int height;
} RenderResult;

typedef unsigned char RGB[3];

struct Vec3 {
    float x, y, z;
    Vec3(float x = 0, float y = 0, float z = 0);
    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator*(float scalar) const;
    Vec3 operator/(float scalar) const;
    float dot(const Vec3& other) const;
    Vec3 cross(const Vec3& other) const;
    float length() const;
    Vec3 normalized() const;
};

struct Ray {
    Vec3 origin, direction;
    Ray(const Vec3& origin, const Vec3& direction);
};

struct Sphere {
    Vec3 center;
    float radius;
    Sphere(const Vec3& center, float radius);
    bool intersect(const Ray& ray, float& t) const;
};

struct Triangle {
    Vec3 v0, v1, v2;
    Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2);
    bool intersect(const Ray& ray, float& t) const;
};

Vec3 trace(const Ray& ray, const std::vector<Sphere>& spheres, const std::vector<Triangle>& triangles);
RenderResult render();

#endif // RAYTRACER_H
