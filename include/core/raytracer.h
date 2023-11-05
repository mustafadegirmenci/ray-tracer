#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>
#include "../geometry/triangle.h"
#include "../geometry/sphere.h"

class RenderResult {
public:
	RenderResult(const char* imageName, int width, int height);
	~RenderResult();
	void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);

public:
	char* image_name;
	Color* image;
	int width;
	int height;
};

class RayTracer {
	Scene scene;

public:
	vector<RenderResult*> render(const Scene&);

private:
	Ray calculateRayFromCamera(const Camera& camera, int x, int y);
    RenderObject* raycast(Ray* ray, float& tMin, RenderObject* ignoredObject);
	Vec3f calculateDiffuse(const Material& mat, const Ray& rayFromLight, const Vec3f& surfaceNormal, const PointLight& light, const Vec3f& intersectionPoint);
    Vec3f calculateIrradiance(const PointLight& pointLight, const Vec3f& intersectionPoint);
	Vec3f clamp(Vec3f& x);

    Vec3f
    calculateSpecular(const Material &mat, const PointLight &pointLight,
                      const Vec3f &rayDirectionFromIntersectionToLight,
                      const Vec3f &rayDirectionFromIntersectionToCamera, const Vec3f &intersectionPoint,
                      const Vec3f &intersectionNormal);

    Vec3f applyShading(RenderObject *hitObject, Ray* ray, const float &tHit);

    Vec3f computeColor(Ray *ray, RenderObject* ignoredObject);
};

#endif // RAYTRACER_H
