#ifndef RAY_TRACER_SPHERE_H
#define RAY_TRACER_SPHERE_H

#include "base/render_object.h"

class Sphere : public RenderObject
{
public:
    int center_vertex_id;
    float radius;
};


#endif //RAY_TRACER_SPHERE_H