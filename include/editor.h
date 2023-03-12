#ifndef EDITOR
#define EDITOR

#include "state.h"
#include "component.h"
#include "SDL_image.h"


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
  int font_width;
  int font_height;
  int font_cols;
  int font_rows;
  int character_width;
  int character_height;
  unsigned int scale;
  SDL_Texture* font_texturesheet;
  
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
  SDL_Surface* surface_font_from_image(const char*);

  void render_buffer(SDL_Renderer*, int, int, Uint32);
  void render_char(SDL_Renderer*, char, int, int, Uint32);
  void render_cursor(SDL_Renderer*, int, int, Uint32);
  
  //void cursor_move_left();
  //void cursor_move_right();
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
