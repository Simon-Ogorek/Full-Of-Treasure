#ifndef __GF2D_PLAYER_H__
#define __GF2D_PLAYER_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

#include "gfc_list.h"

#include "gf2d_entity.h"

typedef struct Player_S
{
    Entity *ent;
    float speed;
}Player;

Player *gf2d_spawn_player();

void gf2d_player_think();


#endif