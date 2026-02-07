#include <raylib.h>
#include "rayRPG_boilerplate.h"
#include "rayRPG_grid_movement.h"
#include "rayRPG_extra_debug.h"
#include "npc.h"
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
    
    RRPG_EntityController player = {0};
    RRPG_EntityController npc = {0};
    RRPG_entity_controller_constructor(&player, (RRPG_Vector2Grid){0, 0}, 3.0f, col_grid);
    RRPG_entity_controller_constructor(&npc, (RRPG_Vector2Grid){3, 3}, 3.0f, col_grid);
    float timer = 0.0f;

    while(!WindowShouldClose())
    {
        RRPG_render_adjust_mouse(CAMW, CAMH);

        BeginTextureMode(target);
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);                
                
                RRPG_walk_entity(&player, RRPG_PLAYER_sense_movement_control(), col_grid);
                RRPG_PLAYER_position_camera_on_player(&camera, &player);
                RRPG_DEBUG_draw_grid(GRID_SIDE, CAMW, CAMH);
                //RRPG_DEBUG_draw_collision_grid(&col_grid);
                npc_random_move(&npc, &timer, col_grid);
                DrawCircle(npc.sprite_position.x, npc.sprite_position.y, 2, BLUE);
            EndMode2D();
            
            RRPG_PLAYER_DEBUG_dispay_player_info(&player);
            RRPG_DEBUG_display_movement_control((Vector2) { 50 , 50 }, 0.5f);

            DrawCircle(CAMW / 2, CAMH / 2, 2, RED);
            
            

        EndTextureMode();

        RRPG_draw_to_screen(CAMW, CAMH, &target);
    }
    RRPG_destroy_collision_grid(&col_grid);
    RRPG_end(&target);

}