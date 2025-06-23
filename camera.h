#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"

// Camera.
typedef struct {
  Vector2D pos; // Position vector.
  Vector2D dir; // Direction vector.
  Vector2D plane; // camera plane vector.
} Camera;

// Move camera and deal collisions.
void move(Camera *cm, int dir, double dt);

// Rotate camera by theta.
void rotate(Camera *cm, int dir, double dt);

#endif
