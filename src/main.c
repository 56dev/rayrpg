#include <raylib.h>
#include "rayRPG_boilerplate.h"
#include "rayRPG_grid_movement.h"
#include "rayRPG_extra_debug.h"
#define CAMW 256
#define CAMH 144
#define GRID_SIDE 16 //px

int main(){

    RenderTexture2D target;
    Camera2D camera = { 0 };
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){CAMW / 2, CAMH / 2};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    RRPG_initialize(
        "RPG",
        CAMW,
        CAMH,
        1280,
        720,
        TEXTURE_FILTER_POINT,
        &target        
    );
    
    RRPG_set_grid_side_length(GRID_SIDE);
    RRPG_Vector2Grid occupied[2] = {
        (RRPG_Vector2Grid) {0, 1},
        (RRPG_Vector2Grid) {0, 2},
    };
    RRPG_CollisionGrid col_grid = RRPG_initialize_collision_grid(
        10,
        10,
        occupied, 
        2
    );
    RRPG_PlayerController player = {0};
    RRPG_PLAYER_constructor(&player, (RRPG_Vector2Grid){0, 0}, 3.0f, col_grid);
    
    while(!WindowShouldClose())
    {
        RRPG_render_adjust_mouse(CAMW, CAMH);
        BeginTextureMode(target);
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                
                int d = RRPG_PLAYER_sense_movement_control();
                RRPG_PLAYER_move_player(&player, d, col_grid);
                RRPG_DEBUG_draw_grid(GRID_SIDE, CAMW, CAMH);
                RRPG_PLAYER_position_camera_on_player(&camera, &player);
                RRPG_DEBUG_draw_collision_grid(&col_grid);
            EndMode2D();
            
            Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
            RRPG_Vector2Grid grid_pos = RRPG_raw_pos_to_grid(mouse_pos, GRID_SIDE);
            RRPG_PLAYER_DEBUG_dispay_player_info(&player);
            RRPG_DEBUG_display_movement_control((Vector2) { 50 , 50 }, 0.5f);
            DrawCircle(CAMW / 2, CAMH / 2, 2, RED);
            
            

        EndTextureMode();
        RRPG_draw_to_screen(CAMW, CAMH, &target);
    }

    RRPG_end(&target);

}