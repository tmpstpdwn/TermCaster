#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "camera.h"

// Cast rays in players fov and draw vertical lines for
// each wallhit based on distance from player to wallhit.
void ray_cast(Camera *cm, int width, int height);

#endif
