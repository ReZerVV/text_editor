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

// ----- DEBUG ----- //
  printf("[INFO]: Line::memory_reallocation() | size[%d] capacity[%d].\n", this->_size, this->_capacity);
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

void Line::remove(const size_t index, const size_t length)
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
  }

// ----- DEBUG ----- //
  debug();
// ----- DEBUG ----- //
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

// ========================= DOCUMENT ========================= //

#define DATA_CAPACITY 2

Document::Document(SDL_Renderer *renderer) : Component(),
// ----- FONT ----- //
  font(new Font {renderer}),
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
  
// ----- CURSOR -----//
// ...

// ----- DATA -----//
// ...
}

Document::~Document()
{
  printf("[INFO]: Document::~Document().\n");
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
  printf("[INFO]: Document::memory_reallocation() | size[%d] capacity[%d].\n", this->size, this->capacity);
// ----- DEBUG ----- //

}

void Document::memory_delete()
{
  for (size_t index = 0; index < capacity; ++index) {
    delete this->data[index];
  }
}

void Document::RENDER(SDL_Renderer* renderer) {}

void Document::UPDATE(SDL_Event* event)
{
  switch (event->type) {
  
  case SDL_TEXTINPUT:
    input(event->text.text, 1);
  break;

  case SDL_KEYDOWN:
    switch (event->key.keysym.sym) {

    case SDLK_LEFT:
    break;

    case SDLK_RIGHT:
    break;

    case SDLK_BACKSPACE:
      remove(this->data[this->cursor->line()]->size(), 1);
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

void Document::input(const char* buffer, const size_t length)
{
  if (this->cursor->line() < size || size == 0) {
    if (size < capacity) {
      this->data[size] = new Line {};
      this->size += 1;
    }
  }

  if (this->cursor->index() < this->data[this->cursor->line()]->size()) {
    this->data[this->cursor->line()]->insert(buffer, length, this->cursor->index());
  } else {
    this->data[this->cursor->line()]->input(buffer, length);
  }
}

void Document::remove(const size_t index, const size_t length)
{
  if (this->cursor->line() == 0) {
    this->data[this->cursor->line()]->remove(index, length);
  } else if (this->data[this->cursor->line()]->size() != 0) {
    this->cursor->line(this->cursor->line() - 1);
    remove(index, length);
  }
}

void Document::search(const char* needle, const size_t length)
{
  for (size_t line = 0; line < size; ++line) {
    size_t index = 0;
    if (this->data[this->cursor->line()]->search(needle, length, index)) {
      printf("[INFO]: SEARCH --> line[%d] index[%d]\n", this->cursor->line(), index);
    }
  }
}

