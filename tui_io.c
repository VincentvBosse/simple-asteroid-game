#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#include "./tui_io.h"

/* Global variable to store the original terminal mode. */
struct termios orig_termios;

void set_raw_terminal_mode(void) {
  struct termios new_termios;

  /* Take two copies - one for now, one for later */
  tcgetattr(0, &orig_termios);
  memcpy(&new_termios, &orig_termios, sizeof(new_termios));

  /* Register cleanup handler, and set the new terminal mode */
  atexit(restore_terminal_mode);
  cfmakeraw(&new_termios);
  tcsetattr(0, TCSANOW, &new_termios);
}

void restore_terminal_mode(void) { tcsetattr(0, TCSANOW, &orig_termios); }

bool stdin_has_changed(void) {
  struct timeval timeout = {0L, 0L};
  fd_set fds;

  /* Init file descriptor set. */
  FD_ZERO(&fds);

  /* We want to track the terminal input file `stdin`. */
  FD_SET(fileno(stdin), &fds);

  /* `select` returns `true`, if any file descriptor in `fds` is ready to be
   * read. */
  return select(fileno(stdin) + 1, &fds, NULL, NULL, &timeout);
}

int read_from_stdin(void) {
  int r;
  unsigned char c;
  if ((r = read(fileno(stdin), &c, sizeof(c))) < 0) {
    return r;
  } else {
    return c;
  }
}

Size2 query_size(void) {
  struct winsize w;

  ioctl(fileno(stdout), TIOCGWINSZ, &w);

  return (Size2){.x = w.ws_col, .y = w.ws_row};
}
