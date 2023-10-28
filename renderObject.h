#ifndef RAY_TRACER_RENDEROBJECT_H
#define RAY_TRACER_RENDEROBJECT_H

#include "utils.h"
#include "globals.h"
#include <vector>
#include <math.h>

struct RenderObject{
    virtual bool intersect(const Ray& ray, float& t) const;
};

#endif //RAY_TRACER_RENDEROBJECT_H
