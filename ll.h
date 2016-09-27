#ifndef ll_h
#define ll_h

#include <stdio.h>

typedef enum {FALSE, TRUE} bool;
#define VERTICAL_JUSTIFICATION 1
#define WEIGHTS_UPPER_BOUND 2
#define NEIGHBOR_RADIUS 100000
#define ALIGNMENT_WEIGHT 0
#define COHESION_WEIGHT 0
#define SEPARATION_WEIGHT 1000

typedef struct Vector {
    float x;
    float y;
    float z;
} Vector;

typedef struct Boid {
    Vector location;
    Vector velocity;
} Boid;

Vector add_vec_vec(Vector, Vector);
Vector mult_vec_val(Vector, float);
Vector normalize_vec(Vector);
float distance_vec_vec(Vector, Vector);

#endif /* ll_h */
