#include <stdlib.h>
#include "spheres.h"
#include "vector.h"
#include <math.h>

// Initialize the world with default capacity of 1
void worldInit(World *world) {
    world->size = 0;
    world->capacity = 1;
    world->spheres = malloc(sizeof(Sphere*) * world->capacity);
}

// Free all memory associated with the world and its spheres
void freeWorld(World *world) {
    for (int i = 0; i < world->size; ++i) {
        free(world->spheres[i]);
    }
    free(world->spheres);
}

// Add a sphere to the world
void addSphere(World *world, Sphere *sphere) {
    if (world->size >= world->capacity) {
        world->capacity *= 2;
        world->spheres = realloc(world->spheres, sizeof(Sphere*) * world->capacity);
    }
    world->spheres[world->size++] = sphere;
}

// Create a new sphere with specified radius, position, and color
Sphere *createSphere(float radius, Vec3 position, Vec3 color) {
    Sphere *sphere = malloc(sizeof(Sphere));
    sphere->r = radius;
    sphere->pos = position;
    sphere->color = color;
    return sphere;
}

int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t) {
    Vec3 V = subtract(rayPos, sphere->pos);
    float a = dot(rayDir, rayDir);
    float b = 2.0f * dot(rayDir, V);
    float c = dot(V, V) - sphere->r * sphere->r;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return 0; // No intersection
    }

    float sqrtDiscriminant = sqrt(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
    float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

    // Only consider the closest positive t
    if (t1 > 0) {
        *t = t1;
    } else if (t2 > 0) {
        *t = t2;
    } else {
        return 0; // Both intersections are behind the ray origin
    }

    return 1; // Intersection found
}