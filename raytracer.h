#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>
#include "mesh.h"
#include "triangle.h"
#include "sphere.h"

typedef struct {
    unsigned char* image;
    int width;
    int height;
} RenderResult;

class RayTracer{
    const std::vector<RenderObject>& renderObjects;

public:
    explicit RayTracer(const vector<RenderObject> &renderObjects);

private:
    Vec3f trace(const Ray& ray);

    RenderResult render();
};

#endif // RAYTRACER_H
