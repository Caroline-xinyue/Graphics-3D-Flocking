#ifndef ll_h
#define ll_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef enum {FALSE, TRUE} bool;
#define VERTICAL_JUSTIFICATION 1
#define WEIGHTS_UPPER_BOUND 2
#define NEIGHBOR_RADIUS 1000
#define ALIGNMENT_WEIGHT 10000
#define COHESION_WEIGHT 0
#define SEPARATION_WEIGHT 0

typedef struct Vector {
  float x;
  float y;
  float z;
} Vector;

typedef struct Boid {
  Vector old_location;
  Vector location;
  Vector velocity;
} Boid;

Boid* make_boid();
Vector add_vec_vec(Vector, Vector);
Vector mult_vec_val(Vector, float);
Vector normalize_vec(Vector);
float distance_vec_vec(Vector, Vector);

#endif /* ll_h */
