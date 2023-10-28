#ifndef RAY_TRACER_RENDEROBJECT_H
#define RAY_TRACER_RENDEROBJECT_H

#include "../../utilities.h"
#include <vector>
#include <math.h>

class RenderObject{
public:
    int material_id;
    virtual bool intersect(const Ray& ray, float& t) const = 0;
};

#endif //RAY_TRACER_RENDEROBJECT_H
