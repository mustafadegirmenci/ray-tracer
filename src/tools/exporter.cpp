#include "../../include/tools/exporter.h"
#include <stdexcept>

void Exporter::exportPpm(const vector<RenderResult*>& results) const {
    for (const RenderResult* result : results) {
        const char* filename = result->image_name;
        const unsigned char* data = result->image;
        int width = result->width;
        int height = result->height;

        FILE* outfile;

        if ((outfile = fopen(filename, "w")) == NULL) {
            throw std::runtime_error("Error: The ppm file cannot be opened for writing.");
        }

        (void)fprintf(outfile, "P3\n%d %d\n255\n", width, height);

        unsigned char color;
        for (int j = 0, idx = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                for (int c = 0; c < 3; ++c, ++idx) {
                    color = data[idx];

                    if (i == width - 1 && c == 2) {
                        (void)fprintf(outfile, "%d", color);
                    } else {
                        (void)fprintf(outfile, "%d ", color);
                    }
                }
            }

            (void)fprintf(outfile, "\n");
        }

        (void)fclose(outfile);
    }
}
