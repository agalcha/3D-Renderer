#include "vector.h"
#include <math.h>

Vec3 add(Vec3 v1, Vec3 v2) {
    return (Vec3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vec3 subtract(Vec3 v1, Vec3 v2) {
    return (Vec3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vec3 scalarMultiply(float s, Vec3 v) {
    return (Vec3){s * v.x, s * v.y, s * v.z};
}

Vec3 scalarDivide(Vec3 v, float d) {
    return (Vec3){v.x / d, v.y / d, v.z / d};
}

Vec3 normalize(Vec3 v) {
    float len = length(v);
    return scalarDivide(v, len);
}

float dot(Vec3 v1, Vec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float length2(Vec3 v) {
    return dot(v, v);
}

float length(Vec3 v) {
    return sqrt(length2(v));
}

float distance2(Vec3 v1, Vec3 v2) {
    return length2(subtract(v1, v2));
}

float distance(Vec3 v1, Vec3 v2) {
    return sqrt(distance2(v1, v2));
}
