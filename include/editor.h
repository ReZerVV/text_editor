#ifndef EDITOR
#define EDITOR

#include "state.h"
#include "component.h"
#include "SDL_image.h"

#include "font.h"

class Document : public Component
{
private:

  /*
   *
   */
  struct Cursor
  {
    SDL_Rect body;
  };
  
  /*
   *
   */
  class Line
  {

  };

private:

  /*
   * FONT
   */
  Font* font;
  
  /*
   * BUFFER
   */
  size_t capacity;
  size_t size;
  char* buffer;

  /*
   *
   */
  Cursor cursor;

public:
  Document(SDL_Renderer*);
  ~Document();
private:
  void RENDER(SDL_Renderer* renderer) override;
  void UPDATE(SDL_Event* event) override;
  void UPDATE(const float delta_time) override;
private:

  void render_buffer(SDL_Renderer*, int, int, Uint32);
  void render_char(SDL_Renderer*, char, int, int, Uint32);
  void render_cursor(SDL_Renderer*, int, int, Uint32);
  
  //void cursor_move_left();
  //void cursor_move_right();
};

class Editor : public State
{
public:
  Editor(SDL_Renderer*);
  ~Editor();
};



#endif // EDITOR
