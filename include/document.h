#ifndef DOCUMENT
#define DOCUMENT

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
    size_t index;
    size_t line;
    SDL_Rect body;
  };
  
  /*
   *
   */
  struct Line
  { 
    size_t capacity;
    size_t size;
    char* buffer;

    Line();
    ~Line();

    char& operator[](const size_t);
  };

private:

  /*
   * FONT
   */
  Font* font;
  
  /*
   * CURSOR
   */
  Cursor cursor;

  /*
   * LINE
   */
  size_t capacity;
  size_t size;
  Line* data;

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
  
  void cursor_move_left(size_t);
  void cursor_move_right(size_t);
  void cursor_move_up(const size_t);
  void cursor_move_down(const size_t);
  
  void new_line();
  void input(const char, const size_t);
  void remove(const size_t);
};

#endif // DOCUMENT
