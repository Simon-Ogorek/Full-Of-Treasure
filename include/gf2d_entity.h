#ifndef __GF2D_ENTITY_C__
#define __GF2D_ENTITY_C__

#include <SDL.h>

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

#include "gfc_shape.h"

#include "gfc_list.h"

enum Entity_Status
{
    Active,
    Inactive
};

typedef struct Entity_S
{
    Sprite *sprite;
    GFC_Vector3D position;
    GFC_Vector3D next_movement;

    Uint8 collides;

    GFC_Rect collide_rect;

    float rotation;

    int health;
    int max_health;
    float speed;

    enum Entity_Status status;

    char* name;

    GFC_Vector2D image_size;
    GFC_Vector2D working_size;

    float animation_frame;
    float animation_speed;

    Uint8 dead;

    void (*draw)(struct Entity *self);
    void (*update)(struct Entity *self);
    void (*think)(struct Entity *self);

    void *calling_parent;
}Entity;

void gf2d_entity_init(int count, char* config_filepath);

Entity * gf2d_create_entity(char *name);

void gf2d_delete_entity(Entity *ent);

void gf2d_think_all();
void gf2d_update_all();
void gf2d_draw_all();

void gf2d_entity_manager_slog();
Entity *gf2d_find_entity(char* name);

void gf2d_update_collisions_entity(Entity *ent, Uint8 resize);

void gf2d_entity_pause(Uint8 TorF);

void gf2d_get_entity(char * name);

void gf2d_entity_set_pause(Uint8 TorF);

Uint8 gf2d_entity_get_pause();

void gf2d_update_entity(Entity *ent);
#endif