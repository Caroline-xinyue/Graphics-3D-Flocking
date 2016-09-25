#ifndef hw3_h
#define hw3_h

#include <stdio.h>
//#include <ll.h>
#include <math.h>

#define BOIDS_NUM 10
#define BOID_RADIUS 5
// TODO(tluan): don't go below 0

void init();
void reshape(GLFWwindow *w, int width, int height);
void draw_boid();
void draw_wireframe_boid();

#endif /* hw4_h */
