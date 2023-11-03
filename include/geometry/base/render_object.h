#ifndef RAY_TRACER_RENDER_OBJECT_H
#define RAY_TRACER_RENDER_OBJECT_H

#include "../../utilities.h"
#include <vector>
#include <math.h>

class RenderObject{
public:
    int material_id;
    virtual Vec3f getNormal(const Scene& scene) = 0;
};

#endif //RAY_TRACER_RENDER_OBJECT_H
