#include "raycaster.h"
#include "camera.h"
#include "config.h"
#include "utils.h"
#include "map.h"
#include "renderer.h"
#include <math.h>

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
      hit = wall(xy.x, xy.y);
    }

    if (!hit) {
      continue;
    }

    if (side == 0) perp_wall_dist = (side_dist.x - delta_dist.x);
    else           perp_wall_dist = (side_dist.y - delta_dist.y);

    render_vline(perp_wall_dist, height, x);
  }
}
