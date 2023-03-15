#include "document.h"
#include "log.h"

// ========================= LINE ========================= //

#define BUF_CAPACITY 2

Line::Line()
  :
  _capacity(BUF_CAPACITY), // 1KB - capacity buffer.
  _size(0),
  _buffer(new char[_capacity])
{

}

Line::~Line()
{
  delete this->_buffer;
}

void Line::memory_reallocation()
{
  this->_capacity *= 2;
  char* new_buffer = new char[_capacity];
  memcpy(
    new_buffer,
    this->_buffer,
    this->_size
      );
  delete this->_buffer;
  this->_buffer = new_buffer;
}

void Line::debug()
{
  printf("[DUBUG]:\n");
  std::cout << "BUFFER --> [";
  for (size_t index = 0; index < _size; ++index) {
    std::cout << this->_buffer[index];
  }
  std::cout << ']' << std::endl;
  printf("SIZE --> [%d] | CAPACITY --> [%d].\n", this->_size, this->_capacity);
}


size_t Line::size() const
{
  return this->_size;
}

size_t Line::capacity() const
{
  return this->_capacity;
}

char Line::at(const size_t index) const
{
  if (0 <= index && index < this->_size) {
    return this->_buffer[index];
  }
  return '\0';
}


bool Line::insert(const char *buffer, const size_t length, const size_t index)
{
  if (_size + length < _capacity) {
    memmove(
      &this->_buffer[index + length],
      &this->_buffer[index],
      this->_size - index
        );
    memcpy(
      &this->_buffer[index],
      buffer,
      length
        );
    this->_size += length;

// ----- DEBUG ----- //
    debug();
    return true;
  } else {
    memory_reallocation();
    return insert(buffer, length, index);
  }

// ----- DEBUG ----- //
  debug();
  return false;
}

bool Line::input(const char *buffer, const size_t length)
{
  if (_size + length < _capacity) {
    memcpy(
      &this->_buffer[_size],
      buffer,
      length
          );
    this->_size += length;
// ----- DEBUG ----- //
    debug();
    return true;
  } else {
    memory_reallocation();
    return input(buffer, length);
  }

// ----- DEBUG ----- //
  debug();
  return false;
}

bool Line::remove(const size_t index, const size_t length)
{
  if (this->_size != 0) {
    if (index < this->_size) {
      memmove(
        &this->_buffer[index],
        &this->_buffer[index + length],
        this->_size - index
        );
    }
    this->_size -= length; 
// ----- DEBUG ----- //
    debug();
    return true;
  }
  
// ----- DEBUG ----- //
  debug();
  return false;
}

bool Line::search(const char* sub_buffer, const size_t length, size_t &result) const
{
  for (size_t index = 0; index < this->_size; ++index) {
    for (size_t sub_index = 0; sub_index < length; ++sub_index) {
      if (this->_buffer[index + sub_index] != sub_buffer[sub_index]) {
        break;
      }
      if (sub_index == length - 1) {
        result = index;
        return true;
      }
    }
  }
  return false;
}

// ========================= CURSOR ========================= //

Cursor::Cursor()
  :
  _index(0),
  _line(0)
{
}

Cursor::~Cursor()
{
}

size_t Cursor::index() const
{
  return this->_index;
}

size_t Cursor::line() const
{
  return this->_line;
}

SDL_Rect *Cursor::body()
{
  return &this->_body;
}

void Cursor::index(const size_t index)
{
  this->_index = index;
}

void Cursor::line(const size_t line)
{
  this->_line = line;
}

void Cursor::body(SDL_Rect rect)
{
  this->_body = rect;
}

void Cursor::up(const size_t step)
{
  this->_line += step;}

void Cursor::down(const size_t step)
{
  this->_line -= step;}

void Cursor::right(const size_t step)
{
  this->_index += step;
}

void Cursor::left(const size_t step)
{
  this->_index -= step;
}

// ========================= RENDERER ========================= //

Renderer::Renderer(SDL_Renderer *renderer)
  :
  _renderer(renderer)
{
}

Renderer::~Renderer()
{
}

void Renderer::data(Line **data, const size_t size, Font *font, const int start_x, const int start_y)
{
  int x = start_x;
  int y = start_y;
  for (size_t line = 0; line < size; ++line) {
    for (size_t index = 0; index < data[line]->size(); ++index) {
      SDL_Rect dest = {
        .x = x,
        .y = y,
        .w = font->width(),
        .h = font->height()
      };
      SDL_RenderCopy(this->_renderer, font->font_sheet, font->get(data[line]->at(index)), &dest);

      x += font->width();
    }
    y += font->height();
    x = start_x;
  }
}

void Renderer::text(const char *buffer, const size_t length, Font *font, const int start_x, const int start_y)
{
  int x = start_x;
  int y = start_y;
  for (size_t index = 0; index < length; ++index) {
    if (buffer[index] == '\n') {
      y += font->height();
      x = start_x;
      continue;
    }

    SDL_Rect dest = {
      .x = x,
      .y = y,
      .w = font->width(),
      .h = font->height(),
    };

    SDL_RenderCopy(this->_renderer, font->font_sheet, font->get(buffer[index]), &dest);

    x += font->width();
  }
}

void Renderer::rect(SDL_Rect *rect, Uint32 color)
{
  SDL_SetRenderDrawColor(this->_renderer,
    (color >> (8 * 0)) & 0xff,
    (color >> (8 * 1)) & 0xff,
    (color >> (8 * 2)) & 0xff,
    (color >> (8 * 3)) & 0xff
    );
  SDL_RenderDrawRect(this->_renderer, rect);
}

void Renderer::rect(int x, int y, int w, int h, Uint32 color)
{
  SDL_Rect rect = {
    .x = x,
    .y = y,
    .w = w,
    .h = h,
  };
  SDL_SetRenderDrawColor(this->_renderer,
    (color >> (8 * 3)) & 0xff,
    (color >> (8 * 2)) & 0xff,
    (color >> (8 * 1)) & 0xff,
    (color >> (8 * 0)) & 0xff
    );
  SDL_RenderDrawRect(this->_renderer, &rect);
}

void Renderer::frect(SDL_Rect *rect, Uint32 color)
{
  SDL_SetRenderDrawColor(this->_renderer,
    (color >> (8 * 3)) & 0xff,
    (color >> (8 * 2)) & 0xff,
    (color >> (8 * 1)) & 0xff,
    (color >> (8 * 0)) & 0xff
    );
  SDL_RenderFillRect(this->_renderer, rect);
}

void Renderer::frect(int x, int y, int w, int h, Uint32 color)
{
  SDL_Rect rect = {
    .x = x,
    .y = y,
    .w = w,
    .h = h,
  };
  SDL_SetRenderDrawColor(this->_renderer,
    (color >> (8 * 0)) & 0xff,
    (color >> (8 * 1)) & 0xff,
    (color >> (8 * 2)) & 0xff,
    (color >> (8 * 3)) & 0xff
    );
  SDL_RenderFillRect(this->_renderer, &rect);
}


// ========================= DOCUMENT ========================= //

#define DATA_CAPACITY 2

Document::Document(SDL_Renderer *renderer) : Component(),
// ----- FONT ----- //
  font(new Font {renderer}),
// ----- RENDER ----- //
  render(new Renderer {renderer}),
// ----- CURSOR ----- //
  cursor(new Cursor {}),
// ----- DATA ----- //
  capacity(DATA_CAPACITY),
  size(0),
  data(new Line*[capacity])
{
// ----- FONT ----- //
  this->font->load_from_file("../asset/charmap.png");
  this->font->scale = 3;
  this->font->color(0xFFFFFFFF);
// ----- DATA ----- //
  create_line();
}

Document::~Document()
{
  delete this->font;
  delete this->cursor;
  delete this->render;
  memory_delete();
}

void Document::memory_reallocation()
{
  this->info_status = "realloc.";

  this->capacity *= 2;
  Line **new_data = new Line*[capacity];
  for (size_t index = 0; index < capacity; ++index) {
    if (this->data[index] != nullptr) {
      new_data[index] = this->data[index];
    }
  }
  delete this->data;
  this->data = new_data;
}

void Document::memory_delete()
{
  for (size_t index = 0; index < capacity; ++index) {
    delete this->data[index];
  }
  delete this->data;
}

void Document::RENDER(SDL_Renderer* renderer)
{
// ----- LOGO ----- //
  this->render->text("Text\n  Editor", 13, this->font, 10,10);
// ----- DATA ----- //
  this->render->data(this->data, this->size, this->font, 100, 100);
// ----- NUM LINE ----- //
  for (size_t line = 0; line < this->size; ++line) {
    this->render->text( "*", 1, this->font, 60, 100 + (line * this->font->width() + 4));
  }
// ----- CURSOR ----- //
  this->render->rect(
    this->cursor->index() * this->font->width() + 100,
    this->cursor->line() * this->font->height() + 100,
    this->font->width(),
    this->font->height(),
    0xFFFF0000
    );

  render_info(200, 10);
}

void Document::render_info(const int start_x, const int start_y)
{
  this->font->scale = 1;

  this->font->color(0xfb8500ff);
  this->render->text(
    this->info_title.c_str(),
    this->info_title.size(),
    this->font,
    start_x,
    start_y
    );
 
  this->font->color(0x8ecae6ff);
  this->render->text(
    this->info_status.c_str(),
    this->info_status.size(),
    this->font,
    start_x + 50,
    start_y
    );

  this->font->color(0xffb703ff);
  this->render->text(
    this->info_data.c_str(),
    this->info_data.size(),
    this->font,
    start_x + 10,
    start_y + 20
    );

  this->render->text(
    this->info_cursor.c_str(),
    this->info_cursor.size(),
    this->font,
    start_x + 10,
    start_y + 40
    );

  this->render->rect(start_x, start_y + 10, 250, 50, 0x023047ff);
  this->font->scale = 3;
  this->font->color(0xffffffff); 
}

void Document::UPDATE(SDL_Event* event)
{
  this->info_status.clear();

  switch (event->type) {
  
  case SDL_TEXTINPUT:
    this->info_status = "text input.";
    
    input(event->text.text, 1);
  break;

  case SDL_KEYDOWN:
  switch (event->key.keysym.sym) {
// ----- MOVE CURSOR ----- //
    case SDLK_UP:
      up(1);
    break;

    case SDLK_DOWN:
      down(1);
    break;

    case SDLK_LEFT:
      left(1);
    break;

    case SDLK_RIGHT:
      right(1);
    break;
// ----- ACTIONS ----- //
    case SDLK_BACKSPACE:
      remove(this->cursor->index(), 1);
    break;

    case SDLK_RETURN:
      create_line();
    break;
    }
  break;

  case SDL_KEYUP:
  break;
  }
}

void Document::UPDATE(const float delta_time)
{
  this->info_title = "[INFO]";
  if (this->info_status.size() == 0) {
    this->info_status = "none.";
  }
  this->info_data = "DATA size -> " + std::to_string(this->size) +
    " capacity -> " + std::to_string(this->capacity) + '\n' +
    "  LINE size -> " + std::to_string(this->data[this->cursor->line()]->size()) +
    " capacity -> " + std::to_string(this->data[this->cursor->line()]->capacity());
  this->info_cursor = "CURSOR line -> " + std::to_string(this->cursor->line()) +
    " index -> " + std::to_string(this->cursor->index());
}

void Document::input(const char* buffer, const size_t length)
{
  this->info_status = "input.";

  if (this->cursor->index() < this->data[this->cursor->line()]->size()) {
    if (this->data[this->cursor->line()]->insert(buffer, length, this->cursor->index())) {
      this->cursor->index(this->cursor->index() + 1);
    }
  } else {
    if (this->data[this->cursor->line()]->input(buffer, length)) {
      this->cursor->index(this->cursor->index() + 1);
    }
  }
}

void Document::remove(const size_t index, const size_t length)
{
  this->info_status = "remove.";

  if (this->data[this->cursor->line()]->remove(index, length)) {
    this->cursor->index(this->cursor->index() - 1);
  } else if (this->cursor->line() != 0) {
    if (this->data[this->cursor->line()]->size() == 0) {
      delete this->data[this->cursor->line()];
      this->size -= 1;
      this->cursor->line(this->cursor->line() - 1);
      this->cursor->index(this->data[this->cursor->line()]->size());
    } else {
      this->cursor->line(this->cursor->line() - 1);
      this->cursor->index(this->data[this->cursor->line()]->size());
      remove(index, length);
    }
  }
}

void Document::search(const char* needle, const size_t length)
{
  this->info_status = "search.";
 
  for (size_t line = 0; line < size; ++line) {
    size_t index = 0;
    if (this->data[this->cursor->line()]->search(needle, length, index)) {
      printf("[INFO]: SEARCH --> line[%d] index[%d]\n", this->cursor->line(), index);
    }
  }
}

void Document::create_line()
{
  this->info_status = "create line.";

  if (this->size < this->capacity) {
    this->data[size] = new Line {};
    this->size += 1;
    this->cursor->line(this->size - 1);
    this->cursor->index(0);
  } else {
    memory_reallocation();
    create_line();
  }
}

void Document::up(const size_t step)
{
  this->info_status = "move up.";

  if (this->cursor->line() - step >= 0) {
    this->cursor->down(step);
    if (this->cursor->index() > this->data[this->cursor->line()]->size()) {
      this->cursor->index(this->data[this->cursor->line()]->size());
    }
  }
}

void Document::down(const size_t step)
{
  this->info_status = "move down.";

  if (this->cursor->line() + step < this->size) {
    this->cursor->up(step);
    if (this->cursor->index() > this->data[this->cursor->line()]->size()) {
      this->cursor->index(this->data[this->cursor->line()]->size());
    }
  }
}

void Document::left(const size_t step)
{
  this->info_status = "move left.";

  if (this->cursor->index() != 0) {
    this->cursor->left(1);
  } else if (this->cursor->line() != 0) {
    this->cursor->down(1);
    this->cursor->index(this->data[this->cursor->line()]->size());
  }
}

void Document::right(const size_t step)
{
  this->info_status = "move right.";

  if (this->cursor->index() + step < this->data[this->cursor->line()]->size()) {
    this->cursor->right(step);
  } else if (this->cursor->line() + step < this->size) {
    this->cursor->up(step);
    this->cursor->index(0);
  }
}
