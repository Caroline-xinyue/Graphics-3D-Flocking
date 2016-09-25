#include "ll.h"
#include "math.h"



void update_velocity(Boid boid) {
    Vector alignment = alignment(boid);
    Vector cohesion = cohesion(boid);
    Vector separation = separation(boid);
    boid.velocity.x += alignment.x * ALIGNMENT_WEIGHT + cohesion.x * COHESION_WEIGHT + separation.x * SEPARATION_WEIGHT;
    boid.velocity.y += alignment.y * ALIGNMENT_WEIGHT + cohesion.y * COHESION_WEIGHT + separation.y * SEPARATION_WEIGHT;
    boid.velocity.z += alignment.z * ALIGNMENT_WEIGHT + cohesion.z * COHESION_WEIGHT + separation.z * SEPARATION_WEIGHT;
    boid.velocity = normalize(boid.velocity);
}

Vector alignment(Boid boid) {
    Vector vector;
    vector.x = 0.0;
    vector.y = 0.0;
    vector.z = 0.0;
    int neighborCount = 0;
    for(int i = 0; i < neighbors.length; i++) {
        if(distance_from(boid.location, neighbors[i].location) < NEIGHBOR_RADIUS) {
            vector = add_vector(vector, neighbors[i].velocity);
            neighborCount++;
        }
    }
    if(neighborCount == 0) {
        return vector;
    } else {
        vector.x /= neighborCount;
        vector.y /= neighborCount;
        vector.z /= neighborCount;
        normalize(vector);
        return vector;
    }
}


Vector trans_update(){

    return add(boid.velocity,boid.location);
}




Vector cohesion(Boid boid) {
    Vector vector;
    vector.x = 0.0;
    vector.y = 0.0;
    vector.z = 0.0;
    int neighborCount = 0;
    for(int i = 0; i < neighbors.length; i++) {
        if(distance_from(boid.location, neighbors[i].location) < NEIGHBOR_RADIUS) {
            vector = add_vector(vector, neighbors[i].location);
            neighborCount++;
        }
    }
    if(neighborCount == 0) {
        return vector;
    } else {
        vector.x = vector.x / neighborCount - boid.location.x;
        vector.y = vector.y / neighborCount - boid.location.y;
        vector.z = vector.z / neighborCount - boid.location.z;
        normalize(vector);
        return vector;
    }
}
//test
Vector separation(Boid boid) {
    Vector vector;
    vector.x = 0.0;
    vector.y = 0.0;
    vector.z = 0.0;
    int neighborCount = 0;
    for(int i = 0; i < neighbors.length; i++) {
        if(distance_from(boid.location, neighbors[i].location) < NEIGHBOR_RADIUS) {
            vector = add_vector(vector, neighbors[i].location);
            vector = add_vector(vector, -boid.location);
            neighborCount++;
        }
    }
    if(neighborCount == 0) {
        return vector;
    } else {
        vector.x = -1 * vector.x / neighborCount;
        vector.y = -1 * vector.y / neighborCount;
        vector.z = -1 * vector.z / neighborCount;
        normalize(vector);
        return vector;
    }
}

Vector normalize(Vector vector) {
    Vector normalized_vector;
    float length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
    normalized_vector.x = vector.x / length;
    normalized_vector.y = vector.y / length;
    normalized_vector.z = vector.z / length;
    return normalized_vector;
}

Vector add_vector(Vector vec1, Vector vec2) {
    Vector vector;
    vector.x = vec1.x + vec2.x;
    vector.y = vec1.y + vec2.y;
    vector.z = vec1.z + vec2.z;
    return vector;
}

Vector multi_value(Vector vec, float val) {
    Vector vector;
    vector.x = vec.x * val;
    vector.y = vec.y * val;
    vector.z = vec.z * val;
    return vector;
}

float distance(Vector vector1, Vector vector2) {
    return sqrt((vector2.x - vector1.x) * (vector2.x - vector1.x) + (vector2.y - vector1.y) * (vector2.y - vector1.y) + (vector2.z - vector1.z) * (vector2.z - vector1.z));
}
