#ifndef __GF2D_CAMERA_H__
#define __GF2D_CAMERA_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

#include "gfc_list.h"

#include "gf2d_entity.h"

typedef struct Camera_S
{
    GFC_Vector3D position;
    GFC_Vector3D target_position;
    Entity *target;
    int smoothing;
    int speed;
}Camera;


void gf2d_camera_init(GFC_Vector3D starting_position, int smoothing, float speed);
void gf2d_camera_assign_position(GFC_Vector3D new_position);
void gf2d_camera_assign_target(Entity *new_target);

void gf2d_camera_update();

void gf2d_camera_offset(GFC_Vector3D *input);


#endif