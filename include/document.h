#ifndef DOCUMENT
#define DOCUMENT

#include "component.h"
#include <SDL_image.h>
#include "font.h"

// ========================= LINE ========================= //

class Line
{
public:

  Line();
  Line(Line &&) = default;
  Line(const Line &) = default;
  Line &operator=(Line &&) = default;
  Line &operator=(const Line &) = default;
  ~Line();

public:

// ----- GET ----- //
  size_t size() const;
  size_t capacity() const;
// ----- ACTIONS ----- //
  void insert(const char *, const size_t, const size_t);
  void input(const char *, const size_t);
  void remove(const size_t, const size_t);
  bool search(const char *, const size_t, size_t &) const;

private:

// ----- MEMORY ----- //
  void memory_reallocation();
// ----- DEBUG ----- //
  void debug();

private:

  size_t _capacity;
  size_t _size;
  char *_buffer;

};

// ========================= CURSOR ========================= //

class Cursor
{
public:
  
  Cursor();
  Cursor(Cursor &&) = default;
  Cursor(const Cursor &) = default;
  Cursor &operator=(Cursor &&) = default;
  Cursor &operator=(const Cursor &) = default;
  ~Cursor();

public:

  size_t index() const;
  size_t line() const;

  void index(const size_t);
  void line(const size_t);

  SDL_Rect *body();

private:

  SDL_Rect _body;
  size_t _index;
  size_t _line;

};

// ========================= DOCUMENT ========================= //

class Document : public Component
{
public:

  Document(SDL_Renderer *);
  Document(Document &&) = default;
  Document(const Document &) = default;
  Document &operator=(Document &&) = default;
  Document &operator=(const Document &) = default;
  ~Document();

private:

  void RENDER(SDL_Renderer *renderer) override;
  void UPDATE(SDL_Event *event) override;
  void UPDATE(const float delta_time) override;

private:

// ----- MEMORY ----- //
  void memory_reallocation();
  void memory_delete();
// ----- ACTIONS ----- //
  void input(const char *, const size_t);
  void remove(const size_t, const size_t);
  void search(const char*, const size_t);

private:

// ----- FONT ----- //
  Font *font;

// ----- CURSOR ----- //
  Cursor* cursor;

// ----- DATA ----- //
  size_t capacity;
  size_t size;
  Line **data;

};

#endif // DOCUMENT
