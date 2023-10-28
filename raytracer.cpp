#include "raytracer.h"
#include <vector>
#include <cmath>
#include <limits>

#pragma region Vec3

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3 Vec3::operator+(const Vec3& other) const {
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(float scalar) const {
    return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::operator/(float scalar) const {
    return Vec3(x / scalar, y / scalar, z / scalar);
}

float Vec3::dot(const Vec3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::cross(const Vec3& other) const {
    return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

float Vec3::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalized() const {
    float len = length();
    return Vec3(x / len, y / len, z / len);
}

Ray::Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) {}

#pragma endregion Vec3

#pragma region Sphere
Sphere::Sphere(const Vec3& center, float radius) : center(center), radius(radius) {}

bool Sphere::intersect(const Ray& ray, float& t) const {
    Vec3 oc = ray.origin - center;
    float a = ray.direction.dot(ray.direction);
    float b = 2.0f * oc.dot(ray.direction);
    float c = oc.dot(oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);
        t = (t1 < t2) ? t1 : t2;
        return true;
    }
    return false;
}

#pragma endregion Sphere

#pragma region Triangle
Triangle::Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2) : v0(v0), v1(v1), v2(v2) {}

bool Triangle::intersect(const Ray& ray, float& t) const {
    Vec3 e1 = v1 - v0;
    Vec3 e2 = v2 - v0;
    Vec3 h = ray.direction.cross(e2);
    float a = e1.dot(h);

    if (a > -std::numeric_limits<float>::epsilon() && a < std::numeric_limits<float>::epsilon())
        return false;

    float f = 1.0f / a;
    Vec3 s = ray.origin - v0;
    float u = f * s.dot(h);

    if (u < 0.0f || u > 1.0f)
        return false;

    Vec3 q = s.cross(e1);
    float v = f * ray.direction.dot(q);

    if (v < 0.0f || u + v > 1.0f)
        return false;

    t = f * e2.dot(q);

    if (t > std::numeric_limits<float>::epsilon())
        return true;

    return false;
}

#pragma endregion Triangle

#pragma region Tracing

Vec3 trace(const Ray& ray, const std::vector<Sphere>& spheres, const std::vector<Triangle>& triangles) {
    float tMin = std::numeric_limits<float>::max();
    float t;
    int closestSphere = -1;
    int closestTriangle = -1;

    // Find the closest intersection point with the spheres
    for (int i = 0; i < spheres.size(); i++) {
        if (spheres[i].intersect(ray, t) && t < tMin) {
            tMin = t;
            closestSphere = i;
            closestTriangle = -1; // Reset the triangle intersection
        }
    }

    // Find the closest intersection point with the triangles
    for (int i = 0; i < triangles.size(); i++) {
        if (triangles[i].intersect(ray, t) && t < tMin) {
            tMin = t;
            closestSphere = -1; // Reset the sphere intersection
            closestTriangle = i;
        }
    }

    if (closestSphere != -1) {
        // Implement shading for spheres here
        // For simplicity, we return the sphere color as is.
        return Vec3(1, 0, 0); // Red sphere
    }

    if (closestTriangle != -1) {
        // Implement shading for triangles here
        // For simplicity, we return the triangle color as is.
        return Vec3(0, 1, 0); // Green triangle
    }

    // If no intersection, return background color
    return Vec3(0.7, 0.8, 1.0); // Blue sky
}


RenderResult render() {
    // Implementation for rendering
    // ...
    RenderResult result;
    return result;
}

#pragma endregion Tracing