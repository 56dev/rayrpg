#include "rayRPG_grid_movement.h"
#include <raylib.h>

void npc_random_move(
    RRPG_EntityController *npc,
    float *timer,
    RRPG_CollisionGrid col_grid
) {
    if(npc->is_moving) {
        RRPG_walk_entity(npc, -1, col_grid);
        return;
    }
    if(*timer > 0.0f) {
        *timer -= GetFrameTime();
        return;
    }
    
    *timer = GetRandomValue(0, 3);
    
    int direction = GetRandomValue(0, 3);
    RRPG_walk_entity(npc, direction, col_grid);

    
    
}