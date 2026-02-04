#pragma once
#include <raylib.h>
#include <math.h>


extern int RRPG_GRID_SIDE_LENGTH;
/*
Like a normal Vector2, but with integer components, for use in indicating grid positions,
but can also be used in places where you need integers. 
*/
typedef struct RRPG_Vector2Grid{
    int x; //x component
    int y; //y component
} RRPG_Vector2Grid;


enum {
    DIRECTION_UP, //1st in the enum (goes clockwise)
    DIRECTION_RIGHT, //2nd in the enum (goes clockwise)
    DIRECTION_DOWN, //3rd in the enum (goes clockwise)
    DIRECTION_LEFT //4th in the enum (goes clockwise)
};

/*
Goes clockwise. Put the named 
direction constants into this array to 
get the corresponding Vector direction.
*/
extern RRPG_Vector2Grid RRPG_DIRECTION_VECTORS[];
typedef struct RRPG_PlayerController{
    RRPG_Vector2Grid position; /*The actual position of the player.
    This is always set instantly.
    This is what controls the collision box of the player*/
    Vector2 sprite_position; /*Where it looks like the player is.
    This is used to smooth out movement between grid cells.*/
    int facing; /*What direction the player is facing.*/
    float speed; /*How fast the player moves between grid cells. Measured in px/sec*/
    bool is_moving; /*Whether the player is currently moving between grid cells.*/
} RRPG_PlayerController;

void RRPG_set_grid_side_length(int g);
void RRPG_DEBUG_draw_grid(int grid_side_length, int extent_x, int extent_y); //Draws the grid semitransparently
RRPG_Vector2Grid RRPG_raw_pos_to_grid(Vector2 raw_pos, int grid_side_length); //Converts a raw WORLD position to GRID position.
Vector2 RRPG_grid_pos_to_raw(RRPG_Vector2Grid grid_pos, int grid_side_length); //Converts a GRID position to raw WORLD position (where new position is perfectly centered on the indicated grid cell).
void RRPG_position_camera_on_grid(Camera2D *camera, RRPG_Vector2Grid grid_pos, int grid_side_length); //Center the camera onto a certain grid cell.


/*PLAYER MOVEMENT FUNCTIONS*/

void RRPG_PLAYER_constructor(
    RRPG_PlayerController *player,
    RRPG_Vector2Grid starting_grid_position,
    float speed
);

int RRPG_PLAYER_sense_movement_control(

);
void RRPG_PLAYER_position_camera_on_player(
    Camera2D *camera,
    RRPG_PlayerController *player
); //Centers the camera onto the player.
void RRPG_PLAYER_move_player(
    RRPG_PlayerController *player,
    int direction
); //Moves the player in that RPG-like way. 

void RRPG_PLAYER_DEBUG_dispay_player_info(
    RRPG_PlayerController *player
); //Displays player information onto the screen.