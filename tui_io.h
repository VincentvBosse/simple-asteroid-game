#ifndef TUI_IO_H
#define TUI_IO_H

#include <stdbool.h>

/* Set the terminal to raw mode.
 *
 * Normally, the terminal uses line buffering, i.e. it waits for the user to
 * write a complete line before it sends the characters to the program.
 *
 * In raw mode, line buffering is disabled, so a pressed key directly arrives at
 * the program.
 *
 * Furthermore, in raw mode, the keys pressed by the user are not automatically
 * displayed in the terminal, but the program has to use `print` if it really
 * wants to display the pressed keys.
 */
void set_raw_terminal_mode(void);

/* Reset the terminal to normal mode.
 *
 * If you exit a program without resetting the terminal mode, your Terminal will
 * behave in misterious ways afterwards.
 */
void restore_terminal_mode(void);

/* Check if the user has pressed a key.
 * This function only works in raw terminal mode.
 */
bool stdin_has_changed(void);

/* Returns the key pressed by the user or an error.
 *
 * If you have previously checked, that `stdin_has_changed()` is true, then the
 * return value is always a valid `char` and not an error, e.g.
 *
 *   if (stdin_has_changed()) {
 *     // This is ok:
 *     char c = read_from_stdin();
 *   }
 */
int read_from_stdin(void);

/* A pair of size_t values. */
typedef struct Size2 {
  size_t x;
  size_t y;
} Size2;

/* Returns the current size of the terminal. The size may change if the user
 * resizes the terminal emulator window.
 */
Size2 query_size(void);

#endif /* TUI_IO_H */
