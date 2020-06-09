#include "sprite.h"

#include <SDL2/SDL_image.h>

#include <stdexcept>

using std::string;

Sprite::Sprite(const string& pathToImage, SDL_Renderer* renderer)
    : renderer(renderer) {
  load(pathToImage);
}

Sprite::Sprite(SDL_Surface* image, SDL_Renderer* renderer)
    : renderer(renderer) {
  load(image);
}

Sprite::~Sprite() { SDL_DestroyTexture(texture); }

void Sprite::draw() {
  SDL_Rect destination = {position.x, position.y, size.width, size.height};
  SDL_RenderCopy(renderer, texture, nullptr, &destination);
}

void Sprite::move(const Point& position) { this->position = position; }

void Sprite::resize(const Size& size) { this->size = size; }

void Sprite::load(const string& filepath) {
  SDL_Surface* image = IMG_Load(filepath.c_str());
  if (image == nullptr) {
    std::invalid_argument e("Error when load image: " + filepath + ":\n" +
                            SDL_GetError());
    throw e;
  }

  texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);
}

void Sprite::load(SDL_Surface* image) {
  if (image == nullptr) {
    std::invalid_argument e("Invalid image");
    throw e;
  }

  texture = SDL_CreateTextureFromSurface(renderer, image);
}

Rect Sprite::getRect() const { return {position, size}; }
