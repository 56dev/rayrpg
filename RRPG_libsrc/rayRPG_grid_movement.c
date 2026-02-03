#include "raylib.h"
#include "rayRPG_grid_movement.h"
#include <math.h>

void RRPG_DEBUG_draw_grid(int grid_side_length, int extent_x, int extent_y)
{
    for(int x = 0; x < extent_x; x += grid_side_length)
        for(int y = 0; y < extent_x; y += grid_side_length){
            Color color = RED;
            if ((x / grid_side_length + y / grid_side_length) % 2 == 0)
                color = ORANGE;
            else
                color = RED;
            color.a = 50;
            DrawRectangle(x, y, grid_side_length, grid_side_length, color);
        }            
}

Vector2Grid RRPG_raw_pos_to_grid(Vector2 raw_pos, int grid_side_length)
{
    int x = (int)floor(raw_pos.x / grid_side_length);
    int y = (int)floor(raw_pos.y / grid_side_length);
    return (Vector2Grid){x, y};
}

Vector2 RRPG_grid_pos_to_raw(Vector2Grid grid_pos, int grid_side_length)
{
    float rx = (float)grid_side_length * ((float)grid_pos.x + 0.5f);
    float ry = (float)grid_side_length * ((float)grid_pos.y + 0.5f);
    return (Vector2){rx, ry};
}

void RRPG_position_camera_on_grid(Camera2D *camera, Vector2Grid grid_pos, int grid_side_length)
{       
    camera->target = RRPG_grid_pos_to_raw(grid_pos, grid_side_length);
}

void RRPG_move_player(
    RRPG_PlayerController *player, 
    int direction, 
    Vector2Grid g_pos_begin, 
    Vector2Grid g_pos_end)
{
    if(!player->is_moving)
    
}