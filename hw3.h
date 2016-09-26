#ifndef hw3_h
#define hw3_h

#include <stdio.h>
#include <math.h>
#include "time.h"

#include "ll.h"

#define GRID_TRANSLATEX -10000
#define GRID_TRANSLATEZ -10000
#define NUM_GRID_X 50
#define NUM_GRID_Z 50
#define GRID_SIZE 400
#define BOIDS_NUM 10
#define BOID_RADIUS 0.2
#define CUBE_SIZE 0.2
// TODO(tluan): don't go below 0

Boid* boids;
float boid_angle;
float boid_speed;
int boids_num = BOIDS_NUM;
GLfloat boid_vertices[4][3];
GLfloat grid_vertices[4][3];

void init();
void init_boids();
void init_grid_vertices(GLfloat, GLfloat);
void init_boid_vertices();
void draw();
void framebuffer_size_callback(GLFWwindow *w, int width, int height);
void reshape(GLFWwindow *w, int width, int height);
void draw_floor();
void draw_boid();
void draw_wireframe_boid();
void draw_cube();
void draw_grid();
void draw_wireframe_cube();
void update_velocity();
Vector update_alignment(Boid);
Vector update_cohesion(Boid);
Vector update_separation(Boid);
float randomGenerator();
#endif /* hw3_h */
