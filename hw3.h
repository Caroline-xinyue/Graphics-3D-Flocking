#ifndef hw3_h
#define hw3_h

#include <stdio.h>
#include <math.h>
#include "ll.h"

#define BOIDS_NUM 2
#define BOID_RADIUS 1
// TODO(tluan): don't go below 0


Boid* boids;
float boid_angle;
float boid_speed;
int boids_num = BOIDS_NUM;
GLfloat boid_vertices[4][3];

void init();
void draw();
void framebuffer_size_callback(GLFWwindow *w, int width, int height);
void reshape(GLFWwindow *w, int width, int height);
void draw_boid();
void draw_wireframe_boid();
void draw_cube();
void draw_wireframe_cube();
#endif /* hw3_h */
