#include "document.h"
#include "log.h"


// ========================= LINE ========================= //

#define KB 1024

Line::Line()
  :
  _capacity(KB), // 1KB - capacity buffer.
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

// ----- DEBUG ----- //
  printf("[DEBUG]: Line::memory_reallocation() | size[%d] capacity[%d].\n", this->_size, this->_capacity);
// ----- DEBUG ----- //

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

void Line::insert(const char *buffer, const size_t length, const size_t index)
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
  } else {
    memory_reallocation();
    insert(buffer, length, index);
  }

// ----- DEBUG ----- //
  debug();
// ----- DEBUG ----- //
}

void Line::input(const char *buffer, const size_t length)
{
  if (_size + length < _capacity) {
    memcpy(
      &this->_buffer[_size],
      buffer,
      length
          );
    this->_size += length;
  } else {
    memory_reallocation();
    input(buffer, length);
  }

// ----- DEBUG ----- //
  debug();
// ----- DEBUG ----- //
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

// ========================= DOCUMENT ========================= //

#define CAPACITY 1

Document::Document(SDL_Renderer *renderer) : Component(),
// ----- FONT ----- //
  font(new Font {renderer}),
// ----- CURSOR ----- //
  cursor(new Cursor {}),
// ----- DATA ----- //
  capacity(CAPACITY),
  size(0),
  data(new Line*[capacity])
{
// ----- FONT ----- //
  this->font->load_from_file("../asset/charmap.png");
  this->font->scale = 3;
  
// ----- CURSOR -----//
// ...

// ----- DATA -----//
// ...
}

Document::~Document()
{
  delete this->font;
  delete this->cursor;
  memory_delete();
}

void Document::memory_reallocation()
{
  this->capacity *= 2;
  Line **new_data = new Line*[capacity];
  memcpy(
    new_data,
    this->data,
    size
      );
  memory_delete();

// ----- DEBUG ----- //
  printf("[DEBUG]: Document::memory_reallocation() | size[%d] capacity[%d].\n", this->size, this->capacity);
// ----- DEBUG ----- //

}

void Document::memory_delete()
{
  for (size_t index = 0; index < size; ++index) {
    delete this->data[index];
  }
}

void Document::RENDER(SDL_Renderer* renderer) {}

void Document::UPDATE(SDL_Event* event)
{
  switch (event->type) {
  
  case SDL_TEXTINPUT:
    data_input(event->text.text, 1);
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

void Document::data_input(const char* buffer, const size_t length)
{  if (this->cursor->line() < size || size == 0) {
    this->data[size] = new Line {};
    this->size += 1;
  }
  if (this->cursor->index() < this->data[this->cursor->line()]->size()) {
    this->data[this->cursor->line()]->insert(buffer, length, this->cursor->index());
  } else {
    this->data[this->cursor->line()]->input(buffer, length);
  }
}
