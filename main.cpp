#include "ppm.h"
#include "raytracer.h"
#include "globals.h"

int main(int argc, char* argv[])
{
    scene.loadFromXml(argv[1]);

    RenderResult result = render();

    write_ppm("test.ppm", result.image, result.width, result.height);
}
