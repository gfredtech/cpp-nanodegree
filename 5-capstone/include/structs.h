#ifndef TICTACTOE_STRUCTS_H
#define TICTACTOE_STRUCTS_H
#include <cstdint>

struct Point {
  Point() : x(0), y(0) {}

  Point(uint32_t x, uint32_t y) : x(x), y(y) {}

  bool operator==(const Point& rhs) const {
    return ((x == rhs.x) and (y == rhs.y));
  }

  bool operator!=(const Point& rhs) const {
    return ((x != rhs.x) or (y != rhs.y));
  }

  uint32_t x, y;
};

struct Size {
  Size() : width(0), height(0) {}

  Size(uint32_t width, uint32_t height) : width(width), height(height) {}

  bool operator==(const Size& rhs) const {
    return ((width == rhs.width) and (height == rhs.height));
  }

  bool operator!=(const Size& rhs) const {
    return ((width != rhs.width) or (height != rhs.height));
  }

  uint32_t width, height;
};

struct Rect {
  Rect() : topLeft(0, 0), size(0, 0) {}

  Rect(const Point& topLeft, const Size& size) : topLeft(topLeft), size(size) {}

  bool contain(const Point& point) const {
    return (point.x >= topLeft.x) and (point.x <= topLeft.x + size.width) and
           (point.y >= topLeft.y) and (point.y <= topLeft.y + size.height);
  }

  bool intersects(const Rect& rect) const {
    return (middle() == rect.middle()) or contain(rect.topLeft) or
           contain(rect.bottomRight()) or
           contain(Point({rect.topLeft.x, rect.bottomRight().y})) or
           contain({rect.topLeft.y, rect.bottomRight().x});
  }

  Point middle() const {
    return {topLeft.x + size.width / 2, topLeft.y + size.height / 2};
  }

  uint32_t width() const { return size.width; }

  uint32_t height() const { return size.height; }

  Point bottomRight() const {
    return {topLeft.x + size.width, topLeft.y + size.height};
  }

  Point topLeft;
  Size size;
};

#endif
