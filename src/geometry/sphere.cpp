#include "../../include/geometry/sphere.h"

Vec3f Sphere::getNormal(const Scene& scene, const Vec3f& intersectionPoint)
{
	// Get the center of the sphere using center_vertex_id
	Vec3f sphereCenter = scene.vertex_data[center_vertex_id - 1];

	// Calculate the normal vector by subtracting the intersection point from the sphere's center
	Vec3f normal = intersectionPoint - sphereCenter;

	// Normalize the normal vector to obtain a unit normal vector
	normal.normalized();

	return normal;
}
