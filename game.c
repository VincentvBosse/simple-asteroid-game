#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "game_lib.h"
#include "tui.h"
#include "vec.h"

int main(void) {
  tui_init();

  /* Initialize the random number generator with the current time, such that
   * `rand()` produces different random numbers every time we run the program.
   */
  srand(time(NULL));

  /* We require that the terminal window has space for at least 40 columns and
   * 20 rows. Otherwise, we exit the programm with an error message.
   * We assume that the terminal size doesn't change after the game has started.
   */
  Size2 min_term_size = {40, 20};
  Size2 term_size = tui_size();
  if (term_size.x < min_term_size.x || term_size.y < min_term_size.y) {
    tui_shutdown();
    printf("ERROR: terminal has to be at least of size %lu x %lu.\n",
           min_term_size.x, min_term_size.y);
    exit(1);
  }

  /* The game field starts at position {1, 1}. This accounts for the white frame
   * drawn around the game field.
   *
   * The game field ends a few rows above the bottom of the terminal, so we have
   * space to render the info text (see screenshots).
   */
  Int2 field_begin = {1, 1};
  Int2 field_end = {term_size.x - 1, term_size.y - 3};
  Int2 field_size = {field_end.x - field_begin.x, field_end.y - field_begin.y};

  /* Initialize the game state, which you have to manipulate in the functions
   * from game_lib.c. The GameState-struct is defined and documented in
   * game_lib.h. */
  GameState game_state = {
      .term_size = {term_size.x, term_size.y}, /* Convert Size2 to Int2. */
      .field_begin = field_begin,
      .field_end = field_end,
      .field_size = field_size,
      .ship =
          {
              .pos = {1, field_size.y /
                             2}, /* At the beginning, the ship is positioned */
              .health = 3,       /* left to the center of the game field. */
              .powerup_time = 0,
          },
      .points = 0,
      .projectiles = vec_new(),
      .asteroids = vec_new(),
      .powerups = vec_new(),
      .explosions = vec_new(),
      .time_step = 0,
  };

  while (1) {
    /* Handle Keyboard Input */

    if (stdin_has_changed()) {
      char c = read_from_stdin();
      if (handle_input(&game_state, c)) {
        break;
      }
    }

    /* Update the GameState. */

    move_projectiles(&game_state);
    handle_projectile_asteroid_collisions(&game_state);

    move_asteroids(&game_state);
    spawn_asteroids(&game_state);
    handle_projectile_asteroid_collisions(&game_state);
    handle_asteroid_ship_collisions(&game_state);

    move_powerups(&game_state);
    spawn_powerups(&game_state);
    handle_powerup_ship_collisions(&game_state);

    move_explosions(&game_state);

    /* Exit the game, if the ship was hit by enough asteroids. */
    if (game_state.ship.health <= 0) {
      break;
    }

    /* If the ship currently has a powerup, decrease the time
     * until the powerup is gone. */
    if (game_state.ship.powerup_time > 0) {
      game_state.ship.powerup_time--;
    }

    /* Draw the GameState in the terminal. */

    tui_clear();

    draw_info_bar(&game_state);
    draw_frame(&game_state);
    draw_ship(&game_state);
    draw_projectiles(&game_state);
    draw_asteroids(&game_state);
    draw_powerups(&game_state);
    draw_explosions(&game_state);

    tui_update();

    /* Increase time step and wait for 10000 µs (0.01 s). */

    game_state.time_step++;

    usleep(10000);
  }

  /* Free our vectors and their data storage. */
  vec_free(game_state.explosions);
  vec_free(game_state.powerups);
  vec_free(game_state.asteroids);
  vec_free(game_state.projectiles);

  tui_shutdown();

  return 0;
}
