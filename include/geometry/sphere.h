#ifndef RAY_TRACER_SPHERE_H
#define RAY_TRACER_SPHERE_H

#include "base/render_object.h"

class Sphere : public RenderObject
{
public:
    Vec3f center_vertex;
    float radius;
    Vec3f getNormal(const Scene& scene, const Vec3f& intersectionPoint) override;
};


#endif //RAY_TRACER_SPHERE_H
