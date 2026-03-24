#include "gf2d_camera.h"
#include "simple_logger.h"
static Camera camera = {0};

void gf2d_camera_init(GFC_Vector3D starting_position, int smoothing, float speed)
{
    camera.position = starting_position;

    camera.smoothing = smoothing;
    camera.speed = speed;

    camera.target = NULL;
    camera.target_position = starting_position;
}

void gf2d_camera_assign_position(GFC_Vector3D new_position)
{
    camera.target_position = new_position;
    camera.target = NULL;
}
void gf2d_camera_assign_target(Entity *new_target)
{
    camera.target = new_target;
}

void gf2d_camera_update()
{
    if (camera.target)
    {
        gfc_vector3d_copy(camera.position, camera.target->position);  
        camera.position.x -= 400/3;
        camera.position.y -= 240/4;
        
    }
    /*
    

    GFC_Vector3D difference;
    difference = gfc_vector3d_subbed(camera.target_position, camera.position);
    slog("camera %f %f %f | target %f %f %f", gfc_vector3d_to_slog(camera.position), gfc_vector3d_to_slog(camera.target_position));
    float distance = gfc_vector3d_magnitude(difference);

    gfc_vector3d_normalize(&difference);

    slog("camera heading : %f %f %f ", gfc_vector3d_to_slog(difference));
    gfc_vector3d_add(camera.position, camera.position, difference);
    */



    
}

Uint8 gf2d_camera_is_target(Entity *ptr)
{
    return (ptr == camera.target);
}

void gf2d_camera_offset(GFC_Vector3D *input)
{
    input->x += camera.position.x;
    input->y += camera.position.y;
    input->z += camera.position.z;

}

GFC_Vector3D * gf2d_camera_get_offset()
{
    return &camera.position;

}
