#ifndef RAY_TRACER_TRIANGLE_H
#define RAY_TRACER_TRIANGLE_H

#include "base/render_object.h"

class Triangle : public RenderObject
{
public:
    Vec3f vertex_0;
    Vec3f vertex_1;
    Vec3f vertex_2;
    Vec3f normal;
    Vec3f getNormal(const Scene& scene, const Vec3f& intersectionPoint) override;

private:
    bool isCalculated = false;
};


#endif //RAY_TRACER_TRIANGLE_H
