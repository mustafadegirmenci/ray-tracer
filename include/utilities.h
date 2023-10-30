#ifndef RAY_TRACER_UTILITIES_H
#define RAY_TRACER_UTILITIES_H

#include <string>
#include <cmath>
#include <vector>

class Triangle;
class Sphere;

using namespace std;

//Notice that all the structures are as simple as possible
//so that you are not enforced to adopt any style or design.
struct Vec3f
{
    float x, y, z;
    explicit Vec3f(float x = 0, float y = 0, float z = 0);
    Vec3f operator+(const Vec3f& other) const;
    Vec3f operator-(const Vec3f& other) const;
    Vec3f operator*(float scalar) const;
    Vec3f operator/(float scalar) const;
    float dot(const Vec3f& other) const;
    Vec3f cross(const Vec3f& other) const;
    float length() const;
    Vec3f normalized() const;
};

struct Color
{
    int r, g, b;
};

struct Vec4f
{
    float x, y, z, w;
};

struct Ray {
    Vec3f origin, direction;
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

struct Scene
{
    Color background_color;
    float shadow_ray_epsilon;
    int max_recursion_depth;
    std::vector<Camera> cameras;
    Vec3f ambient_light;
    std::vector<PointLight> point_lights;
    std::vector<Material> materials;
    std::vector<Vec3f> vertex_data;
    std::vector<Triangle> triangles;
    std::vector<Sphere> spheres;
};

#endif //RAY_TRACER_UTILITIES_H
