/* [[ INCLUDES ]] */

#define TB_IMPL

#include <stdio.h>
#include <math.h>
#include "termbox2.h"
#include "termcaster.h"

/* [[ MAIN ]] */

int main() {

  // Termbox2 setup.
  if (tb_init() != 0) {
      fprintf(stderr, "Failed to initialize termbox\n");
      return 1;
  }

  // Screen diamension.
  int width =  tb_width(); // cols.
  int height =  tb_height(); // rows.

  // Camera.
  Camera cm = {
    {5.5,           6}, // Initial position.
    {1,             0}, //    "    direction.
    {0,  PLANE_HWIDTH}  //    "    plane vector.
  };
 
  // Gamestate: ON / OFF.
  GameState gs = ON;

  // Event struct.
  struct tb_event ev;

  // Variables to calculate delta time.
  double last_time, current_time, delta_time;
  last_time = get_time_in_seconds();

  // Gameloop.
  while (gs) {

    // Delta time calculation.
    current_time = get_time_in_seconds();
    delta_time = current_time - last_time;
    last_time = current_time;

    // Clear terminal buffer.
    tb_clear();

    // Deal with events.
    tb_peek_event(&ev, 0);
    if (ev.type == TB_EVENT_KEY) {
      switch (ev.key) {
        case TB_KEY_ARROW_UP:
          move(&cm,  1, delta_time);
          break;
        case TB_KEY_ARROW_DOWN:
          move(&cm, -1, delta_time);
          break;
        case TB_KEY_ARROW_LEFT:
          rotate(&cm, -1, delta_time);
          break;
        case TB_KEY_ARROW_RIGHT:
          rotate(&cm,  1, delta_time);
          break;
        case TB_KEY_ESC:
          gs = OFF;
          break;
        }
    } else if (ev.type == TB_EVENT_RESIZE) {
      width = ev.w;
      height = ev.h;
    }
    
    // Raycasting.
    Vector2D ray, xy, side_dist, delta_dist, step;
    for (int x = 0; x < width; x++) {
      // Set the ray vector.
      double cameraX = 2 * x / (double)width - 1;
      ray.x = cm.dir.x + cm.plane.x * cameraX;
      ray.y = cm.dir.y + cm.plane.y * cameraX;

      // Get camera pos in grid coordinates.
      xy.x = (int)cm.pos.x;
      xy.y = (int)cm.pos.y;

      // Set delta_dist values.
      delta_dist.x = fabs(1/ray.x);
      delta_dist.y = fabs(1/ray.y);

      double perp_wall_dist; // Distance from camera plane to wall.
      int hit = 0; // Did we hit?
      int side; // Which side did we hit?

      // Set step_dist, side values.
      if (ray.x < 0) {
        step.x = -1;
        side_dist.x = (cm.pos.x - xy.x) * delta_dist.x;
      } else {
        step.x = 1;
        side_dist.x = (xy.x + 1 - cm.pos.x) * delta_dist.x;
      }
      if (ray.y < 0) {
        step.y = -1;
        side_dist.y = (cm.pos.y - xy.y) * delta_dist.y;
      } else {
        step.y = 1;
        side_dist.y = (xy.y + 1 - cm.pos.y) * delta_dist.y;
      }

      // Check for wall hit.
      while (!hit) {
        if (side_dist.x < side_dist.y) {
          side_dist.x += delta_dist.x;
          xy.x += step.x;
          side = 0;
        } else {
          side_dist.y += delta_dist.y;
          xy.y += step.y;
          side = 1;
        }
        if (map[(int)(xy.y)][(int)(xy.x)] > 0) hit = 1;
      }

      // Calculate wall height.
      if (side == 0) perp_wall_dist = (side_dist.x - delta_dist.x);
      else           perp_wall_dist = (side_dist.y - delta_dist.y);
      int line_height = (int)(height / perp_wall_dist);

      // Draw wall stripe.
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

    // Flush terminal buffer.
    tb_present();
    } 

  // Termbox2 cleanup.
  tb_shutdown();
  return 0;
}

/* [[ END ]] */
