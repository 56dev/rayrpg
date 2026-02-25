#include <raylib.h>
#include "rayRPG_boilerplate.h"
#include "rayRPG_tiles.h"
#define CAMW 256*6
#define CAMH 144*6
int main() {
    RenderTexture2D target;
    Camera2D camera = (Camera2D){
        .offset = (Vector2){CAMW / 2, CAMH / 2},
        .target = (Vector2){0, 0},
        .rotation = 0.0f,
        .zoom = 6.0f,
    };
    
    RRPG_initialize(
        "rayRPG tilemaker",
        CAMW,
        CAMH,
        CAMW,
        CAMH,
        TEXTURE_FILTER_POINT,
        &target
    );

    //Texture2D atlas = RRPGTM_load_atlas("sprites/atlases/Sprite-0001.png");
    RRPG_TileSet tileset = RRPGTM_create_tileset_from_atlas("sprites/atlases/Sprite-0001.png", "data/tilesets/test.rrpgatl", 16);

    while(!WindowShouldClose()) {
        RRPG_render_adjust_mouse(CAMW, CAMH);

        if(GetMouseWheelMove() < 0) {
            camera.zoom -= GetFrameTime() * 20.0f;
        } else if(GetMouseWheelMove() > 0) {
            camera.zoom += GetFrameTime() * 20.0f;
        }

        BeginTextureMode(target);
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                RRPGTM_display_tileset(tileset, 16, camera);
                DrawText(
                    TextFormat("%.2f, %.2f", GetMousePosition().x, GetMousePosition().y),
                    -10, -10, 10, RED
                );
            EndMode2D();
        EndTextureMode();

        RRPG_draw_to_screen(CAMW, CAMH, &target);
    }
    RRPG_end(&target);
}