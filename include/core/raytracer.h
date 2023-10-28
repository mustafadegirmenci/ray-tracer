#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>
#include "../geometry/triangle.h"
#include "../geometry/sphere.h"

typedef struct {
    const char* image_name;
    unsigned char* image;
    int width;
    int height;
} RenderResult;

class RayTracer{
    Scene scene;

public:
    vector<RenderResult> render(Scene);

private:
    Ray calculateRay(Camera camera, int x, int y);
    bool raycast(const Ray& ray, RenderObject& hitObject, float &tMin);
    bool intersectSphere(Sphere sphere, const Ray& ray, float& t) const;
    bool intersectTriangle(Triangle triangle, const Ray& ray, float& t) const;
};

#endif // RAYTRACER_H
