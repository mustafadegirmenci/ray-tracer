#include "../../include/tools/exporter.h"
#include <stdexcept>

void Exporter::exportPpm(const vector<RenderResult*>& results) const {
	for (const RenderResult* result : results) {
		const char* filename = result->image_name;
		const Color* data = result->image;
		int width = result->width;
		int height = result->height;

		FILE* outfile;

		if ((outfile = fopen(filename, "w")) == NULL) {
			throw std::runtime_error("Error: The ppm file cannot be opened for writing.");
		}

		(void)fprintf(outfile, "P3\n%d %d\n255\n", width, height);

		Color color;
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				color = data[j * width + i];
				(void)fprintf(outfile, "%d %d %d\t", color.r, color.g, color.b);
			}

			(void)fprintf(outfile, "\n");
		}

		(void)fclose(outfile);
	}
}
