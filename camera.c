#include "camera.h"
#include "config.h"
#include "map.h"
#include <math.h>

void move(Camera *cm, int dir, double dt) {
  double next_x = cm->pos.x + cm->dir.x * SPEED * dir * dt;
  double next_y = cm->pos.y + cm->dir.y * SPEED * dir * dt;
  if (!wall(next_x, cm->pos.y)) {
      cm->pos.x = next_x;
  }
  if (!wall(cm->pos.x, next_y)) {
      cm->pos.y = next_y;
  }
}

void rotate(Camera *cm, int dir, double dt) {
  double x = cm->dir.x;
  double y = cm->dir.y;
  double theta = ANGULAR_SPEED * dir * dt;
  cm->dir.x = x * cos(theta) - y * sin(theta); 
  cm->dir.y = x * sin(theta) + y * cos(theta); 
  cm->plane.x = -(cm->dir.y * PLANE_HWIDTH);
  cm->plane.y = (cm->dir.x * PLANE_HWIDTH);
}
