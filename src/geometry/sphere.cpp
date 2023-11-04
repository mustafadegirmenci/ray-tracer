#include "../../include/geometry/sphere.h"

Vec3f Sphere::getNormal(const Scene& scene, const Vec3f& intersectionPoint)
{
	// Calculate the normal vector by subtracting the intersection point from the sphere's center
	Vec3f normal = intersectionPoint - center_vertex;

	// Normalize the normal vector to obtain a unit normal vector
    normal = normal.normalized();

	return normal;
}

bool Sphere::intersect(const Ray &ray, float &t) {
    Vec3f oc = ray.origin - center_vertex;
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
    else if (discriminant == 0) {
        t = -b / (2.0f * a);
        return true;
    }

    return false;
}

