#include <iostream>
#include <numeric>
#include <vector>

enum class Player { User, Bot };

struct Move {
  int row{0};
  int amount{0};

  Move() = default;
  Move(int row, int amount) : row{row}, amount{amount} {};
};

int calculateNimSum(std::vector<int> state) {
  int res{0};
  for (int i : state) {
    res ^= i;
  }
  return res;
}

class Nim {
private:
  std::vector<int> state{3, 4, 5};
  Player currentPlayer{Player::User};
  Player awaitingPlayer{Player::Bot};

  void printState() {
    for (int i : state) {
      for (int j{0}; j < i; j++) {
        std::cout << '#';
      }
      std::cout << "\n";
    }
  }

  Move getMove() {
    Move mv{};
    switch (currentPlayer) {
    case Player::User:
      std::cout << "Введите ход вида \"строка кол-во\": ";
      std::cin >> mv.row >> mv.amount;
      --mv.amount;
      break;
    case Player::Bot:
      std::cout << "Ход компьютера: ";
      mv = findOptimalMove();
      std::cout << mv.row << " " << mv.amount << "\n";
      break;
    }
    std::swap(currentPlayer, awaitingPlayer);
    return mv;
  }

  void move(Move mv) {
    state[mv.row] -= mv.amount;
  } // обработать случай когда значения больше возможных

  Move findOptimalMove() {
    if (calculateNimSum(state) == 0) {
      for (int j{0}; j < state.size(); ++j) {
        if (state[j] != 0)
          return Move{j, 1};
      }
    } else {
      for (int j{0}; j < state.size(); ++j) {
        std::vector<int> potentialMove{state};
        potentialMove[j] = 0;
        int newValue{calculateNimSum(potentialMove)};
        if (newValue < state[j])
          return Move{j, state[j] - newValue};
      }
    }
  }

  bool gameEnded() { return std::reduce(state.begin(), state.end()) == 0; }

public:
  Nim() = default;
  Nim(Player startingPlayer) : currentPlayer{startingPlayer} {};

  Player play() {
    while (!gameEnded()) {
      printState();
      move(getMove());
    }
    return awaitingPlayer;
  }
};

int main(int argc, char const *argv[]) {
  Nim game{};
  Player winner{game.play()};
  switch (winner) {
  case Player::User:
    std::cout << "Победил игрок";
    break;
  case Player::Bot:
    std::cout << "Победил компьютер";
    break;
  }
  return 0;
}
