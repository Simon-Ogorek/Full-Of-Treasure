#ifndef __GF2D_ENEMY_C__
#define __GF2D_ENEMY_C__

#include <SDL.h>

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

#include "gfc_shape.h"

#include "gfc_list.h"

#include "gf2d_entity.h"

enum Enemy_State
{
    ES_Inactive,
    ES_Unaware,
    ES_Chase,
    ES_Attack
};

typedef struct Enemy_S
{
    Entity *ent;
    enum Enemy_State state;
    float awareness_range;
    float attack_range;
    float cooldown;
    GFC_Vector3D random_walk;
}Enemy;

void gf2d_enemy_init(int count);

Enemy * gf2d_create_enemy(char *name);

void gf2d_think_enemy(Entity *ent);
void gf2d_update_enemy(Entity *ent);

void gf2d_delete_enemy(Enemy *enemy);

#endif