#include "gf2d_entity.h"
#include "simple_logger.h"

struct Entity_Manager
{
    Entity *all_ents;
    int count;
} entityManager;

void gf2d_entity_init(int count)
{
    int i;

    entityManager.all_ents = (Entity *)gfc_allocate_array(sizeof(Entity), count);
    entityManager.count = count;

    for (i = 0; i < count; i++)
    {
        entityManager.all_ents[i].status = Inactive;
    }
}

void gf2d_draw_entity(Entity *ent)
{
    gf2d_sprite_draw_image(ent->sprite, ent->pos);
}

Entity * gf2d_create_entity(char* name)
{
    Entity *ent;
    int i;

    for (i = 0; i < entityManager.count; i++)
    {
        ent = &entityManager.all_ents[i];

        if (ent->status == Inactive)
        {
            ent->status = Active;

            ent->name = (name ? name : "default");
            return ent;
        }
    }

    slog("No inactive ent found");
    return NULL;

}
void gf2d_delete_entity(Entity *ent)
{
    free(ent->sprite);
    ent->status = Inactive;
}
void gf2d_draw_all()
{
    Entity *ent;
    int i;

    for (i = 0; i < entityManager.count; i++)
    {
        ent = &entityManager.all_ents[i];

        if (ent->status == Active)
        {
            gf2d_draw_entity(ent);
        }
    }
}

void gf2d_entity_manager_slog()
{
    Entity *ent;
    int i;
    
    for (i = 0; i < entityManager.count; i++)
    {
        ent = &entityManager.all_ents[i];

        if (ent->status == Active)
        {
            slog("%s | %i | %f, %f", ent->name, i, ent->pos.x, ent->pos.y);
        }
    }
}
