#include "field.h"

#include <stdexcept>

Field::Field() {
  for (auto& row : field) {
    row.fill(CellState::Empty);
  }
}

void Field::newGame() {
  state = GameState::Continues;

  for (auto& row : field) {
    row.fill(CellState::Empty);
  }
  firstPlayer = nextPlayer(firstPlayer);
  currentPlayer = firstPlayer;
}

void Field::selectCell(const Point& cell_coords) {
  if (cell_coords.x > 3 and cell_coords.y > 3) {
    std::invalid_argument e("Coords out of field");
    throw e;
  }

  auto& cell = field.at(cell_coords.x).at(cell_coords.y);

  switch (cell) {
    case CellState::Empty:
      cell = (currentPlayer == Player::X) ? CellState::X : CellState::O;
      currentPlayer = nextPlayer(currentPlayer);
      updateGameState();
      break;

    case CellState::X:
    case CellState::O:
      std::invalid_argument e("Cell is occupied");
      throw e;
      break;
  }
}

Field::GameState Field::getGameState() const { return state; }

bool Field::checkDeadhead() const {
  short emptyCellsCount = 9;

  for (auto& row : field) {
    for (auto& cell : row) {
      if (cell != CellState::Empty) {
        emptyCellsCount--;
      }
    }
  }
  if (emptyCellsCount == 0) {
    return true;
  }
  return false;
}

std::tuple<bool, Field::Player> Field::checkVictory() const {
  Player winner;
  bool isVictory = false;

  for (short c = 0; c < 3; c++) {
    if (field[c][0] != CellState::Empty and field[c][0] == field[c][1] and
        field[c][0] == field[c][2]) {
      winner = cellStateToPlayer(field[c][0]);
      isVictory = true;
    }
    if (field[0][c] == field[1][c] and field[0][c] == field[2][c] and
        field[0][c] != CellState::Empty) {
      winner = cellStateToPlayer(field[0][c]);
      isVictory = true;
    }
  }
  if ((field[1][1] != CellState::Empty) and
      ((field[0][0] == field[1][1] and field[0][0] == field[2][2]) or
       (field[0][2] == field[1][1] and field[0][2] == field[2][0]))) {
    winner = cellStateToPlayer(field[1][1]);
    isVictory = true;
  }

  return std::make_tuple(isVictory, winner);
}

void Field::updateGameState() {
  auto victoryCheckResult = checkVictory();
  if (std::get<0>(victoryCheckResult) == true) {
    state = (std::get<1>(victoryCheckResult) == Player::X) ? GameState::XWin
                                                           : GameState::OWin;

  } else if (checkDeadhead()) {
    state = GameState::Deadhead;

  } else {
    state = GameState::Continues;
  }
}

Field::CellState Field::getCellState(const Point& coords) const {
  return field.at(coords.x).at(coords.y);
}

constexpr Field::Player Field::nextPlayer(Player player) {
  return (player == Player::X) ? Player::O : Player::X;
}

constexpr Field::Player Field::cellStateToPlayer(CellState state) {
  return (state == CellState::X) ? Player::X : Player::O;
}
