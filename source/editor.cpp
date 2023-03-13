#include "editor.h"
#include <cmath>
#include "log.h"

Document::Document(SDL_Renderer* renderer) : Component(),
  capacity(1024),
  size(0),
  buffer(new char[capacity]),
  font(new Font(renderer))
{
  this->font->load_from_file("../asset/charmap.png");
  this->font->scale = 5;
  cursor.body.w = font->width * font->scale;
  cursor.body.h = font->height * font->scale;
}
Document::~Document() 
{
  delete font;
  delete buffer;
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
      if ((cursor.body.x + font->width * font->scale) > size * font->width * font->scale)
        cursor.body.x = size * font->width * font->scale;
      else
        cursor.body.x += font->width * font->scale;
    }
  break;

  case SDL_KEYDOWN:
    switch (event->key.keysym.sym) {
    case SDLK_LEFT:
      if ((cursor.body.x - font->width * font->scale) < 0)
        cursor.body.x = 0;
      else
        cursor.body.x -= font->width * font->scale; 
    break;

    case SDLK_RIGHT:
      if ((cursor.body.x + font->width * font->scale) > size * font->width * font->scale)
        cursor.body.x = size * font->width * font->scale;
      else
        cursor.body.x += font->width * font->scale;
    break;

    case SDLK_BACKSPACE:
      if (size > 0) {
        --size;
        if ((cursor.body.x - font->width * font->scale) < 0)
          cursor.body.x = 0;
        else
          cursor.body.x -= font->width * font->scale;
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
    x += font->width * font->scale;
  }
}

void Document::render_char(SDL_Renderer* renderer, char character, int x, int y, Uint32 color)
{
  SDL_Rect dst = {
    .x = x,
    .y = y,
    .w = font->width * font->scale,
    .h = font->height * font->scale
  };
  
  SDL_SetTextureColorMod(font->font_sheet,
    (color >> (8 * 2)) & 0xFF,
    (color >> (8 * 1)) & 0xFF,
    (color >> (8 * 0)) & 0xFF
  );
  SDL_RenderCopy(renderer, font->font_sheet, font->get(character), &dst);
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
