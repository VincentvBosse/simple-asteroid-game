#ifndef ANSI_CODES_H
#define ANSI_CODES_H

#include <stddef.h>

/* Regular text */
#define FG_BLACK "\e[0;30m"
#define FG_RED "\e[0;31m"
#define FG_GREEN "\e[0;32m"
#define FG_YELLOW "\e[0;33m"
#define FG_BLUE "\e[0;34m"
#define FG_MAGENTA "\e[0;35m"
#define FG_CYAN "\e[0;36m"
#define FG_WHITE "\e[0;37m"

/* Bold text */
#define FG_BOLD_BLACK "\e[1;30m"
#define FG_BOLD_RED "\e[1;31m"
#define FG_BOLD_GREEN "\e[1;32m"
#define FG_BOLD_YELLOW "\e[1;33m"
#define FG_BOLD_BLUE "\e[1;34m"
#define FG_BOLD_MAGENTA "\e[1;35m"
#define FG_BOLD_CYAN "\e[1;36m"
#define FG_BOLD_WHITE "\e[1;37m"

/* Underlined text */
#define FG_UNDERLINE_BLACK "\e[4;30m"
#define FG_UNDERLINE_RED "\e[4;31m"
#define FG_UNDERLINE_GREEN "\e[4;32m"
#define FG_UNDERLINE_YELLOW "\e[4;33m"
#define FG_UNDERLINE_BLUE "\e[4;34m"
#define FG_UNDERLINE_MAGENTA "\e[4;35m"
#define FG_UNDERLINE_CYAN "\e[4;36m"
#define FG_UNDERLINE_WHITE "\e[4;37m"

/* Background */
#define BG_BLACK "\e[40m"
#define BG_RED "\e[41m"
#define BG_GREEN "\e[42m"
#define BG_YELLOW "\e[43m"
#define BG_BLUE "\e[44m"
#define BG_MAGENTA "\e[45m"
#define BG_CYAN "\e[46m"
#define BG_WHITE "\e[47m"

/* High intensty background */
#define BG_HI_BLACK "\e[0;100m"
#define BG_HI_RED "\e[0;101m"
#define BG_HI_GREEN "\e[0;102m"
#define BG_HI_YELLOW "\e[0;103m"
#define BG_HI_BLUE "\e[0;104m"
#define BG_HI_MAGENTA "\e[0;105m"
#define BG_HI_CYAN "\e[0;106m"
#define BG_HI_WHITE "\e[0;107m"

/* High intensty text */
#define FG_HI_BLACK "\e[0;90m"
#define FG_HI_RED "\e[0;91m"
#define FG_HI_GREEN "\e[0;92m"
#define FG_HI_YELLOW "\e[0;93m"
#define FG_HI_BLUE "\e[0;94m"
#define FG_HI_MAGENTA "\e[0;95m"
#define FG_HI_CYAN "\e[0;96m"
#define FG_HI_WHITE "\e[0;97m"

/* Bold high intensity text */
#define FG_BOLD_HI_BLACK "\e[1;90m"
#define FG_BOLD_HI_RED "\e[1;91m"
#define FG_BOLD_HI_GREEN "\e[1;92m"
#define FG_BOLD_HI_YELLOW "\e[1;93m"
#define FG_BOLD_HI_BLUE "\e[1;94m"
#define FG_BOLD_HI_MAGENTA "\e[1;95m"
#define FG_BOLD_HI_CYAN "\e[1;96m"
#define FG_BOLD_HI_WHITE "\e[1;97m"

/* Reset to default style */
#define COLOR_RESET "\e[0m"

/* Show/Hide the cursor in the terminal.
 *
 * By default the cursor is shown, i.e. the character at the current cursor
 * position is highlighted by swapping foreground and background color, which
 * you usually notice as a white block, which indicates where the next input
 * goes.
 *
 * While our program uses a TUI, we want to hide the cursor, because it's going
 * to jump around like crazy while we're updating the screen.
 */
#define CURSOR_HIDE "\e[?25l"
#define CURSOR_SHOW "\e[?25h"

/* Clear all previously text in the terminal, i.e. the prompt or output from
 * previously executed programs.
 */
#define CLEAR_SCREEN "\e[2J"

/* Move the terminal cursor to column x and row y, such that the next `printf`
 * starts printing at this position.
 */
void move_cursor_to(size_t x, size_t y);

#endif /* ANSI_CODES_H */
