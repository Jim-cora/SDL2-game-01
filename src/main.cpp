#include <iostream>
#include <string>
#include "game_structure.h"
#include <SDL.h>

using namespace std;

int main(int argc, char *argv[])
{
  game& game = game::getInstance();

  game.gameInit();

  std::cout << "Game Initialized... start!" << std::endl;

  game.gameRun();
  
    return 0;
}