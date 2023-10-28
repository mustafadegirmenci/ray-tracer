#ifndef RAY_TRACER_MESH_H
#define RAY_TRACER_MESH_H

#include "renderObject.h"

class Mesh : public RenderObject
{
    std::vector<Face> faces;
    Mesh();
    bool intersect(const Ray& ray, float& t) const override;
};

#endif //RAY_TRACER_MESH_H
