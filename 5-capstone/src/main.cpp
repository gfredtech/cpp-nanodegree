#include <ctime>
#include <thread>

#include "window.h"

int main() {
  Window window;
  while (window.isRunning()) {
    auto start = std::clock() / (double)CLOCKS_PER_SEC;

    window.drawFrame();

    auto end = std::clock() / (double)CLOCKS_PER_SEC;
    auto diff = 1 / 30.0 - (end - start);
    // prevents too many unneccessary re-renders
    if (diff > 0) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(uintmax_t(diff * 1000)));
    }
  }

  return 0;
}
