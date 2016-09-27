#include <stdlib.h>
#include <math.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#else
#include <GLFW/glfw3.h>
#endif
#include "hw3.h"

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
Vector transCube;


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
    {1.0, 1.0, 0.0},
    {1.0, 0.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0}
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

void init_boid_vertices() {
    /*
    GLfloat boid_temp_vertices[][3]= {
        {-1, 1, -BOID_RADIUS},
        {-1, 1, BOID_RADIUS},
        {-1 + sqrt(3) * BOID_RADIUS / 2.0, 1 + BOID_RADIUS / 2.0, 0},
        {-1 - sqrt(3) * BOID_RADIUS / 2.0, 1 + BOID_RADIUS / 2.0, 0}
    };
    */

    GLfloat boid_temp_vertices[][3] = {
        {0, 0, -BOID_RADIUS},
        {0, 0, BOID_RADIUS},
        {sqrt(3) * BOID_RADIUS / 2.0, BOID_RADIUS / 2.0, 0},
        {-sqrt(3) * BOID_RADIUS / 2.0, BOID_RADIUS / 2.0, 0}
    };
    
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 3; j++) {
            boid_vertices[i][j] = boid_temp_vertices[i][j];
        }
    }
}

int main(int argc, char **argv) {
    GLFWwindow* window;
    GLfloat angle = 0.0;
    
    // Initializes random number generator
    srand(time(NULL));
    
    // Initialize the library
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    
    // Create a window and its OpenGL context
    window = glfwCreateWindow(1000, 1000, "3D Flocking", NULL, NULL);
    if(!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwSetWindowPos(window, 100, 100);
    // Make the window's context current
    glfwMakeContextCurrent(window);
    
    // Assign reshape() to be the function called whenever a reshape event occurs
    glfwSetWindowSizeCallback(window, reshape);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Assign keyboard() to be the function called whenever
    // a key is pressed or released
    glfwSetKeyCallback(window, keyboard);
    
    // Assign mouse() to be the function called whenever
    // a mouse button is pressed or released
    glfwSetMouseButtonCallback(window, mouse);
    
    // Assign cursor() to be the function called whenever
    // a cursor moves
    glfwSetCursorPosCallback(window, cursor);
    
    init();
    // Loop until the user closes the window
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 18000, 0, 0, 0, 0, 1, 0, 0);
        //gluLookAt(2000, 0, 18000, 0, 0, 0, 0, 1, 0);
        //gluLookAt(2000 * sin(angle), 2000, 2000 * cos(angle), 0, 0, 0, 0, 1, 0);
        angle += 0.02;
        if(angle >= 360.0) {
            angle = 0.0;
        }
        draw();
        glfwSwapBuffers(window);
        
        // Poll for and process events
        glfwPollEvents();
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void draw() {
    draw_floor();
    draw_boid();
    draw_wireframe_boid();
    draw_cube();
    draw_wireframe_cube();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    resize();
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    init_boids();
    init_boid_vertices();
    init_grid_vertices();
    init_grid_color();
    init_grid_indices();
}

void reshape(GLFWwindow *w, int width, int height) {
    resize();
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  switch(key) {
  case GLFW_KEY_UP:
    transCube.z = transCube.z + 100;
    break;
  case GLFW_KEY_DOWN:
    transCube.z = transCube.z - 100;
    break;
  case GLFW_KEY_LEFT:
    transCube.x = transCube.x - 100;
    break;
  case GLFW_KEY_RIGHT:
    transCube.x = transCube.x + 100;
    break;
  case 's':
  case 'S':
    break;
  case 'q':
  case 'Q':
    glfwSetWindowShouldClose(w, GL_TRUE);
    break;
  default:
    break;
  }
}

void cursor(GLFWwindow* window, double xpos, double ypos) {
}

void mouse(GLFWwindow* window, int button, int action, int mods) {
}

void resize() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 0.5, 400000);
    glMatrixMode(GL_MODELVIEW);
}

void framebuffer_size_callback(GLFWwindow *w, int width, int height) {
    // Set the new viewport size
    glViewport(0, 0, width, height);
}

void init_boids() {
    boids = malloc(sizeof(Boid) * ARR_SIZE);
    for(int i = 0; i < BOIDS_NUM; i++) {
        boids[i].location.x = randomGenerator();
        boids[i].location.y = randomGenerator();
        boids[i].location.z = randomGenerator();
    }
    for(int i = 0; i < BOIDS_NUM; i++) {
        boids[i].velocity.x = 20;
        boids[i].velocity.y = 20;
        boids[i].velocity.z = 0;
    }
}

void init_grid_vertices() {
    for(int i = 0; i < grid_vertices_num; i++) {
        grid_vertices[i][0] = GRID_TRANSLATEX + (i % (NUM_GRID_X + 1)) * GRID_SIZE;
        grid_vertices[i][1] = 0;
        grid_vertices[i][2] = GRID_TRANSLATEZ + (i / (NUM_GRID_X + 1)) * GRID_SIZE;
        printf("grid_vertices[i][0]: %f\n", grid_vertices[i][0]);
        printf("grid_vertices[i][1]: %f\n", grid_vertices[i][1]);
        printf("grid_vertices[i][2]: %f\n", grid_vertices[i][2]);
    }
}

void init_grid_indices() {
    for(int i = 0; i < grid_indices_num; i++) {
        //printf("i: %d\n", i);
        if(i % 4 == 0 && i % NUM_GRID_X != 0) {
            grid_indices[i] = i / 4;
            grid_indices[i + 1] = grid_indices[i] + (NUM_GRID_X + 1);
            grid_indices[i + 2] = grid_indices[i + 1] + 1;
            grid_indices[i + 3] = grid_indices[i] + 1;
             printf("grid_indices[i]: %d\n", grid_indices[i]);
             printf("grid_indices[i + 1]: %d\n", grid_indices[i + 1]);
             printf("grid_indices[i + 2]: %d\n", grid_indices[i + 2]);
             printf("grid_indices[i + 3]: %d\n", grid_indices[i + 3]);
        }
    }
}

void init_grid_color() {
    for(int i = 0; i < grid_vertices_num; i++) {
        if(i % (GRID_SIZE + 1) == 0 || i >= (grid_vertices_num - GRID_SIZE)) {
            grid_colors[i][0] = GRAY;
            grid_colors[i][1] = GRAY;
            grid_colors[i][2] = GRAY;
        } else if(i % 2 == 0) {
            grid_colors[i][0] = BLACK;
            grid_colors[i][1] = BLACK;
            grid_colors[i][2] = BLACK;
        } else {
            grid_colors[i][0] = WHITE;
            grid_colors[i][1] = WHITE;
            grid_colors[i][2] = WHITE;
        }
    }
}

void init_cube_velocity(){
    transCube.x = 0;
    transCube.y = 0;
    transCube.z = 0;
}

void draw_floor() {
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, grid_vertices);
    glColorPointer(3, GL_FLOAT, 0, grid_colors);
    glDrawElements(GL_QUADS, grid_indices_num, GL_UNSIGNED_INT, grid_indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void draw_cube() {
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
    glColorPointer(3, GL_FLOAT, 0, cube_colors);
    glTranslatef(transCube.x,transCube.y,transCube.z);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cube_indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

GLfloat randomGenerator(){
    return (GLfloat)((rand() % 100) / 100.0);
}

void draw_wireframe_cube() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
    glColor3f(1.0, 1.0, 1.0);
    glDrawElements(GL_LINES, 48, GL_UNSIGNED_BYTE, cube_wireframe_indices);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_boid() {
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, boid_vertices);
    glColorPointer(3, GL_FLOAT, 0, boid_colors);
    for(int i = 0; i < boids_num; i++) {
        glPushMatrix();
        printf("transx: %f\n", boids[i].location.x);
        printf("transy %f\n", boids[i].location.y);
        printf("transz: %f\n", boids[i].location.z);
        glTranslatef(boids[i].location.x, boids[i].location.y, boids[i].location.z);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, boid_indices);
        glPopMatrix();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void draw_wireframe_boid() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, boid_vertices);
    glColor3f(1.0, 1.0, 1.0);
    for(int i = 0; i < boids_num; i++) {
        printf("v: %f\n", boids[i].velocity.x);
        printf("l: %f\n", boids[i].location.x);
        boids[i].location = add_vec_vec(boids[i].velocity,boids[i].location);
         printf("l2: %f\n", boids[i].location.x);
        glPushMatrix();
        glTranslatef(boids[i].location.x, boids[i].location.y, boids[i].location.z);
        glDrawElements(GL_LINES, 12, GL_UNSIGNED_BYTE, boid_wireframe_indices);
        glPopMatrix();
    }
    for(int i = 0; i < boids_num; i++) {
        update_velocity(boids[i]);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void update_velocity(Boid boid) {
    Vector alignment = update_alignment(boid);
    Vector cohesion = update_cohesion(boid);
    Vector separation = update_separation(boid);
    printf("alignment: %f\n", alignment.x);
    printf("cohesion: %f\n", cohesion.x);
    printf("separation: %f\n", separation.x);
    boid.velocity.x += alignment.x * ALIGNMENT_WEIGHT + cohesion.x * COHESION_WEIGHT + separation.x * SEPARATION_WEIGHT;
    boid.velocity.y += alignment.y * ALIGNMENT_WEIGHT + cohesion.y * COHESION_WEIGHT + separation.y * SEPARATION_WEIGHT;
    boid.velocity.z += alignment.z * ALIGNMENT_WEIGHT + cohesion.z * COHESION_WEIGHT + separation.z * SEPARATION_WEIGHT;
    printf("BEFORE\n");
    printf("boid.velocity.x: %f\n", boid.velocity.x);
    printf("boid.velocity.y: %f\n", boid.velocity.y);
    printf("boid.velocity.z: %f\n", boid.velocity.z);
    boid.velocity = normalize_vec(boid.velocity);
   // printf("AFTER\n");
   // printf("boid.velocity.x: %f\n", boid.velocity.x);
   // printf("boid.velocity.y: %f\n", boid.velocity.y);
   // printf("boid.velocity.z: %f\n", boid.velocity.z);
}

Vector update_alignment(Boid boid) {
    Vector vector;
    vector.x = 0.0;
    vector.y = 0.0;
    vector.z = 0.0;
    int neighborCount = 0;
    for(int i = 0; i < boids_num; i++) {
        if(distance_vec_vec(boid.location, boids[i].location) < NEIGHBOR_RADIUS) {
            vector.x = vector.x + boids[i].velocity.x;
            vector.y = vector.y + boids[i].velocity.y;
            vector.z = vector.z + boids[i].velocity.z;
            neighborCount++;
        }
    }
    if(neighborCount == 0) {
      return vector;
    } else {
        vector.x /= neighborCount;
        vector.y /= neighborCount;
        vector.z /= neighborCount;
        vector.x = vector.x - boid.velocity.x;
        vector.y = vector.y - boid.velocity.y;
        vector.z = vector.z - boid.velocity.z;
       // normalize_vec(vector);
        return vector;
    }
}

Vector update_cohesion(Boid boid) {
    Vector vector;
    vector.x = 0.0;
    vector.y = 0.0;
    vector.z = 0.0;
    int neighborCount = 0;
    for(int i = 0; i < boids_num; i++) {
        if(distance_vec_vec(boid.location, boids[i].location) < NEIGHBOR_RADIUS) {
             vector.x =  vector.x +boids[i].location.x;
             vector.y =  vector.y +boids[i].location.y;
             vector.z =  vector.z +boids[i].location.z;
            neighborCount++;
        }
    }
    if(neighborCount == 0) {
        return vector;
    } else {
        vector.x = vector.x / (neighborCount) - boid.location.x;
        vector.y = vector.y / (neighborCount) - boid.location.y;
        vector.z = vector.z / (neighborCount) - boid.location.z;
        //normalize_vec(vector);
        return vector;
    }
}

Vector update_separation(Boid boid) {
    Vector vector;
    vector.x = 0.0;
    vector.y = 0.0;
    vector.z = 0.0;
    int neighborCount = 0;
    for(int i = 0; i < boids_num; i++) {
        if(distance_vec_vec(boid.location, boids[i].location) < NEIGHBOR_RADIUS) {
            vector.x = vector.x - (boids[i].location.x - boid.location.x);
            vector.y = vector.y - (boids[i].location.y - boid.location.y);
            vector.z = vector.z - (boids[i].location.z - boid.location.z);
            
        }
    }
    if(neighborCount == 0) {
        return vector;
    } else {
        vector = mult_vec_val(vector, -1);
        //printf("boid.velocity.x: %f\n", vector.x);
        //printf("boid.velocity.y: %f\n", vector.y);
        //printf("boid.velocity.z: %f\n", vector.z);
        return vector;
    }
}
