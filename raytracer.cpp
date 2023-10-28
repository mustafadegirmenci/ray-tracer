#include "raytracer.h"
#include "sphere.h"

#include <vector>
#include <cmath>
#include <limits>

#pragma region Tracing

Vec3f trace(const Ray& ray, const std::vector<RenderObject>& renderObjects) {
    float tMin = std::numeric_limits<float>::max();
    float t;
    int closestRenderObjectIndex = -1;

    // Find the closest intersection point with the spheres
    for (int i = 0; i < renderObjects.size(); i++) {
        if (renderObjects[i].intersect(ray, t) && t < tMin) {
            tMin = t;
            closestRenderObjectIndex = i;
        }
    }

    if (closestRenderObjectIndex != -1) {
        // Implement shading for spheres here
        // For simplicity, we return the sphere color as is.
        return Vec3f(1, 0, 0); // Red sphere
    }

    // If no intersection, return background color
    return Vec3f(0.7, 0.8, 1.0); // Blue sky
}


#pragma endregion Tracing

RenderResult RayTracer::render() {
    
    return RenderResult();
}
