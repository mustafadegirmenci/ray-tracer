#ifndef __HW1__PARSER__
#define __HW1__PARSER__

#include <string>
#include <vector>
#include "utils.h"
#include "mesh.h"
#include "triangle.h"
#include "sphere.h"

namespace parser
{
    struct Scene
    {
        //Data
        Vec3i background_color;
        float shadow_ray_epsilon;
        int max_recursion_depth;
        std::vector<Camera> cameras;
        Vec3f ambient_light;
        std::vector<PointLight> point_lights;
        std::vector<Material> materials;
        std::vector<Vec3f> vertex_data;
        std::vector<Triangle> triangles;
        std::vector<Sphere> spheres;
        std::vector<Mesh> meshes;

        //Functions
        void loadFromXml(const std::string &filepath);
        Vec3f getVertex(int index);
    };
}

#endif
