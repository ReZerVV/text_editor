#ifndef FONT
#define FONT

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "log.h"

class Font
{
public:

  Font(SDL_Renderer*);
  Font(Font &&) = default;
  Font(const Font &) = default;
  Font &operator=(Font &&) = default;
  Font &operator=(const Font &) = default;
  ~Font();

public:

  bool load_from_file(const char*);
// ----- GET ----- //
  SDL_Rect* get(const char) const;
  size_t width() const;
  size_t height() const;
// ----- SET ----- //
  void color(Uint32);

public:

  SDL_Texture*  font_sheet;
  float scale;


private:

  /*
   * 
   */
  int font_sheet_cols;
  int font_sheet_rows;
  int font_sheet_width;
  int font_sheet_height;

  /*
   *
   */
  const int ASCII_LOW = 32;
  const int ASCII_HIGH = 126;

  SDL_Renderer* renderer;
  SDL_Rect*     atlas;

  int _width;
  int _height;
  Uint32 _color;

};

#endif // FONT
