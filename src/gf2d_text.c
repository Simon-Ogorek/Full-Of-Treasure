#include "gf2d_text.h"
#include "gf2d_sprite.h"
static struct Text
{
    TTF_Font *treeFont;
}Text_Manager;

void gf2d_text_init()
{
    if (TTF_Init())
    {
        slog("%s", SDL_GetError());
    }
    Text_Manager.treeFont = TTF_OpenFont("font/Jost-Medium.ttf",24);

    if (!Text_Manager.treeFont)
    {
        slog("Bad Font");
    }
}

void gf2d_text_draw(int fontID, char* text, GFC_Vector2D pos, SDL_Color color, Uint32 maxWidth )
{
    SDL_Surface *textSurf = NULL;
    switch (fontID)
    {
        case TREE_FONT:
            textSurf = TTF_RenderText_Blended_Wrapped(Text_Manager.treeFont, text, color, maxWidth);
            break;

        default:
            slog("ERROR: No matching fontID");

    }

    if (!textSurf)
    {
        slog("ERROR: textSurf null: %s", TTF_GetError());
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(),textSurf);
    //SDL_Rect textRect = {(int)pos.x, (int)pos.y, textSurf->w, textSurf->h};
    //SDL_RenderCopy(gf2d_graphics_get_renderer(), textTexture, NULL, &textRect);

    // Caching this would'nt be a bad idea | OPTIMIZE
    Sprite *textSprite = gf2d_sprite_new();
    textSprite->texture = textTexture;
    textSprite->frame_w = textSurf->w;
    textSprite->frame_h = textSurf->h;
    textSprite->ref_count = 1;
    textSprite->surface = textSurf;

    //slog("%i, %i", textSurf->w, textSurf->h );
    

    gf2d_sprite_draw(textSprite, pos, NULL,NULL,NULL,NULL,NULL,NULL);
    gf2d_sprite_free(textSprite);
}   