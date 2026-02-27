#include <SFML/Graphics.hpp>
#include <fstream>
int main() {
  constexpr float WIDTH = 320, HEIGHT = 240;
  std::ofstream logFile("error.log");
  sf::err().rdbuf(logFile.rdbuf());
}