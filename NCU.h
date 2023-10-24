#include <ncurses.h>

#define absolute(x) ((x) < 0 ? -(x) : x)

int curses_screen_width = 0;
int curses_screen_height = 0;

/* Initialize ncurses with sane defaults
 */
void curses_init(int width, int height) {
  /* Init curses */
  initscr();
  /* Disable buffering and get one character-at-a-time input */
  cbreak();
  /* Get Special keys */
  keypad(stdscr, TRUE);
  /* Suppress echoing of typed keys */
  noecho();
  /* Hide cursor */
  curs_set(0);

  /* Set globals */
  curses_screen_width = width;
  curses_screen_height = height;
}

/*
 * Gracfully stop ncurses
 */
void curses_stop() { endwin(); }

/* Get one character of input
 */
int curses_input() { return getch(); }

/* Clear the screen
 */
void curses_clear() { erase(); }

void curses_point(int y, int x, char toPrint) { mvprintw(y, x, "%c", toPrint); }

void curses_line(int start_y, int start_x, int stop_y, int stop_x,
                 char toPrint) {
  int dx = absolute(stop_x - start_x);
  int sx = start_x < stop_x ? 1 : -1;
  int dy = -absolute(stop_y - start_y);
  int sy = start_y < stop_y ? 1 : -1;
  int error = dx + dy;

  while (1) {
    mvprintw(start_y, start_x, "%c", toPrint);
    if (start_y == stop_y && start_x == stop_x)
      break;
    int e2 = 2 * error;
    if (e2 >= dy) {
      if (start_x != stop_x) {
        error = error + dy;
        start_x = start_x + sx;
      }
    }
    if (e2 <= dx) {
      if (start_y != stop_y) {
        error = error + dx;
        start_y = start_y + sy;
      }
    }
  }
}

void curses_border() {
  curses_line(0, 0, curses_screen_height, 0, '|');
  curses_line(0, 0, 0, curses_screen_width, '-');
  curses_line(0, curses_screen_width, curses_screen_height, curses_screen_width,
              '|');
  curses_line(curses_screen_height, 0, curses_screen_height,
              curses_screen_width, '-');

  curses_point(0, 0, '+');
  curses_point(curses_screen_height, 0, '+');
  curses_point(0, curses_screen_width, '+');
  curses_point(curses_screen_height, curses_screen_width, '+');
}
