#ifndef DOCUMENT
#define DOCUMENT

#include "component.h"
#include "SDL_image.h"
#include "font.h"

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

  void insert(const char *, const size_t, const size_t);
  void input(const char *, const size_t);

private:

  void memory_reallocation();

private:

  size_t capacity;
  size_t size;
  char *buffer;

};

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
  
  void memory_reallocation();
  void memory_delete();

  //void render_buffer(SDL_Renderer*, int, int, Uint32);
  //void render_char(SDL_Renderer*, char, int, int, Uint32);
  //void render_cursor(SDL_Renderer*, int, int, Uint32);

private:

  // Font
  Font *font;

  // Data
  size_t capacity;
  size_t size;
  Line **data;

};

#endif // DOCUMENT
