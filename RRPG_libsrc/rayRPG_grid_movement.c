#include <raylib.h>
#include "rayRPG_grid_movement.h"
#include <math.h>
#include <raymath.h>
#include <limits.h>
 RRPG_Vector2Grid RRPG_DIRECTION_VECTORS[] = {
    (RRPG_Vector2Grid){0, -1},
    (RRPG_Vector2Grid){1, 0},
    (RRPG_Vector2Grid){0, 1},
    (RRPG_Vector2Grid){-1, 0},
};
int RRPG_GRID_SIDE_LENGTH = 0;
void RRPG_set_grid_side_length(int g)
{
    static bool has_been_set = false;
    if(!has_been_set) {
        RRPG_GRID_SIDE_LENGTH = g;
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

RRPG_Vector2Grid RRPG_raw_pos_to_grid(
    Vector2 raw_pos, 
    int grid_side_length)
{
    int x = (int)floor(raw_pos.x / grid_side_length);
    int y = (int)floor(raw_pos.y / grid_side_length);
    return (RRPG_Vector2Grid){x, y};
}

Vector2 RRPG_grid_pos_to_raw(
    RRPG_Vector2Grid grid_pos, 
    int grid_side_length)
{
    float rx = (float)grid_side_length * ((float)grid_pos.x + 0.5f);
    float ry = (float)grid_side_length * ((float)grid_pos.y + 0.5f);
    return (Vector2){rx, ry};
}

void RRPG_position_camera_on_grid(
    Camera2D *camera, 
    RRPG_Vector2Grid grid_pos, 
    int grid_side_length)
{       
    camera->target = RRPG_grid_pos_to_raw(grid_pos, grid_side_length);
}

void RRPG_PLAYER_constructor(
    RRPG_PlayerController *player,
    RRPG_Vector2Grid starting_grid_position,
    float speed
){
    player->position = starting_grid_position;
    player->sprite_position = RRPG_grid_pos_to_raw(player->position, RRPG_GRID_SIDE_LENGTH);
    player->facing = DIRECTION_DOWN;
    player->speed = speed;
    player->is_moving = false;
}

void RRPG_PLAYER_position_camera_on_player(
    Camera2D *camera,
    RRPG_PlayerController *player
)
{
    camera->target = player->sprite_position;
}

void RRPG_PLAYER_move_player(
    RRPG_PlayerController *player,
    int direction
)
{
    static RRPG_Vector2Grid previous_position = (RRPG_Vector2Grid){INT_MIN, INT_MIN};
    static Vector2 next_sprite_position = (Vector2){INFINITY, INFINITY};
    static bool has_started_moving_sprite = false;
    static float progress = 0.0f;
    if(direction >= 0 && direction <= 3) {
        if(!player->is_moving) {
            player->is_moving = true;
            goto SET;
        }        
    }
    if(player->is_moving) {
        Vector2 previous_sprite_position = RRPG_grid_pos_to_raw(previous_position, 
            RRPG_GRID_SIDE_LENGTH);        
            next_sprite_position = RRPG_grid_pos_to_raw(player->position, RRPG_GRID_SIDE_LENGTH);

        progress += player->speed * GetFrameTime();
        if(progress >= 1.0f) progress = 1.0f;

        player->sprite_position.x = Lerp(
            previous_sprite_position.x, 
            next_sprite_position.x, 
            progress
        );
        player->sprite_position.y = Lerp(
            previous_sprite_position.y, 
            next_sprite_position.y, 
            progress
        );

        if(progress == 1.0f)
        {
            if(direction >= 0 && direction <= 3) {
                goto SET;

            }else {
                player->is_moving = false;
                
            }
            
        }
    }
    return;

    
    SET:
    player->facing = direction;
    previous_position = player->position;
    player->position = (RRPG_Vector2Grid){
    .x = previous_position.x + RRPG_DIRECTION_VECTORS[player->facing].x,
    .y = previous_position.y + RRPG_DIRECTION_VECTORS[player->facing].y};
    progress = 0.0f;
        
}


    

    


void __util_push_back(int *a, int n, int insert)
{
    for(int i = 0; i < n - 1; ++i)
    {
        a[i] = a[i + 1];
    }
    a[n - 1] = insert;
}
/*probably not the loveliest implementation, but whatever*/
void __util_remove(int *a, int n, int r)
{
    int i;
    for(i = 0; i < n; ++i)
    {
        if(a[i] == r) {
            //push all previous elements forward to fill the space
            for(; i > 0; --i)
            {
                a[i] = a[i - 1];
            }
            a[0] = -1;
            break;
        } 
    }

    
    
    
}


int RRPG_PLAYER_sense_movement_control(

) {
    static int buf[4] = { -1, -1, -1, -1 };
    if(IsKeyPressed(KEY_W)) {  
        __util_push_back(buf, 4, DIRECTION_UP);
    }  if(IsKeyPressed(KEY_A)) {
        __util_push_back(buf, 4, DIRECTION_LEFT);
    }  if(IsKeyPressed(KEY_S)) {
        __util_push_back(buf, 4, DIRECTION_DOWN);
    }  if(IsKeyPressed(KEY_D)) {
        __util_push_back(buf, 4, DIRECTION_RIGHT);
    } 

    if(IsKeyReleased(KEY_W)) {
        __util_remove(buf, 4, DIRECTION_UP);
    }
    if(IsKeyReleased(KEY_A)) {
        __util_remove(buf, 4, DIRECTION_LEFT);
    }
    if(IsKeyReleased(KEY_S)) {
        __util_remove(buf, 4, DIRECTION_DOWN);
    }
    if(IsKeyReleased(KEY_D)) {
        __util_remove(buf, 4, DIRECTION_RIGHT);
    }
    return buf[3];
    
}

void RRPG_PLAYER_DEBUG_dispay_player_info(
    RRPG_PlayerController *player
) {
    char *text = "";
    switch(player->facing){
        case DIRECTION_DOWN:
        text = "facing down";
        break;
        case DIRECTION_LEFT:
        text = "facing left";
        break;
        case DIRECTION_RIGHT:
        text = "facing right";
        break;
        case DIRECTION_UP:
        text = "facing up";
        break;
    }
    DrawText(text, 0, 0, 10, RED);
    DrawText((player->is_moving ? "is moving" : "is not moving"), 0, 10, 10, RED);
}