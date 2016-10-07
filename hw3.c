#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#else
#include <GLFW/glfw3.h>
#endif
#include "hw3.h"

int main(int argc, char **argv) {
  GLFWwindow* window;
  
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
  
  init();
  // Loop until the user closes the window
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    init_view();
    draw();
    if(animeState == RESUME) {
      update();
    }
    glfwSwapBuffers(window);
    
    // Poll for and process events
    glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  resize();
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
  boids = malloc(sizeof(Boid*) * BOIDS_ARR_LEN);
  for(int i = 0; i < BOIDS_ARR_LEN; i++) {
    boids[i] = NULL;
  }
  init_boids();
  init_grid();
  init_cube();
}

void draw() {
  draw_floor();
  draw_boid();
  draw_wireframe_boid();
  draw_boid_shadow();
  draw_cube();
  draw_wireframe_cube();
}

void init_view() {
    Vector midpoint;
    Vector centroid;
    Vector zDirection;
    zDirection.x = 0;
    zDirection.y = 1;
    zDirection.z = 0;
    int neighborCount = 0;
    GLfloat r = 0;
    centroid.x = 0; centroid.y = 0; centroid.z = 0;
    for(int i = 0; i < boids_num; i++) {
        centroid = add_vec_vec(centroid, boids[i]->location);
        neighborCount++;
    }
    centroid = mult_vec_val(centroid, 1.0f / neighborCount);
    for(int i = 0; i < boids_num; i++) {
        GLfloat temp = distance_vec_vec(boids[i]->location, centroid);
        if(temp > r){
	  r = temp;
        }
    }
    Vector u = add_vec_vec(cube_location, mult_vec_val(centroid,-1));
    u = normalize_vec(u);
    GLfloat d = distance_vec_vec(cube_location, centroid);
    midpoint = mult_vec_val(add_vec_vec(centroid, cube_location), 1.0f / 2);
    if(viewMode == TRAILING) {
        if(boids_num == 0) {
            myLookAt(trailView_prev.x, trailView_prev.y, trailView_prev.z, cube_location.x, cube_location.y, cube_location.z, 0, 1, 0);
        } else {
            GLfloat distance_trailing = d + 1.3 * r;
            Vector trailView = mult_vec_val(u, distance_trailing);
            trailView = add_vec_vec(centroid, mult_vec_val(trailView,-1));
            trailView.y =  trailView.y + d + r + TRAIL_CONSTANT;
            trailView_prev = trailView;
            myLookAt(trailView.x, trailView.y, trailView.z, midpoint.x, midpoint.y, midpoint.z, 0, 1, 0);
        }
	perspective_angle = 30;
    } else if(viewMode == SIDE) {
        if(boids_num == 0) {
            myLookAt(directionSide_prev.x, directionSide_prev.y, directionSide_prev.z, cube_location.x, cube_location.y, cube_location.z, 0, 1, 0);
        } else {
            Vector directionSide = cross_vec_vec(u, zDirection);
            directionSide = normalize_vec(directionSide);
            GLfloat distanceSide = d + 2 * r;
            directionSide = add_vec_vec(midpoint, mult_vec_val(directionSide, distanceSide));
            directionSide.y = directionSide.y + d + r;
            directionSide_prev = directionSide;
            myLookAt(directionSide.x, directionSide.y, directionSide.z, midpoint.x, midpoint.y, midpoint.z, 0, 1, 0);
        }
	perspective_angle = 40;
    } else {
      if(boids_num == 0) {
	myLookAt(0, DEFAULT_EYE, 0, cube_location.x, cube_location.y, cube_location.z, 0, 0, -1);
      } else {
        myLookAt(0, DEFAULT_EYE, 0, midpoint.x, midpoint.y, midpoint.z, 0, 0, -1);
      }
    }
}

void reshape(GLFWwindow *w, int width, int height) {
  resize();
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  if(action == GLFW_RELEASE || action == GLFW_REPEAT) {
    switch(key) {
      if(!autoMode) {
        case GLFW_KEY_UP:
	  if(cube_location.z < -GRID_TRANSLATEZ) {
	    cube_location.z = cube_location.z + CUBE_UPDATE_VEL;
	  }
          break;
        case GLFW_KEY_DOWN:
	  if(cube_location.z > GRID_TRANSLATEZ) {
	    cube_location.z = cube_location.z - CUBE_UPDATE_VEL;
	  }
          break;
        case GLFW_KEY_LEFT:
	  if(cube_location.x > GRID_TRANSLATEX) {
	    cube_location.x = cube_location.x - CUBE_UPDATE_VEL;
	  }
          break;
        case GLFW_KEY_RIGHT:
	  if(cube_location.x < -GRID_TRANSLATEX) {
	    cube_location.x = cube_location.x + CUBE_UPDATE_VEL;
	  }
          break;
        case 'k':
        case 'K':
	  if(cube_location.y > FLOOR_HEIGHT + CUBE_SIZE) {
	    cube_location.y = cube_location.y - CUBE_UPDATE_VEL;
	  }
          break;
        case 'l':
        case 'L':
          cube_location.y = cube_location.y + CUBE_UPDATE_VEL;
          break;
        case 'b':
        case 'B':
          cube_velocity.x *= CUBE_ACCEL;
          cube_velocity.y *= CUBE_ACCEL;
          break;
        case 'c':
        case 'C':
          cube_velocity.x /= CUBE_ACCEL;
          cube_velocity.y /= CUBE_ACCEL;
          break;
    }
    case PLUS:
      add_boid();
      break;
    case MINUS:
      delete_boid();
      break;
    case 'v':
    case 'V':
      viewMode = DEFAULT;
      break;
    case 't':
    case 'T':
      viewMode = TRAILING;
      break;
    case 's':
    case 'S':
      viewMode = SIDE;
      break;
    case 'p':
    case 'P':
      animeState = RESUME - animeState;
      break;
    case 'd':
    case 'D':
      if(animeState == RESUME) {
	    animeState = PAUSED;
      }
      if(animeState == PAUSED) {
	    update();
      }
      break;
    case 'w':
    case 'W':
      birdMotion = BANKING - birdMotion;
      break;
    case 'm':
    case 'M':
      autoMode = (autoMode == FALSE) ? TRUE : FALSE;
      init_cube_vel_auto();
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
}

void init_boid_and_shadow_vertices() {
  GLfloat boid_temp_vertices[][3] = {
    {0, 0, -BOID_RADIUS},
    {0, 0, BOID_RADIUS},
    {sqrt(3) * BOID_RADIUS / 2.0, BOID_RADIUS / 2.0, BOID_RADIUS},
    {-sqrt(3) * BOID_RADIUS / 2.0, BOID_RADIUS / 2.0, BOID_RADIUS}
  };
  
  GLfloat boid_shadow_temp_vertices[][3] = {
    {0, 0, -BOID_RADIUS},
    {0, 0, BOID_RADIUS},
    {sqrt(3) * BOID_RADIUS / 2.0, 0, BOID_RADIUS},
    {-sqrt(3) * BOID_RADIUS / 2.0, 0, BOID_RADIUS}
  };
  
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 3; j++) {
      boid_vertices[i][j] = boid_temp_vertices[i][j];
      boid_vertices_shadow[i][j]=boid_shadow_temp_vertices[i][j];
    }
  }
}

void resize() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  myPerspective(perspective_angle, 1, 0.5, 400000);
  glMatrixMode(GL_MODELVIEW);
}

void framebuffer_size_callback(GLFWwindow *w, int width, int height) {
  // Set the new viewport size
  glViewport(0, 0, width, height);
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

void init_boids() {
  for(int i = 0; i < BOIDS_NUM; i++) {
    init_boid(i);
  }
  init_boid_and_shadow_vertices();
  init_view_setup();
}

void init_view_setup() {
    trailView_prev.x = 0; trailView_prev.y = 0; trailView_prev.z = 0;
    directionSide_prev.x = 0; directionSide_prev.y = 0; directionSide_prev.z = 0;
}

void init_boid(int num_boid) {
  Vector loc;
  Vector vel;
  loc.x = BOID_INIT_LOCX + randomGenerator();
  loc.y = BOID_INIT_LOCY + randomGenerator();
  loc.z = BOID_INIT_LOCZ + randomGenerator();
  vel.x = BOID_INIT_VELX;
  vel.y = BOID_INIT_VELY;
  vel.z = BOID_INIT_VELZ;
  Boid* boid = make_boid(loc, vel);
  *(boids + num_boid) = boid;
}

void add_boid() {
  if(boids_num >= boids_arr_length) {
    boids = realloc(boids, 2 * boids_arr_length);
    boids_arr_length *= 2;
  }
  #if DEBUG
  printf("Boids_num: %d\n", boids_num);
  print_boids_info();
  #endif
  init_boid(boids_num);
  #if DEBUG
  printf("add boids_num:%d\n", boids_num);
  #endif
  boids_num++;
}

void delete_boid() {
  if(boids_num == 0) {
    printf("No more boids to delete\n");
    return;
  }
  #if DEBUG
  printf("delete boids_num:%d\n", boids_num);
  #endif
  if(boids[boids_num - 1] != NULL) {
    free(boids[boids_num - 1]);
  }
  boids[boids_num - 1] = NULL;
  boids_num--;
  #if DEBUG
  printf("Boids_num: %d\n", boids_num);
  print_boids_info();
  #endif
}

void init_grid() {
    init_grid_vertices();
    init_grid_color();
    init_grid_indices();
}

void init_grid_vertices() {
  for(int i = 0; i < grid_vertices_num; i++) {
    grid_vertices[i][0] = GRID_TRANSLATEX + (i % (NUM_GRID_X + 1)) * GRID_SIZE;
    grid_vertices[i][1] = FLOOR_HEIGHT;
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
    if(i % 2 != 0) {
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

void init_cube() {
    init_cube_location();
    init_cube_vel_auto();
}

void init_cube_location() {
    cube_location.x = CUBE_INIT_LOCX;
    cube_location.y = CUBE_INIT_LOCY;
    cube_location.z = CUBE_INIT_LOCZ;
}

/* Enable Client State in init*/
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
  glTranslatef(cube_location.x, cube_location.y, cube_location.z);
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
    glTranslatef(boids[i]->location.x, boids[i]->location.y, boids[i]->location.z);
    glRotatef(boids[i]->angle, 0, 1, 0);
    update_banking(boids[i]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, boid_indices);
    glPopMatrix();
  }
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void draw_boid_shadow() {
  glColor3f(0.4, 0.4, 0.4);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, boid_vertices_shadow);
  for(int i = 0; i < boids_num; i++) {
    glPushMatrix();
    glTranslatef(boids[i]->location.x, FLOOR_HEIGHT + 150, boids[i]->location.z);
    glRotatef(boids[i]->angle, 0, 1, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, boid_indices);
    glPopMatrix();
  }
  glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_wireframe_boid() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, boid_vertices);
  glColor3f(1.0, 1.0, 1.0);
  for(int i = 0; i < boids_num; i++) {
    glPushMatrix();
    glTranslatef(boids[i]->location.x, boids[i]->location.y, boids[i]->location.z);
    glRotatef(boids[i]->angle, 0, 1, 0);
    update_banking(boids[i]);
    glDrawElements(GL_LINES, 12, GL_UNSIGNED_BYTE, boid_wireframe_indices);
    glPopMatrix();
  }
  glDisableClientState(GL_VERTEX_ARRAY);
}

void update_banking(Boid* boid) {
  if(birdMotion == BANKING) {
    angleOfWings = 85;
    float tempAngle = calculate_angle(boid->velocity.x, boid->velocity.z, 0, -BOID_RADIUS);
    if(tempAngle > 0) {
      glRotatef(BOID_BANK_ANGLE, 0, 0, -1);
    } else {
      glRotatef(-BOID_BANK_ANGLE, 0, 0, -1);
    }
  }
}

void update() {
  #if DEBUG
    print_debug_info();
  #endif
  if(autoMode) {
    update_cube_velocity();
  }  
  update_boids();
}

void init_cube_vel_auto() {
    cube_velocity.x = randomGenerator() / 10.0f;
    cube_velocity.y = 0;
    cube_velocity.z = randomGenerator() / 10.0f;
}

void update_boid_wings(){
    GLfloat boid_temp_vertices[][3] = {
        {0, 0, -BOID_RADIUS},
        {0, 0, BOID_RADIUS},
        {sin(angleOfWings * M_PI/180) * BOID_RADIUS , cos(angleOfWings * M_PI/180) * BOID_RADIUS, BOID_RADIUS},
        {-sin(angleOfWings * M_PI/180) * BOID_RADIUS , cos(angleOfWings * M_PI/180) * BOID_RADIUS, BOID_RADIUS}
    };
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 3; j++) {
            boid_vertices[i][j] = boid_temp_vertices[i][j];
        }
    }
}

void update_cube_velocity() {
  check_boundary();
  cube_location = add_vec_vec(cube_location, cube_velocity);
}

void check_boundary() {
  if(cube_location.x < -CUBE_BOUNDARY || cube_location.x > CUBE_BOUNDARY){
    cube_velocity.x = -cube_velocity.x;
  }
  if(cube_location.z < -CUBE_BOUNDARY || cube_location.z > CUBE_BOUNDARY){
    cube_velocity.z = -cube_velocity.z;
  }
}

void update_boids() {
  for(int i = 0; i < boids_num; i++) {
    update_boid_velocity(boids[i]);
    update_boid_location(boids[i]);
    update_boid_angle(boids[i]);
  }
  if(birdMotion == FLAPPING) {
    angleOfWings = angleOfWings % 90;
    angleOfWings  = angleOfWings + 2;
    update_boid_wings();
  }
}

void update_boid_velocity(Boid* boid) {
  Vector alignment = update_alignment(*boid);
  Vector cohesion = update_cohesion(*boid);
  Vector separation = update_separation(*boid);
  Vector tendency = update_tendency(boid->location, cube_location);
  boid->velocity.x += alignment.x * ALIGNMENT_WEIGHT + cohesion.x * COHESION_WEIGHT + separation.x * SEPARATION_WEIGHT - tendency.x * tendency_to_goal_weight;
  boid->velocity.y += alignment.y * ALIGNMENT_WEIGHT + cohesion.y * COHESION_WEIGHT + separation.y * SEPARATION_WEIGHT - tendency.y * tendency_to_goal_weight;
  boid->velocity.z += alignment.z * ALIGNMENT_WEIGHT + cohesion.z * COHESION_WEIGHT + separation.z * SEPARATION_WEIGHT - tendency.z * tendency_to_goal_weight;
  boid->velocity = normalize_vec(boid->velocity);
  boid->velocity = mult_vec_val(boid->velocity, BOID_VEL_FACTOR);
}

void update_boid_location(Boid* boid) {
  boid->location = add_vec_vec(boid->location, boid->velocity); 
  if(boid->location.x > GRID_TRANSLATEX && boid->location.x < -GRID_TRANSLATEX && boid->location.z > GRID_TRANSLATEZ && boid->location.z < -GRID_TRANSLATEZ) {
    tendency_to_goal_weight = 15;
  } else {
    tendency_to_goal_weight = 100;
  }
}

void update_boid_angle(Boid* boid) {
  boid->angle = calculate_angle(0, -BOID_RADIUS, boid->velocity.x, boid->velocity.z) * 180.0f / M_PI;
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
    vector = normalize_vec(vector);
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
    vector = normalize_vec(vector);
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
    if(distance_vec_vec(boid.location, boids[i]->location) < SEPERATION_RADIUS) {
      vector.x = vector.x - (boids[i]->location.x - boid.location.x);
      vector.y = vector.y - (boids[i]->location.y - boid.location.y);
      vector.z = vector.z - (boids[i]->location.z - boid.location.z);
      neighborCount++;
    }
  }
  if(neighborCount == 0) {
    return vector;
  } else {
    vector = mult_vec_val(vector, -1.0f);
    vector = normalize_vec(vector);
    return vector;
  }
}

Vector update_tendency(Vector place, Vector boid) {
  Vector vector =  add_vec_vec(place, mult_vec_val(boid, -1.0f));
  vector = normalize_vec(vector);
  return vector;
}

GLfloat randomGenerator() {
  return (GLfloat)((rand() % 500));
}

GLfloat calculate_angle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
  GLfloat dot_product = x1 * x2 + y1 * y2;
  GLfloat determinant = x1 * y2 - y1 * x2;
  GLfloat angle = atan2(determinant, dot_product);
  return -angle;
}

void myLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx,
	      GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy,
	      GLfloat upz) {
  Vector forward, side, up;
  GLfloat matrix[16];
  
  forward.x = centerx - eyex;
  forward.y = centery - eyey;
  forward.z = centerz - eyez;
  
  up.x = upx;
  up.y = upy;
  up.z = upz;
  
  side.x = 0.0f;
  side.y = 0.0f;
  side.z = 0.0f;
  
  forward = normalize_vec(forward);
    
  /* Side = forward x up */
  side = cross_vec_vec(forward, up);
  side = normalize_vec(side);
  
  /* Recompute up as: up = side x forward */
  up = cross_vec_vec(side, forward);
  
  matrix[0] = side.x;
  matrix[4] = side.y;
  matrix[8] = side.z;
  matrix[12] = 0.0f;
  
  matrix[1] = up.x;
  matrix[5] = up.y;
  matrix[9] = up.z;
  matrix[13] = 0.0f;
  
  matrix[2] = -forward.x;
  matrix[6] = -forward.y;
  matrix[10] = -forward.z;
  matrix[14] = 0.0f;
  
  matrix[3] = matrix[7] = matrix[11] = 0.0f;
  matrix[15] = 1.0f;
  
  glMultMatrixf(matrix);
  glTranslated(-eyex, -eyey, -eyez);
}

void myPerspective(GLfloat fovy, GLfloat aspect, GLfloat near, GLfloat far) {
  GLfloat f = 1/tan(fovy / 2 * 180.0f / M_PI);
  GLfloat matrix[16];
  matrix[0] = f/aspect;
  matrix[1] = matrix[2] = matrix[3] = 0;
  matrix[4] = matrix[6] = matrix[7] = 0;
  matrix[5] = f;
  matrix[8] = matrix[9] = 0;
  matrix[10] = (far + near)/(near - far);
  matrix[11] = -1;
  matrix[12] = matrix[13] = matrix[15] = 0;
  matrix[14] = (2 * far * near) / (near - far);
  glMultMatrixf(matrix);
}

void print_debug_info() {
  print_boids_info();
  print_goal_info();
}

void print_goal_info() {
  printf("Cube Position x: %f, y: %f, z: %f\n", cube_location.x, cube_location.y, cube_location.z);
}

void print_boids_info() {
  for(int i = 0; i < boids_num; i++) {
    printf("%dth Boids Position x: %f, y: %f, z: %f\n", i, boids[i]->location.x, boids[i]->location.y, boids[i]->location.z);
    printf("%dth Boids Velocity x: %f, y: %f, z: %f\n", i, boids[i]->velocity.x, boids[i]->velocity.y, boids[i]->location.z);
    printf("%dth Boids Angle angle: %f\n", i, boids[i]->angle);
  }
  printf("\n");
}
