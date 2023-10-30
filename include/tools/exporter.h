#ifndef __ppm_h__
#define __ppm_h__

#include "../core/raytracer.h"

class Exporter {
public:
    void exportPpm(const vector<RenderResult*>& results) const;
};

#endif // __ppm_h__
