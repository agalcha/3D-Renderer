#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "spheres.h"
#include "color.h"
#include <math.h>


void render(FILE *out, int imageWidth, int imageHeight, Vec3 camera_pos, float viewportWidth, float viewportHeight, float focalLength, World *world, Vec3 blackColor, Vec3 lposition, float lbright) {

    //PPM header
    fprintf(out, "P3\n%d %d\n255\n", imageWidth, imageHeight);

    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            Vec3 finalColor = blackColor;

#ifdef MS2
            //without anti-aliasing
            float u = (x - imageWidth / 2.0f) * (viewportWidth / imageWidth);
            float v = (imageHeight / 2.0f - y) * (viewportHeight / imageHeight);

            Vec3 ray_direction = normalize((Vec3){u, v, -focalLength});

            Vec3 pixel_color = blackColor;

            //checking if the rays intersect with the spheres
            if (world->size > 0) {
                float closestT = INFINITY;
                Sphere *closestSphere = NULL;

                for (int i = 0; i < world->size; i++) {
                    float t;
                    if (doesIntersect(world->spheres[i], camera_pos, ray_direction, &t)) {
                        if (t < closestT) {
                            closestT = t;
                            closestSphere = world->spheres[i];
                        }
                    }
                }

                //determining pixel color using all the formulas provided
                if (closestSphere) {
                    Vec3 intersection_point = add(camera_pos, scalarMultiply(closestT, ray_direction));

                    Vec3 normal = normalize(subtract(intersection_point, closestSphere->pos));

                    Vec3 light_direction = normalize(subtract(lposition, intersection_point));

                    float distance_to_light = distance(lposition, intersection_point);

                    Vec3 origin_shadow_ray = add(intersection_point, scalarMultiply(0.001f, normal));

                    //checking for shadows
                    int shadow = 0;
                    for (int i = 0; i < world->size; i++) {
                        float t;

                        if (doesIntersect(world->spheres[i], origin_shadow_ray, light_direction, &t) && t > 0.001f) {
                            shadow = 1;
                            break;
                        }
                    }

                    float intensity = lbright / (distance_to_light * distance_to_light);
                    float brightness = fmaxf(0.0f, dot(light_direction, normal)) * intensity;

                    if (shadow) {
                        brightness *= 0.1f;
                    }

                    brightness = fminf(1.0f, brightness); //clamp brightness
                    pixel_color = scalarMultiply(brightness, closestSphere->color);
                }
            }

            finalColor = pixel_color;

#else // FS

            finalColor = (Vec3){0.0f, 0.0f, 0.0f}; // Reset for anti-aliasing


            for (int ay = 0; ay < 3; ay++) {
                for (int ax = 0; ax < 3; ax++) {
                    
                    float X = (x + (ax / 3.0f)) + 0.17;
                    float Y = (y +(ay / 3.0f)) + 0.17;

                    float u = (X - imageWidth / 2.0f) * (viewportWidth / imageWidth);
                    float v = (imageHeight / 2.0f - Y) * (viewportHeight / imageHeight);

                    Vec3 ray_direction = normalize((Vec3){u, v, -focalLength});

                    Vec3 pixel_color = blackColor;

                    //checking if the rays intersect with the spheres
                    if (world->size > 0) {
                        float closestT = INFINITY;
                        Sphere *closestSphere = NULL;

                        for (int i = 0; i < world->size; i++) {
                            float t;

                            if (doesIntersect(world->spheres[i], camera_pos, ray_direction, &t)) {
                                if (t < closestT) {
                                    closestT = t;
                                    closestSphere = world->spheres[i];
                                }
                            }
                        }

                        //determining pixel color using all the formulas provided
                        if (closestSphere) {
                            Vec3 intersection_point = add(camera_pos, scalarMultiply(closestT, ray_direction));

                            Vec3 normal = normalize(subtract(intersection_point, closestSphere->pos));

                            Vec3 light_direction = normalize(subtract(lposition, intersection_point));

                            float distance_to_light = distance(lposition, intersection_point);

                            Vec3 origin_shadow_ray = add(intersection_point, scalarMultiply(0.001f, normal));

                            //checking for shadows
                            int shadow = 0;
                            for (int i = 0; i < world->size; i++) {
                                float t;
                                if (doesIntersect(world->spheres[i], origin_shadow_ray, light_direction, &t) && t > 0.001f) {
                                    shadow = 1;
                                    break;
                                }
                            }

                            //calculating the light intensity and color
                            float intensity = lbright / (distance_to_light * distance_to_light);
                            float brightness = fmaxf(0.0f, dot(light_direction, normal)) * intensity;

                            if (shadow) {
                                brightness *= 0.1f;
                            }

                            brightness = fminf(1.0f, brightness); //clamp brightness
                            pixel_color = scalarMultiply(brightness, closestSphere->color);
                        }
                    }

                    finalColor = add(finalColor, pixel_color);
                }
            }
            if (world->size <= 0) {
                finalColor = blackColor;
            }

            finalColor = scalarDivide(finalColor, 9.0f); //average for anti-aliasing
#endif

            writeColour(out, finalColor);
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    //open input file for reading
    FILE *in = fopen(argv[1], "r");
    if (in == NULL) {
        fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
        return 1;
    }

    //open output file for writing
    FILE *out = fopen(argv[2], "w");
    if (out == NULL) {
        fprintf(stderr, "Error: Cannot open output file %s\n", argv[2]);
        fclose(in);
        return 1;
    }

    //default color
    Vec3 blackColor = {0.0f, 0.0f, 0.0f}; 

    //initialize the world
    World world;
    worldInit(&world);

    //Getting input
    int imageWidth, imageHeight;
    float viewportHeight, viewportWidth, focalLength;

    if (fscanf(in, "%d %d", &imageWidth, &imageHeight) != 2) {
        fprintf(stderr, "Error: Failed to read image dimensions\n");
        fclose(in);
        fclose(out);
        return 1;
    }
    
    if (fscanf(in, "%f", &viewportHeight) != 1 || fscanf(in, "%f", &focalLength) != 1) {
        fprintf(stderr, "Error: Failed to read viewport height or focal length\n");
        fclose(in);
        fclose(out);
        return 1;
    }

    viewportWidth = ((float)imageWidth / imageHeight) * viewportHeight;


    Vec3 lposition;
    float lbright;
    if (fscanf(in, "%f %f %f %f", &lposition.x, &lposition.y, &lposition.z, &lbright) != 4) {
        fprintf(stderr, "Error: Failed to read light position and brightness\n");
        fclose(in);
        fclose(out);
        return 1;
    }

    
    int numcolors;
    if (fscanf(in, "%d", &numcolors) != 1) {
        fprintf(stderr, "Error: Failed to read number of colors\n");
        fclose(in);
        fclose(out);
        return 1;
    }

    // Read colors and sort them
    unsigned int *colors = malloc(sizeof(unsigned int) * numcolors);
    Vec3 *sortedColors = malloc(sizeof(Vec3) * numcolors);

    for (int i = 0; i < numcolors; i++) {
        if (fscanf(in, "%x", &colors[i]) != 1) {
            fprintf(stderr, "Error: Failed to read color %d\n", i + 1);
            fclose(in);
            fclose(out);
            free(colors);
            free(sortedColors);
            return 1;
        }
    }

    qsort(colors, numcolors, sizeof(unsigned int), compareColor);

    for (int i = 0; i < numcolors; i++) {
        sortedColors[i] = unpackRGB(colors[i]);
    }
    
    int bg_color_index;
    if (fscanf(in, "%d", &bg_color_index) != 1) {
        fprintf(stderr, "Error: Failed to read background color index\n");
        fclose(in);
        fclose(out);
        free(colors);
        free(sortedColors);
        return 1;
    }

    #ifdef FS
    blackColor = sortedColors[bg_color_index];
    #endif

    int numspheres;
    if (fscanf(in, "%d", &numspheres) != 1) {
        fprintf(stderr, "Error: Failed to read number of spheres\n");
        fclose(in);
        fclose(out);
        free(colors);
        free(sortedColors);
        return 1;
    }

    //read each sphere's details
    for (int i = 0; i < numspheres; i++) {
        Vec3 sposition;
        float radius;
        int color_index;

        if (fscanf(in, "%f %f %f %f %d", &sposition.x, &sposition.y, &sposition.z, &radius, &color_index) != 5) {
            fprintf(stderr, "Error: Failed to read sphere %d details\n", i + 1);
            fclose(in);
            fclose(out);
            free(colors);
            free(sortedColors);
            return 1;
        }

        //create a sphere with the default color
        Sphere *sphere = NULL;
        #ifdef FS
        sphere = createSphere(radius, sposition, sortedColors[color_index]);
        #else
        sphere = createSphere(radius, sposition, (Vec3){1.0f, 1.0f, 1.0f});
        #endif
        addSphere(&world, sphere);

    }
    free(colors);
    free(sortedColors);
    

#ifdef MS1
    // Add
    Vec3 sum = add(blackColor, lposition);
    fprintf(out, "(%.1f, %.1f, %.1f) + (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
            blackColor.x, blackColor.y, blackColor.z, lposition.x, lposition.y, lposition.z, sum.x, sum.y, sum.z);

    // Subtract
    Vec3 diff = subtract(blackColor, lposition);
    fprintf(out, "(%.1f, %.1f, %.1f) - (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
            blackColor.x, blackColor.y, blackColor.z, lposition.x, lposition.y, lposition.z, diff.x, diff.y, diff.z);

    // Scalar Multiply
    Vec3 scaled = scalarMultiply(viewportWidth, lposition);
    fprintf(out, "%.1f * (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
            viewportWidth, lposition.x, lposition.y, lposition.z, scaled.x, scaled.y, scaled.z);

    // Normalize
    Vec3 normalized = normalize(lposition);
    fprintf(out, "normalize(%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n",
            lposition.x, lposition.y, lposition.z, normalized.x, normalized.y, normalized.z);

    // Loop through spheres and perform vector operations for each sphere
    for (int i = 0; i < world.size; i++) {
        Sphere *sphere = world.spheres[i];
        fprintf(out, "\n");

        // Scalar Divide
        Vec3 divided = scalarDivide(sphere->color, sphere->r);
        fprintf(out, "(%.1f, %.1f, %.1f) / %.1f = (%.1f, %.1f, %.1f)\n",
                sphere->color.x, sphere->color.y, sphere->color.z, sphere->r, divided.x, divided.y, divided.z);

        // Dot product
        float dotProduct = dot(lposition, sphere->pos);
        fprintf(out, "dot((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",
                lposition.x, lposition.y, lposition.z, sphere->pos.x, sphere->pos.y, sphere->pos.z, dotProduct);

        // Distance
        float dist = distance(lposition, sphere->pos);
        fprintf(out, "distance((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n",
                lposition.x, lposition.y, lposition.z, sphere->pos.x, sphere->pos.y, sphere->pos.z, dist);

        // Length
        float len = length(sphere->pos);
        fprintf(out, "length(%.1f, %.1f, %.1f) = %.1f\n", sphere->pos.x, sphere->pos.y, sphere->pos.z, len);
    }
#endif

#ifdef MS2
    
    Vec3 camera_pos = {0.0f, 0.0f, 0.0f};
    render(out, imageWidth, imageHeight, camera_pos, viewportWidth, viewportHeight, focalLength, &world, blackColor, lposition, lbright);

#endif

#ifdef FS
    
    Vec3 camera_pos = {0.0f, 0.0f, 0.0f};
    render(out, imageWidth, imageHeight, camera_pos, viewportWidth, viewportHeight, focalLength, &world, blackColor, lposition, lbright);

#endif


    // Cleanup
    freeWorld(&world);
    fclose(in);
    fclose(out);
    return 0;
}