#include "gf2d_weather.h"

#include "gf2d_graphics.h"
#include "simple_logger.h"

typedef struct Rain_Particle
{
    GFC_Vector2D pos;
    char inUse;
}Rain_Particle;

static struct Weather_Manager
{
    char isNight;
    char isRaining;
    
    Sprite *night_effect;
    float night_color_percentage;

    Sprite *rain_effect;
    Rain_Particle rain_particle_pool[256];
    float time_between_droplets;
    float last_droplet_time;
    float droplet_fall_speed;

} weather_manager;



void gf2d_weather_init()
{
    gf2d_graphics_set_blend_mode(SDL_BLENDMODE_BLEND);
    weather_manager.night_color_percentage = 0.2;
    weather_manager.rain_effect = gf2d_sprite_load_image("weather/rain.png");
    weather_manager.time_between_droplets = 0.1;
    weather_manager.last_droplet_time = weather_manager.time_between_droplets;
    weather_manager.droplet_fall_speed = 2;
    weather_manager.isNight = 1;
    weather_manager.isRaining = 1;
}

void gf2d_weather_update()
{


    if (weather_manager.isRaining)
    {
        //slog("Is raining");
        weather_manager.last_droplet_time -= 1.0f/60.0f;
        char need_to_spawn_droplet = 0;
        if (weather_manager.last_droplet_time <= 0)
        {
            need_to_spawn_droplet = 1;
            weather_manager.last_droplet_time = weather_manager.time_between_droplets;
        }
        Rain_Particle *temp;
        for (int i = 0; i < 256; i++)
        {
            temp = &weather_manager.rain_particle_pool[i];
            if (temp->inUse)
            {
                temp->pos.y += weather_manager.droplet_fall_speed;
                gf2d_sprite_draw_image(weather_manager.rain_effect, temp->pos);
                if (temp->pos.y >= 270)
                {
                    temp->inUse = false;
                    //slog("removing a rain drop");
                }
            }
            else if (need_to_spawn_droplet)
            {
                temp->inUse = true;
                need_to_spawn_droplet = false;
                temp->pos.x = gfc_crandom() * 200 + 200; // 0 - 400
                temp->pos.y = -40;
                //slog("creating a rain drop");

            }
        }
    }
    // this needs to happen after or the filter is wrong
    if (weather_manager.isNight)
    {
        SDL_SetRenderDrawColor(gf2d_graphics_get_renderer(), (256*weather_manager.night_color_percentage),(256*weather_manager.night_color_percentage),(256*weather_manager.night_color_percentage), 150);

        SDL_RenderFillRect(gf2d_graphics_get_renderer(), NULL);
    }
}
void gf2d_weather_effect_night_start()
{
    weather_manager.isNight = 1;
}
void gf2d_weather_effect_night_end()
{
    weather_manager.isNight = 0;
}
void gf2d_weather_effect_rain_start()
{
    weather_manager.isRaining = true;
}
void gf2d_weather_effect_rain_end()
{
    weather_manager.isRaining = false;
    Rain_Particle temp;
    for (int i = 0; i < 256; i++)
    {
        temp = weather_manager.rain_particle_pool[i];
        temp.inUse = 0;
    }
}