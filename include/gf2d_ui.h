#ifndef __GF2D_UI_H__
#define __GF2D_UI_H__

#include <SDL.h>
#include "gf2d_sprite.h"
#include "gfc_types.h"
#include "gfc_color.h"
#include "gfc_vector.h"
#include "gfc_text.h"
#include "gf2d_entity.h"

void gf2d_ui_init(Entity *tracked);

void gf2d_ui_update();
#endif
