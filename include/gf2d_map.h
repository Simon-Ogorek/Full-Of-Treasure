#ifndef __GF2D_MAP_C__
#define __GF2D_MAP_C__

#include "gf2d_sprite.h"
#include "gfc_vector.h"
#include "gfc_list.h"
#include "gfc_shape.h"
typedef struct Map_Tile_S
{
    Sprite *sprite;
    GFC_Vector2D pos;
} Map_Tile;

typedef struct Map_Chunk_S
{
    GFC_List tiles;
    GFC_Vector2D pos;
}Map_Chunk;

void gf2d_map_init(char *map_file, int editorMode);

void gf2d_map_draw();
void map_update();
void map_think();

GFC_Rect gf2d_map_bounds();


#endif