#ifndef RENDERER
#define RENDERER

#include <SDL2/SDL.h>
#include <SDL_image.h>

// ========================= RENDERER ========================= //

class Renderer
{
public:
  
  Renderer(SDL_Renderer *);
  Renderer(Renderer &&) = default;
  Renderer(const Renderer &) = default;
  Renderer &operator=(Renderer &&) = default;
  Renderer &operator=(const Renderer &) = default;
  ~Renderer();

private:
 
  SDL_Renderer *renderer;

};

#endif // !RENDERER
