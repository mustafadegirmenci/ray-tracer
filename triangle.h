#ifndef RAY_TRACER_TRIANGLE_H
#define RAY_TRACER_TRIANGLE_H

#include "renderObject.h"

class Triangle : public RenderObject
{
    Face indices;
    Triangle();
    bool intersect(const Ray& ray, float& t) const override;
};


#endif //RAY_TRACER_TRIANGLE_H
