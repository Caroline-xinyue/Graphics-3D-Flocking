#ifndef ll_h
#define ll_h

#include <stdio.h>

typedef enum {FALSE, TRUE} bool;
#define VERTICAL_JUSTIFICATION 1
#define WEIGHTS_UPPER_BOUND 2
#define NEIGHBOR_RADIUS 200
#define ALIGNMENT_WEIGHT 0.5
#define COHESION_WEIGHT 0.5
#define SEPARATION_WEIGHT 0.5

struct Vector {
    float x;
    float y;
    float z;
} Vector;

struct Boid {
    Vector location;
    Vector velocity;
} Boid;

Vector add_vector(Vector, Vector);
Vector mult_value(Vector, float);
void normalize(Vector);
int distance(Vector, Vector);

void update_velocity();
Vector alignment(Boid);
Vector cohesion(Boid);
Vector separation(Boid);

#endif /* ll_h */
