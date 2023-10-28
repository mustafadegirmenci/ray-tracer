#include "sphere.h"

Sphere::Sphere(int materialİd) : material_id(materialİd) {}

bool Sphere::intersect(const Ray& ray, float& t) const {
    Vec3f oc = ray.origin - scene.getVertex(center_vertex_id);
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

Sphere::Sphere() {

}

