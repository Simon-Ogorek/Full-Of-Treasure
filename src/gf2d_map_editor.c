#include "gf2d_map_editor.h"
#include "gf2d_entity.h"
#include "gf2d_map.h"
static struct Map_Editor
{
    GFC_Vector3D position;

    Entity *cursor_ent;
    int cursor_tile_x;
    int cursor_tile_y;
    int cursor_tile_z;
    int cursor_speed;

    Tile *next_tile_ptr; 
    Uint8 curr_idx;
}editor;

void gf2d_map_editor_init(Camera *cam)
{
    gf2d_camera_assign_position(editor.position);
    editor.cursor_ent = gf2d_create_entity("cursor");
    editor.cursor_tile_x = 0;
    editor.cursor_tile_y = 0;
    editor.cursor_tile_z = 0;
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

    if (keys[SDL_SCANCODE_SPACE])
    {
        editor.cursor_tile_x += editor.cursor_speed;
    }
}
void gf2d_map_editor_update();
void gf2d_map_editor_draw();