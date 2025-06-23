#ifndef UTILS_H
#define UTILS_H

// Gamestate.
typedef enum {
    OFF,
    ON,
} GameState;

// 2D Vector.
typedef struct {
  double x, y;
} Vector2D;

// Get current time in seconds.
double get_time_in_seconds();

#endif
