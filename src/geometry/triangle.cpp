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
	normal.normalized();

	isCalculated = true;

	return normal;
}
