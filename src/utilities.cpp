#include "../include/utilities.h"

// Define the constructor for Vec3f
Vec3f::Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}

// Define the + operator for Vec3f
Vec3f Vec3f::operator+(const Vec3f& other) const {
    return Vec3f(x + other.x, y + other.y, z + other.z);
}

// Define the - operator for Vec3f
Vec3f Vec3f::operator-(const Vec3f& other) const {
    return Vec3f(x - other.x, y - other.y, z - other.z);
}

// Define the * operator for Vec3f (scalar multiplication)
Vec3f Vec3f::operator*(float scalar) const {
    return Vec3f(x * scalar, y * scalar, z * scalar);
}

Vec3f Vec3f::operator*(const Vec3f& other) const
{
    return Vec3f(x * other.x, y * other.y, z * other.z);
}

// Define the / operator for Vec3f (scalar division)
Vec3f Vec3f::operator/(float scalar) const {
    return Vec3f(x / scalar, y / scalar, z / scalar);
}

Vec3f Vec3f::operator/(const Vec3f& other) const
{
    return Vec3f(x / other.x, y / other.y, z / other.z);
}

// Define the dot product for Vec3f
float Vec3f::dot(const Vec3f& other) const {
    return x * other.x + y * other.y + z * other.z;
}

// Define the cross product for Vec3f
Vec3f Vec3f::cross(const Vec3f& other) const {
    return Vec3f(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

// Define the length of Vec3f
float Vec3f::sqrLength() const {
    if (!isSqrLengthCached){
        cachedSqrLength = x * x + y * y + z * z;
        isSqrLengthCached = true;
    }

    return cachedSqrLength;
}

float Vec3f::length() const {
    if (!isLengthCached){
        cachedLength = sqrt(sqrLength());
        isLengthCached = true;
    }

    return cachedLength;
}

// Define the normalized Vec3f
Vec3f Vec3f::normalized() const {
    float oldLength = length();
    return Vec3f(x / oldLength, y / oldLength, z / oldLength);
}
