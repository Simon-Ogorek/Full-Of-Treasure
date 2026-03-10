#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_entity.h"
#include "gf2d_map.h"
#include "gf2d_camera.h"
#include "gf2d_player.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    


    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    GFC_Color mouseGFC_Color = gfc_color8(255,255,255,200);
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        400,
        240,
        400,
        240,
        gfc_vector4d(0,0,0,255),
        1);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    
    /* New Stuff */
    int editor_mode = 0;

    if (argv[1] && !strcmp(argv[1], "editor"))
    {
        editor_mode = 1;
        slog("editor mode active");
    }
    
    gf2d_entity_init(256, "sprites/sprite.json");
    gf2d_map_init("map/maptiles.json", editor_mode);

    Player *player = gf2d_spawn_player();
    gf2d_camera_init(gfc_vector3d(0,0,0), 1, 0.1);
    gf2d_camera_assign_target(player->ent);

    
    float cubeTimer = 0;
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    slog("press [escape] to quit");
    /*main game loop*/

    int cube = 0;
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        


        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            //gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));
            
            //UI elements last  
            /*gf2d_sprite_draw(
                mouse,
                gfc_vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseGFC_Color,
                (int)mf);*/

            gf2d_sprite_draw(
                mouse,
                gfc_vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseGFC_Color,
                (int)mf);

            gf2d_map_draw();
            
            gf2d_think_all();
            gf2d_update_all();
            gf2d_draw_all();
            
            gf2d_camera_update();

            

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());

        //gf2d_entity_manager_slog();
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
