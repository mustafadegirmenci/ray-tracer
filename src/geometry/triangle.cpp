#include "../../include/geometry/triangle.h"

Vec3f Triangle::getNormal(const Scene& scene, const Vec3f& intersectionPoint) {
	if (isCalculated) {
		return normal;
	}

	// Calculate two edges of the triangle
	Vec3f edge1 = vertex_1 - vertex_0;
	Vec3f edge2 = vertex_2 - vertex_0;

	// Calculate the cross product of the edges to get the normal vector
	normal = edge1.cross(edge2);

	// Normalize the normal vector
    normal = normal.normalized();

	isCalculated = true;

	return normal;
}

bool Triangle::intersect(Ray* ray, float &t) {
    Vec3f e1 = vertex_1 - vertex_0;
    Vec3f e2 = vertex_2 - vertex_0;
    Vec3f h = ray->direction.cross(e2);
    float a = e1.dot(h);

    if (a > -std::numeric_limits<float>::epsilon() && a < std::numeric_limits<float>::epsilon())
        return false;

    float f = 1.0f / a;
    Vec3f s = ray->origin - vertex_0;
    float u = f * s.dot(h);

    if (u < 0.0f || u > 1.0f)
        return false;

    Vec3f q = s.cross(e1);
    float v = f * ray->direction.dot(q);

    if (v < 0.0f || u + v > 1.0f)
        return false;

    t = f * e2.dot(q);

    if (t > std::numeric_limits<float>::epsilon()){
        if (t < 0){
            return false;
        }
        return true;
    }

    return false;
}
