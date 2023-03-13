#include "editor.h"
#include "document.h"

Editor::Editor(SDL_Renderer* renderer) : State()
{
  Create(new Document(renderer));
}
Editor::~Editor() {}
