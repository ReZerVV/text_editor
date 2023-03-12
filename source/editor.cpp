#include "editor.h"

#include "log.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Document::Document(SDL_Renderer* renderer) : Component(),
  scale(5)
{
  //SDL_Surface* font_surface = surface_font_from_image("../asset/charmap.png");
  //font_texturesheet = SDL_CreateTextureFromSurface(renderer, font_surface);
  //SDL_FreeSurface(font_surface);
  SDL_Surface* font_surface = IMG_Load("../asset/charmap.png");
  if (font_surface != NULL)
  {
    font_width = font_surface->w;
    font_height = font_surface->h;
    font_texturesheet = SDL_CreateTextureFromSurface(renderer, font_surface);
    SDL_FreeSurface(font_surface);
  }
  else 
  {
    LOG("load bitmap.");
  }
}
Document::~Document() 
{
  SDL_DestroyTexture(font_texturesheet);
}

void Document::RENDER(SDL_Renderer* renderer)
{
  const SDL_Rect dest = {
    .x = 10,
    .y = 10,
    .w = font_width * scale,
    .h = font_height * scale
  };
  SDL_RenderCopy(renderer, font_texturesheet, NULL, &dest);
}
void Document::UPDATE(SDL_Event* event) {}
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


Cursor::Cursor() : Component() 
{
  body.x = 10;
  body.y = 10;
}
Cursor::~Cursor() {}

void Cursor::RENDER(SDL_Renderer* renderer) 
{
  body.w = 10;
  body.h = 20;
  
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
