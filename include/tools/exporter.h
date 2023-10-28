#ifndef __ppm_h__
#define __ppm_h__

#include "../core/raytracer.h"

class Exporter {
public:
    void exportPpm(vector<RenderResult> results);
    void write_ppm(const char* filename, unsigned char* data, int width, int height);
};

#endif // __ppm_h__
