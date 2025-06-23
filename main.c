/* [[ INCLUDES ]] */

#define TB_IMPL

#include "termbox2.h"
#include "config.h"
#include "utils.h"
#include "camera.h"
#include "raycaster.h"

/* [[ MAIN ]] */

int main() {

  // Termbox2 setup.
  if (tb_init() != 0) {
      return 1;
  }

  // Screen dimensions.
  int width = tb_width(); // cols.
  int height = tb_height(); // rows.

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

    // Clear terminal buffer.
    tb_clear();
    
    // Raycasting.
    ray_cast(&cm, width, height);

    // Flush / Render terminal buffer.
    tb_present();
    } 

  // Termbox2 cleanup.
  tb_shutdown();
  return 0;
}

/* [[ END ]] */
