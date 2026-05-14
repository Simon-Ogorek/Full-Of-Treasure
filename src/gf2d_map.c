#include "simple_json.h"
#include "simple_logger.h"
#include "gf2d_sprite.h"
#include "gf2d_map.h"
#include "gfc_hashmap.h"
#include "gfc_types.h"
#include "gfc_vector.h"
#include <SDL_image.h>
#include "gf2d_weather.h"

#include "gf2d_camera.h"

#define Tile_Delimeter (short)65535;


static struct Map_Manager
{
    Tile *tile_defs[256];
    Sprite *file_sprites[16];

    Uint16 *map;
    int tile_count;

    GFC_Rect bound_rect;

    SJson *map_info_JSON;
    int tile_width, tile_height;

    char map_binary_file_path[64];

    int level;
    int max_level;

    Uint8 collision_map[200][200];
    char stale_map;
}map_manager;



Tile_Definition * gf2d_map_get_tile(Uint32 tile_idx)
{
    //slog("trying to access at %i of count %i", tile_idx + 1, gfc_list_get_count(map_manager.tile_defs));
    return ((Tile_Definition*)map_manager.tile_defs[tile_idx]);
}

Sprite * gf2d_map_get_file_by_idx(Uint8 file_idx)
{
    return ((Sprite*)map_manager.file_sprites[file_idx]);
}

char *gf2d_map_get_binary_path()
{
    return map_manager.map_binary_file_path;
}

void gf2d_map_init(char *map_file, int editorMode)
{

    map_manager.level = 0;
    map_manager.max_level = 9;
    SJson *map_info_JSON = sj_load(map_file);

    SJson *map_tilesets_JSON = sj_object_get_value(map_info_JSON, "tilesets");

    int tile_width, tile_height;
    SJson *map_tile_width_JSON =  sj_object_get_value(map_info_JSON, "widthInPixels");
    sj_get_integer_value(map_tile_width_JSON, &tile_width);
    SJson *map_tile_height_JSON =  sj_object_get_value(map_info_JSON, "heightInPixels");
    sj_get_integer_value(map_tile_height_JSON, &tile_height);
    map_manager.tile_width = tile_width;
    map_manager.tile_height = tile_height;
    map_manager.map_info_JSON = map_info_JSON;

    map_manager.stale_map = 1;

    int tile_index = 1; // 0 is null 
    int file_index = 0; // 0 is fine here;

    for (int i = 0; i < sj_array_get_count(map_tilesets_JSON); i++)
    {
        #pragma region Tileset
        SJson *tileset_JSON = sj_array_get_nth(map_tilesets_JSON, i);

        if (!tileset_JSON)
        {
            slog("Bad JSON in %s at entry %i for map", map_file, i);
            continue;
        }

        SJson *tileset_name_JSON = sj_object_get_value(tileset_JSON, "name");
        if (!tileset_name_JSON)
        {
            slog("Bad JSON in %s at entry %i for map", map_file, i);
            continue;
        }
        char* tileset_name = sj_get_string_value(tileset_name_JSON);

        SJson *tileset_file_JSON = sj_object_get_value(tileset_JSON, "filepath");
        if (!tileset_file_JSON)
        {
            slog("Bad JSON in %s at entry %i for map", map_file, i);
            continue;
        }
        char* tileset_file = sj_get_string_value(tileset_file_JSON);
        
        SDL_Surface* temp_image = IMG_Load(tileset_file);

        int image_height = temp_image->h;
        int image_width = temp_image->w;

        SDL_FreeSurface(temp_image);

        SJson* sheet_width_JSON = sj_object_get_value(tileset_JSON, "sheet_width");
        int sheet_width;
        sj_get_integer_value(sheet_width_JSON, &sheet_width);

        SJson* sheet_height_JSON = sj_object_get_value(tileset_JSON, "sheet_height");
        int sheet_height;
        sj_get_integer_value(sheet_height_JSON, &sheet_height);

        if (!sheet_height_JSON || !sheet_width_JSON)
        {
            slog("Bad JSON in %s at entry %i for map", map_file, i);
            continue;
        }

        slog("DEBUG ( iw : %i | ih : %i | sw : %i | sh : %i )", image_width, image_height, sheet_width, sheet_height);

        slog_sync();

        Sprite* tileset_sprite = gf2d_sprite_load_all(
            tileset_file,
            image_width / sheet_width,
            image_height / sheet_height,
            sheet_width,
            NULL,
            0
        );

        map_manager.file_sprites[file_index] = tileset_sprite;

        #pragma endregion

        #pragma region Tile Defintion

        SJson *tiles_JSON = sj_object_get_value(tileset_JSON, "tiles");

        if (!tiles_JSON)
        {
            slog("Bad JSON in %s at entry %i for map", map_file, i);
            continue;
        }

        Tile_Definition *tile = (Tile_Definition *)malloc(sizeof(Tile_Definition) * sj_array_get_count(tiles_JSON));
        for (int j = 0; j < sj_array_get_count(tiles_JSON); j++)
        {
            SJson *tile_info_JSON = sj_array_get_nth(tiles_JSON, j);
            if (!tile_info_JSON)
            {
                slog("Bad JSON in %s at entry %i for map", map_file, j);
                continue;
            }
        
            tile->tileset_file_idx = file_index;

            int tile_x, tile_y;
            
            SJson *tile_x_JSON = sj_object_get_value(tile_info_JSON, "x");
            sj_get_integer_value(tile_x_JSON, &tile_x);

            SJson *tile_y_JSON = sj_object_get_value(tile_info_JSON, "y");
            sj_get_integer_value(tile_y_JSON, &tile_y);

            tile->frame = (sheet_width * tile_y) + tile_x;

            SJson *tile_name_JSON = sj_object_get_value(tile_info_JSON, "name");
            char *tile_name = sj_get_string_value(tile_name_JSON);

            if (!tile_name_JSON || !tile_y_JSON || !tile_x_JSON)
            {
                slog("Bad JSON in %s at entry %i for map", map_file, j);
                continue;
            }
            
            slog("Making a new tile at %i at %p", tile_index, tile);
            //gfc_list_append(map_manager.tile_defs, tile_index);
            map_manager.tile_defs[tile_index] = tile;
            tile++;
            tile_index++;

        }
        file_index++;


    }

    SJson *map_layout_JSON = sj_object_get_value(map_manager.map_info_JSON, "map_layout");

    if (!map_layout_JSON)
    {
        slog("Bad JSON in draw");
        return;
    }

    SJson *map_tiles_amount_JSON = sj_object_get_value(map_layout_JSON, "count");

    if (!map_tiles_amount_JSON)
    {
        slog("Bad tile amount JSON in draw");
        return;
    }

    sj_get_integer_value(map_tilesets_JSON, &map_manager.tile_count);

    slog("tile count: %i", map_manager.tile_count);
    map_manager.tile_count = 25000 * 8;

    map_manager.map = (Uint16*)calloc(sizeof(Uint16),map_manager.tile_count);
    
    char * filepath = sj_get_string_value(sj_object_get_value(map_layout_JSON,"path"));
    strcpy(map_manager.map_binary_file_path, filepath);
    sj_get_integer_value(sj_object_get_value(map_layout_JSON, "levels"),&map_manager.max_level);

    char path[64];
    snprintf(path, 64, "%s%s%i%s", map_manager.map_binary_file_path, "map", map_manager.level, ".bin");
    slog("Opening map at path %s", path);
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Error opening binary file");
        return -1;
    }

    fread(map_manager.map, sizeof(Uint16), map_manager.tile_count, file);

    map_manager.bound_rect.x = -1000;
    map_manager.bound_rect.y = -1000;
    map_manager.bound_rect.w = 10000*32;
    map_manager.bound_rect.h = 7000*16;
    #pragma endregion
}

GFC_Rect gf2d_map_bounds()
{
    //slog("world rect");
    //gfc_rect_slog(map_manager.bound_rect);
    return map_manager.bound_rect;
}

char gf2d_map_is_colliding(GFC_Vector3D pos)
{
    int width = map_manager.tile_width;
    int height = map_manager.tile_height;

    int converted_x = (int)(pos.x / width + pos.y / height);
    int converted_y = (int)(-pos.x / width + pos.y / height);

    return map_manager.collision_map[converted_x][converted_y];
}

void gf2d_map_draw()
{
    #pragma region Map_Spawning

    //printf("===========================================\n");
    Uint16 *tile = (Uint16 *)map_manager.map;
    int tile_x = 0;
    int tile_y = 0;
    int tile_z = 0;

    if (map_manager.stale_map)
    {
        memset(map_manager.collision_map, 0, sizeof(map_manager.collision_map));
    }
    //printf("Started");
    while (*tile != 0)
    {
        //printf("Tile: %hu\n", *tile);
        if (*tile == 65535)
        {
            tile_y++;
            tile_x=0;
            tile++;
            continue;
        }

        if (*tile == 65534)
        {
            tile_z++;
            tile_x = 0;
            tile_y = 0;
            tile++;
            continue;
        }

        if (*tile == 65533)
        {
            tile_x++;
            tile++;
            continue;
        }

        if (*tile > 65390)
        {
            return;
        }
        Tile_Definition* tile_DEF = gf2d_map_get_tile(*tile);
        int tile_width = map_manager.tile_width;
        int tile_height = map_manager.tile_height;

        GFC_Vector3D offsetedPos = {0};
        gf2d_camera_offset(&offsetedPos);

        if (map_manager.stale_map && (tile_z > 0 || *tile == 2))
        {
            map_manager.collision_map[tile_x][tile_y] = 1;
        }

        float scaleTileToCordsX = (tile_x - tile_y) * (tile_width  / 2.0f);
        float scaleTileToCordsY = (tile_x + tile_y) * (tile_height / 2.0f);

        GFC_Vector2D pos = gfc_vector2d(scaleTileToCordsX, scaleTileToCordsY);

        //slog("Original Position: %f %f | offset : %f %f %f", gfc_vector3d_to_slog(offsetedPos));
        
        pos.x -= offsetedPos.x;
        pos.y -= offsetedPos.y;
        pos.y -= tile_z * tile_height;

        GFC_Vector4D clip = {0,0,1,1};
        //slog("drawing map tile at %f, %f", pos.x, pos.y);
        gf2d_sprite_render(
            gf2d_map_get_file_by_idx(tile_DEF->tileset_file_idx),
            pos,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            &clip,
            tile_DEF->frame
        );
        //slog("Draw a tile at %i %i\n", tile_x,tile_y);
        tile++;
        tile_x++;
    }

    map_manager.stale_map = 0;


    
    #pragma endregion
}   
void map_update();
void map_think();

void gf2d_map_teleport_next()
{
    if (map_manager.level < map_manager.max_level)
    {
        map_manager.level++;
        map_manager.stale_map = 1;
        gf2d_entity_cleanup(1);
        char path[64];
        snprintf(path, 64, "%s%s%i%s", map_manager.map_binary_file_path, "map", map_manager.level, ".bin");
        slog("Opening map at path %s", path);
        FILE *file = fopen(path, "rb");
        if (file == NULL) {
            perror("Error opening binary file");
            return -1;
        }

        fread(map_manager.map, sizeof(Uint16), map_manager.tile_count, file);

        gf2d_weather_effect_night_end();
        gf2d_weather_effect_rain_end();
    }
    else
    {
        slog("Trying to go to next level when already at the end");
    }
    
}

void gf2d_map_reset()
{
    map_manager.level = 0;
    map_manager.stale_map = 1;
    char path[64];
    snprintf(path, 64, "%s%s%i%s", map_manager.map_binary_file_path, "map", map_manager.level, ".bin");
    slog("Opening map at path %s", path);
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Error opening binary file");
        return -1;
    }

    gf2d_weather_effect_night_start();
    gf2d_weather_effect_rain_start();

    fread(map_manager.map, sizeof(Uint16), map_manager.tile_count, file);
}


