/* [[ INCLUDES ]] */

#include <stdio.h>
#include <sys/time.h>
#include <math.h>

/* [[ DEFINES ]] */

// Map.
#define MAPROWS 10
#define MAPCELLS 10

const int map[MAPROWS][MAPCELLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// FOV = 2 * arctan(PLANE_HWIDTH).
// 0.66 gives 66 degrees in FOV.
#define PLANE_HWIDTH 0.66

// Camera normal, angular speed.
#define SPEED 1
#define ANGULAR_SPEED 5

/* [[ STRUCTS ]] */

// Gamestate.
typedef enum {
    OFF,
    ON,
} GameState;

// 2D Vector.
typedef struct {
  double x, y;
} Vector2D;

// Camera.
typedef struct {
  Vector2D pos;
  Vector2D dir;
  Vector2D plane;
} Camera;

// Projection plane / screen.
typedef struct {
  int w, h;
  Vector2D plane;
} Screen;

/* [[ DCL ]] */

// Helpers.
double get_time_in_seconds(); // get current time in seconds.

// Camera.
void move(Camera *cm, int dir, double dt); // Move camera and deal collisions.
void rotate(Camera *cm, int dir, double dt); // Rotate camera by theta.
void set_plane(Camera *cm); // Set proj plane wrt camera.

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
}

void set_plane(Camera *cm) {
  cm->plane.x = -(cm->dir.y * PLANE_HWIDTH);
  cm->plane.y = (cm->dir.x * PLANE_HWIDTH);
}

/* [[ END ]] */
