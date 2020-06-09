#ifndef TICTACTOE_FIELD_H
#define TICTACTOE_FIELD_H

#include <array>
#include <tuple>

#include "structs.h"

using std::array;

class Field {
 public:
  enum class Player { X, O };
  enum class CellState { Empty, X, O };
  enum class GameState { Continues, Deadhead, XWin, OWin };

 public:
  Field();

  void newGame();
  void selectCell(const Point& cell_coords);
  GameState getGameState() const;
  CellState getCellState(const Point& coords) const;

 private:
  static constexpr Player cellStateToPlayer(CellState state);
  static constexpr Player nextPlayer(Player player);
  bool checkDeadhead() const;
  std::tuple<bool, Player> checkVictory() const;
  void updateGameState();

  array<array<CellState, 3>, 3> field;
  Player firstPlayer = Player::X;
  Player currentPlayer;
  GameState state;
};

#endif