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
	Ray calculateViewingRay(const Camera& camera, int x, int y);
    RenderObject* raycast(const Ray& ray, float& tMin);
	Ray calculateShadowRay(const Vec3f& origin, const Vec3f& destination);
	Vec3f calculateDiffuse(const Material& mat, const Ray& shadowRay, const Vec3f& surfaceNormal);
	Vec3f calculateSpecular(const Material& mat, const Ray& shadowRay, const Vec3f& surfaceNormal, const Vec3f& viewDirection, const Vec3f& lightIntensity);
	Vec3f clamp(Vec3f& x);
};

#endif // RAYTRACER_H
