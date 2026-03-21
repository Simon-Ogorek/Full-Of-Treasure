#include "gf2d_ui.h"
#include "gf2d_entity.h"
#include "simple_logger.h"

static struct UI
{
    GFC_Vector2D heartUIOrigin;
    Entity *tracked_ent;
    Sprite *heartFull;
    Sprite *heartEmpty;
}UI_Manager;


void gf2d_ui_init(Entity *tracked)
{
    UI_Manager.heartFull = gf2d_sprite_load_image("sprites/heart_full.png");
    UI_Manager.heartEmpty = gf2d_sprite_load_image("sprites/heart_dead.png");
    UI_Manager.tracked_ent = tracked;

    UI_Manager.heartUIOrigin.x = 2;
    UI_Manager.heartUIOrigin.y = 2;
}

void gf2d_ui_update()
{
    int hearts_filled = UI_Manager.tracked_ent->health;
    GFC_Vector2D currHeartDrawPos;
    currHeartDrawPos.x = UI_Manager.heartUIOrigin.x;
    currHeartDrawPos.y = UI_Manager.heartUIOrigin.y;


    for (int i = 0; i < UI_Manager.tracked_ent->max_health;i++ )
    {
        if (hearts_filled > 0)
        {
            gf2d_sprite_draw(UI_Manager.heartFull, currHeartDrawPos, NULL, NULL, NULL, NULL, NULL, NULL);
            hearts_filled--;
        }
        else
        {
            gf2d_sprite_draw(UI_Manager.heartEmpty, currHeartDrawPos, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        currHeartDrawPos.x += 20;
    }
}