#include "gf2d_map_editor.h"
#include "gf2d_entity.h"
#include "gf2d_map.h"

typedef struct Editor_Tile
{
    Tile_Definition *def;
    int x;
    int y;
    int z;
} Editor_Tile;

static struct Map_Editor
{
    GFC_Vector3D position;

    Entity *cursor_ent;
    int cursor_tile_x;
    int cursor_tile_y;
    int cursor_tile_z;
    int cursor_speed;

    Uint8 curr_idx;

    GFC_List *new_tiles;

    char* map_binary_path;
}editor;

void gf2d_map_editor_init(Camera *cam)
{
    gf2d_camera_assign_position(editor.position);
    editor.cursor_ent = gf2d_create_entity("cursor");
    editor.cursor_tile_x = 0;
    editor.cursor_tile_y = 0;
    editor.cursor_tile_z = 0;

    editor.new_tiles = gfc_list_new();

    editor.map_binary_path = gf2d_map_get_binary_path();
}

void gf2d_map_editor_think()
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_W])
    {
        editor.cursor_tile_y += editor.cursor_speed;
    }
    if (keys[SDL_SCANCODE_S])
    {
        editor.cursor_tile_y -= editor.cursor_speed;
    }
    if (keys[SDL_SCANCODE_A])
    {
        editor.cursor_tile_x -= editor.cursor_speed;
    }
    if (keys[SDL_SCANCODE_D])
    {
        editor.cursor_tile_x += editor.cursor_speed;
    }
    if (keys[SDL_SCANCODE_LCTRL])
    {
        editor.cursor_tile_z -= editor.cursor_speed;
    }
    if (keys[SDL_SCANCODE_LSHIFT])
    {
        editor.cursor_tile_z += editor.cursor_speed;
    }

    if (keys[SDL_SCANCODE_SPACE] && editor.curr_idx != 0)
    {
        Editor_Tile *next_tile = (Editor_Tile*)malloc(sizeof(Editor_Tile));
        next_tile->def = gf2d_map_get_tile(editor.curr_idx);
        next_tile->x = editor.cursor_tile_x;
        next_tile->y = editor.cursor_tile_y;
        next_tile->z = editor.cursor_tile_z;

        gfc_list_append(editor.new_tiles, next_tile);
    }

    if (keys[SDL_SCANCODE_TAB] && keys[SDL_SCANCODE_DELETE])
    {
        gf2d_map_editor_save();
    }
}
void gf2d_map_editor_update();
void gf2d_map_editor_draw();

int compare_tiles(void *a, void *b)
{
    Editor_Tile A = *(Editor_Tile *)a;
    Editor_Tile B = *(Editor_Tile *)b;

    if (A.z != B.z)
    {
        if (A.z < B.z)
            return -1;
        else
            return 1;
    }
    if (A.y != B.y)
    {
        if (A.y < B.y)
            return -1;
        else
            return 1;
    }
    if (A.x != B.x)
    {
        if (A.x < B.x)
            return -1;
        else
            return 1;
    }

    return 0;
}

void gf2d_map_editor_save()
{
    gfc_list_sort(editor.new_tiles, compare_tiles);

    int x,y,z = 0;

    int next_tile_list_idx = 0;
    Editor_Tile *next_tile = (Editor_Tile*)gfc_list_get_nth(editor.new_tiles, next_tile_list_idx);
    
    FILE *file = fopen(editor.map_binary_path, "rb");
    if (file == NULL) {
        perror("Error opening binary file");
        return -1;
    }

    Uint8 next_binary;
    fread(&next_binary, sizeof(Uint8), 1, file);

    unsigned short blank = 1;
    unsigned short data = 256;
    unsigned short yDelim = 65535;
    unsigned short zDelim = 65534;

    FILE *file_write = fopen("mapNEW.bin", "wb");

    data = blank;

    while (file || next_tile_list_idx < gfc_list_get_count(editor.new_tiles)) 
    {


        fwrite(&data, sizeof(unsigned short),1, file_write);

    }
}