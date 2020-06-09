#include "window.h"

#include <SDL_ttf.h>

#include <ctime>

Window::Window() {
  SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);
  TTF_Init();
  window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, size.width, size.height,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_SetRenderDrawColor(renderer, 10, 25, 10, 255);

  background =
      unique_ptr<Sprite>(new Sprite("../assets/images/back.png", renderer));
  background->move({0, 0});

  buttonBackground = unique_ptr<Sprite>(
      new Sprite("../assets/images/button_back.png", renderer));

  cellBackground = unique_ptr<Sprite>(
      new Sprite("../assets/images/cell_back.png", renderer));
  X = unique_ptr<Sprite>(new Sprite("../assets/images/X.png", renderer));
  O = unique_ptr<Sprite>(new Sprite("../assets/images/O.png", renderer));

  auto* font = TTF_OpenFont("../assets/font/28_days_later.ttf", 256);
  SDL_Color foregroundColor = {230, 230, 110, 0};

  auto* textSurface = TTF_RenderText_Blended(font, "New game", foregroundColor);
  textNewGame = unique_ptr<Sprite>(new Sprite(textSurface, renderer));
  SDL_FreeSurface(textSurface);

  textSurface = TTF_RenderText_Blended(font, "Options", foregroundColor);
  textOptions = unique_ptr<Sprite>(new Sprite(textSurface, renderer));
  SDL_FreeSurface(textSurface);

  textSurface = TTF_RenderText_Blended(font, "Exit", foregroundColor);
  textExit = unique_ptr<Sprite>(new Sprite(textSurface, renderer));
  SDL_FreeSurface(textSurface);

  TTF_CloseFont(font);

  resize(size.width, size.height);
}

Window::~Window() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  TTF_Quit();
  SDL_Quit();
}

void Window::drawFrame() {
  processEvents();

  SDL_RenderClear(renderer);

  background->draw();
  switch (state) {
    case State::Game:
      drawField();
      break;
    case State::Menu:
      drawMenu();
      break;
  }

  SDL_RenderPresent(renderer);
}

void Window::drawField() {
  Size cellSize = {fieldSize / 3, fieldSize / 3};
  for (short c = 0; c < 3; c++) {
    for (short i = 0; i < 3; i++) {
      Point cellTopLeft = {(size.width - fieldSize) / 4 + c * cellSize.width,
                           (size.height - fieldSize) / 2 + i * cellSize.height};

      cellBackground->move(cellTopLeft);
      cellBackground->draw();

      if (field.getCellState({c, i}) == Field::CellState::X) {
        X->move(cellTopLeft);
        X->draw();

      } else if (field.getCellState({c, i}) == Field::CellState::O) {
        O->move(cellTopLeft);
        O->draw();
      }
    }
  }
}

void Window::drawMenu() {
  auto button = buttonBackground->getRect().size;

  for (short i = 0; i < 3; i++) {
    buttonBackground->move({offsetToTopLeftCorner,
                            offsetToTopLeftCorner + i * (button.height + 10)});
    buttonBackground->draw();

    auto buttonTopLeft = buttonBackground->getRect().topLeft;
    switch (i) {
      case 0:
        textNewGame->move(
            {buttonTopLeft.x + button.width / 10, buttonTopLeft.y});
        textNewGame->draw();
        break;
      case 1:
        textOptions->move(
            {buttonTopLeft.x + button.width / 6, buttonTopLeft.y});
        textOptions->draw();
        break;
      case 2:
        textExit->move({buttonTopLeft.x + button.width / 4, buttonTopLeft.y});
        textExit->draw();
        break;
    }
  }
}

bool Window::isRunning() const { return running; }

void Window::processEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            if (field.getGameState() == Field::GameState::Continues) {
              state = ((state == State::Game) ? State::Menu : State::Game);
            }
            break;
        }
        break;

      case SDL_MOUSEBUTTONDOWN: {
        Point mouse = {event.motion.x, event.motion.y};

        if (state == State::Game) {
          if (field.getGameState() == Field::GameState::Continues) {
            proccessPlayerTurn({mouse.x, mouse.y});
          }

        } else if (state == State::Menu) {
          for (short c = 0; c < 3; c++) {
            auto buttonSize = buttonBackground->getRect().size;
            Rect buttonRect(
                {offsetToTopLeftCorner,
                 offsetToTopLeftCorner + (buttonSize.height + 10) * c},
                buttonSize);

            if (buttonRect.contain(mouse)) {
              switch (c) {
                case 0:
                  field.newGame();
                  state = State::Game;
                  break;
                case 1:
                  break;
                case 2:
                  running = false;
                  break;
              }
            }
          }
        }
        break;
      }
      case SDL_WINDOWEVENT:
        switch (event.window.event) {
          case SDL_WINDOWEVENT_RESIZED:
            resize(event.window.data1, event.window.data2);
            break;
        }
        break;
      case SDL_QUIT:
        running = false;
        break;
    }
  }
}

void Window::proccessPlayerTurn(const Point& mouse_coords) {
  static Rect field_rect(
      {(size.width - fieldSize) / 4, (size.height - fieldSize) / 2},
      {fieldSize, fieldSize});

  if (field_rect.contain(mouse_coords)) {
    try {
      field.selectCell(
          {(mouse_coords.x - (size.width - fieldSize) / 4) / (fieldSize / 3),
           (mouse_coords.y - (size.height - fieldSize) / 2) / (fieldSize / 3)});

    } catch (std::invalid_argument e) {
      ;
    }

    auto gameState = field.getGameState();
    if (gameState != Field::GameState::Continues) {
      drawFrame();
      gameOver();
    }
  }
}

void Window::gameOver() {
  string result = "Game over, ";
  switch (field.getGameState()) {
    case Field::GameState::Deadhead:
      result += "deadhead";
      break;
    case Field::GameState::XWin:
      result += "X wins";
      break;
    case Field::GameState::OWin:
      result += "O wins";
      break;
  }

  showGameOverMessageBox(result);
}

void Window::showGameOverMessageBox(std::string result) {
  const int NEW_GAME_BUTTON_ID = 1;
  const int GO_TO_MENU_BUTTON_ID = 2;

  SDL_MessageBoxButtonData buttons[] = {
      {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, NEW_GAME_BUTTON_ID, "New Game"},
      {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, GO_TO_MENU_BUTTON_ID,
       "Go to menu"}};

  SDL_MessageBoxData data = {SDL_MESSAGEBOX_INFORMATION,
                             window,
                             "Game over",
                             result.c_str(),
                             2,
                             buttons};

  int buttonPressed = 0;
  SDL_ShowMessageBox(&data, &buttonPressed);

  SDL_PumpEvents();
  SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);

  switch (buttonPressed) {
    case NEW_GAME_BUTTON_ID:
      field.newGame();
      break;

    case GO_TO_MENU_BUTTON_ID:
      state = State::Menu;
      break;
  }
}

void Window::resize(int width, int height) {
  size = {width, height};
  background->resize(size);
  offsetToTopLeftCorner =
      (size.width > size.height) ? size.height / 5 : size.width / 5;

  fieldSize = (size.width > size.height)
                  ? size.height - offsetToTopLeftCorner * 2
                  : size.width - offsetToTopLeftCorner * 2;
  Size cellSize = {fieldSize / 3, fieldSize / 3};
  cellBackground->resize(cellSize);
  X->resize(cellSize);
  O->resize(cellSize);

  buttonBackground->resize({size.width - offsetToTopLeftCorner * 2,
                            (size.height - offsetToTopLeftCorner * 2) / 3});
  Size button = buttonBackground->getRect().size;
  textNewGame->resize({button.width * 0.8, button.height});
  textOptions->resize({button.width * 0.6, button.height});
  textExit->resize({button.width * 0.4, button.height});
}
