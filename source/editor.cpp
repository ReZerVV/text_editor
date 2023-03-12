#include "editor.h"
#include <cmath>
#include "log.h"

Document::Document(SDL_Renderer* renderer) : Component(),
  font_cols(18), font_rows(7),
  scale(2),
  capacity(1024), size(0),
  buffer(new char[capacity])
{
  SDL_Surface* font_surface = IMG_Load("../asset/charmap.png");
  if (font_surface != NULL)
  {
    font_width = font_surface->w;
    font_height = font_surface->h;
    character_width = font_width / font_cols;
    character_height = font_height / font_rows;
    font_texturesheet = SDL_CreateTextureFromSurface(renderer, font_surface);
    SDL_FreeSurface(font_surface);
  }
  else 
  {
    LOG("load bitmap.");
    exit(1);
  }
  
  cursor.body.w = character_width * scale + 2;
  cursor.body.h = character_height * scale + 4;
}
Document::~Document() 
{
  delete buffer;
  SDL_DestroyTexture(font_texturesheet);
}


void Document::RENDER(SDL_Renderer* renderer)
{
  render_buffer(renderer, 0,0, 0xFFFFFFFF);
  render_cursor(renderer, 0,0, 0xFFFFFFFF);
}


void Document::UPDATE(SDL_Event* event)
{
  switch (event->type) {
  case SDL_TEXTINPUT:
    if ((size + 1) < capacity) {
      buffer[size] = *event->text.text;
      ++size;
      if ((cursor.body.x + character_width * scale) > size * character_width * scale)
        cursor.body.x = size * character_width * scale;
      else
        cursor.body.x += character_width * scale;
      }
    break;
  case SDL_KEYDOWN: 
    switch (event->key.keysym.sym) {
      case SDLK_LEFT:
        if ((cursor.body.x - character_width * scale) < 0)
          cursor.body.x = 0;
        else
          cursor.body.x -= character_width * scale; 
        break;
      case SDLK_RIGHT:
        if ((cursor.body.x + character_width * scale) > size * character_width * scale)
          cursor.body.x = size * character_width * scale;
        else
          cursor.body.x += character_width * scale;
        break;
      case SDLK_BACKSPACE:
        if (size > 0) {
          --size;
        if ((cursor.body.x - character_width * scale) < 0)
          cursor.body.x = 0;
        else
          cursor.body.x -= character_width * scale;
        }
        break;
    }
    break;
  }
}

void Document::UPDATE(const float delta_time) {}

void Document::render_buffer(SDL_Renderer* renderer, int x, int y, Uint32 color)
{
  for (size_t index = 0; index < size; ++index) {
    render_char(renderer, buffer[index], x, y, color);
    x += character_width * scale;
  }
}

void Document::render_char(SDL_Renderer* renderer, char character, int x, int y, Uint32 color)
{
  const size_t index = static_cast<int>(character) - 32;
  const size_t texture_x = index % font_cols;
  const size_t texture_y = index / font_cols;
  
  const SDL_Rect src = {
    .x = texture_x * character_width,
    .y = texture_y * character_height,
    .w = character_width,
    .h = character_height
  };
  const SDL_Rect dest = {
    .x = x,
    .y = y,
    .w = static_cast<int>(character_width * scale),
    .h = static_cast<int>(character_height * scale)
  };

  SDL_SetTextureColorMod(font_texturesheet, 
    (color >> (8 * 2)) & 0xFF,
    (color >> (8 * 1)) & 0xFF,
    (color >> (8 * 0)) & 0xFF
  );
  SDL_RenderCopy(renderer, font_texturesheet, &src, &dest);
}

void Document::render_cursor(SDL_Renderer* renderer, int x, int y, Uint32 color)
{
  cursor.body.y = y;

  SDL_SetRenderDrawColor(renderer,
    (color >> (8 * 2)) & 0xFF,
    (color >> (8 * 1)) & 0xFF,
    (color >> (8 * 0)) & 0xFF,
    0x00
  );
  SDL_RenderDrawRect(renderer, &cursor.body);
  SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0xFF);
}

Editor::Editor(SDL_Renderer* renderer) : State()
{
  Create(new Document(renderer));
}
Editor::~Editor() {}
