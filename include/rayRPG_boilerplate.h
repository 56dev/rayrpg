#pragma once 
#include <raylib.h>

/*
The code for these functions was taken from the basic window and resizable window
raylib examples.
*/


void RRPG_initialize(                       //Does most of the initialization by itself.
    const char *title,
    int camera_width,
    int camera_height,
    int starting_screen_width,
    int starting_screen_height,
    int texture_filter,
    RenderTexture2D *OUT_target
);

void RRPG_end(RenderTexture2D *target);     //Does the base de-initialization stuff by itself.
void RRPG_render_adjust_mouse(              //Call this at the VERY START of every game-loop iteration in order to make sure the mouse cursor plays nicely with the resizable screen.
    int camera_width,
    int camera_height
);
void RRPG_draw_to_screen(                   //Call this at the VERY END of every game-loop iteration to actually draw all your changes to the screen.
    int camera_width,
    int camera_height,
    RenderTexture2D *target
);