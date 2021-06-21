#ifndef TUI_H
#define TUI_H

#include <stdio.h>

#include "./ansi_codes.h"
#include "./tui_io.h"
#include "./tui_matrix.h"

/* Configure the terminal for interactive use.
 *
 * Specifically:
 * - sets the terminal mode to raw
 * - clears the terminal content and hides the cursor
 * - initializes the global matrices to match the current terminal size.
 */
void tui_init(void);

/* Reset the terminal for normal use.
 *
 * Specifically:
 * - brings the terminal back to normal mode
 * - clears the terminal content, shows the cursor, resets the terminal
 *   colors, and moves the cursor to position (0,0), such that the terminal
 *   looks exactly like a freshly opened terminal window after the program has
 *   been closed.
 */
void tui_shutdown(void);

/* Retrieve the cell of the character at position (x, y). */
Cell *tui_cell_at(size_t x, size_t y);

/* Render string `s` with styles `text_color` and `background_color` starting
 * from position `(x, y)`. If the matrix row is not long enough to hold the
 * string, only parts of the string are shown.
 *
 * The string `s` is not allowed to contain any line breaks \n.
 */
void tui_set_str_at(size_t x, size_t y, const char *s, const char *text_color,
                    const char *background_color);

/* Query the current terminal size and resize the matrices if necessary. */
Size2 tui_size(void);

/* Print changes done with `tui_cell_at` and `tui_set_str_at` to the terminal.
 */
void tui_update(void);

/* Set all cells to be equal to `c`. */
void tui_clear_with(Cell *c);

/* Set all cells be the space character with black background and white text
 * color. */
void tui_clear(void);

#endif /* TUI_H */
