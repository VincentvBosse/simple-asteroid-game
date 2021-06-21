#ifndef TUI_INTERNAL_H
#define TUI_INTERNAL_H

/* Representation of a terminal cell at a certain (x,y) position. */
typedef struct Cell {
  char content;           /* The character at this position */
  const char *text_color; /* ANSI Code for the text color of this character */
  const char *background_color; /* ANSI Code for the background color of this
                                   character */
} Cell;

/* Representation of the terminal content as a matrix of `cells`. */
typedef struct Matrix Matrix;

/* Allocate a new matrix of size `width` x `height` where each cell is
 * initialized to be a copy of `def`.
 */
Matrix *matrix_new(size_t width, size_t height, Cell *def);

/* Deallocate a matrix. */
void matrix_free(Matrix *m);

/* Set all cells of `m` to be equal to `c`. */
void matrix_clear_with(Matrix *m, Cell *c);

/* Set all cells of `m` to be the space character with black background and
 * white text color. */
void matrix_clear(Matrix *m);

/* Returns the width of the matrix. */
size_t matrix_width(Matrix *m);

/* Returns the height of the matrix. */
size_t matrix_height(Matrix *m);

/* Retrieve the Cell corresponding to the terminal character at column `x` and
 * row `y`. Uses `assert` to abort the program if `x` and `y` are not valid
 * indices.
 */
Cell *matrix_cell_at(Matrix *m, size_t x, size_t y);

/* Write string `s` with styles `text_color` and `background_color` into the
 * matrix `m`, starting from position `(x, y)`. If the matrix row is not long
 * enough to hold the string, only the characters which fit into the row are
 * shown.
 *
 * This function does not handle line breaks like '\n', so the string `s` is not
 * allowed to contain line breaks.
 */
void matrix_set_str_at(Matrix *m, size_t x, size_t y, const char *s,
                       const char *text_color, const char *background_color);

/* Resize the matrix.
 * If the new size is smaller, old cells are removed.
 * If the new size is larger, new cells are initialized to be equal to `def`.
 */
void matrix_resize(Matrix *m, size_t width, size_t height, Cell *def);

/* For each cell in `new`, which is different from the corresponding cell in
 * `old`:
 *
 * - print the cell from `new` with the correct color at the correct position;
 *
 * - update the corresponding cell in `old` to be equal to the cell in `new` to
 *   reflect the current state of the terminal;
 *
 * After the cells are redrawn and updated, the cursor position is moved to the
 * last column of the last row and stdout is flushed.
 *
 * Note: It does *not* make sense to use any linebreak like '\n' in the
 * definition of this function.
 */
void matrix_print_update(Matrix *old, Matrix *new);

#endif /* TUI_INTERNAL_H */
