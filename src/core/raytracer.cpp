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
				Ray rayFromCamera = calculateRayFromCamera(camera, x, y);

				float tHit;
				RenderObject* hitObject = raycast(rayFromCamera, tHit);
				if (hitObject != nullptr) {
					// TODO: Can be optimized by storing this value inside the hitObject
					Material mat = scene.materials[hitObject->material_id];
					Vec3f diffuseColor, specularColor;
					Vec3f color = scene.ambient_light * mat.ambient;

					size_t lightCount = sceneToRender.point_lights.size();
					for (size_t i = 0; i < lightCount; i++)
					{
						PointLight light = sceneToRender.point_lights[i];
						Vec3f intersectionPoint = rayFromCamera.origin + rayFromCamera.direction * tHit;
						Ray rayFromLight = calculateRayFromLight(intersectionPoint, light.position);

                        Vec3f normal = hitObject->getNormal(sceneToRender, intersectionPoint);

						diffuseColor = calculateDiffuse(mat, rayFromLight, normal);

						specularColor = calculateSpecular(
                                mat,
                                light,
                                rayFromLight.direction,
                                rayFromCamera.direction * -1,
                                intersectionPoint,
                                normal
                                );
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

	// Calculate light direction from the surface point to the light source
	Vec3f lightDirection = rayFromLight.direction.normalized();

	// Calculate the diffuse reflection component using Lambert's cosine law
	float diffuseIntensity = std::max(0.0f, surfaceNormal.dot(lightDirection));

	// Diffuse reflection color
	Vec3f diffuseColor = mat.diffuse;

	// Calculate the final diffuse reflection contribution
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
