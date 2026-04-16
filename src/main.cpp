#include "voyc/token.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: 'voyc [file.voy]'" << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << argv[1] << std::endl;
    return 1;
  }

  std::string src((std::istreambuf_iterator<char>(file))
  , std::istreambuf_iterator<char>());
  std::cout << src;
  
  return 0;
}