#ifndef GAME_LIB_H
#define GAME_LIB_H

#include "tui.h"
#include "vec.h"

/** DATA STRUCTURES ***********************************************************/

typedef struct Int2 {
  int x;
  int y;
} Int2;

typedef struct Explosion {
  Int2 pos; /* Position where the explosion originally started in field
               coordinates. */
  int age;  /* How many time steps the explosion already exists. */
} Explosion;

typedef struct Ship {
  Int2 pos;   /* The ship's position in field coordinates. */
  int health; /* How many asteroids the ship can still crash into before game
                 over. */
  int powerup_time; /* How many time steps until the powerup runs out. 0 if the
                       ship currently has no powerup. */
} Ship;

typedef struct GameState {
  Int2 term_size;   /* The terminal matrix is at least this big. */
  Int2 field_begin; /* The terminal coordinates of the upper left corner of the
                       game field. */
  Int2 field_end;  /* The terminal coordinates *one past* the lower right corner
                      of the game field. */
  Int2 field_size; /* How many rows and columns are in the game field. */
  Ship ship;
  int points;       /* How many points the player has scored by shooting down
                       asteroids and collecting powerups. */
  Vec *projectiles; /* The positions of the currently active projectiles in
                       field coordinates. */
  Vec *asteroids;   /* The positions of the currently active asteroids in field
                       coordinates. */
  Vec *powerups;    /* The positions of the currently active powerups in field
                       coordinates. */
  Vec *explosions;  /* The currently active `Explosion`s. */
  int time_step; /* How many iterations the while-loop in game.c has already run
                    through. */
} GameState;

/** DRAWING *******************************************************************/

/* Draws the info data below the game field (ship's health, points, distance,
 * remaining powerup time). */
void draw_info_bar(GameState *gs);

/* Draws a white border *around* the game field (area between `field_begin` and
 * `field_end`). */
void draw_frame(GameState *gs);

/* Like `tui_cell_at` but uses (x,y) coordinates, which are relative to
 * `gs->field_begin`. */
Cell *field_cell_at(GameState *gs, int x, int y);

/* Returns true iff (x,y) are valid game field coordinates. */
bool is_field_coordinate(GameState *gs, int x, int y);

/* Draws the ship at it's current position. */
void draw_ship(GameState *gs);

/* Draws all projectiles at their current position. */
void draw_projectiles(GameState *gs);

/* Draws all asteroids at their current position. */
void draw_asteroids(GameState *gs);

/* Draws all powerups at their current position. */
void draw_powerups(GameState *gs);

/* Draws all explosions at their current position. */
void draw_explosions(GameState *gs);

/** INPUT HANDLING ************************************************************/

/* How to change the GameState `gs` if the user pressed the key `c`. */
bool handle_input(GameState *gs, char c);

/** SIMULATING ANOTHER GAME STEP **********************************************/

/* Move all asteroids one step to the left, if the time step is divisible by 5,
 * otherwise do nothing. Asteroids, which would leave the game field, are
 * removed.
 */
void move_asteroids(GameState *gs);

/* Move all projectiles one step to the right.
 * Projectiles, which would leave the game field, are removed.
 */
void move_projectiles(GameState *gs);

/* Move all powerups one step to the left.
 * Powerups, which would leave the game field, are removed.
 */
void move_powerups(GameState *gs);

/* Increment the age of all explosions by one.
 * Explosions, which would reach age == 6, are removed.
 */
void move_explosions(GameState *gs);

/* Spawn new asteroids in the rightmost column of the game field. */
void spawn_asteroids(GameState *gs);

/* Spawn new powerups in the rightmost column of the game field. */
void spawn_powerups(GameState *gs);

/* Returns true iff a ship at `ship_pos` would collide with an asteroid/powerup
   at `pos`. All parts of the ship, which are drawn as pink cells, are
   considered for collision. */
bool collides_with_ship(Int2 ship_pos, Int2 pos);

/* Check if projectiles collide with asteroids. For each projectile which
 * collides with an asteroid, remove both from the game state.
 */
void handle_projectile_asteroid_collisions(GameState *gs);

/* Check if the ship collides with a powerup. For each powerup which
 * collides with the ship, the powerup is removed and the ship's powerup_time is
 * set to 1000.
 */
void handle_powerup_ship_collisions(GameState *gs);

/* Check if the ship collides with an asteroid. For each asteroid which collides
 * with the ship, the asteroid is removed and the ship's health is reduced by 1.
 */
void handle_asteroid_ship_collisions(GameState *gs);

#endif /* GAME_LIB_H */
