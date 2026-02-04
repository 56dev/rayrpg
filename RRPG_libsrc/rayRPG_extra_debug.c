#include "rayRPG_extra_debug.h"

#include <raylib.h>
void RRPG_DEBUG_display_movement_control(
    Vector2 pos,
    float scale
) {
    const float key_dim = 20 * scale;
    Color colors[4] = { GRAY, GRAY, GRAY, GRAY };
    if(IsKeyDown(KEY_W)) {  
        colors[0] = RED;
    }  if(IsKeyDown(KEY_A)) {
        colors[3] = RED;
    }  if(IsKeyDown(KEY_S)) {
       colors[2] = RED;
    }  if(IsKeyDown(KEY_D)) {
        colors[1] = RED;
    } 

    DrawRectangle(pos.x + key_dim, pos.y, key_dim, key_dim, colors[0]);
    DrawRectangle(pos.x + key_dim * 2, pos.y + key_dim, key_dim, key_dim, colors[1]);
    DrawRectangle(pos.x + key_dim, pos.y + key_dim, key_dim, key_dim, colors[2]);
    DrawRectangle(pos.x, pos.y + key_dim, key_dim, key_dim, colors[3]);

}