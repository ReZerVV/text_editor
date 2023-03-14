#include "application.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "log.h"
#include "editor.h"

class TextEditor : public Application
{
public:
  TextEditor() : Application(0,0) 
  {
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
      ERROR(IMG_GetError());
      exit(1);
    }
    if (TTF_Init() != 0) {
      ERROR(TTF_GetError());
      exit(1);
    }

    Create(new Editor(window, renderer));
  }
  ~TextEditor() 
  {
    IMG_Quit();
    TTF_Quit();
  }
};

int main(int argc, char** argv)
{
  TextEditor app;
  app.Start();
  return 0;
}
