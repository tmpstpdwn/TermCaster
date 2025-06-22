/* [[ INCLUDES ]] */

#include <stdio.h>
#include <sys/time.h>
#include <math.h>

/* [[ DEFINES ]] */

// Map diamensions.
#define MAPROWS 15
#define MAPCELLS 15

// FOV = 2 * arctan(PLANE_HWIDTH).
// 0.66 gives 66 degrees in FOV.
#define PLANE_HWIDTH 0.66

// Camera normal, angular speed.
#define SPEED 2
#define ANGULAR_SPEED 1.5

// Raycaster
#define MAX_DRAW_DIST 20

/* [[ MAP ]] */

const int map[MAPROWS][MAPCELLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {2, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 4},
    {2, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 2, 0, 4},
    {2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 4},
    {2, 0, 0, 2, 2, 0, 2, 0, 2, 2, 0, 0, 2, 0, 4},
    {2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 4},
    {2, 0, 0, 2, 0, 0, 3, 3, 0, 2, 0, 0, 2, 0, 4},
    {2, 0, 0, 2, 2, 2, 3, 3, 2, 2, 2, 0, 2, 0, 4},
    {2, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 2, 0, 4},
    {2, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 2, 0, 4},
    {2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 4},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 4},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
    {2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}
};

/* [[ ENUMS ]] */

// Gamestate.
typedef enum {
    OFF,
    ON,
} GameState;

/* [[ STRUCTS ]] */

// 2D Vector.
typedef struct {
  double x, y;
} Vector2D;

// Camera.
typedef struct {
  Vector2D pos; // Position vector.
  Vector2D dir; // Direction vector.
  Vector2D plane; // camera plane vector.
} Camera;

/* [[ DCL ]] */

// Helpers.
double get_time_in_seconds(); // Get current time in seconds.

// Camera.
void move(Camera *cm, int dir, double dt); // Move camera and deal collisions.
void rotate(Camera *cm, int dir, double dt); // Rotate camera by theta.

// Raycaster.
void ray_cast(Camera *cm, int width, int height); // Cast rays in players fov and draw vertical lines for each wallhit based on-
// distance from player to wallhit.

/* [[ DEF ]] */

double get_time_in_seconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) + (tv.tv_usec / 1000000.0);
}

void move(Camera *cm, int dir, double dt) {
  double next_x = cm->pos.x + cm->dir.x * SPEED * dir * dt;
  double next_y = cm->pos.y + cm->dir.y * SPEED * dir * dt;
  if (map[(int)cm->pos.y][(int)next_x] == 0) {
      cm->pos.x = next_x;
  }
  if (map[(int)next_y][(int)cm->pos.x] == 0) {
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

void ray_cast(Camera *cm, int width, int height) {
  Vector2D ray, xy, side_dist, delta_dist, step;
  for (int x = 0; x < width; x++) {

    double cameraX = 2 * x / (double)width - 1;
    ray.x = cm->dir.x + cm->plane.x * cameraX;
    ray.y = cm->dir.y + cm->plane.y * cameraX;

    xy.x = (int)cm->pos.x;
    xy.y = (int)cm->pos.y;

    delta_dist.x = fabs(1/ray.x);
    delta_dist.y = fabs(1/ray.y);

    double perp_wall_dist;
    double ray_dist = 0;
    int hit = 0;
    int side;

    if (ray.x < 0) {
      step.x = -1;
      side_dist.x = (cm->pos.x - xy.x) * delta_dist.x;
    } else {
      step.x = 1;
      side_dist.x = (xy.x + 1 - cm->pos.x) * delta_dist.x;
    }
    if (ray.y < 0) {
      step.y = -1;
      side_dist.y = (cm->pos.y - xy.y) * delta_dist.y;
    } else {
      step.y = 1;
      side_dist.y = (xy.y + 1 - cm->pos.y) * delta_dist.y;
    }

    while (!hit && ray_dist < MAX_DRAW_DIST) {
      if (side_dist.x < side_dist.y) {
        ray_dist = side_dist.x;
        side_dist.x += delta_dist.x;
        xy.x += step.x;
        side = 0;
      } else {
        ray_dist = side_dist.y;
        side_dist.y += delta_dist.y;
        xy.y += step.y;
        side = 1;
      }
      if (map[(int)(xy.y)][(int)(xy.x)] > 0) hit = 1;
    }

    if (!hit) {
      continue;
    }

    if (side == 0) perp_wall_dist = (side_dist.x - delta_dist.x);
    else           perp_wall_dist = (side_dist.y - delta_dist.y);
    int line_height = (int)(height / perp_wall_dist);

    int draw_start = height / 2 - line_height / 2;
    draw_start = (draw_start < 0)? 0: draw_start;

    int draw_end = height / 2 + line_height / 2; 
    draw_end = (draw_end >= height)? height - 1: draw_end;

    for (int y = draw_start; y <= draw_end; y++) {
      int color;
      switch (map[(int)(xy.y)][(int)(xy.x)]) {
        case 1:
          color = TB_GREEN;
          break;
        case 2:
          color = TB_BLUE;
          break;
        case 3:
          color = TB_YELLOW;
          break;
        case 4:
          color = TB_CYAN;
          break;
      }
      tb_set_cell(x, y, ' ', TB_BLACK, color);
    }
  }
}

/* [[ END ]] */
