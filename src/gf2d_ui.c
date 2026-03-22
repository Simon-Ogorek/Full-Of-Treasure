#include "gf2d_ui.h"
#include "gf2d_entity.h"
#include "simple_logger.h"
#include "gf2d_player.h"
#include "gf2d_text.h"

struct Tree_Status
{
    Uint8 purchased;
    Uint8 selected;
};

static struct UI
{
    GFC_Vector2D heartUIOrigin;
    Player *tracked;
    Sprite *heartFull;
    Sprite *heartEmpty;

    Uint8 tree_active;
    int8_t tree_idx;
    Uint8 tree_full;

    Sprite *tree_sprite_background;
    Sprite *tree_sprite_foreground;

    Sprite *tree_element_unpurchased;
    Sprite *tree_element_unpurchased_selected;
    Sprite *tree_element_purchased;
    Sprite *tree_element_purchased_selected;

    struct Tree_Status left;
    struct Tree_Status middle;
    struct Tree_Status right;

    Uint8 ui_button_pressed;

}UI_Manager;


void gf2d_ui_init(Player *tracked)
{
    UI_Manager.heartFull = gf2d_sprite_load_image("sprites/heart_full.png");
    UI_Manager.heartEmpty = gf2d_sprite_load_image("sprites/heart_dead.png");

    UI_Manager.tree_sprite_background = gf2d_sprite_load_image("images/skill_background.png");
    UI_Manager.tree_sprite_foreground = gf2d_sprite_load_image("images/skill_foreground.png");
    UI_Manager.tree_element_purchased = gf2d_sprite_load_image("images/skill_elem_purchased.png");
    UI_Manager.tree_element_purchased_selected = gf2d_sprite_load_image("images/skill_elem_purchased_selected.png");
    UI_Manager.tree_element_unpurchased = gf2d_sprite_load_image("images/skill_elem_unpurchased.png");
    UI_Manager.tree_element_unpurchased_selected = gf2d_sprite_load_image("images/skill_elem_unpurchased_selected.png");

    UI_Manager.tracked = tracked;

    UI_Manager.heartUIOrigin.x = 2;
    UI_Manager.heartUIOrigin.y = 2;

    UI_Manager.tree_idx = 1;

    UI_Manager.tree_active = 0;
}

void draw_hearts()
{
    int hearts_filled = UI_Manager.tracked->ent->health;
    GFC_Vector2D currHeartDrawPos;
    currHeartDrawPos.x = UI_Manager.heartUIOrigin.x;
    currHeartDrawPos.y = UI_Manager.heartUIOrigin.y;


    for (int i = 0; i < UI_Manager.tracked->ent->max_health; i++ )
    {
        //slog("%i | %i",i, UI_Manager.tracked->ent->max_health );
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

Sprite * which_element_to_draw(struct Tree_Status element)
{
    if (element.purchased)
    {
        if (element.selected)
            return UI_Manager.tree_element_purchased_selected;
        else
            return UI_Manager.tree_element_purchased;
    }
    else
    {
        if (element.selected)
            return UI_Manager.tree_element_unpurchased_selected;
        else
            return UI_Manager.tree_element_unpurchased;
    }
    
}
void reset_tree_selection()
{
    UI_Manager.left.selected = 0;
    UI_Manager.right.selected = 0;
    UI_Manager.middle.selected = 0;
}
void draw_tree()
{
    SDL_Color text_color = {0,0,0,255};
    char skillPointsText[32];
    slog("%i", UI_Manager.tracked->skillPoints);
    sprintf(skillPointsText,"%i", UI_Manager.tracked->skillPoints);
    gf2d_text_draw(TREE_FONT, skillPointsText, gfc_vector2d(378, 1), text_color, 380);

    gf2d_sprite_draw(which_element_to_draw(UI_Manager.left), gfc_vector2d(55,84), NULL, NULL, NULL, NULL, NULL, NULL);
    gf2d_sprite_draw(which_element_to_draw(UI_Manager.middle), gfc_vector2d(164,5), NULL, NULL, NULL, NULL, NULL, NULL);
    gf2d_sprite_draw(which_element_to_draw(UI_Manager.right), gfc_vector2d(272,84), NULL, NULL, NULL, NULL, NULL, NULL);
    gf2d_sprite_draw(UI_Manager.tree_sprite_foreground, gfc_vector2d(0,0), NULL, NULL, NULL, NULL, NULL, NULL);

}

void gf2d_ui_update()
{
    if (UI_Manager.tree_active)
    {
        gf2d_sprite_draw(UI_Manager.tree_sprite_background, gfc_vector2d(0,0), NULL, NULL, NULL, NULL, NULL, NULL);
        const Uint8 * keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_LEFT] && !UI_Manager.ui_button_pressed)
        {
            UI_Manager.tree_idx--;
            if (UI_Manager.tree_idx < 0)
                UI_Manager.tree_idx++;
        }

        if (keys[SDL_SCANCODE_RIGHT] && !UI_Manager.ui_button_pressed)
        {
            UI_Manager.tree_idx++;
            if (UI_Manager.tree_idx > 2)
                UI_Manager.tree_idx--;
        }

        if (!UI_Manager.middle.purchased)
            UI_Manager.tree_idx = 1;

        reset_tree_selection();

        Uint8 buy_key = keys[SDL_SCANCODE_SPACE];
        SDL_Color text_color = {0,0,0,255};
        switch(UI_Manager.tree_idx)
        {
            case 0:
                UI_Manager.left.selected = 1;
                
                gf2d_text_draw(TREE_FONT, "Use the sharp (not the blunt) side of the sword for more damage", gfc_vector2d(30, 170), text_color, 380);
                if (buy_key)
                {
                    if (gf2d_player_buy_damage(UI_Manager.tracked))
                    {
                        UI_Manager.left.purchased = 1;
                    }
                }
                break;
            case 1:
                UI_Manager.middle.selected = 1;
                gf2d_text_draw(TREE_FONT, "Learn how to use your sword and slash enemies in front of you!", gfc_vector2d(30, 170), text_color, 380);
                if (buy_key)
                {
                    if (gf2d_player_buy_melee(UI_Manager.tracked))
                    {
                        UI_Manager.middle.purchased = 1;
                    }
                }
                break;
            case 2:
                UI_Manager.right.selected = 1;
                gf2d_text_draw(TREE_FONT, "Learn how to breath better, giving you better running ability", gfc_vector2d(30, 170), text_color, 380);
                if (buy_key)
                {
                    if (gf2d_player_buy_speed(UI_Manager.tracked))
                    {
                        UI_Manager.right.purchased = 1;
                    }
                }
                break;
        }

        UI_Manager.ui_button_pressed = keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_SPACE];




        draw_tree();
        return;
    }

    draw_hearts();
}


void gf2d_ui_set_tree(Uint8 TorF)
{
    UI_Manager.tree_active = TorF;

    if (TorF)
    {
        UI_Manager.tree_idx = 1;

        reset_tree_selection();

        UI_Manager.middle.selected = 1;

        UI_Manager.left.purchased = 0;
        UI_Manager.right.purchased = 0;
        UI_Manager.middle.purchased = 0;

        if (UI_Manager.tracked->damage_upgrade)
            UI_Manager.left.purchased = 1;
        if (UI_Manager.tracked->melee_upgrade)
            UI_Manager.middle.purchased = 1;
        if (UI_Manager.tracked->speed_upgrade)
            UI_Manager.right.purchased = 1;
    }

}


