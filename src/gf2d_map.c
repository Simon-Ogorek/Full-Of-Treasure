#include "simple_json.h"
#include "simple_logger.h"
#include "gf2d_sprite.h"
#include "gf2d_map.h"
#include "gfc_hashmap.h"
#include "gfc_types.h"

#include <SDL_image.h>

#include "gf2d_camera.h"

#define Tile_Delimeter (short)65535;

typedef struct Tile
{
    unsigned char tile_idx;
    unsigned char flags;
}Tile;


static struct Map_Manager
{
    GFC_List *chunks;
    GFC_HashMap *tiles;

    Tile *map;
    int tile_count;

    SJson *map_info_JSON;
    int tile_width, tile_height;
}map_manager;

typedef struct Tile_Definition
{
    unsigned char tileset_file_idx;
    unsigned char frame;
}Tile_Definition;



Tile_Definition *fetch_tile(char* name)
{
    return (Tile_Definition *)gfc_hashmap_get(map_manager.tiles, name);
}

void gf2d_map_init(char *map_file, int editorMode)
{
    map_manager.chunks = gfc_list_new();
    map_manager.tiles = gfc_hashmap_new();

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
            NULL
        );

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
        
            tile->tileset_file_idx = 1; // TODO

            int tile_x, tile_y;
            
            SJson *tile_x_JSON = sj_object_get_value(tile_info_JSON, "x");
            sj_get_integer_value(tile_x_JSON, &tile_x);

            SJson *tile_y_JSON = sj_object_get_value(tile_info_JSON, "y");
            sj_get_integer_value(tile_x_JSON, &tile_y);

            tile->frame = (sheet_width * tile_y) + tile_x;

            SJson *tile_name_JSON = sj_object_get_value(tile_info_JSON, "name");
            char *tile_name = sj_get_string_value(tile_name_JSON);

            if (!tile_name_JSON || !tile_y_JSON || !tile_x_JSON)
            {
                slog("Bad JSON in %s at entry %i for map", map_file, j);
                continue;
            }

            gfc_hashmap_insert(map_manager.tiles, tile_name, tile);
            tile++;
        }
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
    map_manager.tile_count = 25000;

    map_manager.map = (Tile*)malloc(sizeof(Tile) * map_manager.tile_count);
    
    char * filepath = sj_get_string_value(sj_object_get_value(map_layout_JSON,"path"));

    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        perror("Error opening binary file");
        return -1;
    }

    fread(map_manager.map, sizeof(Tile), map_manager.tile_count, file);


    #pragma endregion
}

void gf2d_map_draw()
{
    #pragma region Map_Spawning

    //printf("===========================================\n");
    Tile *tile = map_manager.map;
    int tile_x = 0;
    int tile_y = 0;
    int tile_z = 0;
    //printf("Started");
    while (*(Uint16 *)tile != 0)
    {
        //printf("Tile: %hu\n", *tile);
        if (*(Uint16 *)tile == 65535)
        {
            tile_y++;
            tile_x=0;
            tile++;
            continue;
        }
        Tile_Definition* tile_DEF = fetch_tile  ("grass");
        int tile_width = map_manager.tile_width;
        int tile_height = map_manager.tile_height;

        GFC_Vector3D offsetedPos = {0};
        gf2d_camera_offset(&offsetedPos);

        float scaleTileToCordsX = (tile_width/2 * (tile_y % 2 == 0));
        float scaleTileToCordsY = tile_height/2 + (tile_height/2 * tile_z);

        GFC_Vector2D pos = gfc_vector2d(
            tile_width * tile_x + scaleTileToCordsX,
            tile_y * scaleTileToCordsY);

        //slog("Original Position: %f %f | offset : %f %f %f", gfc_vector3d_to_slog(offsetedPos));
        
        pos.x -= offsetedPos.x;
        pos.y -= offsetedPos.y;

        
        //slog("drawing map tile at %f, %f", pos.x, pos.y);
        gf2d_sprite_render(
            gf2d_sprite_load_image("map/BasicTileset.png"), // TODO TO MAKE A INDEX
            pos,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            tile_DEF->frame
        );
        //slog("Draw a tile at %i %i\n", tile_x,tile_y);
        tile++;
        tile_x++;
    }
    
    #pragma endregion
}   
void map_update();
void map_think();



