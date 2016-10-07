#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#else
#include <GLFW/glfw3.h>
#endif
#include "ll.h"

Vector normalize_vec(Vector vector) {
    Vector normalized_vector;
    normalized_vector.x = 0;
    normalized_vector.y = 0;
    normalized_vector.z = 0;
    float length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
    if(length != 0.0) {
        normalized_vector.x = vector.x / length;
        normalized_vector.y = vector.y / length;
        normalized_vector.z = vector.z / length;
    }
    return normalized_vector;
}

Boid* make_boid(Vector loc, Vector vel) {
    Boid* new;
    if ( (new = (Boid*) malloc( sizeof(Boid) ) )!= NULL) {
        new -> location.x = loc.x;
        new -> location.y = loc.y;
        new -> location.z = loc.z;
        new -> velocity.x = vel.x;
        new -> velocity.y = vel.y;
        new -> velocity.z = vel.z;
        new -> angle = 0.0f;
    }
    else {
        printf("Out of memory!\n");
        exit(0);
    }
    return new;
}

Vector add_vec_vec(Vector vec1, Vector vec2) {
    Vector vector;
    vector.x = vec1.x + vec2.x;
    vector.y = vec1.y + vec2.y;
    vector.z = vec1.z + vec2.z;
    return vector;
}

Vector mult_vec_val(Vector vec, float val) {
    Vector vector;
    vector.x = vec.x * val;
    vector.y = vec.y * val;
    vector.z = vec.z * val;
    return vector;
}

Vector cross_vec_vec(Vector vec1, Vector vec2) {
    Vector vector;
    vector.x = vec1.y * vec2.z - vec1.z * vec2.y;
    vector.y = vec1.z * vec2.x - vec1.x * vec2.z;
    vector.z = vec1.x * vec2.y - vec1.y * vec2.x;
    return vector;
}

float distance_vec_vec(Vector vector1, Vector vector2) {
    return sqrt((vector2.x - vector1.x) * (vector2.x - vector1.x) + (vector2.y - vector1.y) * (vector2.y - vector1.y) + (vector2.z - vector1.z) * (vector2.z - vector1.z));
}
