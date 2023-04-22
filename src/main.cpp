#include <iostream>

#include "../include/colors.h"
#include "../include/cursor.h"
#include <csignal>
#include <cstdlib>

const unsigned short MaxX = 19;
const unsigned short MaxY = 19;
const std::string Title = " Gobang ";

class Cursor {
public:
  Cursor() = default;
  Cursor(Cursor &&) = default;
  Cursor(const Cursor &) = default;
  Cursor &operator=(Cursor &&) = default;
  Cursor &operator=(const Cursor &) = default;
  ~Cursor() = default;

  void move(short relX, short relY) {
    unsigned short tempX = relX + x;
    unsigned short tempY = relY + y;
    if (tempX >= 0 && tempX < MaxX)
      x = tempX;
    if (tempY >= 0 && tempY < MaxY)
      y = tempY;
  }

  std::pair<unsigned short, unsigned short> get() { return {x, y}; }

  void reset() {
    x = MaxX / 2;
    y = MaxY / 2;
  }

private:
  unsigned short x = MaxX / 2;
  unsigned short y = MaxY / 2;
};

class Gobang {
public:
  Gobang() = default;
  Gobang(Gobang &&) = default;
  Gobang(const Gobang &) = default;
  Gobang &operator=(Gobang &&) = default;
  Gobang &operator=(const Gobang &) = default;
  ~Gobang() = default;

  void setPoint(Cursor cursor, unsigned short num) {
    auto [x, y] = cursor.get();
    map[y][x] = num;
  }
  unsigned short getPoint(Cursor cursor) {
    auto [x, y] = cursor.get();
    return map[y][x];
  }

  void print(Cursor cursor) {
    using namespace std;

    auto [cursorX, cursorY] = cursor.get();

    cout << BOLDBLUE;
    cout << "╭";
    for (int i = 0; i < (MaxX + 1) - Title.size() / 2; i++)
      cout << "─";
    cout << Title;
    for (int i = 0; i < (MaxX + 1) - Title.size() / 2; i++)
      cout << "─";
    cout << "╮\n";
    cout << RESET;

    for (int y = 0; y < MaxY; y++) {
      cout << BOLDBLUE << "│ " << RESET;
      for (int x = 0; x < MaxX; x++) {
        if (x == cursorX && y == cursorY) {
          setbgcolor(88, 91, 112);
        } else if (x % 2 == y % 2) {
          setbgcolor(49, 50, 68);
        }
        cout << Chars[map[y][x]] << RESET;
      }
      cout << BOLDBLUE << " │\n" << RESET;
    }

    cout << BOLDBLUE;
    cout << "╰";
    for (int i = 0; i < (MaxX + 1) * 2; i++)
      cout << "─";
    cout << "╯\n";
    cout << RESET;
  }

  void reset() {
    for (int y = 0; y < MaxY; y++) {
      for (int x = 0; x < MaxX; x++) {
        map[y][x] = 0;
      }
    }
  }

private:
  unsigned short map[MaxY][MaxX]{0};
  std::string Chars[3]{"  ", " ", " "};
};

class Side {
public:
  Side() = default;
  Side(Side &&) = default;
  Side(const Side &) = default;
  Side &operator=(Side &&) = default;
  Side &operator=(const Side &) = default;
  ~Side() = default;

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

  moveCursor(0, 0);
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
        if (gobang.getPoint(cursor))
          break;
        gobang.setPoint(cursor, side.get() == "white" ? 1 : 2);
        side.toggle();
        break;
      }

      moveCursor(0, 0);
      gobang.print(cursor);
    }
  }
}
