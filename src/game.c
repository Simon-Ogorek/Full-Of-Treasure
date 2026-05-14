#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_entity.h"
#include "gf2d_map.h"
#include "gf2d_camera.h"
#include "gf2d_player.h"
#include "gf2d_ui.h"
#include "gf2d_enemy.h"
#include "gf2d_weather.h"
#include <pthread.h>
#include <SDL_mixer.h>

Player *player;
char editor_mode;
Camera * cam;
void *loadNonCoreFucntionality(void *arg)
{
    
    return NULL;
}
int main(int argc, char * argv[])
{
    /*variable declarations*/
    
    float timer = 20;
    
    pthread_t thread_id;
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    GFC_Color mouseGFC_Color = gfc_color8(255,255,255,200);
    if (argv[1] && !strcmp(argv[1], "editor"))
    {
        editor_mode = 1;
        slog("editor mode active");
    }
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "Game",
        1200,
        700,
        400,
        240,
        gfc_vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    int delay = 100; // ts runs way too fast for the loading screen to be visible
    Sprite *loadingSprites[9];
    char finalPath[64];
    for (int i = 0; i < 9; i++)
    {
        snprintf(finalPath, 64,"images/menu/Loading%d.png", i);
        loadingSprites[i] = gf2d_sprite_load_image(finalPath);
    }
    int i = 0;
    GFC_Vector2D origin = {0};
    gf2d_sprite_draw_image(loadingSprites[i++], origin);
    gf2d_graphics_next_frame();
    SDL_Delay(delay);
    slog("sprite init");
    gf2d_entity_init(256, "sprites/sprite.json");
    gf2d_sprite_draw_image(loadingSprites[i++], origin);
    gf2d_graphics_next_frame();
    SDL_Delay(delay);
    slog("ent init");
    gf2d_map_init("map/maptiles.json", editor_mode);
    gf2d_sprite_draw_image(loadingSprites[i++], origin);
    gf2d_graphics_next_frame();
    SDL_Delay(delay);
    slog("map init");
    gf2d_weather_init();
    gf2d_sprite_draw_image(loadingSprites[i++], origin);
    gf2d_graphics_next_frame();
    SDL_Delay(delay);
    slog("weather init");
    gf2d_text_init();
    gf2d_sprite_draw_image(loadingSprites[i++], origin);
    gf2d_graphics_next_frame();
    SDL_Delay(delay);
    slog("text init");
    gf2d_sprite_draw_image(loadingSprites[i++], origin);
    gf2d_graphics_next_frame();
    SDL_Delay(delay);
    slog("ui init");
    gf2d_enemy_init(512);
    gf2d_sprite_draw_image(loadingSprites[i++], origin);
    gf2d_graphics_next_frame();
    SDL_Delay(delay);
    slog("enemy init");
    cam = gf2d_camera_init(gfc_vector3d(0,0,0), 1, 0.1);
    
    gf2d_sprite_draw_image(loadingSprites[i++], origin);
    gf2d_graphics_next_frame();
    SDL_Delay(delay);
    slog("cam init");

    gf2d_sound_init();

    for (i = 0; i < 9; i++)
    {
        gf2d_sprite_free(loadingSprites[i]);
    }
    
    
    SDL_ShowCursor(SDL_DISABLE);

    if (argc > 1)
        gf2d_map_editor_init(cam);

    float cubeTimer = 0;
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0,0);
    slog("press [escape] to quit");

    Uint8 pausedPressed = 0;
    char inGame = 0;
    int cube = 0;
    
    Mix_Music *bgm = Mix_LoadMUS("src/main_music.mp3");
    Mix_PlayMusic(bgm, -1);
    while(!done)
    {

        while (!inGame)
        {
            SDL_PumpEvents();
            gf2d_sprite_draw_image(gf2d_sprite_load_image("images/menu/MainMenu.png"), origin);
            gf2d_graphics_next_frame();
            keys = SDL_GetKeyboardState(NULL);
            if (keys[SDL_SCANCODE_ESCAPE])
            {
                done = 1;
                inGame = 1;
            }
            if (keys[SDL_SCANCODE_A])
            {
                inGame = 1;

                    
                

                Entity *waystone = gf2d_create_entity("teleporter");
                waystone->position.x = 400;
                waystone->position.y = 400;
                gf2d_update_collisions_entity(waystone, 1);
                player = gf2d_spawn_player();
                player->ent->position.x = 400;
                player->ent->position.y = 400;
                gf2d_enemy_reinit();
                gf2d_ui_init(player);
                gf2d_camera_assign_target(player->ent);
            }
        }
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        


        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            //gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));

        gf2d_map_draw();
        
        gf2d_think_all();
        gf2d_update_all();
        gf2d_draw_all();
        
        gf2d_camera_update();
        gf2d_weather_update();
        gf2d_ui_update();

        timer -= 0.1;
        

        if (timer <= 0)
        {
            timer = 20;
            srand(time(NULL)); 
            Enemy * enemy;
            if (rand() % 2)
                enemy = gf2d_create_enemy("goblin");
            else
                enemy = gf2d_create_enemy("zombie");

            enemy->ent->position.x = gfc_random_int(1000);
            enemy->ent->position.y = gfc_random_int(1000);
        }

        if (player->ent->dead)
        {
            player->ent->dead = 0;
            gf2d_sound_play_die();

            gf2d_entity_cleanup(0);
            gf2d_map_reset();

            inGame = 0;
        }

            

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE]) {done = 1;} // exit condition

        if (keys[SDL_SCANCODE_BACKSPACE] && !pausedPressed)
        {
            gf2d_entity_set_pause(!gf2d_entity_get_pause());
            gf2d_ui_set_tree(gf2d_entity_get_pause());
        }

        pausedPressed = keys[SDL_SCANCODE_BACKSPACE];
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());

        //gf2d_entity_manager_slog();
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
