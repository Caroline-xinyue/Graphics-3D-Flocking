#ifndef hw3_h
#define hw3_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "time.h"
#include "ll.h"
#include "GL/glu.h"

typedef enum {FALSE, TRUE} bool;
typedef enum {PAUSED, RESUME} State;
typedef enum {DEFAULT, TRAILING, SIDE} Mode;
typedef enum {FLAPPING, BANKING} Motion;

#define WIDTH 800
#define HEIGHT 800
#define DEFAULT_EYE 18000
#define TRAIL_CONSTANT 2000
#define BOIDS_ARR_LEN 200
#define BOIDS_NUM 10
#define BOID_RADIUS 200
#define BOID_BANK_ANGLE 50
#define GRID_TRANSLATEX -10000
#define GRID_TRANSLATEZ -10000
#define GRID_SIZE 400
#define CUBE_BOUNDARY 10000
#define CUBE_SIZE 250
#define NUM_GRID_X 50
#define NUM_GRID_Z 50
#define BOID_INIT_LOCX 2400
#define BOID_INIT_LOCY 1200
#define BOID_INIT_LOCZ 150
#define CUBE_INIT_LOCX 2000
#define CUBE_INIT_LOCY 1000
#define CUBE_INIT_LOCZ 0
#define BOID_INIT_VELX 0
#define BOID_INIT_VELY 0
#define BOID_INIT_VELZ 0
#define CUBE_UPDATE_VEL 60
#define CUBE_ACCEL 1.1f
#define FLOOR_HEIGHT -5000
#define BLACK 0.9
#define WHITE 0.5
#define PLUS 61
#define MINUS 45

#define NEIGHBOR_RADIUS 1000
#define SEPERATION_RADIUS 100
#define ALIGNMENT_WEIGHT 0.1
#define COHESION_WEIGHT 0.2
#define SEPARATION_WEIGHT 0.2
#define BOID_VEL_FACTOR 100

Boid** boids;
State animeState = RESUME;
Mode viewMode = DEFAULT;
bool autoMode = TRUE;
Motion birdMotion = FLAPPING;
Vector trailView_prev;
Vector directionSide_prev;
int angleOfWings = 0;
GLint grid_vertices_num = (NUM_GRID_X + 1) * (NUM_GRID_Z + 1);
GLint grid_indices_num = 4 * NUM_GRID_X * NUM_GRID_Z;
GLint boids_arr_length = BOIDS_ARR_LEN;
GLint boids_num = BOIDS_NUM;
GLfloat perspective_angle = 60;
GLfloat boid_vertices[4][3];
GLfloat boid_vertices_shadow[4][3];
GLuint grid_indices[4 * NUM_GRID_X * NUM_GRID_Z];
GLfloat grid_colors[(NUM_GRID_X + 1) * (NUM_GRID_Z + 1)][3];
GLfloat grid_vertices[(NUM_GRID_X + 1) * (NUM_GRID_Z + 1)][3];
Vector cube_location;
Vector cube_velocity;
GLfloat tendency_to_goal_weight = 15;

GLfloat cube_vertices[][3] = {
    {-CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0},
    {CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0},
    {CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0},
    {-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0},
    {-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0},
    {CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0},
    {CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0},
    {-CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0}
};

GLubyte cube_indices[24] = {
    0, 3, 2, 1,
    1, 2, 5, 6,
    0, 1, 6, 7,
    0, 7, 4, 3,
    7, 6, 5, 4,
    3, 4, 5, 2
};

GLubyte cube_wireframe_indices[48] = {
    0, 3,
    3, 2,
    2, 1,
    1, 0,
    1, 2,
    2, 5,
    5, 6,
    6, 1,
    0, 1,
    1, 6,
    6, 7,
    7, 0,
    0, 7,
    7, 4,
    4, 3,
    3, 0,
    7, 6,
    6, 5,
    5, 4,
    4, 7,
    4, 5,
    5, 2,
    2, 3,
    3, 4
};

GLfloat cube_colors[][3] = {
    {1.0, 1.0, 1.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 0.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 0.0, 0.0},
    {1.0, 1.0, 0.0},
    {1.0, 0.0, 0.0}
};

GLubyte boid_indices[6] = {
    0, 1, 2,
    1, 0, 3
};

GLubyte boid_wireframe_indices[12] = {
    0, 1,
    1, 2,
    2, 0,
    1, 0,
    0, 3,
    3, 1
};

GLfloat boid_colors[][3] = {
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0}
};

GLfloat boid_colors_shadow[][3] = {
    {0.5, 0.4, 0.5},
    {0.5, 0.4, 0.5},
    {0.5, 0.4, 0.5},
    {0.5, 0.4, 0.5}
};

void init();
void init_view();
void init_view_setup();
void init_boids();
void init_boid();
void init_boid_and_shadow_vertices();
void init_grid();
void init_grid_vertices();
void init_grid_indices();
void init_grid_color();
void init_cube();
void init_cube_location();
void init_cube_vel_auto();
void add_boid();
void delete_boid();
void clear_boids();
void draw();
void draw_floor();
void draw_boid();
void draw_boid_shadow();
void draw_wireframe_boid();
void draw_cube();
void draw_wireframe_cube();
void resize();
void reshape(GLFWwindow *w, int width, int height);
void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);
void cursor(GLFWwindow* window, double xpos, double ypos);
void mouse(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow *w, int width, int height);
void update();
void update_banking();
void update_boids();
void update_boid_velocity(Boid*);
void update_boid_location(Boid*);
void update_boid_angle(Boid*);
void update_boid_wings();
void update_cube_velocity();
Vector update_alignment(Boid);
Vector update_cohesion(Boid);
Vector update_separation(Boid);
Vector update_tendency(Vector, Vector);
void check_boundary();
void randomCubeVelocity();
GLfloat randomGenerator();
GLfloat calculate_angle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void print_debug_info();
void print_goal_info();
void print_boids_info();
void myLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx,
              GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy,
              GLfloat upz);
void myPerspective(GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far);
#endif /* hw3_h */
