#include "parser.h"
#include "ppm.h"
#include "raytracer.h"

int main(int argc, char* argv[])
{
    parser::Scene scene;

    scene.loadFromXml(argv[1]);

    RenderResult result = render();

    write_ppm("test.ppm", result.image, result.width, result.height);
}
