#ifndef __GF2D_MAP_EDITOR_C__
#define __GF2D_MAP_EDITOR_C__


#include "gf2d_entity.h"
#include "gf2d_player.h"
#include "gf2d_camera.h"

void gf2d_map_editor_init(Camera *camera);

void gf2d_map_editor_think();
void gf2d_map_editor_update();
void gf2d_map_editor_draw();

#endif