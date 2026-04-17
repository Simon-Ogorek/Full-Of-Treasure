#include "gf2d_entity.h"
#include "simple_logger.h"
#include "simple_json.h"
#include <SDL_image.h>
#include "gfc_hashmap.h"
#include "gfc_config_def.h"
#include "gf2d_camera.h"

#include "gf2d_draw.h"
#include "gf2d_map.h"

static struct Entity_Manager
{
    Entity *all_ents;
    int count;
    Uint8 paused;
} entityManager;


void gf2d_preload_sprites(char* config_filepath)
{
    int i;
    
    SJson * imagePathJSON;
    char *imagePath;

    SJson *spriteJSON = sj_load("sprites/sprite.json");

    SJson *spriteInfoJSON = sj_object_get_value(spriteJSON, "sprites");

    SJson *spritePropertiesJSON;

    for (i = 0; i < sj_array_get_count(spriteInfoJSON); i++)
    {
        spritePropertiesJSON = sj_array_get_nth(spriteInfoJSON, i);
        if (!spritePropertiesJSON)
        {
            slog("Bad JSON in %s at entry %i for sprites", config_filepath, i);
            continue;
        }
        imagePathJSON = sj_object_get_value(spritePropertiesJSON, "filepath");
        if (!imagePathJSON)
        {
            slog("Bad JSON in %s at entry %i for sprites", config_filepath, i);
            continue;
        }
        imagePath = sj_get_string_value(imagePathJSON);
        if (!imagePath)
        {
            slog("Bad JSON in %s at entry %i for image path in sprites", config_filepath, i);
            continue;
        }
        SDL_Surface *temp = IMG_Load(imagePath);
        if (!temp)
        {
            slog("Bad Image in %s at entry %i for sprites", config_filepath, i);
            continue;
        }

        int frameWidth = 0;
        int frameHeight = 0;
        if (!sj_get_integer_value(sj_object_get_value(spritePropertiesJSON, "sheet_width"), &frameWidth) ||
            !sj_get_integer_value(sj_object_get_value(spritePropertiesJSON, "sheet_height"), &frameHeight))
        {
            slog("Bad Height/ Width in %s at entry %i for sprites", config_filepath, i);
            continue;
        }

        gf2d_sprite_load_all(imagePath, 
            temp->w / frameWidth,
            temp->h / frameHeight,
            frameWidth,
            false,
            0);
        SDL_FreeSurface(temp);



    }
    gfc_config_def_init();
    gfc_config_def_load(config_filepath);

}


void gf2d_entity_init(int count, char* config_filepath)
{
    int i;

    entityManager.all_ents = (Entity *)gfc_allocate_array(sizeof(Entity), count);
    memset(entityManager.all_ents,0,sizeof(Entity)*count);

    entityManager.count = count;

    for (i = 0; i < count; i++)
    {
        entityManager.all_ents[i].status = Inactive;
    }

    gf2d_preload_sprites("sprites/sprite.json");

}

void gf2d_draw_entity(Entity *ent)
{
    GFC_Vector3D pos = ent->position;
    GFC_Vector3D *offset = gf2d_camera_get_offset();
    //slog("%s, (%f,%f,%f)", ent->name, gfc_vector3d_to_slog(pos));
    pos.x -= offset->x;
    pos.y -= offset->y;
    //slog("%s, (%f,%f,%f)", ent->name, gfc_vector3d_to_slog(pos));
    if (ent->dead)
        return;

    //slog("ent : %f %f %f | offseted : %f %f %f", gfc_vector3d_to_slog(ent->position), gfc_vector3d_to_slog(pos));
    gf2d_sprite_draw( ent->sprite, 
        gfc_vector3dxy(pos),
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        (int)ent->animation_frame );
        //gf2d_draw_rect(ent->collide_rect, gfc_color(1,1,1,1));
    };

void gf2d_think_entity(Entity *ent)
{
    return;
}

void gf2d_update_entity(Entity *ent)
{
    Entity *compEnt;
    ent->animation_frame += ent->animation_speed;
    ent->animation_frame = (ent->animation_frame >= ent->sprite->total_frames-1) ? 0 : ent->animation_frame;
    //slog("current frame of %s is %f at %f", ent->name, ent->animation_frame, ent->animation_speed);
    if (ent->collides)
    {
        //slog("%s rect:", ent->name);
        //gfc_rect_slog(ent->collide_rect);

        if (!gfc_rect_overlap(ent->collide_rect,gf2d_map_bounds()))
        {
            ent->position = gfc_vector3d_subbed(ent->position, ent->next_movement);
            gf2d_update_collisions_entity(ent, 0);
            return;
        }
        for (int i = 0; i < entityManager.count; i++)
        {
            compEnt = &entityManager.all_ents[i];

            if (compEnt == ent)
                continue;

            if (compEnt->status == Active && compEnt->collides)
            {
                if (gfc_rect_overlap(ent->collide_rect,compEnt->collide_rect))
                {
                    slog("movement repulsed");
                    
                    ent->position = gfc_vector3d_subbed(ent->position, ent->next_movement);
                    ent->position = gfc_vector3d_subbed(ent->position, ent->next_movement);
                    gf2d_update_collisions_entity(ent, 0);
                    return;
                }
                    
            }
        }
    }

    ent->position = gfc_vector3d_added(ent->position, ent->next_movement);
    gf2d_update_collisions_entity(ent, 0);
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
            SJson *ent_info = gfc_config_def_get_by_name("sprites", name);
            char *filepath = sj_get_string_value(sj_object_get_value(ent_info, "filepath"));
            ent->sprite = gf2d_sprite_load_image(filepath);
            int code = sj_get_float_value(sj_object_get_value(ent_info, "animation_speed"), &ent->animation_speed);

            ent->update = &gf2d_update_entity;
            ent->think = &gf2d_think_entity;
            ent->draw = &gf2d_draw_entity;

            //slog("%f speed %i", ent->animation_speed, code);
            return ent;
        }
    }

    slog("No inactive ent found");
    return NULL;

}

void gf2d_update_collisions_entity(Entity *ent, Uint8 resize)
{
    ent->collide_rect.x = ent->position.x;
    ent->collide_rect.y = ent->position.y;

    GFC_Vector3D *offset = gf2d_camera_get_offset();
    ent->collide_rect.x -= offset->x;
    ent->collide_rect.y -= offset->y;
    if (resize || 1)
    {
        ent->collide_rect.w = ent->sprite->frame_w;
        ent->collide_rect.h = ent->sprite->frame_h;
    }

    if (gf2d_camera_is_target(ent))
    {
        if (resize || 1)
        {
            ent->collide_rect.w /= 3;
            ent->collide_rect.h /= 3;
        }
        

        ent->collide_rect.x += 42;
        ent->collide_rect.y += 50;
    }

    ent->collides = 1;

    return;
}

Entity *gf2d_find_entity(char* name)
{
    Entity *ent;
    int i;

    for (i = 0; i < entityManager.count; i++)
    {
        ent = &entityManager.all_ents[i];

        if (ent->status != Inactive && strcmp(ent->name, name) == 0)
        {
            ent->status = Active;
            return ent;
        }
    }
}
void gf2d_delete_entity(Entity *ent)
{
    free(ent->sprite);
    ent->status = Inactive;
}
void gf2d_think_all()
{
    Entity *ent;
    int i;

    if (entityManager.paused)
        return;

    for (i = 0; i < entityManager.count; i++)
    {
        ent = &entityManager.all_ents[i];

        if (ent->status != Active)
            continue;

        if (!ent->think)
        {
            gf2d_think_entity(ent);
            continue;
        }

        ent->think(ent);
    }
}

void gf2d_update_all()
{
    Entity *ent;
    int i;

    if (entityManager.paused)
        return;

    for (i = 0; i < entityManager.count; i++)
    {
        ent = &entityManager.all_ents[i];

        if (ent->status != Active)
            continue;

        if (!ent->update)
        {
            gf2d_update_entity(ent);
            continue;
        }

        ent->update(ent);
    }
}

void gf2d_draw_all()
{
    Entity *ent;
    int i;

    for (i = 0; i < entityManager.count; i++)
    {
        ent = &entityManager.all_ents[i];

        if (ent->status != Active)
            continue;

        if (!ent->update)
        {
            gf2d_draw_entity(ent);
            continue;
        }

        ent->draw(ent);
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
            slog("%s | %i | %f, %f", ent->name, i, ent->position.x, ent->position.y);
        }
    }
}

void gf2d_entity_set_pause(Uint8 TorF)
{
    entityManager.paused = TorF;
    slog("set pause");
}

Uint8 gf2d_entity_get_pause()
{
    return entityManager.paused;
}
