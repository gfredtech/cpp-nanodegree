#ifndef TICTACTOE_WINDOW_H
#define TICTACTOE_WINDOW_H

#include <memory>

#include "field.h"
#include "sprite.h"

using std::unique_ptr;
class SDL_Window;
class SDL_Renderer;

class Window {
 public:
  Window();
  ~Window();

  void drawFrame();
  bool isRunning() const;

 private:
  enum class State { Menu, Game };

  void drawField();
  void drawMenu();
  void processEvents();
  void proccessPlayerTurn(const Point& mouse_coords);
  void gameOver();
  void showGameOverMessageBox(string result);
  void resize(int width, int height);

  Field field;

  State state = State::Menu;

  SDL_Window* window;
  SDL_Renderer* renderer;

  unique_ptr<Sprite> textNewGame;
  unique_ptr<Sprite> textOptions;
  unique_ptr<Sprite> textExit;

  unique_ptr<Sprite> X;
  unique_ptr<Sprite> O;
  unique_ptr<Sprite> cellBackground;
  unique_ptr<Sprite> background;
  unique_ptr<Sprite> buttonBackground;

  int offsetToTopLeftCorner = 100;
  Size size = {800, 600};
  int fieldSize = 300;

  bool running = true;
};

#endif