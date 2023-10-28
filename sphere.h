#ifndef RAY_TRACER_SPHERE_H
#define RAY_TRACER_SPHERE_H

#include "renderObject.h"

struct Sphere : RenderObject
{
    int material_id;
    int center_vertex_id;
    float radius;
    Sphere();

    explicit Sphere(int materialId);

    bool intersect(const Ray& ray, float& t) const override;
};


#endif //RAY_TRACER_SPHERE_H
