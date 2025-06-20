#define TB_IMPL

#include <stdio.h>
#include "termbox2.h"
#include "caster.h"

int main() {

  // termbox2 setup.
  if (tb_init() != 0) {
      fprintf(stderr, "Failed to initialize termbox\n");
      return 1;
  }
  tb_clear();
  tb_present();

  Vector2D xy = {
    tb_width(),
    tb_height()
  };

  Camera cm = {
    {9,             6},
    {1,             0},
    {0,  PLANE_HWIDTH}
  };
 
  GameState gs = ON;

  struct tb_event ev;

  double last_time, current_time, delta_time;
  last_time = get_time_in_seconds();

  while (gs) {
    current_time = get_time_in_seconds();
    delta_time = current_time - last_time;
    last_time = current_time;

    tb_clear();

    tb_peek_event(&ev, 0);
    if (ev.type == TB_EVENT_KEY) {
      switch (ev.key) {
        case TB_KEY_ARROW_UP:
          move(&cm, 1, delta_time);
          break;
        case TB_KEY_ARROW_DOWN:
          move(&cm, -1, delta_time);
          break;
        case TB_KEY_ARROW_LEFT:
          rotate(&cm, 1, delta_time);
          break;
        case TB_KEY_ARROW_RIGHT:
          rotate(&cm, -1, delta_time);
          break;
        case TB_KEY_ESC:
          gs = OFF;
          break;
        }
      }

    tb_present();
    } 

  tb_shutdown();
  return 0;
}
