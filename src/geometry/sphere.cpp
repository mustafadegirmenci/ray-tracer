#include "../../include/geometry/sphere.h"

Vec3f Sphere::getNormal(const Scene& scene, const Vec3f& intersectionPoint)
{
	// Calculate the normal vector by subtracting the intersection point from the sphere's center
	Vec3f normal = intersectionPoint - center_vertex;

	// Normalize the normal vector to obtain a unit normal vector
	normal.normalized();

	return normal;
}
