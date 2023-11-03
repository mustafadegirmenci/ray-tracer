#include "../../include/geometry/triangle.h"

Vec3f Triangle::getNormal(const Scene& scene, const Vec3f& intersectionPoint) {
	if (isCalculated) {
		return normal;
	}

	Vec3f v1, v2, v3;

	v1 = scene.vertex_data[indices.v0_id - 1];
	v2 = scene.vertex_data[indices.v1_id - 1];
	v3 = scene.vertex_data[indices.v2_id - 1];

	// Calculate two edges of the triangle
	Vec3f edge1 = v2 - v1;
	Vec3f edge2 = v3 - v1;

	// Calculate the cross product of the edges to get the normal vector
	normal = edge1.cross(edge2);

	// Normalize the normal vector
	normal.normalized();

	isCalculated = true;

	return normal;
}
