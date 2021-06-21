#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "tui.h"

#include "game_lib.h"

void draw_info_bar(GameState *gs) {
  char buf[255];
  sprintf(buf, "LIFES: %d    POINTS: %d    DISTANCE: %d    POWERUP: %d",
          gs->ship.health, gs->points, gs->time_step, gs->ship.powerup_time);
  tui_set_str_at(0, gs->term_size.y - 1, buf, FG_WHITE, BG_BLACK);
}

void draw_frame(GameState *gs) {
  Cell c = (Cell){
      .content = ' ', .text_color = FG_WHITE, .background_color = BG_WHITE};
  Int2 frame_begin = {gs->field_begin.x - 1, gs->field_begin.y - 1};
  Int2 frame_end = {gs->field_end.x + 1, gs->field_end.y + 1};
  for (size_t x = frame_begin.x; x < frame_end.x; ++x) {
    *tui_cell_at(x, frame_begin.y) = c;
    *tui_cell_at(x, frame_end.y - 1) = c;
  }
  for (size_t y = frame_begin.y; y < frame_end.y; ++y) {
    *tui_cell_at(frame_begin.x, y) = c;
    *tui_cell_at(frame_end.x - 1, y) = c;
  }
}

bool is_field_coordinate(GameState *gs, int x, int y) {
  Int2 size = gs->field_size;
  bool x_is_valid = 0 <= x && x < size.x;
  bool y_is_valid = 0 <= y && y < size.y;
  return x_is_valid && y_is_valid;
}

Cell *field_cell_at(GameState *gs, int x, int y) {
  /* Same as `assert(is_field_coordinate(gs, x, y))` but prints a stack trace if
   * used with the address sanitizer.
   *
   * The stack trace allows you to find out which function call caused the
   * invalid coordinates, which greatly simplifies debugging.
   * (You will probably run into this.)
   *
   * To get the stack trace, we use an ugly hack: if the assertion fails, we
   * simply cause a segmentation fault by writing to the NULL-Pointer, which the
   * address sanitizer then detects and spits out a stack trace for us :3
   */
  if (!is_field_coordinate(gs, x, y)) {
    tui_shutdown();
    printf(FG_RED "ASSERTION FAILED: Coordinate (%d, %d) is not a valid game "
                  "field coordinate.\n\n" COLOR_RESET,
           x, y);
    fflush(stdout);
    int *null = NULL;
    *null = 42;
  }

  return tui_cell_at(x + gs->field_begin.x, y + gs->field_begin.y);
}

void draw_ship(GameState *gs) {
  Cell shipbody = {
      .content = ' ', .background_color = BG_HI_CYAN, .text_color = FG_HI_CYAN};
  Cell shipweapon = {
      .content = '>', .background_color = BG_BLACK, .text_color = FG_HI_YELLOW};
  Cell shiptruster = {
      .content = '=', .background_color = BG_BLACK, .text_color = FG_HI_YELLOW};
  Cell exhaust = {
      .content = '-', .background_color = BG_BLACK, .text_color = FG_YELLOW};
  // exhaust
  *field_cell_at(gs, gs->ship.pos.x, gs->ship.pos.y - 1) = exhaust;
  *field_cell_at(gs, gs->ship.pos.x, gs->ship.pos.y) = exhaust;
  *field_cell_at(gs, gs->ship.pos.x, gs->ship.pos.y + 1) = exhaust;
  // thrusters
  *field_cell_at(gs, gs->ship.pos.x + 1, gs->ship.pos.y - 1) = shiptruster;
  *field_cell_at(gs, gs->ship.pos.x + 1, gs->ship.pos.y) = shiptruster;
  *field_cell_at(gs, gs->ship.pos.x + 1, gs->ship.pos.y + 1) = shiptruster;
  // body
  *field_cell_at(gs, gs->ship.pos.x + 2, gs->ship.pos.y - 1) = shipbody;
  *field_cell_at(gs, gs->ship.pos.x + 2, gs->ship.pos.y) = shipbody;
  *field_cell_at(gs, gs->ship.pos.x + 2, gs->ship.pos.y + 1) = shipbody;
  *field_cell_at(gs, gs->ship.pos.x + 3, gs->ship.pos.y) = shipbody;
  *field_cell_at(gs, gs->ship.pos.x + 4, gs->ship.pos.y) = shipbody;
  // weapon
  *field_cell_at(gs, gs->ship.pos.x + 5, gs->ship.pos.y) = shipweapon;
  if (gs->ship.powerup_time > 0) {
    *field_cell_at(gs, gs->ship.pos.x + 3, gs->ship.pos.y - 1) = shipweapon;
    *field_cell_at(gs, gs->ship.pos.x + 3, gs->ship.pos.y + 1) = shipweapon;
  }
}

void draw_projectiles(GameState *gs) {
  Cell projectile = {
      .content = '>', .background_color = BG_BLACK, .text_color = FG_HI_RED};
  for (int i = vec_length(gs->projectiles) - 1; i >= 0; i--) {
    Int2 *currentprojectile = *(vec_at(gs->projectiles, i));
    *field_cell_at(gs, currentprojectile->x, currentprojectile->y) = projectile;
  }
}

void draw_asteroids(GameState *gs) {
  Cell asteriod = {
      .content = ' ', .background_color = BG_WHITE, .text_color = FG_WHITE};
  for (int i = vec_length(gs->asteroids) - 1; i >= 0; i--) {
    Int2 *currentasteroid = *(vec_at(gs->asteroids, i));
    *field_cell_at(gs, currentasteroid->x, currentasteroid->y) = asteriod;
  }
}

void draw_powerups(GameState *gs) {
  Cell powewrup = {
      .content = '@', .background_color = BG_BLACK, .text_color = FG_HI_GREEN};
  for (int i = vec_length(gs->powerups) - 1; i >= 0; i--) {
    Int2 *currentpowerup = *(vec_at(gs->powerups, i));
    *field_cell_at(gs, currentpowerup->x, currentpowerup->y) = powewrup;
  }
}

void draw_explosions(GameState *gs) {
  Cell explosion = {.content = '#',
                    .background_color = BG_HI_RED,
                    .text_color = FG_HI_YELLOW};
  for (int i = vec_length(gs->explosions) - 1; i >= 0; i--) {
    Explosion *cureentexplosion = *(vec_at(gs->explosions, i));
    // in -x direction
    if (is_field_coordinate(
            gs, cureentexplosion->pos.x - (2 * cureentexplosion->age),
            cureentexplosion->pos.y)) {
      *field_cell_at(gs, cureentexplosion->pos.x - (2 * cureentexplosion->age),
                     cureentexplosion->pos.y) = explosion;
    }
    // in +x direction
    if (is_field_coordinate(
            gs, cureentexplosion->pos.x + (2 * cureentexplosion->age),
            cureentexplosion->pos.y)) {
      *field_cell_at(gs, cureentexplosion->pos.x + (2 * cureentexplosion->age),
                     cureentexplosion->pos.y) = explosion;
    }
    // in -y direction
    if (is_field_coordinate(gs, cureentexplosion->pos.x,
                            cureentexplosion->pos.y - cureentexplosion->age)) {
      *field_cell_at(gs, cureentexplosion->pos.x,
                     cureentexplosion->pos.y - cureentexplosion->age) =
          explosion;
    }
    // in +y direction
    if (is_field_coordinate(gs, cureentexplosion->pos.x,
                            cureentexplosion->pos.y + cureentexplosion->age)) {
      *field_cell_at(gs, cureentexplosion->pos.x,
                     cureentexplosion->pos.y + cureentexplosion->age) =
          explosion;
    }
  }
}

bool handle_input(GameState *gs, char c) {
  if (c == 'q')
    return true;
  if (c == 'w') {
    if (gs->ship.pos.y > 2) {
      gs->ship.pos.y--;
      return false;
    }
  }
  if (c == 'a') {
    if (gs->ship.pos.x > 1) {
      gs->ship.pos.x--;
      return false;
    }
  }
  if (c == 's') {
    if (gs->ship.pos.y < (gs->field_size.y - 3)) {
      gs->ship.pos.y++;
      return false;
    }
  }
  if (c == 'd') {
    if (gs->ship.pos.x < (gs->field_size.x - 7)) {
      gs->ship.pos.x++;
      return false;
    }
  }
  if (c == ' ') {
    Int2 *newprojectile = malloc(sizeof(Int2));
    if (newprojectile == NULL) {
      exit(1);
    }
    newprojectile->x = gs->ship.pos.x + 5;
    newprojectile->y = gs->ship.pos.y;
    if (vec_push(gs->projectiles, newprojectile) == false) {
      exit(1);
    }
    if (gs->ship.powerup_time > 0) {
      Int2 *upperpowerupprojectile = malloc(sizeof(Int2));
      if (upperpowerupprojectile == NULL) {
        exit(1);
      }
      upperpowerupprojectile->x = gs->ship.pos.x + 2;
      upperpowerupprojectile->y = gs->ship.pos.y - 1;
      if (vec_push(gs->projectiles, upperpowerupprojectile) == false) {
        exit(1);
      }
      Int2 *lowerpowerupprojectile = malloc(sizeof(Int2));
      if (lowerpowerupprojectile == NULL) {
        exit(1);
      }
      lowerpowerupprojectile->x = gs->ship.pos.x + 2;
      lowerpowerupprojectile->y = gs->ship.pos.y + 1;
      if (vec_push(gs->projectiles, lowerpowerupprojectile) == false) {
        exit(1);
      }
    }
    return false;
  }
  return false;
}

bool collides_with_ship(Int2 ship_pos, Int2 pos) {
  if (pos.x == ship_pos.x + 2 && pos.y == ship_pos.y - 1) {
    return true;
  }
  if (pos.x == ship_pos.x + 2 && pos.y == ship_pos.y) {
    return true;
  }
  if (pos.x == ship_pos.x + 2 && pos.y == ship_pos.y + 1) {
    return true;
  }
  if (pos.x == ship_pos.x + 3 && pos.y == ship_pos.y) {
    return true;
  }
  if (pos.x == ship_pos.x + 4 && pos.y == ship_pos.y) {
    return true;
  }
  return false;
}

void move_projectiles(GameState *gs) {
  for (int i = vec_length(gs->projectiles) - 1; i >= 0; i--) {
    Int2 *currentprojectile = *(vec_at(gs->projectiles, i));
    if (currentprojectile->x < gs->field_size.x - 1) {
      currentprojectile->x++;
    } else {
      vec_remove(gs->projectiles, i);
    }
  }
}

void move_asteroids(GameState *gs) {
  if (gs->time_step % 5 == 0) {
    for (int i = vec_length(gs->asteroids) - 1; i >= 0; i--) {
      Int2 *currentasteroid = *(vec_at(gs->asteroids, i));
      if (currentasteroid->x > 1) {
        currentasteroid->x--;
      } else {
        vec_remove(gs->asteroids, i);
      }
    }
  }
}

void move_powerups(GameState *gs) {
  for (int i = vec_length(gs->powerups) - 1; i >= 0; i--) {
    Int2 *currentpowerup = *(vec_at(gs->powerups, i));
    if (currentpowerup->x > 1) {
      currentpowerup->x--;
    } else {
      vec_remove(gs->powerups, i);
    }
  }
}

void move_explosions(GameState *gs) {
  for (int i = vec_length(gs->explosions) - 1; i >= 0; i--) {
    Explosion *currentexplosion = *(vec_at(gs->explosions, i));
    currentexplosion->age++;
    if (currentexplosion->age > 5) {
      vec_remove(gs->explosions, i);
    }
  }
}

void spawn_asteroids(GameState *gs) {
  if (gs->time_step % 5 == 0) {
    for (int i = 0; i <= gs->field_size.y - 1; i++) {
      int random = rand() % 50;
      if (random == 0) {
        Int2 *newasteroid = malloc(sizeof(Int2));
        if (newasteroid == NULL) {
          exit(1);
        }
        newasteroid->x = gs->field_size.x - 1;
        newasteroid->y = i;
        if (vec_push(gs->asteroids, newasteroid) == false) {
          exit(1);
        }
      }
    }
  }
}

void spawn_powerups(GameState *gs) {
  int random = rand() % 200;
  if (random == 0) {
    Int2 *newpowerup = malloc(sizeof(Int2));
    if (newpowerup == NULL) {
      exit(1);
    }
    newpowerup->x = gs->field_size.x - 1;
    newpowerup->y = rand() % gs->field_size.y;
    if (vec_push(gs->powerups, newpowerup) == false) {
      exit(1);
    }
  }
}

void handle_projectile_asteroid_collisions(GameState *gs) {
  for (int i = vec_length(gs->asteroids) - 1; i >= 0; i--) {
    for (int u = vec_length(gs->projectiles) - 1; u >= 0; u--) {
      Int2 *currentasteroid = *(vec_at(gs->asteroids, i));
      Int2 *currentprojectile = *(vec_at(gs->projectiles, u));
      if (currentasteroid->x == currentprojectile->x &&
          currentasteroid->y == currentprojectile->y) {
        Explosion *newexplosion = malloc(sizeof(Explosion));
        if (newexplosion == NULL) {
          exit(1);
        }
        newexplosion->age = 0;
        newexplosion->pos = *(currentprojectile);
        if (vec_push(gs->explosions, newexplosion) == false) {
          exit(1);
        }
        vec_remove(gs->asteroids, i);
        vec_remove(gs->projectiles, u);
        gs->points += 5;
        break;
      }
    }
  }
}

void handle_powerup_ship_collisions(GameState *gs) {
  for (int i = vec_length(gs->powerups) - 1; i >= 0; i--) {
    Int2 *currentpowerup = *(vec_at(gs->powerups, i));
    if (collides_with_ship(gs->ship.pos, *(currentpowerup))) {
      vec_remove(gs->powerups, i);
      gs->ship.powerup_time = 1000;
      gs->points += 50;
    }
  }
}

void handle_asteroid_ship_collisions(GameState *gs) {
  for (int i = vec_length(gs->asteroids) - 1; i >= 0; i--) {
    Int2 *currentasteroid = *(vec_at(gs->asteroids, i));
    if (collides_with_ship(gs->ship.pos, *(currentasteroid))) {
      Explosion *newexplosion = malloc(sizeof(Explosion));
      if (newexplosion == NULL) {
        exit(1);
      }
      newexplosion->age = 0;
      newexplosion->pos = *(currentasteroid);
      if (vec_push(gs->explosions, newexplosion) == false) {
        exit(1);
      }
      vec_remove(gs->asteroids, i);
      gs->ship.health--;
    }
  }
}
