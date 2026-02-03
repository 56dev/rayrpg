#include "rayRPG_boilerplate.h"
#include "raylib.h"
#include "raymath.h"

/*
    Does most of the initialization by itself.
*/
void RRPG_initialize(
    const char *title,
    int camera_width, /*Width of the camera in px.*/
    int camera_height, /*Height of the camera in px.*/
    int starting_screen_width, /*Width of the screen at startup, in px.*/
    int starting_screen_height, /*Height of the screen at startup, in px.*/
    int texture_filter, 
    RenderTexture2D *OUT_target /*Reference to the texture you will use to render the game.*/
)
{
    SetConfigFlags( FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT );
    InitWindow(starting_screen_width, starting_screen_height, title);
    SetWindowMinSize(300, 300);
    SetTargetFPS(60);
    *OUT_target = LoadRenderTexture(camera_width, camera_height);
    SetTextureFilter(OUT_target->texture, texture_filter);
    InitAudioDevice();
}
#define MIN(a,b) ((a) < (b) ? (a) : (b))

void RRPG_set_render(
    int camera_width,
    int camera_height
){
    float scale = MIN((float)GetScreenWidth()/camera_width, (float)GetScreenHeight()/camera_height);
    Vector2 mouse = GetMousePosition();
    Vector2 virtualMouse = { 0 };
    virtualMouse.x = (mouse.x - (GetScreenWidth() - (camera_width*scale))*0.5f)/scale;
    virtualMouse.y = (mouse.y - (GetScreenHeight() - (camera_height*scale))*0.5f)/scale;
    virtualMouse = Vector2Clamp(
        virtualMouse, 
        (Vector2){0, 0}, 
        (Vector2){ 
            (float)camera_width, 
            (float)camera_height} 
        );
    SetMouseOffset((-(GetScreenWidth() - (camera_width*scale))*0.5f),
                (-(GetScreenHeight() - (camera_height*scale))*0.5f));
    SetMouseScale(1/scale, 1/scale);
}


void RRPG_draw_to_screen(
    int camera_width,
    int camera_height,
    RenderTexture2D *target
)
{
    float scale = MIN((float)GetScreenWidth()/camera_width, (float)GetScreenHeight()/camera_height);
    BeginDrawing();

    ClearBackground(BLACK);
    DrawTexturePro(target->texture, 
        (Rectangle){ 
            0.0f, 0.0f, 
            (float)target->texture.width, 
            (float)-target->texture.height},
        (Rectangle){
            (GetScreenWidth() - ((float)camera_width*scale))*0.5f, 
            (GetScreenHeight() - ((float)camera_height*scale))*0.5f,
            (float)camera_width*scale, (float)camera_height*scale
        },
        (Vector2){0, 0}, 
        0,
        WHITE
        );        

    EndDrawing();
}

/*
ends the program
*/
void RRPG_end(RenderTexture2D *target)
{
    UnloadRenderTexture(*target);
    CloseAudioDevice();
    CloseWindow();
}

