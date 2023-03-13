#include "document.h"
#include "log.h"


// ========================= LINE ========================= //

#define KB 1024
Line::Line()
  :
  capacity(KB), // 1KB - capacity
  size(0),
  buffer(new char[capacity])
{
  memory_reallocation();
}

Line::~Line()
{
  delete this->buffer;
}

void Line::memory_reallocation()
{
  this->capacity *= 2;
  char* new_buffer = new char[capacity];
  memcpy(
    new_buffer,
    this->buffer,
    size
      );
  delete this->buffer;
}

void Line::insert(const char* buffer, const size_t length, const size_t index)
{
  if (size + length < capacity) {
    memmove(
      &this->buffer[index + length],
      &this->buffer[index],
      size - index
        );
    memcpy(
      &this->buffer[index],
      buffer,
      length
        );
    this->size += length;
  } else {
    memory_reallocation();
    insert(buffer, length, index);
  }
  for (size_t index = 0; index < size; ++index) {
    std::cout << this->buffer[index];
  }
  std::cout << " size = " << this->size << std::endl;
}

void Line::input(const char* buffer, const size_t length)
{
  if (size + length < capacity) {
    memcpy(
      &this->buffer[size],
      buffer,
      length
          );
    this->size += length;
  } else {
    memory_reallocation();
    input(buffer, length);
  }
  for (size_t index = 0; index < size; ++index) {
    std::cout << this->buffer[index];
  }
  std::cout << " size = " << this->size << std::endl;
}

// ========================= DOCUMENT ========================= //

Document::Document(SDL_Renderer* renderer) : Component(),
  font(new Font {renderer}),
  data(new Line {})
{
  this->font->load_from_file("../asset/charmap.png");
  this->font->scale = 3;
}
Document::~Document() 
{
  delete font;

  delete data;
}

void Document::RENDER(SDL_Renderer* renderer)
{
  //render_cursor(renderer, 0,0, 0xFFFFFFFF);
  //render_buffer(renderer, 0,0, 0xFFFFFFFF);
}
/*
void Document::render_buffer(SDL_Renderer* renderer, int x, int y, Uint32 color) {}

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
  SDL_SetTextureAlphaMod(font->font_sheet,(color >> (8 * 3)) & 0xFF);
  SDL_RenderCopy(renderer, font->font_sheet, font->get(character), &dst);
}

void Document::render_cursor(SDL_Renderer* renderer, int x, int y, Uint32 color)
{
  this->cursor.body.x = cursor.index * font->width * font->scale;
  this->cursor.body.y = cursor.line * font->height * font->scale;
  this->cursor.body.w = font->width * font->scale;
  this->cursor.body.h = font->height * font->scale;

  SDL_SetRenderDrawColor(renderer,
    (color >> (8 * 2)) & 0xFF,
    (color >> (8 * 1)) & 0xFF,
    (color >> (8 * 0)) & 0xFF,
    0x00
  );
  SDL_RenderFillRect(renderer, &cursor.body);
  SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0xFF);
}
*/
void Document::UPDATE(SDL_Event* event)
{
  switch (event->type) {
  
  case SDL_TEXTINPUT:
    this->data->input(event->text.text, 1);
  break;

  case SDL_KEYDOWN:
    switch (event->key.keysym.sym) {

    case SDLK_LEFT:
    break;

    case SDLK_RIGHT:
    break;

    case SDLK_BACKSPACE:
    break;

    case SDLK_RETURN:
    break;
    }
  break;

  case SDL_KEYUP:
  break;
  }
}

void Document::UPDATE(const float delta_time) {}
