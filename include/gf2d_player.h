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

    Uint8 melee_upgrade;
    Uint8 damage_upgrade;
    Uint8 speed_upgrade;

    int skillPoints
}Player;

Player *gf2d_spawn_player();

void gf2d_player_think();

Uint8 gf2d_player_can_afford_upgrade(Player* player);

Uint8 gf2d_player_buy_melee(Player* player);
Uint8 gf2d_player_buy_speed(Player* player);
Uint8 gf2d_player_buy_damage(Player* player);


#endif