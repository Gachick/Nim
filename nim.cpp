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
    Move mv{};
    switch (currentPlayer) {
    case Player::User:
      std::cout << "Введите ход вида \"строка кол-во\": ";
      std::cin >> mv.row >> mv.amount;
      if (std::cin.fail())
        throw std::runtime_error{"Недействительные входные данные."};
      --mv.row;
      if (mv.row >= state.size() || mv.amount > state[mv.row] || mv.row < 0 ||
          mv.amount < 1)
        throw std::runtime_error{"Неверное значение ряда или количества."};
      break;
    case Player::Bot:
      std::cout << "Ход компьютера: ";
      mv = findOptimalMove();
      std::cout << mv.row + 1 << " " << mv.amount << "\n";
      break;
    }
    std::swap(currentPlayer, awaitingPlayer);
    return mv;
  }

  void move(Move mv) { state[mv.row] -= mv.amount; }

  Move findOptimalMove() {
    Move mv;
    if (calculateNimSum(state) == 0) {
      for (int j{0}; j < state.size(); ++j) {
        if (state[j] != 0)
          mv = {j, 1};
      }
    } else {
      for (int j{0}; j < state.size(); ++j) {
        std::vector<int> potentialMove{state};
        potentialMove[j] = 0;
        int newValue{calculateNimSum(potentialMove)};
        if (newValue < state[j])
          mv = {j, state[j] - newValue};
      }
    }
    return mv;
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
        move(getMove());
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
