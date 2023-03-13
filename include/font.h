#ifndef FONT
#define FONT

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "log.h"

class Font 
{
public:

  SDL_Texture*  font_sheet;

  int width;
  int height;
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

public:

  Font(SDL_Renderer*);
  ~Font();

public:

  /*
   *
   */
  bool load_from_file(const char*);

  /*
   *
   */
  SDL_Rect* get(const char) const;
  
};

#endif // FONT
