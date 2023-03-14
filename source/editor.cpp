#include "editor.h"
#include "document.h"

Editor::Editor(SDL_Window *window, SDL_Renderer* renderer) : State()
{
  Create(new Document(window, renderer));
}
Editor::~Editor() {}
