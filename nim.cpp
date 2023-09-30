#include <iostream>
#include <limits>
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

  void printIntroduction() {
    std::cout
        << "<<Ним>>\nПравила игры:\n- На столе распологается 12 фишек.\n- "
           "Игроки по очереди забирают одну или несколько фишек из любого "
           "ряда.\n- Не разрешается за один ход брать фишки из нескольких "
           "рядов.\n- Выигрывает тот, кто возьмёт последнюю фишку (фишки).\n";
  }

  void printState() {
    std::cout << "\n";
    for (int i{0}; i < state.size(); ++i) {
      std::cout << i + 1 << ") ";
      for (int j{0}; j < state[i]; j++) {
        std::cout << '#';
      }
      std::cout << "\n";
    }
  }

  Move getMove() {
    Move move{};
    switch (currentPlayer) {
    case Player::User:
      std::cout << "Введите ход вида \"строка кол-во\": ";
      std::cin >> move.row >> move.amount;
      if (std::cin.fail())
        throw std::runtime_error{"Недействительные входные данные."};
      --move.row;
      if (move.row >= state.size() || move.amount > state[move.row] ||
          move.row < 0 || move.amount < 1)
        throw std::runtime_error{"Неверное значение ряда или количества."};
      break;
    case Player::Bot:
      std::cout << "Ход компьютера: ";
      move = findOptimalMove();
      std::cout << move.row + 1 << " " << move.amount << "\n";
      break;
    }
    std::swap(currentPlayer, awaitingPlayer);
    return move;
  }

  void makeMove(Move move) { state[move.row] -= move.amount; }

  Move findOptimalMove() {
    Move optimalMove;
    if (calculateNimSum(state) == 0) {
      for (int j{0}; j < state.size(); ++j) {
        if (state[j] != 0)
          optimalMove = {j, 1};
      }
    } else {
      for (int j{0}; j < state.size(); ++j) {
        std::vector<int> potentialMove{state};
        potentialMove[j] = 0;
        int newValue{calculateNimSum(potentialMove)};
        if (newValue < state[j])
          optimalMove = {j, state[j] - newValue};
      }
    }
    return optimalMove;
  }

  bool gameEnded() { return std::reduce(state.begin(), state.end()) == 0; }

public:
  Nim() = default;
  Nim(Player startingPlayer, Player secondPlayer)
      : currentPlayer{startingPlayer} {};

  Player play() {
    printIntroduction();
    while (!gameEnded()) {
      printState();
      try {
        makeMove(getMove());
      } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
    return awaitingPlayer;
  }
};

int main(int argc, char const *argv[]) {
  Nim game{};
  Player winner{game.play()};
  switch (winner) {
  case Player::User:
    std::cout << "Победил игрок.";
    break;
  case Player::Bot:
    std::cout << "Победил компьютер.";
    break;
  }
  return 0;
}
