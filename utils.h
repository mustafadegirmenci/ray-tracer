#ifndef RAY_TRACER_UTILS_H
#define RAY_TRACER_UTILS_H

#include <string>

using namespace std;

//Notice that all the structures are as simple as possible
//so that you are not enforced to adopt any style or design.
struct Vec3f
{
    float x, y, z;
    Vec3f(float x = 0, float y = 0, float z = 0);
    Vec3f operator+(const Vec3f& other) const;
    Vec3f operator-(const Vec3f& other) const;
    Vec3f operator*(float scalar) const;
    Vec3f operator/(float scalar) const;
    float dot(const Vec3f& other) const;
    Vec3f cross(const Vec3f& other) const;
    float length() const;
    Vec3f normalized() const;
};

struct Vec3i
{
    int x, y, z;
};

struct Vec4f
{
    float x, y, z, w;
};

struct Ray {
    Vec3f origin, direction;
    Ray(const Vec3f& origin, const Vec3f& direction);
};

struct Camera
{
    Vec3f position;
    Vec3f gaze;
    Vec3f up;
    Vec4f near_plane;
    float near_distance;
    int image_width, image_height;
    string image_name;
};

struct PointLight
{
    Vec3f position;
    Vec3f intensity;
};

struct Material
{
    bool is_mirror;
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
    Vec3f mirror;
    float phong_exponent;
};

struct Face
{
    int v0_id;
    int v1_id;
    int v2_id;
};

#endif //RAY_TRACER_UTILS_H
