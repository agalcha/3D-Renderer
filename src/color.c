#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "spheres.h"

void writeColour(FILE *ppmFile, Vec3 color) {
    int r = (int)(255.999 * fminf(fmaxf(color.x, 0.0f), 1.0f));
    int g = (int)(255.999 * fminf(fmaxf(color.y, 0.0f), 1.0f));
    int b = (int)(255.999 * fminf(fmaxf(color.z, 0.0f), 1.0f));

    fprintf(ppmFile, "%d %d %d\n", r, g, b);
}

Vec3 unpackRGB(unsigned int packedRGB) {
    float r = ((packedRGB >> 16) & 0xFF) / 255.0f;
    float g = ((packedRGB >> 8) & 0xFF) / 255.0f;
    float b = (packedRGB & 0xFF) / 255.0f;
    return (Vec3){r, g, b};
}

int compareColor(const void *a, const void *b)
{
    int a1 = 0, b1 = 0;
    for (int i = 0; i < (int)sizeof(int); i++)
    {
        a1 |= (*((unsigned char*)a + i) & 0x0F) << (i * 8);
        b1 |= (*((unsigned char*)b + i) & 0x0F) << (i * 8);
    }
    
    return (a1 < b1) ? -1 : (b1 < a1) ? 1 : (*((int*)a) < *((int*)b)) ? -1 : (*((int*)a) > *((int*)b)) ? 1 : 0;
}