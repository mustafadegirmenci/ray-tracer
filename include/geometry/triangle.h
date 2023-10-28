#ifndef RAY_TRACER_TRIANGLE_H
#define RAY_TRACER_TRIANGLE_H

#include "base/render_object.h"

class Triangle : public RenderObject
{
public:
    Face indices;
};


#endif //RAY_TRACER_TRIANGLE_H
