#ifndef EDITOR
#define EDITOR

#include "state.h"

class Editor : public State
{
public:
  Editor(SDL_Renderer*);
  ~Editor();
};



#endif // EDITOR
