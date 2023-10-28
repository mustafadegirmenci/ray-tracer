#include <limits>
#include "triangle.h"

bool Triangle::intersect(const Ray &ray, float &t) const {
    Vec3f e1 = scene.getVertex(indices.v1_id) - scene.getVertex(indices.v0_id);
    Vec3f e2 = scene.getVertex(indices.v2_id) - scene.getVertex(indices.v0_id);
    Vec3f h = ray.direction.cross(e2);
    float a = e1.dot(h);

    if (a > -std::numeric_limits<float>::epsilon() && a < std::numeric_limits<float>::epsilon())
        return false;

    float f = 1.0f / a;
    Vec3f s = ray.origin - scene.getVertex(indices.v0_id);
    float u = f * s.dot(h);

    if (u < 0.0f || u > 1.0f)
        return false;

    Vec3f q = s.cross(e1);
    float v = f * ray.direction.dot(q);

    if (v < 0.0f || u + v > 1.0f)
        return false;

    t = f * e2.dot(q);

    if (t > std::numeric_limits<float>::epsilon())
        return true;

    return false;
}

Triangle::Triangle() {

}
