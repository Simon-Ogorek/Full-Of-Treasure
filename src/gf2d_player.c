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

    return player;

}

void gf2d_player_think(Entity *ent)
{
    Player *player = (Player *)ent->calling_parent;

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_W])
    {
        player->ent->position.y += player->speed;
    }
    if (keys[SDL_SCANCODE_S])
    {
        player->ent->position.y -= player->speed;
    }
    if (keys[SDL_SCANCODE_A])
    {
        player->ent->position.x += player->speed;
    }
    if (keys[SDL_SCANCODE_D])
    {
        player->ent->position.x -= player->speed;
    }

    
}