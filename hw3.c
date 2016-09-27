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

Vector cube_location; // move cube with keyboard

int main(int argc, char **argv) {
    GLFWwindow* window;
    //GLfloat angle = 0.0;
    
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
        gluLookAt(2000, 2000 , 2000, 0, 0, 0, 0, 0, -1);
        //gluLookAt(2000, 0, 18000, 0, 0, 0, 0, 1, 0);
        /*
        gluLookAt(2000 * sin(angle), 2000, 2000 * cos(angle), 0, 0, 0, 0, 1, 0);
        angle += 0.02;
        if(angle >= 360.0) {
            angle = 0.0;
        }
         */
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
    boids = malloc(sizeof(Boid*) * ARR_SIZE);
    for(int i = 0; i < BOIDS_NUM; i++) {
        boids[i] = NULL;
    }
    init_boids();
    init_boid_vertices();
    init_grid_vertices();
    init_grid_color();
    init_grid_indices();
    init_cube_location();
}

void reshape(GLFWwindow *w, int width, int height) {
    resize();
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  switch(key) {
  case GLFW_KEY_UP:
    cube_location.z = cube_location.z + CUBE_VELOCITY;
    break;
  case GLFW_KEY_DOWN:
    cube_location.z = cube_location.z - CUBE_VELOCITY;
    break;
  case GLFW_KEY_LEFT:
    cube_location.x = cube_location.x - CUBE_VELOCITY;
    break;
  case GLFW_KEY_RIGHT:
    cube_location.x = cube_location.x + CUBE_VELOCITY;
    break;
  case 61:
    add_boid();
    break;
  case 45:
    delete_boid();
    break;
  case 's':
  case 'S':
    break;
  case 'q':
  case 'Q':
    glfwSetWindowShouldClose(w, GL_TRUE);
    clear_boids();
    break;
  default:
    break;
  }
}

void cursor(GLFWwindow* window, double xpos, double ypos) {
}

void mouse(GLFWwindow* window, int button, int action, int mods) {
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

void clear_boids() {
    for(int i = 0; i < boids_num; i++) {
        if(boids[i] != NULL) {
            free(boids[i]);
            boids[i] = NULL;
        }
    }
    free(boids);
    boids_num = 0;
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
    for(int i = 0; i < boids_num; i++) {
        init_boid(i);
    }
}

void init_boid(int num_boid) {
    Vector old_loc;
    Vector loc;
    Vector vel;
    old_loc.x = 0;
    old_loc.y = 0;
    old_loc.z = 0;
    loc.x = randomGenerator();
    loc.y = randomGenerator();
    loc.z = randomGenerator();
    vel.x = 20;
    vel.y = 0;
    vel.z = 20;
    Boid* boid = make_boid(old_loc, loc, vel);
    *(boids + num_boid) = boid;
}

void add_boid() {
    if(boids_num >= ARR_SIZE) {
        boids = realloc(boids, 2 * ARR_SIZE);
    }
    init_boid(boids_num);
    boids_num++;
}

void delete_boid() {
    if(boids_num == 0) {
        printf("No more boids to delete\n");
        return;
    }
    if(boids_num <= ARR_SIZE / 2) {
        boids = realloc(boids, 0.5 * ARR_SIZE);
    }
    free(boids[boids_num - 1]);
    boids[boids_num - 1] = NULL;
    boids_num--;
}

void init_grid_vertices() {
    for(int i = 0; i < grid_vertices_num; i++) {
        printf("i: %d\n", i);
        grid_vertices[i][0] = GRID_TRANSLATEX + (i % (NUM_GRID_X + 1)) * GRID_SIZE;
        grid_vertices[i][1] = -FLOOR_HEIGHT;
        grid_vertices[i][2] = GRID_TRANSLATEZ + (i / (NUM_GRID_X + 1)) * GRID_SIZE;
    }
}

void init_grid_indices() {
    int j = 0;
    for(int i = 0; i < grid_vertices_num; i++) {
        if(i < grid_vertices_num - (NUM_GRID_X + 1) && (i - NUM_GRID_X) % (NUM_GRID_X + 1) != 0) {
            grid_indices[j] = i;
            grid_indices[j + 1] = i + NUM_GRID_X + 1;
            grid_indices[j + 2] = i + NUM_GRID_X + 2;
            grid_indices[j + 3] = i + 1;
            j+=4;
        }
    }
}

void init_grid_color() {
    for(int i = 0; i < grid_vertices_num; i++) {
        printf("colori: %d", i);
        if(i % 2 != 0) {
            printf("BLACK\n");
            grid_colors[i][0] = BLACK;
            grid_colors[i][1] = BLACK;
            grid_colors[i][2] = BLACK;
        } else {
            printf("WHITE\n");
            grid_colors[i][0] = WHITE;
            grid_colors[i][1] = WHITE;
            grid_colors[i][2] = WHITE;
        }
    }
}

void init_cube_location(){
    cube_location.x = 0;
    cube_location.y = 0;
    cube_location.z = 0;
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
    glTranslatef(cube_location.x,cube_location.y,cube_location.z);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cube_indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
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
        boids[i]->location = add_vec_vec(boids[i]->velocity, boids[i]->location);
        glTranslatef(boids[i]->location.x, boids[i]->location.y, boids[i]->location.z);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, boid_indices);
        //glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, boid_head_indices);
        glPopMatrix();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    for(int i = 0; i < boids_num; i++) {
        update_velocity(boids[i]);
    }
}

void draw_wireframe_boid() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, boid_vertices);
    glColor3f(1.0, 1.0, 1.0);
    for(int i = 0; i < boids_num; i++) {
        glPushMatrix();
        glTranslatef(boids[i]->location.x, boids[i]->location.y, boids[i]->location.z);
        glDrawElements(GL_LINES, 12, GL_UNSIGNED_BYTE, boid_wireframe_indices);
        glPopMatrix();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void update_velocity(Boid* boid) {
    Vector alignment = update_alignment(*boid);
    Vector cohesion = update_cohesion(*boid);
    Vector separation = update_separation(*boid);
    Vector tendency = tendencyTo(boid->location, cube_location);

    boid->velocity.x += alignment.x * ALIGNMENT_WEIGHT + cohesion.x * COHESION_WEIGHT + separation.x * SEPARATION_WEIGHT - tendency.x * TENDENCY_TO_GOAL_WEIGHT;
    boid->velocity.y += alignment.y * ALIGNMENT_WEIGHT + cohesion.y * COHESION_WEIGHT + separation.y * SEPARATION_WEIGHT - tendency.y * TENDENCY_TO_GOAL_WEIGHT;
    boid->velocity.z += alignment.z * ALIGNMENT_WEIGHT + cohesion.z * COHESION_WEIGHT + separation.z * SEPARATION_WEIGHT - tendency.z * TENDENCY_TO_GOAL_WEIGHT;
    boid->velocity = normalize_vec(boid->velocity);
    boid->velocity = mult_vec_val(boid->velocity, BOID_VEL_FACTOR);
    printf("AFTER\n");
    printf("boid->velocity.x: %f\n", boid->velocity.x);
    printf("boid->velocity.y: %f\n", boid->velocity.y);
    printf("boid->velocity.z: %f\n", boid->velocity.z);
}

Vector update_alignment(Boid boid) {
    Vector vector;
    vector.x = 0.0;
    vector.y = 0.0;
    vector.z = 0.0;
    int neighborCount = 0;
    for(int i = 0; i < boids_num; i++) {
        if(distance_vec_vec(boid.location, boids[i]->location) < NEIGHBOR_RADIUS) {
            vector.x = vector.x + boids[i]->velocity.x;
            vector.y = vector.y + boids[i]->velocity.y;
            vector.z = vector.z + boids[i]->velocity.z;
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
        if(distance_vec_vec(boid.location, boids[i]->location) < NEIGHBOR_RADIUS) {
             vector.x =  vector.x + boids[i]->location.x;
             vector.y =  vector.y + boids[i]->location.y;
             vector.z =  vector.z + boids[i]->location.z;
            neighborCount++;
        }
    }
    if(neighborCount == 0) {
        return vector;
    } else {
        vector.x = vector.x / neighborCount - boid.location.x;
        vector.y = vector.y / neighborCount - boid.location.y;
        vector.z = vector.z / neighborCount - boid.location.z;
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
        if(distance_vec_vec(boid.location, boids[i]->location) < 2000) {
            vector.x = vector.x - (boids[i]->location.x - boid.location.x);
            vector.y = vector.y - (boids[i]->location.y - boid.location.y);
            vector.z = vector.z - (boids[i]->location.z - boid.location.z);
        }
    }
    if(neighborCount == 0) {
        return vector;
    } else {
        vector = mult_vec_val(vector, -1);
        return vector;
    }
}

Vector tendencyTo(Vector place, Vector boid){
    return add_vec_vec(place, mult_vec_val(boid, -1));
}

GLfloat randomGenerator(){
    return (GLfloat)((rand() % 100) / 100.0);
}
