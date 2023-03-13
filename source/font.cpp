#include "font.h"

Font::Font(SDL_Renderer* renderer)
  :
  scale(1),
  font_sheet_cols(18),
  font_sheet_rows(7),
  renderer(renderer),
  atlas(new SDL_Rect[ASCII_HIGH - ASCII_LOW + 1])
{}
Font::~Font()
{
  SDL_DestroyTexture(font_sheet);
  delete this->atlas;
}

bool Font::load_from_file(const char* file_path)
{
  if (renderer == NULL) {
    ERROR("Renderer not found.");
    return false;
  }

  SDL_Surface* font_surface = IMG_Load(file_path);
  if (font_surface == NULL) {
    ERROR("Failed to open the file.");
    return false;
  }

  this->font_sheet = SDL_CreateTextureFromSurface(renderer, font_surface);
  SDL_FreeSurface(font_surface);
  if (this->font_sheet == NULL) {
    ERROR("Failed to create font texture.");
    return false;
  }

  SDL_QueryTexture(this->font_sheet, NULL, NULL, &font_sheet_width, &font_sheet_height);
  this->width = font_sheet_width / font_sheet_cols;
  this->height = font_sheet_height / font_sheet_rows; 

  for (size_t ascii = ASCII_LOW; ascii < ASCII_HIGH; ++ascii) {
    const size_t index = ascii - ASCII_LOW;
    const size_t texture_x = index % font_sheet_cols;
    const size_t texture_y = index / font_sheet_cols;

    this->atlas[index] = {
      .x = texture_x * width,
      .y = texture_y * height,
      .w = width,
      .h = height
    };
  }

  return true;
}

SDL_Rect* Font::get(const char character) const
{
  const size_t index = static_cast<int>(character) - ASCII_LOW;
  if (0 <= index && index <= (ASCII_HIGH - ASCII_LOW + 1))
    return &this->atlas[index];
  return nullptr;
}
