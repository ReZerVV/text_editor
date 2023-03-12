#ifndef EDITOR
#define EDITOR

#include "state.h"
#include "component.h"
#include "SDL_image.h"


class Document : public Component
{
private:

  /*
   * FONT
   */
  int font_width;
  int font_height;
  unsigned int scale;
  SDL_Texture* font_texturesheet;

public:
  Document(SDL_Renderer*);
  ~Document();
private:
  void RENDER(SDL_Renderer* renderer) override;
  void UPDATE(SDL_Event* event) override;
  void UPDATE(const float delta_time) override;
private:
  SDL_Surface* surface_font_from_image(const char*);
};

class Cursor : public Component
{
private:
  SDL_Rect body;
public:
  Cursor();
  ~Cursor();
private:
  void RENDER(SDL_Renderer* renderer) override;
  void UPDATE(SDL_Event* event) override;
  void UPDATE(const float delta_time) override;
private:
};

class Editor : public State
{
public:
  Editor(SDL_Renderer*);
  ~Editor();
};



#endif // EDITOR
