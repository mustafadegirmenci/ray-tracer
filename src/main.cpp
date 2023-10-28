#include "../include/tools/exporter.h"
#include "../include/tools/importer.h"

int main(int argc, char* argv[])
{
    Importer importer;
    Scene parsedScene = importer.importXml(argv[1]);

    RayTracer rayTracer;
    vector<RenderResult> results = rayTracer.render(parsedScene);

    Exporter exporter;
    exporter.exportPpm(results);
}
