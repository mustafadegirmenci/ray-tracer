#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>
#include "../geometry/triangle.h"
#include "../geometry/sphere.h"

class RenderResult {
public:
    RenderResult(const char *imageName, int width, int height);
    ~RenderResult();
    void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);

public:
    char* image_name;
    Color* image;
    int width;
    int height;
};

class RayTracer{
    Scene scene;

public:
    vector<RenderResult*> render(const Scene&);

private:
    Ray calculateRay(const Camera& camera, int x, int y);
    bool raycast(const Ray& ray, RenderObject& hitObject, float &tMin);
    bool intersectSphere(Sphere sphere, const Ray& ray, float& t) const;
    bool intersectTriangle(Triangle triangle, const Ray& ray, float& t) const;
    Vec3f clamp(Vec3f& x);
};

#endif // RAYTRACER_H
