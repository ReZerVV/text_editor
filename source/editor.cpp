#include "editor.h"
#include <cmath>
#include "log.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Document::Document(SDL_Renderer* renderer) : Component(),
  font_cols(18), font_rows(7),
  scale(1),
  capacity(1024), size(0),
  buffer(new char[capacity])
{
  //SDL_Surface* font_surface = surface_font_from_image("../asset/charmap.png");
  //font_texturesheet = SDL_CreateTextureFromSurface(renderer, font_surface);
  //SDL_FreeSurface(font_surface);
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
}
Document::~Document() 
{
  delete buffer;
  SDL_DestroyTexture(font_texturesheet);
}

void Document::RENDER(SDL_Renderer* renderer)
{
  render_buffer(renderer, 10,10, 0xFFFFFFFF);
}
void Document::UPDATE(SDL_Event* event)
{
  switch (event->type) {
  case SDL_TEXTINPUT:
    if ((size + 1) < capacity) 
    {
      buffer[size] = *event->text.text;
      ++size;
    }
    break;
  }
}

void Document::UPDATE(const float delta_time) {}

SDL_Surface* Document::surface_font_from_image(const char* file_path)
{
  int n;
  unsigned char* pixels = stbi_load(file_path, &font_width, &font_height, &n, STBI_rgb_alpha);
  if (pixels == NULL) {
    ERROR("Load bitmap font.");
    exit(1);
  }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  const Uint32 rmask = 0xFF000000;
  const Uint32 gmask = 0x00FF0000;
  const Uint32 bmask = 0x0000FF00;
  const Uint32 amask = 0x000000FF;
#else
  const Uint32 rmask = 0x000000FF;
  const Uint32 gmask = 0x0000FF00;
  const Uint32 bmask = 0x00FF0000;
  const Uint32 amask = 0xFF000000;
#endif
  
  const int depth = 32;
  const int pitch = 4 * font_width;

  return static_cast<SDL_Surface*>(SDL_CreateRGBSurfaceFrom(
    static_cast<void*>(pixels),
    font_width, font_height, depth, pitch,
    rmask, gmask, bmask, amask
  )); 
}

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


Cursor::Cursor() : Component() 
{
  body.x = 10;
  body.y = 10;
}
Cursor::~Cursor() {}

void Cursor::RENDER(SDL_Renderer* renderer) 
{
  body.w = 7;
  body.h = 9;
  
  SDL_SetRenderDrawColor(renderer, 0xFF,0xFF,0xFF,0x00);
  SDL_RenderDrawRect(renderer, &body);
  SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0x00);
}

void Cursor::UPDATE(SDL_Event* event) 
{
  switch (event->type) {
  case SDL_KEYDOWN: 
    switch (event->key.keysym.sym) {
    case SDLK_LEFT: body.x -= 10; break;
    case SDLK_RIGHT: body.x += 10; break;
    }
    break;
  }
}
void Cursor::UPDATE(const float delta_time) {}


Editor::Editor(SDL_Renderer* renderer) : State()
{
  Create(new Document(renderer));
  Create(new Cursor());
}
Editor::~Editor() {}
