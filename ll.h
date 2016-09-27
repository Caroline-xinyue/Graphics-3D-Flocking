#ifndef ll_h
#define ll_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef enum {FALSE, TRUE} bool;
#define VERTICAL_JUSTIFICATION 1
#define WEIGHTS_UPPER_BOUND 2
#define NEIGHBOR_RADIUS 100000
#define ALIGNMENT_WEIGHT 10
#define COHESION_WEIGHT 10
#define SEPARATION_WEIGHT 0
#define TENDENCY_TO_GOAL_WEIGHT 0.1 
#define BOID_VEL_FACTOR 80

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

typedef struct Goal {
  Vector location;
  Vector velocity;
} Goal;

Boid* make_boid();
Vector add_vec_vec(Vector, Vector);
Vector mult_vec_val(Vector, float);
Vector normalize_vec(Vector);
float distance_vec_vec(Vector, Vector);

#endif /* ll_h */
