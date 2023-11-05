#include "../../include/core/raytracer.h"
#include <limits>
#include <cstring>
#include <iostream>

RenderObject* RayTracer::raycast(const Ray& ray, float& tMin) {
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
				Ray ray = calculateViewingRay(camera, x, y);

				float tHit;
				RenderObject* hitObject = raycast(ray, tHit);
				if (hitObject != nullptr) {
					// TODO: Can be optimized by storing this value inside the hitObject
					Material mat = scene.materials[hitObject->material_id];
					Vec3f diffuseColor, specularColor;
					Vec3f color = scene.ambient_light * mat.ambient;

					size_t lightCount = sceneToRender.point_lights.size();
					for (size_t i = 0; i < lightCount; i++)
					{
						PointLight light = sceneToRender.point_lights[i];
						Vec3f intersectionPoint = ray.origin + ray.direction * tHit;
						Ray shadowRay = calculateShadowRay(intersectionPoint, light.position);

						float tObject;
						RenderObject* pObject = raycast(shadowRay, tObject);
						float tLight = (light.position - shadowRay.origin).x / shadowRay.direction.x;

						//if (pObject != nullptr && tObject < tLight) {
						//	continue;
						//}

						Vec3f normal;
						normal = hitObject->getNormal(sceneToRender, intersectionPoint);

						diffuseColor = calculateDiffuse(mat, shadowRay, normal);
						specularColor = calculateSpecular(mat, shadowRay, normal, ray.direction * -1, light.intensity);
						color = color + diffuseColor + specularColor;
					}

					// Set color as object's color
					color = clamp(color);
					result->setPixel(x, y, (unsigned char) (int) color.x, (unsigned char) (int) color.y, (unsigned char) (int) color.z);
				}
				else {
					// Set color as background color
					result->setPixel(x, y, (unsigned char) scene.background_color.r, (unsigned char) scene.background_color.g, (unsigned char) scene.background_color.b);
				}
			}
		}

		results.push_back(result);
	}

	return results;
}

Ray RayTracer::calculateViewingRay(const Camera& camera, int x, int y) {
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
	ray.direction = s - e;

	return ray;
}

Ray RayTracer::calculateShadowRay(const Vec3f& origin, const Vec3f& destination)
{
	Ray ray;

	// Calculate direction as the normalized vector from origin to destination
	ray.origin = origin;
	ray.direction = (destination - origin).normalized();

	return ray;
}

Vec3f RayTracer::calculateDiffuse(const Material& mat, const Ray& shadowRay, const Vec3f& surfaceNormal)
{
	Vec3f color;

	// Calculate light direction from the surface point to the light source
	Vec3f lightDirection = shadowRay.direction;

	// Calculate the diffuse reflection component using Lambert's cosine law
	float diffuseIntensity = std::max(0.0f, surfaceNormal.dot(lightDirection));

	// Diffuse reflection color
	Vec3f diffuseColor = mat.diffuse;

	// Calculate the final diffuse reflection contribution
	color = diffuseColor * diffuseIntensity;

	return color;
}

Vec3f RayTracer::calculateSpecular(const Material& mat, const Ray& shadowRay, const Vec3f& surfaceNormal, const Vec3f& viewDirection, const Vec3f& lightIntensity) {

	Vec3f color;
	float shininess = mat.phong_exponent;

	// Calculate the reflection direction using the formula: R = 2(N . L)N - L
	Vec3f lightDirection = shadowRay.direction;
	Vec3f reflectionDirection = 2 * surfaceNormal.dot(lightDirection) * surfaceNormal - lightDirection;

	// Calculate the specular intensity using the view direction and reflection direction
	float specularIntensity = std::pow(std::max(0.0f, reflectionDirection.dot(viewDirection)), shininess);

	// Specular reflection color
	Vec3f specularColor = mat.specular;

	// Calculate the final specular reflection contribution
	color = specularColor * lightIntensity * specularIntensity;

	return color;
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
