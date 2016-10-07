#ifndef ll_h
#define ll_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "GL/glu.h"

typedef struct Vector {
  GLfloat x;
  GLfloat y;
  GLfloat z;
} Vector;

typedef struct Boid {
  Vector location;
  Vector velocity;
  float angle;
} Boid;

Boid* make_boid();
Vector add_vec_vec(Vector, Vector);
Vector mult_vec_val(Vector, GLfloat);
Vector normalize_vec(Vector);
Vector cross_vec_vec(Vector vec1, Vector vec2);
float distance_vec_vec(Vector, Vector);

#endif /* ll_h */
