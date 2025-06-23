#include "renderer.h"
#include "termbox2.h"

void render_vline(double wall_dist, int height, int col) {
  int line_height = (int)(height / wall_dist);

  int draw_start = height / 2 - line_height / 2;
  draw_start = (draw_start < 0)? 0: draw_start;

  int draw_end = height / 2 + line_height / 2; 
  draw_end = (draw_end >= height)? height - 1: draw_end;

  for (int row = draw_start; row <= draw_end; row++) {
    tb_set_cell(col, row, ' ', TB_BLACK, TB_GREEN);
  }
}
