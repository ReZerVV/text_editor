#include "application.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

class menu : public State
{
public:
  menu() : State() {}
};

class app : public Application
{
public:
  app() : Application(100,100) {
    Create(new menu());
  }
};

int main(int argc, char** argv)
{
  app te;
  te.Start();
  return 0;
}
