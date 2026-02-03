#include <raylib.h>
#include "rayRPG_grid_movement.h"
#include <math.h>
 Vector2Grid DIRECTION_VECTORS[] = {
    (Vector2Grid){0, -1},
    (Vector2Grid){1, 0},
    (Vector2Grid){0, 1},
    (Vector2Grid){-1, 0},
};
int GRID_SIDE_LENGTH = 0;
void RRPG_set_grid_side_length(int g)
{
    static bool has_been_set = false;
    if(!has_been_set) {
        GRID_SIDE_LENGTH = g;
        has_been_set = true;
    } else {
        TraceLog(LOG_WARNING, "Did you mean to try to set \
            the grid side length again? You are not allowed to \
            reset the grid side length within one session.");
    }
}

void RRPG_DEBUG_draw_grid(int grid_side_length, int extent_x, int extent_y)
{
    for(int x = 0; x < extent_x; x += grid_side_length)
        for(int y = 0; y < extent_y; y += grid_side_length){
            Color color = RED;
            if ((x / grid_side_length + y / grid_side_length) % 2 == 0)
                color = ORANGE;
            else
                color = RED;
            color.a = 50;
            DrawRectangle(x, y, grid_side_length, grid_side_length, color);
        }            
}

Vector2Grid RRPG_raw_pos_to_grid(
    Vector2 raw_pos, 
    int grid_side_length)
{
    int x = (int)floor(raw_pos.x / grid_side_length);
    int y = (int)floor(raw_pos.y / grid_side_length);
    return (Vector2Grid){x, y};
}

Vector2 RRPG_grid_pos_to_raw(
    Vector2Grid grid_pos, 
    int grid_side_length)
{
    float rx = (float)grid_side_length * ((float)grid_pos.x + 0.5f);
    float ry = (float)grid_side_length * ((float)grid_pos.y + 0.5f);
    return (Vector2){rx, ry};
}

void RRPG_position_camera_on_grid(
    Camera2D *camera, 
    Vector2Grid grid_pos, 
    int grid_side_length)
{       
    camera->target = RRPG_grid_pos_to_raw(grid_pos, grid_side_length);
}

void RRPG_position_camera_on_player(
    Camera2D *camera,
    RRPG_PlayerController *player
)
{
    camera->target = player->sprite_position;
}

void RRPG_move_player(
    RRPG_PlayerController *player
)
{
    static Vector2Grid previous_position = (Vector2Grid){-9999, -9999};
    previous_position = player->position;
    player->position = (Vector2Grid){
        .x = previous_position.x + DIRECTION_VECTORS[player->facing].x,
        .y = previous_position.y + DIRECTION_VECTORS[player->facing].y};
    player->sprite_position = RRPG_grid_pos_to_raw(player->position, GRID_SIDE_LENGTH);

    
}

void RRPG_sense_movement_control(
    RRPG_PlayerController *player
) {
    if(IsKeyDown(KEY_W)) {
        player->is_moving = true;
        player->facing = DIRECTION_UP;
    } else if(IsKeyDown(KEY_A)) {
        player->is_moving = true;
        player->facing = DIRECTION_LEFT;
    } else if(IsKeyDown(KEY_S)) {
        player->is_moving = true;
        player->facing = DIRECTION_DOWN;
    } else if(IsKeyDown(KEY_D)) {
        player->is_moving = true;
        player->facing = DIRECTION_RIGHT;
    }else {
        player->is_moving = false;
    }
}