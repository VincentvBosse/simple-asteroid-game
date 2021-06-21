#include <stdio.h>

#include "./ansi_codes.h"

/* ANSI code to move the cursor to a specific row and column in the terminal.
 *
 * If we print this ANSI Code and afterwards some regular text,
 * then the regular text will start at this position. If at this position was
 * previously some other text, the text will be overwritten.
 *
 * The indexing starts at row=1 and col=1 ...
 */
#define CURSOR_TO(row, col) "\e[" row ";" col "f"

void move_cursor_to(size_t x, size_t y) {
  printf(CURSOR_TO("%ld", "%ld"), y + 1, x + 1);
}
