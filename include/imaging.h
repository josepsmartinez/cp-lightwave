#ifndef IMAGING_H
#define IMAGING_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "math_ext.h"

void stretchContrast (unit** u, int N, int NFRAMES, unit* zero);
void saveFrames (unit* *u, int N, int NFRAMES, int y_points, int zero, int bw);

#endif // IMAGING_H
