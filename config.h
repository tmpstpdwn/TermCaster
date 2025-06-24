#ifndef CONFIG_H
#define CONFIG_H 

// Map dimensions.
#define MAPROWS 15
#define MAPCELLS 15

// FOV = 2 * arctan(PLANE_HWIDTH).
// 0.66 gives 66 degrees in FOV.
#define PLANE_HWIDTH 0.66

// Camera normal, angular speed.
#define SPEED 2
#define ANGULAR_SPEED 2

// Raycaster
#define MAX_DRAW_DIST 20

#endif
