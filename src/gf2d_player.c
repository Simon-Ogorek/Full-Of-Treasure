#include "gf2d_player.h"
#include "simple_logger.h"

Player *gf2d_spawn_player()
{
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

    player->skillPoints = 1;

    return player;

}

void gf2d_player_think(Entity *ent)
{
    Player *player = (Player *)ent->calling_parent;

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    float playerSpeed = player->speed;

    if (keys[SDL_SCANCODE_LSHIFT])
    {
        playerSpeed *= 1.5;
    }

    if (keys[SDL_SCANCODE_W])
    {
        player->ent->position.y += playerSpeed;
    }
    if (keys[SDL_SCANCODE_S])
    {
        player->ent->position.y -= playerSpeed;
    }
    if (keys[SDL_SCANCODE_A])
    {
        player->ent->position.x += playerSpeed;
    }
    if (keys[SDL_SCANCODE_D])
    {
        player->ent->position.x -= playerSpeed;
    }

    
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