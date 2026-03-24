#include "gf2d_enemy.h"
#include "gf2d_player.h"
#include "simple_logger.h"
static struct Enemy_Manager
{
    Enemy *all_enemies;
    Player *player;
    int count;
} enemyManager;

void gf2d_enemy_init(int count)
{
    int i;

    enemyManager.player = gf2d_player_get();

    enemyManager.all_enemies = (Enemy *)gfc_allocate_array(sizeof(Enemy), count);
    memset(enemyManager.all_enemies,0,sizeof(Enemy)*count);

    enemyManager.count = count;
}

Enemy * gf2d_create_enemy(char *name)
{
    Enemy *enemy;
    int i;

    for (i = 0; i < enemyManager.count; i++)
    {
        enemy = &enemyManager.all_enemies[i];

        if (enemy->state == ES_Inactive)
        {
            enemy->state = ES_Unaware;
            
            enemy->ent = gf2d_create_entity(name);

            enemy->ent->think = &gf2d_think_enemy;
            enemy->ent->update = &gf2d_update_enemy;
            enemy->ent->status = Active;

            enemy->cooldown = -1;
            enemy->awareness_range = 400;
            enemy->attack_range = 5;

            enemy->ent->position.x = 200;
            enemy->ent->position.y = 200;
            enemy->ent->speed = 1;

            enemy->ent->calling_parent = (void *)enemy;

            slog("ptr of enemy is %p", (void *)enemy);
            
            //slog("%f speed %i", ent->animation_speed, code);
            return enemy;
        }
    }

    slog("No inactive enemy found");
    return NULL;
}

void perform_attack(Enemy *enemy)
{
    enemy->cooldown = 12;
    enemyManager.player->ent->health--;
    return;
}

void gf2d_think_enemy(Entity *ent)
{   
    Enemy *enemy = (Enemy *)ent->calling_parent;
    if (enemy->state == ES_Inactive)
    {
        slog("trying to think on a Inactive enemy");
        return;
    }

    slog("enemy state is %i", enemy->state);
    slog("distance is %f, awareness: %f, range: %f", gfc_vector3d_magnitude_between(ent->position, enemyManager.player->ent->position) ,enemy->awareness_range, enemy->attack_range);

    ent->status = Active;

    if (enemy->state == ES_Unaware)
    {
        if (gfc_vector3d_magnitude_between(ent->position, enemyManager.player->ent->position) < enemy->awareness_range)
        {
            enemy->state = ES_Chase;
        }
    }

    if (enemy->state == ES_Chase)
    {
        if (gfc_vector3d_magnitude_between(ent->position, enemyManager.player->ent->position) > enemy->awareness_range)
        {
            enemy->state = ES_Unaware;
        }
        if (gfc_vector3d_magnitude_between(ent->position, enemyManager.player->ent->position) <= enemy->attack_range)
        {
            enemy->state = ES_Attack;
            
        }
    }

    if (enemy->state == ES_Attack)
    {
        if (enemy->cooldown >= 0 )
        {
            enemy->state = ES_Unaware;
        }
    }

    enemy->cooldown -= 0.1;

    switch (enemy->state)
    {

        case ES_Unaware:
            if (gfc_random_int(600) == 1)
            {
                GFC_Vector3D randomDir;
                gfc_vector3d_set(randomDir, 1,1,1);
                gfc_vector3d_randomize(&randomDir, randomDir);
                gfc_vector3d_normalize(&randomDir);
                gfc_vector3d_scale(randomDir, randomDir, enemy->ent->speed);

                enemy->random_walk.x = randomDir.x;
                enemy->random_walk.y = randomDir.y;
                enemy->random_walk.z = randomDir.z;
            }

            gfc_vector3d_add(enemy->ent->position, enemy->ent->position,enemy->random_walk);


            
            break;

        case ES_Chase:
            GFC_Vector3D direction = gfc_vector3d_subbed(enemyManager.player->ent->position, enemy->ent->position);
            gfc_vector3d_normalize(&direction);
            gfc_vector3d_scale(direction, direction, enemy->ent->speed);

            slog("moving enemy by (%f,%f,%f)", gfc_vector3d_to_slog(direction) );

            gfc_vector3d_add(enemy->ent->position, enemy->ent->position, direction);
            break;
        
        case ES_Attack:
            perform_attack(enemy);
            break;
    }

    slog("enemy pos : (%f,%f,%f)", gfc_vector3d_to_slog(enemy->ent->position));
}

void gf2d_update_enemy(Entity *ent)
{
    ent->animation_frame += ent->animation_speed;
    ent->animation_frame = (ent->animation_frame >= ent->sprite->total_frames-1) ? 0 : ent->animation_frame;
}

void gf2d_delete_enemy(Enemy *enemy)
{

}
