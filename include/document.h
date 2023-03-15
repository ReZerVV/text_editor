#ifndef DOCUMENT
#define DOCUMENT

#include <SDL_image.h>
#include "component.h"
#include "font.h"
#include "render.h"
#include <string>


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
  char at(const size_t) const;
// ----- ACTIONS ----- //
  bool insert(const char *, const size_t, const size_t);
  bool input(const char *, const size_t);
  bool remove(const size_t, const size_t);
  bool search(const char *, const size_t, size_t &) const;
// ----- DEBUG ----- //
  void debug();

private:

// ----- MEMORY ----- //
  void memory_reallocation();

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

// ----- GET ----- //
  size_t index() const;
  size_t line() const;
  SDL_Rect *body();
// ----- SET ----- //
  void index(const size_t);
  void line(const size_t);
  void body(SDL_Rect);
// ----- ACTIONS ----- //
  void up(const size_t);
  void down(const size_t);
  void right(const size_t);
  void left(const size_t);

private:

  SDL_Rect _body;
  size_t _index;
  size_t _line;

};


// ========================= RENDER ========================= //

class Renderer
{
public:
  
  Renderer(SDL_Renderer *);
  Renderer(Renderer &&) = default;
  Renderer(const Renderer &) = default;
  Renderer &operator=(Renderer &&) = default;
  Renderer &operator=(const Renderer &) = default;
  ~Renderer();

public:

  void data(Line **, const size_t, Font *, const int, const int);
  void text(const char *, const size_t, Font *, const int, const int);
  void rect(SDL_Rect *, Uint32);
  void rect(int, int, int, int, Uint32);
  void frect(SDL_Rect *, Uint32); 
  void frect(int, int, int, int, Uint32);

private:
 
  SDL_Renderer *_renderer;

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

  void RENDER(SDL_Renderer *) override;
  void UPDATE(SDL_Event *) override;
  void UPDATE(const float) override;

private:

// ----- MEMORY ----- //
  void memory_reallocation();
  void memory_delete();
// ----- ACTIONS ----- //
  void input(const char *, const size_t);
  void remove(const size_t, const size_t);
  void search(const char*, const size_t);
  void create_line();
  void up(const size_t);
  void down(const size_t);
  void left(const size_t);
  void right(const size_t);

// ----- DEBUG ----- //
  void render_info(const int, const int);

private:

// ----- FONT ----- //
  Font *font;

// ----- CURSOR ----- //
  Cursor *cursor;

// ----- RENDER ----- //
  Renderer *render;
  
// ----- DATA ----- //
  size_t capacity;
  size_t size;
  Line **data;

// ----- DEBUG ----- //
  std::string info_title;
  std::string info_cursor;
  std::string info_data;
  std::string info_status;

};

#endif // DOCUMENT
