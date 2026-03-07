#include <raylib.h>
#include "rayRPG_grid_movement.h"
#include <math.h>
#include <raymath.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
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

void RRPG_entity_controller_constructor(
    RRPG_EntityController *entity,
    RRPG_Vector2Grid starting_grid_position,
    float speed,
    RRPG_CollisionGrid col_grid
){
    entity->position = starting_grid_position;
    entity->sprite_position = RRPG_grid_pos_to_raw(entity->position, RRPG_GRID_SIDE_LENGTH);
    entity->facing = DIRECTION_DOWN;
    entity->speed = speed;
    entity->is_moving = false;
    entity->xxprv_move_progress = 0.0f;
    entity->xxprv_next_sprite_position = (Vector2){INFINITY, INFINITY};
    entity->xxprv_previous_position = entity->position;
    RRPG_add_to_collision_state(&col_grid, entity->position);
}

void RRPG_PLAYER_position_camera_on_player(
    Camera2D *camera,
    RRPG_EntityController *player
)
{
    //camera->target = player->sprite_position;
    camera->target.x = (int)player->sprite_position.x;
    camera->target.y = (int)player->sprite_position.y;

}

void RRPG_walk_entity(
    RRPG_EntityController *entity,
    int direction_to_move,
    RRPG_CollisionGrid collision_grid
)
{
    /*
        //UNCOMMENT FOR DEBUG
        Vector2 p = RRPG_grid_pos_to_raw(entity->xxprv_previous_position, RRPG_GRID_SIDE_LENGTH);
        DrawRectangle(p.x - RRPG_GRID_SIDE_LENGTH / 2, p.y - RRPG_GRID_SIDE_LENGTH / 2, RRPG_GRID_SIDE_LENGTH, RRPG_GRID_SIDE_LENGTH, GREEN);
    */
    if(direction_to_move >= 0 && direction_to_move <= 3) {
        if(!entity->is_moving) {
            goto SET;
        }        
    }
    if(entity->is_moving) {
        Vector2 previous_sprite_position = RRPG_grid_pos_to_raw(entity->xxprv_previous_position, 
            RRPG_GRID_SIDE_LENGTH);        
            entity->xxprv_next_sprite_position = RRPG_grid_pos_to_raw(entity->position, RRPG_GRID_SIDE_LENGTH);

        entity->xxprv_move_progress += entity->speed * GetFrameTime();
        if(entity->xxprv_move_progress >= 1.0f) entity->xxprv_move_progress = 1.0f;

        entity->sprite_position.x = Lerp(
            previous_sprite_position.x, 
            entity->xxprv_next_sprite_position.x, 
            entity->xxprv_move_progress
        );
        entity->sprite_position.y = Lerp(
            previous_sprite_position.y, 
            entity->xxprv_next_sprite_position.y, 
            entity->xxprv_move_progress
        );

        if(entity->xxprv_move_progress == 1.0f)
        {
            if(direction_to_move >= 0 && direction_to_move <= 3) {
                goto SET;

            }else {
                entity->is_moving = false;                
            }            
        }
    }

    return;
   
    SET:
        entity->facing = direction_to_move;        
        
        RRPG_Vector2Grid np = (RRPG_Vector2Grid){
            .x = entity->position.x + RRPG_DIRECTION_VECTORS[entity->facing].x,
            .y = entity->position.y + RRPG_DIRECTION_VECTORS[entity->facing].y
        };
        if(RRPG_return_collision_state_at_position(collision_grid, np) == 0) {
            
            entity->xxprv_previous_position = entity->position;
            entity->position = np;
            entity->xxprv_move_progress = 0.0f;
            RRPG_add_to_collision_state(&collision_grid, np);            
            RRPG_subtract_from_collision_state(&collision_grid, entity->xxprv_previous_position);
            entity->is_moving = true;
            
        }else {
            entity->is_moving = false;
        }
        
}

void RRPG_position_entity(
    RRPG_EntityController *entity,
    RRPG_Vector2Grid position,
    RRPG_CollisionGrid col_grid
) {
    RRPG_subtract_from_collision_state(&col_grid, entity->position);
    entity->position = position;
    entity->sprite_position = RRPG_grid_pos_to_raw(entity->position, RRPG_GRID_SIDE_LENGTH);
    entity->is_moving = false;
    entity->xxprv_move_progress = 0.0f;
    entity->xxprv_next_sprite_position = (Vector2){INFINITY, INFINITY};
    entity->xxprv_previous_position = entity->position;
    RRPG_add_to_collision_state(&col_grid, entity->position);
}


    

    


void xxprv_util_push_back(int *a, int n, int insert)
{
    for(int i = 0; i < n - 1; ++i) {
        a[i] = a[i + 1];
    }
    a[n - 1] = insert;
}
/*probably not the loveliest implementation, but whatever*/
void xxprv_util_remove(int *a, int n, int r)
{
    int i;
    for(i = 0; i < n; ++i){
        if(a[i] == r) {
            //push all previous elements forward to fill the space
            for(; i > 0; --i) {
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
        xxprv_util_push_back(buf, 4, DIRECTION_UP);
    }  if(IsKeyPressed(KEY_A)) {
        xxprv_util_push_back(buf, 4, DIRECTION_LEFT);
    }  if(IsKeyPressed(KEY_S)) {
        xxprv_util_push_back(buf, 4, DIRECTION_DOWN);
    }  if(IsKeyPressed(KEY_D)) {
        xxprv_util_push_back(buf, 4, DIRECTION_RIGHT);
    } 

    if(IsKeyReleased(KEY_W)) {
        xxprv_util_remove(buf, 4, DIRECTION_UP);
    }
    if(IsKeyReleased(KEY_A)) {
        xxprv_util_remove(buf, 4, DIRECTION_LEFT);
    }
    if(IsKeyReleased(KEY_S)) {
        xxprv_util_remove(buf, 4, DIRECTION_DOWN);
    }
    if(IsKeyReleased(KEY_D)) {
        xxprv_util_remove(buf, 4, DIRECTION_RIGHT);
    }
    return buf[3];
    
}

void RRPG_PLAYER_DEBUG_dispay_player_info(
    RRPG_EntityController *player
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

/*******COLLISIONS*********/

RRPG_CollisionGrid RRPG_initialize_collision_grid(
    int extent_x, //In # of grid cells, not px.
    int extent_y,//In # of grid cells, not px.
    RRPG_Vector2Grid *occupied, //testing
    int num_occupied
) {
    RRPG_CollisionGrid grid = { 0 };

    grid.extent_x = extent_x;
    grid.extent_y = extent_y;
    int *cells = malloc(extent_x * extent_y * sizeof(int));
    if(!cells) {
        return (RRPG_CollisionGrid) { 0 };
    }
    for(int i = 0; i < extent_x * extent_y; ++i) {
        cells[i] = 0;
    }
    for(int i = 0; i < num_occupied; ++i) {
        int idx = occupied[i].y * extent_x + occupied[i].x;
        if(idx < 0 || idx >= extent_x * extent_y) continue;
        cells[idx] = -1;
    }
    grid.cells = cells;
    return grid;
}

void RRPG_destroy_collision_grid(
    RRPG_CollisionGrid *grid
) {
    free(grid->cells);
    grid->extent_x = 0;
    grid->extent_y = 0;
}

void RRPG_DEBUG_draw_collision_grid(
    const RRPG_CollisionGrid *grid
) {
    for(int i = 0; i < grid->extent_x * grid->extent_y; ++i) {
        if(grid->cells[i] != 0) {
            RRPG_Vector2Grid cell = (RRPG_Vector2Grid){i % grid->extent_x, (int)floor(i / grid->extent_y)};
            Vector2 rect_pos = RRPG_grid_pos_to_raw(cell, RRPG_GRID_SIDE_LENGTH);
            rect_pos.x -= RRPG_GRID_SIDE_LENGTH / 2.0f;
            rect_pos.y -= RRPG_GRID_SIDE_LENGTH / 2.0f;
            DrawRectangle(rect_pos.x, rect_pos.y, RRPG_GRID_SIDE_LENGTH, RRPG_GRID_SIDE_LENGTH, (grid->cells[i] < 0 ? BLACK : BLUE));
        }        
    }
    DrawRectangleLines(0, 0, grid->extent_x * RRPG_GRID_SIDE_LENGTH, grid->extent_y * RRPG_GRID_SIDE_LENGTH, RED);
}

int RRPG_return_collision_state_at_position(
    RRPG_CollisionGrid grid, 
    RRPG_Vector2Grid cell) {
    int idx = cell.y * grid.extent_x + cell.x;
    if(cell.y < 0 || cell.x < 0 || cell.y >= grid.extent_y || cell.x >= grid.extent_x) return -9999;
    return grid.cells[idx];
}

void RRPG_add_to_collision_state(
    RRPG_CollisionGrid *grid,
    RRPG_Vector2Grid cell
) {
    int idx = cell.y * grid->extent_x + cell.x;
    if(cell.y < 0 || cell.x < 0 || cell.y >= grid->extent_y || cell.x >= grid->extent_x) return;
    if(grid->cells[idx] < 0) {
        TraceLog(LOG_WARNING, "RRPG_add_to_collision_state: why are you trying to add a collider to a cell that is permanently blocked?");
        return;
    }
    ++grid->cells[idx];
}

void RRPG_subtract_from_collision_state(
    RRPG_CollisionGrid *grid,
    RRPG_Vector2Grid cell
) {
    int idx = cell.y * grid->extent_x + cell.x;
    if(cell.y < 0 || cell.x < 0 || cell.y >= grid->extent_y || cell.x >= grid->extent_x) return;
    if(grid->cells[idx] <= 0) {
        TraceLog(LOG_WARNING, "RRPG_subtract_to_collision_state: why are you trying to subtract a collider from a cell that is permanently or free?");
        return;
    }
    --grid->cells[idx];
}