#include <iostream>
#include <vector>

enum class Player { User, Bot };

struct Move {
  int row = 0;
  int amount = 0;
};

class Nim {
private:
  std::vector<int> state{3, 4, 5};
  Player currentPlayer = Player::User;

public:
  Nim() = default;
  Nim(Player startingPlayer) : currentPlayer{startingPlayer} {};

  Player play() {
    while (!gameEnded()) {
      printState();
      move(getMove());
    }
    return currentPlayer == Player::User ? Player::Bot : Player::User;
  }

  void printState() {
    for (int i : state) {
      for (int j = 0; j < i; j++) {
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
      break;
    case Player::Bot:
      std::cin >> mv.row >> mv.amount;
      break;
    }
    return mv;
  }

  void move(Move mv) {
    state[mv.row - 1] -= mv.amount;
  } // обработать случай когда значения больше возможных

  bool gameEnded() { return state[0] + state[1] + state[2] == 0; }
};

int main(int argc, char const *argv[]) {
  Nim game{};
  Player winner = game.play();
  return 0;
}
