#include "raylib.h"
#include "rayRPG_boilerplate.h"

int main(){

    RenderTexture2D target;
    #define CAMW 100
    #define CAMH 100
    RRPG_initialize(
        "RPG",
        CAMW,
        CAMH,
        1000,
        1000,
        TEXTURE_FILTER_POINT,
        &target        
    );

    while(!WindowShouldClose())
    {
        RRPG_set_render(CAMW, CAMH);
        BeginTextureMode(target);
        DrawText("Hello!", 1, 1, 10, RED);
        EndTextureMode();
        RRPG_draw_to_screen(CAMW, CAMH, &target);
    }

    RRPG_end(&target);

}