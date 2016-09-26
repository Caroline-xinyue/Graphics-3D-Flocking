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
    {0.0, 1.0, 1.0}
};

void init_grid_vertices(GLfloat gridX, GLfloat gridZ) {
    GLfloat grid_temp_vertices[][3] = {
      {gridX, 0, gridZ},
      {gridX, 0, gridZ + GRID_SIZE},
      {gridX + GRID_SIZE, 0, gridZ + GRID_SIZE},
      {gridX + GRID_SIZE, 0, gridZ}
    };
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 3; j++) {
            grid_vertices[i][j] = grid_temp_vertices[i][j];
            //printf("grid_vertices[i][j]: %f\n", grid_vertices[i][j]);
        }
    }
}

void init_grid_indices(GLfloat gridX, GLfloat gridZ) {
    GLfloat grid_temp_indices[4] = {
        0 + , 1 + , 2 + , 3 +
    };
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 3; j++) {
            grid_vertices[i][j] = grid_temp_vertices[i][j];
            //printf("grid_vertices[i][j]: %f\n", grid_vertices[i][j]);
        }
    }
}

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
    
    init();
    // Loop until the user closes the window
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(5, 0, 0, 0, 0, 0, 0, 1, 0);
        //gluLookAt(10 * sin(angle), 10, 10 * cos(angle), 0, 0, 0, 0, 1, 0);
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
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1.5, 20);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    init_boids();
}

void reshape(GLFWwindow *w, int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1.5, 20);
    glMatrixMode(GL_MODELVIEW);
}

void framebuffer_size_callback(GLFWwindow *w, int width, int height) {
    // Set the new viewport size
    glViewport(0, 0, width, height);
}

void init_boids() {
    boids = malloc(sizeof(Boid) * boids_num);
    for(int i = 0; i < BOIDS_NUM; i++) {
        boids[i].location.x = randomGenerator();
        boids[i].location.y = randomGenerator();
        boids[i].location.z = randomGenerator();
    }
    for(int i = 0; i < BOIDS_NUM; i++) {
        //randomGenerator()
        boids[i].velocity.x = 0;
        boids[i].velocity.y = 0;
        boids[i].velocity.z = 0;
    }
}

void draw_floor() {
    /*
    GLfloat gridX = GRID_TRANSLATEX;
    GLfloat gridY = GRID_TRANSLATEZ;
    glColor3f(1.0, 1.0, 1.0);
    for(int j = 0; j < NUM_GRID_Z; j++) {
        for(int i = 0; i < NUM_GRID_X; i++) {
            gridX += i * GRID_SIZE;
            gridY += j * GRID_SIZE;
            glLoadIdentity();
            init_grid_vertices(gridX, gridY);
            draw_grid();
        }
    }
    */
    init_grid_vertices(0, 0);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 3; j++) {
            printf("grid_vertices[i][j]: %f\n", grid_vertices[i][j]);
        }
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, grid_vertices);
    glColor3f(1.0, 0.0, 0.0);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

/*
void draw_grid() {
    //glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, grid_vertices);
    //glColorPointer(3, GL_FLOAT, 0, grid_colors);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
}
*/

void draw_cube() {
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
    glColorPointer(3, GL_FLOAT, 0, cube_colors);
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
    init_boid_vertices();
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, boid_vertices);
    glColorPointer(3, GL_FLOAT, 0, boid_colors);
    for(int i = 0; i < boids_num; i++) {
        glPushMatrix();
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
        boids[i].location = add_vec_vec(boids[i].velocity,boids[i].location);
        glPushMatrix();
        glTranslatef(boids[i].location.x, boids[i].location.y, boids[i].location.z);
        glDrawElements(GL_LINES, 12, GL_UNSIGNED_BYTE, boid_wireframe_indices);
        glPopMatrix();
    }
    /*for(int i = 0; i < boids_num; i++) {
        update_velocity(boids[i]);
    }*/
    
    glDisableClientState(GL_VERTEX_ARRAY);
}

void update_velocity(Boid boid) {
    Vector alignment = update_alignment(boid);
    Vector cohesion = update_cohesion(boid);
    Vector separation = update_separation(boid);
    boid.velocity.x += alignment.x * ALIGNMENT_WEIGHT + cohesion.x * COHESION_WEIGHT + separation.x * SEPARATION_WEIGHT;
    boid.velocity.y += alignment.y * ALIGNMENT_WEIGHT + cohesion.y * COHESION_WEIGHT + separation.y * SEPARATION_WEIGHT;
    boid.velocity.z += alignment.z * ALIGNMENT_WEIGHT + cohesion.z * COHESION_WEIGHT + separation.z * SEPARATION_WEIGHT;
    printf("BEFORE\n");
    printf("boid.velocity.x: %f\n", boid.velocity.x);
    printf("boid.velocity.y: %f\n", boid.velocity.y);
    printf("boid.velocity.z: %f\n", boid.velocity.z);
    boid.velocity = normalize_vec(boid.velocity);
    printf("AFTER\n");
    printf("boid.velocity.x: %f\n", boid.velocity.x);
    printf("boid.velocity.y: %f\n", boid.velocity.y);
    printf("boid.velocity.z: %f\n", boid.velocity.z);
}

Vector update_alignment(Boid boid) {
    Vector vector;
    vector.x = 0.0;
    vector.y = 0.0;
    vector.z = 0.0;
    int neighborCount = 0;
    for(int i = 0; i < boids_num; i++) {
        if(distance_vec_vec(boid.location, boids[i].location) < NEIGHBOR_RADIUS) {
            vector = add_vec_vec(vector, boids[i].velocity);
            neighborCount++;
        }
    }
    if(neighborCount == 0) {
      return vector;
    } else {
        vector.x /= neighborCount;
        vector.y /= neighborCount;
        vector.z /= neighborCount;
        normalize_vec(vector);
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
            //printf("distance between: %f\n",distance_vec_vec(boid.location, boids[i].location));
            vector = add_vec_vec(vector, boids[i].location);
            neighborCount++;
        }
    }
    if(neighborCount == 0) {
        return vector;
    } else {
        vector.x = vector.x / neighborCount - boid.location.x;
        vector.y = vector.y / neighborCount - boid.location.y;
        vector.z = vector.z / neighborCount - boid.location.z;
        normalize_vec(vector);
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
            vector = add_vec_vec(vector, boids[i].location);
            vector = add_vec_vec(vector, mult_vec_val(boid.location, -1));
            neighborCount++;
        }
    }
    if(neighborCount == 0) {
        return vector;
    } else {
        vector.x = -1 * vector.x / neighborCount;
        vector.y = -1 * vector.y / neighborCount;
        vector.z = -1 * vector.z / neighborCount;
        normalize_vec(vector);
        return vector;
    }
}
