#ifndef COLOR_H
#define COLOR_H

#include "vector.h"
#include <stdio.h>

Vec3 unpackRGB(unsigned int packedRGB);
void writeColour(FILE *ppmFile, Vec3 color);
int compareColor(const void *a, const void *b);

#endif
