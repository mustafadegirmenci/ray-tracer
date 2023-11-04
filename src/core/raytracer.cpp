#include "../../include/core/raytracer.h"
#include <limits>
#include <cstring>
#include <iostream>

bool RayTracer::raycast(const Ray& ray, RenderObject& hitObject, float& tMin) {
	bool hit = false;

	tMin = std::numeric_limits<float>::max();

	//Trace spheres
	float tSphere;
	size_t sphereCount = scene.spheres.size();

	for (int i = 0; i < sphereCount; i++) {
		if (intersectSphere(scene.spheres[i], ray, tSphere) && tSphere < tMin) {
			hit = true;
			tMin = tSphere;
			hitObject = scene.spheres[i];
		}
	}

	//Trace triangles
	float tTriangle;
	size_t triangleCount = scene.triangles.size();

	for (int i = 0; i < triangleCount; i++) {
		if (intersectTriangle(scene.triangles[i], ray, tTriangle) && tTriangle < tMin) {
			hit = true;
			tMin = tTriangle;
			hitObject = scene.triangles[i];
		}
	}

	return hit;
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

				RenderObject hitObject {};
				float tHit;
				if (raycast(ray, hitObject, tHit)) {
					// TODO: Can be optimized by storing this value inside the hitObject
					Material mat = scene.materials[hitObject.material_id];
					Vec3f diffuseColor, specularColor;
					Vec3f color = scene.ambient_light * mat.ambient;
					bool isContinue;

					for (size_t i = 0; i < sceneToRender.point_lights.size(); i++)
					{
						PointLight light = sceneToRender.point_lights[i];
						Vec3f intersectionPoint = ray.origin + ray.direction * tHit;
						Ray shadowRay = calculateShadowRay(intersectionPoint, light.position);

						RenderObject pObject;
						float tObject;
						float tLight = (light.position - shadowRay.origin).x / shadowRay.direction.x;
						if (raycast(shadowRay, pObject, tObject) && tObject < tLight) {
							continue;
						}

						Vec3f normal;
						normal = hitObject.getNormal(sceneToRender, intersectionPoint);

						diffuseColor = calculateDiffuse(mat,shadowRay,normal);
						//specularColor = calculateSpecular();
						color = color + diffuseColor + specularColor;
					}

					// Set color as object's color
					/*color = color + scene.materials[hitObject.material_id - 1].diffuse * 255;*/
					color = clamp(color);
					result->setPixel(x, y, (unsigned char) color.x, (unsigned char) color.y, (unsigned char) color.z);
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

bool RayTracer::intersectSphere(const Sphere& sphere, const Ray& ray, float& t) const {
	Vec3f oc = ray.origin - sphere.center_vertex;
	float a = ray.direction.dot(ray.direction);
	float b = 2.0f * oc.dot(ray.direction);
	float c = oc.dot(oc) - sphere.radius * sphere.radius;
	float discriminant = b * b - 4 * a * c;

	if (discriminant > 0) {
		float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
		float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);
		t = (t1 < t2) ? t1 : t2;
		return true;
	}
	else if (discriminant == 0) {
		t = -b / (2.0f * a);
		return true;
	}

	return false;
}

bool RayTracer::intersectTriangle(const Triangle& triangle, const Ray& ray, float& t) const {
	Vec3f e1 = triangle.vertex_1 - triangle.vertex_0;
	Vec3f e2 = triangle.vertex_2 - triangle.vertex_0;
	Vec3f h = ray.direction.cross(e2);
	float a = e1.dot(h);

	if (a > -std::numeric_limits<float>::epsilon() && a < std::numeric_limits<float>::epsilon())
		return false;

	float f = 1.0f / a;
	Vec3f s = ray.origin - triangle.vertex_0;
	float u = f * s.dot(h);

	if (u < 0.0f || u > 1.0f)
		return false;

	Vec3f q = s.cross(e1);
	float v = f * ray.direction.dot(q);

	if (v < 0.0f || u + v > 1.0f)
		return false;

	t = f * e2.dot(q);

	if (t > std::numeric_limits<float>::epsilon())
		return true;

	return false;
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

Vec3f RayTracer::calculateSpecular(const Vec3f& lightPosition, const Vec3f& surfacePoint, const Vec3f& surfaceNormal, const Vec3f& viewDirection, float shininess)
{
	Vec3f color;

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
