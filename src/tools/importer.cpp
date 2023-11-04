#include "../../include/tools/importer.h"
#include "../../include/third_party/tinyxml2.h"
#include "../../include/geometry/sphere.h"
#include "../../include/geometry/triangle.h"
#include <sstream>
#include <stdexcept>

Scene Importer::importXml(const std::string &filepath)
{
    Scene scene;

    tinyxml2::XMLDocument file;
    std::stringstream stream;

    auto res = file.LoadFile(filepath.c_str());
    if (res)
    {
        throw std::runtime_error("Error: The xml file cannot be loaded.");
    }

    auto root = file.FirstChild();
    if (!root)
    {
        throw std::runtime_error("Error: Root is not found.");
    }

    //Get BackgroundColor
    auto element = root->FirstChildElement("BackgroundColor");
    if (element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0 0 0" << std::endl;
    }
    stream >> scene.background_color.r >> scene.background_color.g >> scene.background_color.b;

    //Get ShadowRayEpsilon
    element = root->FirstChildElement("ShadowRayEpsilon");
    if (element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0.001" << std::endl;
    }
    stream >> scene.shadow_ray_epsilon;

    //Get MaxRecursionDepth
    element = root->FirstChildElement("MaxRecursionDepth");
    if (element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0" << std::endl;
    }
    stream >> scene.max_recursion_depth;

    //Get Cameras
    element = root->FirstChildElement("Cameras");
    element = element->FirstChildElement("Camera");
    Camera camera;
    while (element)
    {
        auto child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Gaze");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Up");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("NearPlane");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("NearDistance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("ImageResolution");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("ImageName");
        stream << child->GetText() << std::endl;

        stream >> camera.position.x >> camera.position.y >> camera.position.z;
        stream >> camera.gaze.x >> camera.gaze.y >> camera.gaze.z;
        stream >> camera.up.x >> camera.up.y >> camera.up.z;
        stream >> camera.near_plane.x >> camera.near_plane.y >> camera.near_plane.z >> camera.near_plane.w;
        stream >> camera.near_distance;
        stream >> camera.image_width >> camera.image_height;
        stream >> camera.image_name;

        camera.v = camera.up.normalized();
        camera.w = (camera.gaze * -1).normalized();
        camera.u = camera.v.cross(camera.w).normalized();
        camera.pixel_width = (camera.near_plane.y - camera.near_plane.x) / camera.image_width;
        camera.pixel_height = (camera.near_plane.w - camera.near_plane.z) / camera.image_height;

        scene.cameras.push_back(camera);
        element = element->NextSiblingElement("Camera");
    }

    //Get Lights
    element = root->FirstChildElement("Lights");
    auto child = element->FirstChildElement("AmbientLight");
    stream << child->GetText() << std::endl;
    stream >> scene.ambient_light.x >> scene.ambient_light.y >> scene.ambient_light.z;
    element = element->FirstChildElement("PointLight");
    PointLight point_light;
    while (element)
    {
        child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Intensity");
        stream << child->GetText() << std::endl;

        stream >> point_light.position.x >> point_light.position.y >> point_light.position.z;
        stream >> point_light.intensity.x >> point_light.intensity.y >> point_light.intensity.z;

        scene.point_lights.push_back(point_light);
        element = element->NextSiblingElement("PointLight");
    }

    //Get Materials
    element = root->FirstChildElement("Materials");
    element = element->FirstChildElement("Material");
    Material material;
    while (element)
    {
        material.is_mirror = (element->Attribute("type", "mirror") != nullptr);

        child = element->FirstChildElement("AmbientReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("DiffuseReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("SpecularReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("MirrorReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("PhongExponent");
        stream << child->GetText() << std::endl;

        stream >> material.ambient.x >> material.ambient.y >> material.ambient.z;
        stream >> material.diffuse.x >> material.diffuse.y >> material.diffuse.z;
        stream >> material.specular.x >> material.specular.y >> material.specular.z;
        stream >> material.mirror.x >> material.mirror.y >> material.mirror.z;
        stream >> material.phong_exponent;

        scene.materials.push_back(material);
        element = element->NextSiblingElement("Material");
    }

    //Get VertexData
    element = root->FirstChildElement("VertexData");
    stream << element->GetText() << std::endl;
    Vec3f vertex;
    while (!(stream >> vertex.x).eof())
    {
        stream >> vertex.y >> vertex.z;
        scene.vertex_data.push_back(vertex);
    }
    stream.clear();

    //Get Meshes
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Mesh");
    while (element)
    {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;

        int mesh_material_id;
        stream >> mesh_material_id;

        child = element->FirstChildElement("Faces");
        stream << child->GetText() << std::endl;
        Triangle mesh_triangle;
        mesh_triangle.material_id = mesh_material_id - 1;
        int v0id, v1id, v2id;
        while (!(stream >> v0id).eof())
        {
            stream >> v1id >> v2id;
            mesh_triangle.vertex_0 = scene.vertex_data[v0id - 1];
            mesh_triangle.vertex_1 = scene.vertex_data[v1id - 1];
            mesh_triangle.vertex_2 = scene.vertex_data[v2id - 1];
            scene.triangles.push_back(mesh_triangle);
        }
        stream.clear();

        element = element->NextSiblingElement("Mesh");
    }
    stream.clear();

    //Get Triangles
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Triangle");
    Triangle triangle;
    while (element)
    {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;

        int matid;
        stream >> matid;
        triangle.material_id = matid - 1;

        child = element->FirstChildElement("Indices");
        stream << child->GetText() << std::endl;

        int v0id, v1id, v2id;
        stream >> v0id >> v1id >> v2id;
        triangle.vertex_0 = scene.vertex_data[v0id - 1];
        triangle.vertex_1 = scene.vertex_data[v1id - 1];
        triangle.vertex_2 = scene.vertex_data[v2id - 1];

        scene.triangles.push_back(triangle);
        element = element->NextSiblingElement("Triangle");
    }

    //Get Spheres
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Sphere");
    Sphere sphere;
    while (element)
    {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;

        int matid;
        stream >> matid;
        sphere.material_id = matid - 1;

        child = element->FirstChildElement("Center");

        stream << child->GetText() << std::endl;

        int centervid;
        stream >> centervid;
        sphere.center_vertex = scene.vertex_data[centervid - 1];

        child = element->FirstChildElement("Radius");
        stream << child->GetText() << std::endl;
        stream >> sphere.radius;

        scene.spheres.push_back(sphere);
        element = element->NextSiblingElement("Sphere");
    }

    return scene;
}
