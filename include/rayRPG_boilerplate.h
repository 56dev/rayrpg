#pragma once 
#include <raylib.h>

/*
The code for these functions was taken from the basic window and resizable window
raylib examples.
*/

void RRPG_initialize(
    const char *title,
    int camera_width,
    int camera_height,
    int starting_screen_width,
    int starting_screen_height,
    int texture_filter,
    RenderTexture2D *OUT_target
);

void RRPG_end(RenderTexture2D *target);
void RRPG_set_render(
    int camera_width,
    int camera_height
);
void RRPG_draw_to_screen(
    int camera_width,
    int camera_height,
    RenderTexture2D *target
);