#ifndef __GF2D_ENEMY_C__
#define __GF2D_ENEMY_C__

#include <SDL.h>

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

#include "gfc_shape.h"

#include "gfc_list.h"

#include "gf2d_entity.h"

void gf2d_weather_init();
void gf2d_weather_update();

void gf2d_weather_effect_night_start();
void gf2d_weather_effect_night_end();

void gf2d_weather_effect_rain_start();
void gf2d_weather_effect_rain_end();

#endif