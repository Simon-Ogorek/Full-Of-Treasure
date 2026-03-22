#ifndef __GF2D_TEXT_H__
#define __GF2D_TEXT_H__
#include <SDL.h>
#include <SDL_ttf.h>
#include "gfc_vector.h"
#include "gf2d_graphics.h"
#include "simple_logger.h"

#define TREE_FONT 0

void gf2d_text_init();

void gf2d_text_draw(int fontID, char* text, GFC_Vector2D pos, SDL_Color color, Uint32 maxWidth );

#endif