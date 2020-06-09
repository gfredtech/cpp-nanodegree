#ifndef TICTACTOE_SPRITE_H
#define TICTACTOE_SPRITE_H
#include <string>

#include "structs.h"

using std::string;
class SDL_Texture;
class SDL_Surface;
class SDL_Renderer;

class Sprite {
 public:
  Sprite(const string& pathToImage, SDL_Renderer* renderer);
  Sprite(SDL_Surface* surface, SDL_Renderer* renderer);
  ~Sprite();

  void draw();
  void move(const Point& position);
  void resize(const Size& size);

  Rect getRect() const;

 private:
  void load(const string& pathToImage);
  void load(SDL_Surface* image);

  Point position;
  Size size;
  SDL_Texture* texture;
  SDL_Renderer* renderer;
};

#endif