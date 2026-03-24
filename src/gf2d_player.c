#include "gf2d_player.h"
#include "simple_logger.h"
#include "gf2d_camera.h"

Player *the_player;
Uint8 attack_pressed;
Player *gf2d_spawn_player()
{
    if (the_player)
        return;
    Player *player = (Player *)malloc(sizeof(Player));

    player->ent = gf2d_create_entity("player");
    player->ent->position.x = 0;
    player->ent->position.y = 0;
    player->ent->position.z = 0;

    player->ent->think = &gf2d_player_think;
    player->ent->calling_parent = (void *)player;
    player->speed = 1;

    player->ent->max_health = 5;
    player->ent->health = 3;

    player->melee_upgrade = 0;
    player->speed_upgrade = 0;
    player->damage_upgrade = 0;

    player->skillPoints = 2;

    gf2d_update_collisions_entity(player->ent, 1);
    the_player = player;
    return player;
}

Player * gf2d_player_get()
{
    return the_player;
}

void gf2d_player_think(Entity *ent)
{
    Player *player = (Player *)ent->calling_parent;
    if (player->ent->health <= 0)
    {
        player->ent->dead = 1;
        player->ent->next_movement.x =0;
        player->ent->next_movement.y =0;
        return;
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    float playerSpeed = player->speed + (player->speed_upgrade * 0.5);

    if (keys[SDL_SCANCODE_LSHIFT])
    {
        playerSpeed *= 1.5;
    }

    gfc_vector3d_scale(player->ent->next_movement, player->ent->next_movement, 0);

    if (keys[SDL_SCANCODE_W])
    {
        player->ent->next_movement.y -= playerSpeed;
    }
    if (keys[SDL_SCANCODE_S])
    {
        player->ent->next_movement.y += playerSpeed;
    }
    if (keys[SDL_SCANCODE_A])
    {
        player->ent->next_movement.x -= playerSpeed;
    }
    if (keys[SDL_SCANCODE_D])
    {
        player->ent->next_movement.x += playerSpeed;
    }

    if (keys[SDL_SCANCODE_E] && !attack_pressed)
    {
        attack_pressed = 1;

        if (player->melee_upgrade)
        {
            slog("I swung my sword");
        }
        else
        {
            player->ent->health--;
        }
    }
    attack_pressed = keys[SDL_SCANCODE_E];
}

Uint8 gf2d_player_buy_melee(Player* player)
{
    if (player->skillPoints <= 0)
        return 0;
    if (player->melee_upgrade)
        return 0;

    player->skillPoints--;
    player->melee_upgrade = 1;
    return 1;
}

Uint8 gf2d_player_buy_speed(Player* player)
{
    if (player->skillPoints <= 0)
        return 0;
    if (player->speed_upgrade)
        return 0;

    player->skillPoints--;
    player->speed_upgrade = 1;
    return 1;
}

Uint8 gf2d_player_buy_damage(Player* player)
{
    if (player->skillPoints <= 0)
        return 0;
    if (player->damage_upgrade)
        return 0;

    player->skillPoints--;
    player->damage_upgrade = 1;
    return 1;
}