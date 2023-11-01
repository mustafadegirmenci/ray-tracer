#ifndef RAY_TRACER_TRIANGLE_H
#define RAY_TRACER_TRIANGLE_H

#include "base/render_object.h"

class Triangle : public RenderObject
{
public:
    Face indices;
    Vec3f normal;
    Vec3f getNormal(const Scene& scene);
private:
    bool isCalculated = false;
};


#endif //RAY_TRACER_TRIANGLE_H
