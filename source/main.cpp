#include "application.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "log.h"

class Cursor : public Component
{
private:
  SDL_Rect position;
  unsigned int offset;
public:
  Cursor(const int font_size) 
    :
    offset(font_size)
  {
    position.x = 10;
    position.y = 10;

    position.w = font_size;
    position.h = font_size + font_size / 2;
  }
  ~Cursor() 
  {

  }
public:
  void RENDER(SDL_Renderer* renderer) override
  {
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderDrawRect(renderer, &position);
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
  }
  void UPDATE(const float) override
  {

  }
  void UPDATE(SDL_Event* event) override 
  {
    switch (event->type) {
    case SDL_KEYDOWN:
      switch (event->key.keysym.sym) {
      case SDLK_LEFT: move_left(); break;
      case SDLK_RIGHT: move_right(); break;
      }
      break;
    case SDL_KEYUP: break;
    }
  }
private:
  void move_left()
  {
    position.x -= offset;
  }
  void move_right()
  {
    position.x += offset;
  }
};

class Document : public State
{
public:
  Document() : State() 
  {
    Create(new Cursor(10));
  }
  ~Document() 
  {

  }
};

class TextEditor : public Application
{
public:
  TextEditor() : Application(100,100) 
  {
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
      ERROR("sdl_image init.");
      exit(1);
    }
    if (TTF_Init() != 0) {
      ERROR("sdl_ttf init.");
      exit(1);
    }
    Create(new Document());
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
