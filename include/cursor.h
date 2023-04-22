#ifndef __CURSOR_H__
#define __CURSOR_H__
#include <iostream>
inline void moveCursor(unsigned x, unsigned y) {
  std::cout << "\033[" << y << ";" << x << "H";
}
#endif
