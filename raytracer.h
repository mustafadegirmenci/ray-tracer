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
    std::vector<RenderObject*> renderObjects;

public:
    RayTracer();
    RenderResult render();

private:
    RenderObject* trace(const Ray& ray);

};

#endif // RAYTRACER_H
