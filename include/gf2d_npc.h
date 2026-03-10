#ifndef __GF2D_NPC_C__
#define __GF2D_NPC_C__

#include <SDL.h>

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

#include "gf2d_entity.h"

#include "gfc_list.h"

enum NPC_Status
{
    Active,
    Inactive
};

typedef struct NPC_S
{
    Entity *ent;

    enum NPC_Status status;

}Entity;

void gf2d_npc_init(int count, char* config_filepath);

Entity * gf2d_create_npc(char *name);

void gf2d_delete_npc(Entity *ent);

void gf2d_get_npc(char * name);
#endif