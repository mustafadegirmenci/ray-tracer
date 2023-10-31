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

//TODO: Complete
vector<RenderResult*> RayTracer::render(const Scene& sceneToRender) {
	scene = sceneToRender;

	size_t cameraCount = scene.cameras.size();
	vector<RenderResult*> results;

	for (int i = 0; i < cameraCount; i++) {
		Camera camera = scene.cameras[i];
		auto* result = new RenderResult(camera.image_name.c_str(), camera.image_width, camera.image_height);

		for (int y = 0; y < camera.image_height; y++) {
			for (int x = 0; x < camera.image_width; x++) {
				Ray ray = calculateRay(camera, x, y);

				RenderObject hitObject{};
				float tHit;
				if (raycast(ray, hitObject, tHit)) {
					// Set color as object's color
                    Vec3f mat = scene.materials[hitObject.material_id - 1].diffuse * 255;
					result->setPixel(x, y, (unsigned char) mat.x, (unsigned char) mat.y, (unsigned char) mat.z);
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


//TODO: Check
Ray RayTracer::calculateRay(const Camera& camera, int x, int y) {
    // Calculate the direction vector from the camera position to the pixel on the image plane
    float image_aspect_ratio = static_cast<float>(camera.image_width) / static_cast<float>(camera.image_height);
    float pixel_width = 4.0f * camera.near_distance * tan(camera.near_plane.w / 2.0f);
    float pixel_height = pixel_width / image_aspect_ratio;

    // Calculate the direction vector from the camera's "gaze" and "up" vectors
    Vec3f right = camera.gaze.cross(camera.up).normalized();
    Vec3f up = right.cross(camera.gaze).normalized();

    // Calculate the point on the image plane corresponding to the pixel
    float u = static_cast<float>(x) / static_cast<float>(camera.image_width - 1);
    float v = static_cast<float>(y) / static_cast<float>(camera.image_height - 1);

    // Calculate the point on the near plane
    Vec3f near_plane_point = camera.position + camera.gaze * camera.near_distance +
                             right * (u - 0.5f) * pixel_width +
                             up * (v - 0.5f) * pixel_height;

    // Calculate the ray direction
    Vec3f ray_direction = (near_plane_point - camera.position).normalized();

    // Create and return the ray
    Ray ray;
    ray.origin = camera.position;
    ray.direction = ray_direction;

    return ray;
}

bool RayTracer::intersectSphere(Sphere sphere, const Ray& ray, float& t) const {
	Vec3f oc = ray.origin - scene.vertex_data[sphere.center_vertex_id - 1];
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

bool RayTracer::intersectTriangle(Triangle triangle, const Ray& ray, float& t) const {
	Vec3f e1 = scene.vertex_data[triangle.indices.v1_id - 1] - scene.vertex_data[triangle.indices.v0_id - 1];
	Vec3f e2 = scene.vertex_data[triangle.indices.v2_id - 1] - scene.vertex_data[triangle.indices.v0_id - 1];
	Vec3f h = ray.direction.cross(e2);
	float a = e1.dot(h);

	if (a > -std::numeric_limits<float>::epsilon() && a < std::numeric_limits<float>::epsilon())
		return false;

	float f = 1.0f / a;
	Vec3f s = ray.origin - scene.vertex_data[triangle.indices.v0_id - 1];
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

RenderResult::RenderResult(const char* imageName, int width, int height) : width(width), height(height) {
	image_name = new char[strlen(imageName) + 1];
	strcpy(image_name, imageName);

	image = new Color[width * height];
}

RenderResult::~RenderResult() {
	delete[] image;
}

void RenderResult::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
	int index = (y * width) + x;
	image[index].r = r;
	image[index].g = g;
	image[index].b = b;

	if (index == 640001) std::cout << "hit" << std::endl;
}
