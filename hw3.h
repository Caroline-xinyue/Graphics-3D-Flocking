#ifndef hw3_h
#define hw3_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "time.h"
#include "ll.h"

#define WIDTH 800
#define HEIGHT 800
#define ARR_SIZE 10000
#define GRID_TRANSLATEX -10000
#define GRID_TRANSLATEZ -10000
#define NUM_GRID_X 50
#define NUM_GRID_Z 50
#define GRID_SIZE 400
#define BOIDS_NUM 100
#define BOID_RADIUS 200
#define CUBE_SIZE 250
#define CUBE_VELOCITY 100
#define FLOOR_HEIGHT 1000
#define BLACK 0.9
#define WHITE 0.5
// TODO(tluan): boids and cube don't go below 0

Boid** boids;
GLfloat boid_angle;
GLfloat boid_speed;
GLint grid_vertices_num = (NUM_GRID_X + 1) * (NUM_GRID_Z + 1);
GLint grid_indices_num = 4 * NUM_GRID_X * NUM_GRID_Z;
GLint boids_num = BOIDS_NUM;
GLfloat boid_vertices[4][3];
GLuint grid_indices[4 * NUM_GRID_X * NUM_GRID_Z];
GLfloat grid_colors[(NUM_GRID_X + 1) * (NUM_GRID_Z + 1)][3];
GLfloat grid_vertices[(NUM_GRID_X + 1) * (NUM_GRID_Z + 1)][3];

void init();
void init_boids();
void init_grid_vertices();
void init_grid_indices();
void init_grid_color();
void init_boid_vertices();
void init_cube_location();
void draw();
void framebuffer_size_callback(GLFWwindow *w, int width, int height);
void draw_floor();
void draw_boid();
void draw_wireframe_boid();
void draw_cube();
void draw_wireframe_cube();
void resize();
void reshape(GLFWwindow *w, int width, int height);
void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);
void cursor(GLFWwindow* window, double xpos, double ypos);
void mouse(GLFWwindow* window, int button, int action, int mods);
void update_velocity();
Vector update_alignment(Boid);
Vector update_cohesion(Boid);
Vector update_separation(Boid);
float randomGenerator();
#endif /* hw3_h */
