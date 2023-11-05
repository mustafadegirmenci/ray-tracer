#include "../../include/core/raytracer.h"
#include <limits>
#include <cstring>
#include <iostream>

RenderObject* RayTracer::raycast(Ray* ray, float& tMin) {
	RenderObject* hitObject = nullptr;

	tMin = std::numeric_limits<float>::max();

	//Trace render objects
	float tRenderObject;
	size_t renderObjectCount = scene.render_objects.size();

	for (int i = 0; i < renderObjectCount; i++) {
		if (scene.render_objects[i]->intersect(ray, tRenderObject) && tRenderObject < tMin) {
			tMin = tRenderObject;
			hitObject = scene.render_objects[i];
		}
	}

	return hitObject;
}

vector<RenderResult*> RayTracer::render(const Scene& sceneToRender) {
	scene = sceneToRender;

	size_t cameraCount = scene.cameras.size();
	vector<RenderResult*> results;

	for (int i = 0; i < cameraCount; i++) {
		Camera camera = scene.cameras[i];
		auto* result = new RenderResult(camera.image_name.c_str(), camera.image_width, camera.image_height);

		for (int y = 0; y < camera.image_height; y++) {
			for (int x = 0; x < camera.image_width; x++) {
                Ray rayFromCamera = calculateRayFromCamera(camera, x, y);
				Vec3f computedColor = computeColor(&rayFromCamera);
                computedColor = clamp(computedColor);
                result->setPixel(x, y,
                                 (unsigned char )(int)(computedColor.x),
                                 (unsigned char )(int)computedColor.y,
                                 (unsigned char )(int)computedColor.z);
            }
		}

		results.push_back(result);
	}

	return results;
}

Vec3f RayTracer::computeColor(Ray *ray) {

    if (ray->depth > scene.max_recursion_depth){
        return Vec3f();
    }

    float tHit;
    RenderObject* hitObject = raycast(ray, tHit);

    if (hitObject != nullptr){
        return applyShading(hitObject, ray, tHit);
    }
    else if (ray->depth == 0){
        Color bg = scene.background_color;
        return Vec3f(bg.r, bg.g, bg.b);
    }
    else{
        return Vec3f();
    }
}

Vec3f RayTracer::applyShading(RenderObject* hitObject, Ray* ray, const float& tHit){

    Material mat = scene.materials[hitObject->material_id];
    Vec3f intersectionPoint = ray->origin + ray->direction * tHit;
    Vec3f intersectionNormal = hitObject->getNormal(scene, intersectionPoint);
    size_t lightCount = scene.point_lights.size();

    Vec3f shadedColor;

    if (mat.is_mirror){

        auto* reflectionRay = new Ray();
        reflectionRay->origin = intersectionPoint;
        reflectionRay->direction = ray->direction +intersectionNormal * 2 * (intersectionNormal.dot(ray->direction));

        reflectionRay->depth = ray->depth + 1;
        shadedColor = shadedColor + computeColor(reflectionRay) * mat.mirror;
    }

    for (size_t lightIndex = 0; lightIndex < lightCount; lightIndex++) {
        PointLight light = scene.point_lights[lightIndex];
        Ray rayFromLight = calculateRayFromLight(intersectionPoint, light.position);

        shadedColor = shadedColor + calculateDiffuse(
                mat,
                rayFromLight,
                intersectionNormal);

        shadedColor = shadedColor + calculateSpecular(
                mat,
                light,
                rayFromLight.direction,
                ray->direction * -1,
                intersectionPoint,
                intersectionNormal);
    }

    return shadedColor;
}

Ray RayTracer::calculateRayFromCamera(const Camera& camera, int x, int y) {
	Ray ray;

	Vec3f v = camera.v;
	Vec3f w = camera.w;
	Vec3f u = camera.u;

	Vec3f e = camera.position;

	Vec3f m = e - w * camera.near_distance;
	Vec3f q = m + u * camera.near_plane.x + v * camera.near_plane.w;

	float su = (x + 0.5f) * camera.pixel_width;
	float sv = (y + 0.5f) * camera.pixel_height;

	Vec3f s = q + u * su - v * sv;

	ray.origin = e;
	ray.direction = (s - e).normalized();

	return ray;
}

Ray RayTracer::calculateRayFromLight(const Vec3f& origin, const Vec3f& destination)
{
	Ray ray;

	ray.origin = origin;
	ray.direction = destination - origin;

	return ray;
}

Vec3f RayTracer::calculateDiffuse(const Material& mat, const Ray& rayFromLight, const Vec3f& surfaceNormal)
{
	Vec3f color;
	Vec3f lightDirection = rayFromLight.direction.normalized();
	float diffuseIntensity = std::max(0.0f, surfaceNormal.dot(lightDirection));
	Vec3f diffuseColor = mat.diffuse;
	color = diffuseColor * diffuseIntensity;

	return color;
}

Vec3f RayTracer::calculateIrradiance(const PointLight& pointLight, const Vec3f& intersectionPoint){
    Vec3f irradiance;

    Vec3f d = pointLight.position - intersectionPoint;
    float d_square = d.dot(d);

    if (d_square != 0.0f){
        irradiance.x = pointLight.intensity.x/d_square;
        irradiance.y = pointLight.intensity.y/d_square;
        irradiance.z = pointLight.intensity.z/d_square;
    }

    return irradiance;
}

Vec3f RayTracer::calculateSpecular(const Material& mat,
                                   const PointLight& pointLight,
                                   const Vec3f& rayDirectionFromIntersectionToLight,
                                   const Vec3f& rayDirectionFromIntersectionToCamera,
                                   const Vec3f& intersectionPoint,
                                   const Vec3f& intersectionNormal) {
    Vec3f sum = rayDirectionFromIntersectionToLight.normalized() + rayDirectionFromIntersectionToCamera.normalized();
    Vec3f halfVector = ((sum) / (sum.length())).normalized();

    float cosAlpha = max(0.0f, intersectionNormal.dot(halfVector));

    return mat.specular * pow(cosAlpha, mat.phong_exponent) * calculateIrradiance(pointLight, intersectionPoint);
}

Vec3f RayTracer::clamp(Vec3f& x)
{
	if (x.x > 255)	x.x = 255;
	else if (x.x < 0)	x.x = 0;

	if (x.y > 255)	x.y = 255;
	else if (x.y < 0)	x.y = 0;

	if (x.z > 255)	x.z = 255;
	else if (x.z < 0)	x.z = 0;

	return x;
}

RenderResult::RenderResult(const char* imageName, int width, int height) : width(width), height(height) {
	image_name = new char[strlen(imageName) + 1];
	strcpy(image_name, imageName);

	image = new Color[width * height];
}

RenderResult::~RenderResult() {
	delete[] image_name;
	delete[] image;
}

void RenderResult::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
	int index = (y * width) + x;
	image[index].r = r;
	image[index].g = g;
	image[index].b = b;
}
