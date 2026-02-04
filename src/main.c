#include <raylib.h>
#include "rayRPG_boilerplate.h"
#include "rayRPG_grid_movement.h"
#define CAMW 200
#define CAMH 200
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
        1000,
        1000,
        TEXTURE_FILTER_POINT,
        &target        
    );

    RRPG_set_grid_side_length(GRID_SIDE);
    RRPG_PlayerController player = {0};
    RRPG_PLAYER_constructor(&player, (RRPG_Vector2Grid){0, 0}, 3.0f);
    while(!WindowShouldClose())
    {
        RRPG_set_render(CAMW, CAMH);
        BeginTextureMode(target);
            ClearBackground(RAYWHITE);
            
            BeginMode2D(camera);
                
                int d = RRPG_PLAYER_sense_movement_control();
                RRPG_PLAYER_move_player(&player, d);
                RRPG_DEBUG_draw_grid(GRID_SIDE, CAMW, CAMH);
                RRPG_PLAYER_position_camera_on_player(&camera, &player);
            EndMode2D();
            
            Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
            RRPG_Vector2Grid grid_pos = RRPG_raw_pos_to_grid(mouse_pos, GRID_SIDE);
            // DrawText(TextFormat("raw: %.2f, %.2f", (mouse_pos.x), (mouse_pos.y)), 0, 0, 10, BLACK);
            // DrawText(TextFormat("grid: %i, %i", (grid_pos.x), (grid_pos.y)), 0, 10, 10, BLACK);
            RRPG_PLAYER_DEBUG_dispay_player_info(&player);
            DrawCircle(CAMW / 2, CAMH / 2, 2, RED);
            
            

        EndTextureMode();
        RRPG_draw_to_screen(CAMW, CAMH, &target);
    }

    RRPG_end(&target);

}