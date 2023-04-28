#include "../include/colors.h"
#include "../include/cursor.h"
#include <csignal>
#include <cstdlib>
#include <iostream>

#define US unsigned short

const US MAX_X = 19;
const US MAX_Y = 19;
const std::string TITLE = " The Gobang Game! ";

class Cursor {
public:
  void move(short relX, short relY) {
    US temp_x = relX + x;
    US temp_y = relY + y;
    if (temp_x >= 0 && temp_x < MAX_X)
      x = temp_x;
    if (temp_y >= 0 && temp_y < MAX_Y)
      y = temp_y;
  }

  std::pair<US, US> get() { return {x, y}; }

  void reset() {
    x = MAX_X / 2;
    y = MAX_Y / 2;
  }

private:
  US x = MAX_X / 2;
  US y = MAX_Y / 2;
};

class Gobang {
public:
  void point_set(Cursor cursor, US num) {
    auto [x, y] = cursor.get();
    map[y][x] = num;
  }
  US point_get(Cursor cursor) {
    auto [x, y] = cursor.get();
    return map[y][x];
  }

  US check() {
    US combo_white = 0;
    US combo_black = 0;
    // horiz check
    for (US y = 0; y < MAX_Y; y++) {
      for (US x = 0; x <= MAX_X - 5; x++) {
        combo_white = 0;
        combo_black = 0;
        for (US n = 0; n < 5; n++) {
          if (map[y][x + n] == 1) {
            combo_white++;
          } else if (map[y][x + n] == 2) {
            combo_black++;
          }
        }
        if (combo_white == 5)
          return 1;
        else if (combo_black == 5)
          return 2;
      }
    }
    // vert check
    for (US x = 0; x < MAX_X; x++) {
      for (US y = 0; y <= MAX_Y - 5; y++) {
        combo_white = 0;
        combo_black = 0;
        for (US n = 0; n < 5; n++) {
          if (map[y + n][x] == 1) {
            combo_white++;
          } else if (map[y + n][x] == 2) {
            combo_black++;
          }
        }
        if (combo_white == 5)
          return 1;
        else if (combo_black == 5)
          return 2;
      }
    }
    // left-top > right-bottom check
    for (US x = 0; x <= MAX_X - 5; x++) {
      for (US y = 0; y <= MAX_Y - 5; y++) {
        combo_white = 0;
        combo_black = 0;
        for (US n = 0; n < 5; n++) {
          if (map[y + n][x + n] == 1) {
            combo_white++;
          } else if (map[y + n][x + n] == 2) {
            combo_black++;
          }
        }
        if (combo_white == 5)
          return 1;
        else if (combo_black == 5)
          return 2;
      }
    }
    // right-top > left-bottom check
    for (US x = 4; x <= MAX_X; x++) {
      for (US y = 0; y <= MAX_Y - 5; y++) {
        combo_white = 0;
        combo_black = 0;
        for (US n = 0; n < 5; n++) {
          if (map[y + n][x - n] == 1) {
            combo_white++;
          } else if (map[y + n][x - n] == 2) {
            combo_black++;
          }
        }
        if (combo_white == 5)
          return 1;
        else if (combo_black == 5)
          return 2;
      }
    }
    return 0;
  }

  void printBoxTop() {
    using namespace std;

    int total_length = MAX_X * 2 + 2;
    int title_length = TITLE.size();
    int left_length = (total_length - title_length) / 2;
    int right_length = total_length - title_length - left_length;

    cout << BOLDBLUE;
    cout << "╭";
    for (int i = 0; i < left_length; i++)
      cout << "─";
    cout << TITLE;
    for (int i = 0; i < right_length; i++)
      cout << "─";
    cout << "╮\n";
    cout << RESET;
  }
  void printBoxBottom() {
    using namespace std;

    int total_length = MAX_X * 2 + 2;

    cout << BOLDBLUE;
    cout << "╰";
    for (int i = 0; i < total_length; i++)
      cout << "─";
    cout << "╯\n";
    cout << RESET;
  }
  void print(Cursor cursor) {
    using namespace std;

    auto [cursor_x, cursor_y] = cursor.get();

    printBoxTop();

    for (int y = 0; y < MAX_Y; y++) {
      cout << BOLDBLUE << "│ " << RESET;
      for (int x = 0; x < MAX_X; x++) {
        if (x == cursor_x && y == cursor_y) {
          setbgcolor(88, 91, 112);
        } else if (x % 2 == y % 2) {
          setbgcolor(49, 50, 68);
        }
        cout << CHARS[map[y][x]] << RESET;
      }
      cout << BOLDBLUE << " │\n" << RESET;
    }

    printBoxBottom();
  }

  void reset() {
    for (int y = 0; y < MAX_Y; y++) {
      for (int x = 0; x < MAX_X; x++) {
        map[y][x] = 0;
      }
    }
  }

private:
  US map[MAX_Y][MAX_X]{0};
  std::string CHARS[3]{"  ", " ", " "};
};

class Side {
public:
  void toggle() { side = side == "white" ? "black" : "white"; }
  std::string get() { return side; }

private:
  std::string side = "white";
};

void bailout() {
  set_mouse_mode(false);
  set_cursor_mode(true);
  set_raw_mode(false);
  clearscreen();
}
void interrupt_handler(int x) { std::exit(0); }

int main() {
  atexit(bailout);
  signal(SIGINT, interrupt_handler);

  set_raw_mode(true);
  std::cout << "Hit space to continue\n";
  while (quick_read() != ' ')
    usleep(10'000);
  clearscreen();

  Cursor cursor{};
  Gobang gobang{};
  Side side{};

  move_cursor(0, 0);
  gobang.print(cursor);

  set_cursor_mode(false);
  set_mouse_mode(false);

  while (true) {
    int c = quick_read();
    if (c == 'q')
      break;
    if (c == ERR)
      usleep(10'000);
    else {
      switch (c) {
      case 'r':
        cursor.reset();
        gobang.reset();
        break;

      case 'h':
      case 159:
        cursor.move(-1, 0);
        break;

      case 'l':
      case 158:
        cursor.move(1, 0);
        break;

      case 'k':
      case 156:
        cursor.move(0, -1);
        break;

      case 'j':
      case 157:
        cursor.move(0, 1);
        break;

      case ' ':
        if (gobang.point_get(cursor))
          break;
        gobang.point_set(cursor, side.get() == "white" ? 1 : 2);
        side.toggle();
        break;
      }

      move_cursor(0, 0);
      gobang.print(cursor);

      if (gobang.check()) {
        switch (gobang.check()) {
        case 1:
          std::cout << " White ";
          break;
        case 2:
          std::cout << " Black ";
          break;
        }
        std::cout << BOLDYELLOW << "Won!\n" << RESET;
        cursor.reset();
        gobang.reset();
      }
    }
  }
}
