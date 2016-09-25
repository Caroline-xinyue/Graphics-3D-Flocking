#include <stdlib.h>
#include <math.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#else
#include <GLFW/glfw3.h>
#endif
#include "hw3.h"

GLfloat boid_colors[][3] = {
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0}
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

void init_boid_vertices() {
    
    GLfloat boid_temp_vertices[][3]= {
        {-1, 1, -BOID_RADIUS},
        {-1, 1, BOID_RADIUS},
        {-1 + sqrt(3) * BOID_RADIUS / 2.0, 1 + BOID_RADIUS / 2.0, 0},
        {-1 - sqrt(3) * BOID_RADIUS / 2.0, 1 + BOID_RADIUS / 2.0, 0}
    };
    /*
    GLfloat boid_temp_vertices[][3]= {
        {0, 0, -BOID_RADIUS},
        {0, 0, BOID_RADIUS},
        {sqrt(3) * BOID_RADIUS / 2.0, BOID_RADIUS / 2.0, 0},
        {-sqrt(3) * BOID_RADIUS / 2.0, BOID_RADIUS / 2.0, 0}
    };
    */
    
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 3; j++) {
            boid_vertices[i][j] = boid_temp_vertices[i][j];
        }
    }
}

int main(int argc, char **argv) {
    GLFWwindow* window;
    float angle = 0.0;
    
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
    
    init();
    // Loop until the user closes the window
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
        gluLookAt(10 * sin(angle), 10, 10 * cos(angle), 0, 0, 0, 0, 1, 0);
        angle += 0.02;
        if(angle >= 360.0) {
            angle = 0.0;
        }
        draw_boid();
        draw_wireframe_boid();
        glfwSwapBuffers(window);
        
        // Poll for and process events
        glfwPollEvents();
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void init() {
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1.5, 20);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
}

void reshape(GLFWwindow *w, int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1.5, 20);
    glMatrixMode(GL_MODELVIEW);
}

void draw_boid() {
    init_boid_vertices();
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, boid_vertices);
    glColorPointer(3, GL_FLOAT, 0, boid_colors);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, boid_indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void draw_wireframe_boid() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, boid_vertices);
    glColor3f(1.0, 1.0, 1.0);
    //for(int i = 0; i < )
    glDrawElements(GL_LINES, 12, GL_UNSIGNED_BYTE, boid_wireframe_indices);
    glDisableClientState(GL_VERTEX_ARRAY);
}
