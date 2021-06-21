#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./ansi_codes.h"
#include "./tui_matrix.h"

/* Representation of the terminal content as a matrix of `cells` for a terminal
 * which is large enough to display `width` x `height` characters.
 */
struct Matrix {
  Cell *cells;
  size_t width;
  size_t height;
};

void cell_print(Cell *c) {
  printf("%s%s%c", c->text_color, c->background_color, c->content);
}

Cell *matrix_cell_at(Matrix *m, size_t x, size_t y) {
  assert(x < m->width);
  assert(y < m->height);
  return m->cells + y * m->width + x;
}

Matrix *matrix_new(size_t width, size_t height, Cell *def) {
  Matrix *m = malloc(sizeof(Matrix));
  if (m == NULL) {
    return NULL;
  }

  *m = (Matrix){.cells = malloc(height * width * sizeof(Cell)),
                .height = height,
                .width = width};

  if (m->cells == NULL) {
    free(m);
    return NULL;
  }

  matrix_clear_with(m, def);

  return m;
}

void matrix_free(Matrix *m) {
  free(m->cells);
  free(m);
}

size_t matrix_width(Matrix *m) { return m->width; }

size_t matrix_height(Matrix *m) { return m->height; }

void matrix_clear_with(Matrix *m, Cell *c) {
  for (size_t y = 0; y < m->height; ++y)
    for (size_t x = 0; x < m->width; ++x)
      *matrix_cell_at(m, x, y) = *c;
}

void matrix_clear(Matrix *m) {
  Cell c = {
      .content = ' ', .text_color = FG_WHITE, .background_color = BG_BLACK};
  matrix_clear_with(m, &c);
}

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void matrix_resize(Matrix *m, size_t width, size_t height, Cell *def) {
  Matrix *new = matrix_new(width, height, def);

  size_t min_height = MIN(height, m->height);
  size_t min_width = MIN(width, m->width);
  for (size_t y = 0; y < min_height; ++y)
    for (size_t x = 0; x < min_width; ++x)
      *matrix_cell_at(new, x, y) = *matrix_cell_at(m, x, y);

  free(m->cells);
  m->cells = new->cells;
  m->width = new->width;
  m->height = new->height;
  free(new);
}

bool cell_eq(Cell *c1, Cell *c2) {
  return c1->content == c2->content &&
         strcmp(c1->text_color, c2->text_color) == 0 &&
         strcmp(c1->background_color, c2->background_color) == 0;
}

void matrix_print_update(Matrix *old, Matrix *new) {
  assert(old->width == new->width);
  assert(old->height == new->height);
  for (size_t y = 0; y < new->height; ++y) {
    for (size_t x = 0; x < new->width; ++x) {
      Cell *cell_old = matrix_cell_at(old, x, y);
      Cell *cell_new = matrix_cell_at(new, x, y);
      if (!cell_eq(cell_old, cell_new)) {
        move_cursor_to(x, y);
        cell_print(cell_new);
        *cell_old = *cell_new;
      };
    }
  }
  if (new->width > 0 && new->height > 0) {
    move_cursor_to(new->width - 1, new->height - 1);
  }
  fflush(stdout);
}

void matrix_set_str_at(Matrix *m, size_t x, size_t y, const char *s,
                       const char *text_color, const char *background_color) {
  while (x < m->width && *s != 0) {
    *matrix_cell_at(m, x, y) = (Cell){.content = *s,
                                      .text_color = text_color,
                                      .background_color = background_color};
    ++x;
    ++s;
  }
}
