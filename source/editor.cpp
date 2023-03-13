#include "editor.h"
#include <cmath>
#include "log.h"

Document::Document(SDL_Renderer* renderer) : Component(),
  capacity(10),
  size(0),
  data(new Line[capacity]),
  font(new Font(renderer))
{
  this->font->load_from_file("../asset/charmap.png");
  this->font->scale = 3;
  this->size = 1;
  this->cursor.index = 0;
  this->cursor.line = 0;
  this->cursor.body.x = 0;
  this->cursor.body.y = 0;
  this->cursor.body.w = font->width * font->scale;
  this->cursor.body.h = font->height * font->scale;
}
Document::~Document() 
{
  delete font;
  delete data;
}


void Document::RENDER(SDL_Renderer* renderer)
{
  render_cursor(renderer, 0,0, 0xFFFFFFFF);
  render_buffer(renderer, 0,0, 0xFFFFFFFF);
}

void Document::UPDATE(SDL_Event* event)
{
  switch (event->type) {
  
  case SDL_TEXTINPUT:
    input(*event->text.text, 1);
  break;

  case SDL_KEYDOWN:
    switch (event->key.keysym.sym) {
        
    case SDLK_LEFT:
      cursor_move_left(0);
    break;

    case SDLK_RIGHT:
      cursor_move_right(0);
    break;

    case SDLK_BACKSPACE:
      remove(1);
    break;

    case SDLK_RETURN:
      new_line();
    break;
    }
  break;

  case SDL_KEYUP:
  break;
  }
}

void Document::UPDATE(const float delta_time) {}

void Document::render_buffer(SDL_Renderer* renderer, int x, int y, Uint32 color)
{
  for (size_t line = 0; line < size; ++line) {
    for (size_t index = 0; index < data[line].size; ++index) {
      if (index == cursor.index && line == cursor.line)
        render_char(renderer, data[line][index], x, y, 0x00000000);
      else
        render_char(renderer, data[line][index], x, y, color);
      x += font->width * font->scale;
    }
    y += font->height * font->scale;
    x = 0;
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
  //SDL_SetTextureAlphaMod(font->font_sheet,(color >> (8 * 3)) & 0xFF);
  SDL_RenderCopy(renderer, font->font_sheet, font->get(character), &dst);
}

/*
 * cursor
 */
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
void Document::cursor_move_left(size_t)
{
  if (cursor.index > 0) {
    cursor.index -= 1;
  }
  else {
    if (cursor.line > 0) {
      cursor.line -= 1;
    }
    cursor.index = data[cursor.line].size;
  }
}

void Document::cursor_move_right(size_t)
{
  if (cursor.index < data[cursor.line].size) {
    cursor.index += 1;
  }
  else {
    if (cursor.line < size) {
      cursor.line += 1;
    }
    cursor.index = 0;
  }
}

/*
 * Line
 */

Document::Line::Line()
  :
  capacity(1024),
  size(0),
  buffer(new char[capacity])
{}

Document::Line::~Line()
{
  delete buffer;
}

char& Document::Line::operator[](const size_t index)
{
  return this->buffer[index];
}

void Document::remove(const size_t length)
{
  printf("[LOG]: index[%d] line[%d]\n", cursor.index, cursor.line);
  if ((data[cursor.line].size - length) < 0 || cursor.index <= 0) {
    if (cursor.line > 0) {
      cursor.line -= 1;
      cursor.index = data[cursor.line].size;
      remove(length);
    }
    else {
      cursor.index = 0;
    }
  }
  else {
    if (cursor.index < data[cursor.line].size) {
      memmove(&data[cursor.line][cursor.index - length],
        &data[cursor.line][cursor.index],
        data[cursor.line].size - cursor.index - length
      );
    }
    this->data[cursor.line].size -= length;
    this->cursor.index -= length;
  }
}

void Document::input(const char character, const size_t length)
{
  if ((data[cursor.line].size + length) < data[cursor.line].capacity) {
    printf("[LOG]: cursor x=%d size=%d\n", cursor.index, data[cursor.line].size);
    if (cursor.index < data[cursor.line].size) {
      memmove(&data[cursor.line][cursor.index + length],
        &data[cursor.line][cursor.index],
        data[cursor.line].size - cursor.index
      );
      this->data[cursor.line][cursor.index] = character;
    }
    else {
      this->data[cursor.line][data[cursor.line].size] = character;
    }
    this->data[cursor.line].size += length;
    this->cursor.index += length;
  }
  else {
    if (cursor.line < size) {
      cursor.line += 1;
      cursor.line = 0;
      input(character, length);
    }
  }
}

void Document::new_line()
{
  const size_t length = 1;
  if (size < (capacity - 1)) {
    if (cursor.line < size) {
      memmove(&data[cursor.line + length],
        &data[cursor.line],
        size - cursor.line
      );
    }
    cursor.line += 1;
    cursor.index = 0;
    size += 1;
  }
  else {
    
  }
}


Editor::Editor(SDL_Renderer* renderer) : State()
{
  Create(new Document(renderer));
}
Editor::~Editor() {}
