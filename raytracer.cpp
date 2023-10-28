#include "raytracer.h"
#include <limits>

#pragma region Tracing

RenderObject* RayTracer::trace(const Ray& ray) {
    float tMin = std::numeric_limits<float>::max();
    float t;
    int closestRenderObjectIndex = -1;

    // Find the closest intersection point with the spheres
    for (int i = 0; i < renderObjects.size(); i++) {
        if (renderObjects[i]->intersect(ray, t) && t < tMin) {
            tMin = t;
            closestRenderObjectIndex = i;
        }
    }

    if (closestRenderObjectIndex == -1){
        return nullptr;
    }

    return renderObjects[closestRenderObjectIndex];
}

#pragma endregion Tracing

RenderResult RayTracer::render() {

    return RenderResult();
}

RayTracer::RayTracer() {
    for (size_t i = 0; i < scene.meshes.size(); i++) {
        renderObjects.push_back((RenderObject*)&scene.meshes[i]);
    }

    for (size_t i = 0; i < scene.spheres.size(); i++) {
        renderObjects.push_back((RenderObject*)&scene.spheres[i]);
    }

    for (size_t i = 0; i < scene.triangles.size(); i++) {
        renderObjects.push_back((RenderObject*)&scene.triangles[i]);
    }
}
