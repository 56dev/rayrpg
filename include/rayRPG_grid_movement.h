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

/*Describes a collision grid whose top left is at 0, 0.*/
typedef struct RRPG_CollisionGrid {
    int *cells; /*Each cell contains an integer. Any number below zero means the cell is occupied by a block or tile. 0 means it is free. Any number above zero indicates the number of entities occupying the cell (these may be temporary).*/
    int extent_x;
    int extent_y;
} RRPG_CollisionGrid;

enum {
    DIRECTION_UP, //1st in the enum (goes clockwise)
    DIRECTION_RIGHT, //2nd in the enum (goes clockwise)
    DIRECTION_DOWN, //3rd in the enum (goes clockwise)
    DIRECTION_LEFT //4th in the enum (goes clockwise)
};

/*
Goes clockwise. Put the named 
direction constants into this array to 
get the corresponding vector direction.
*/
extern  RRPG_Vector2Grid RRPG_DIRECTION_VECTORS[];

//The entity controller. Handles POSITION and MOVEMENT.
typedef struct RRPG_EntityController{
    RRPG_Vector2Grid position; /*The actual position of the entity. This is always set instantly. This is what should control the collision box of the entity.*/
    Vector2 sprite_position; /*Where it looks like the entity is. This is used to smooth out movement between grid cells.*/
    int facing; /*What direction the entity is facing.*/
    float speed; /*How fast the entity moves between grid cells. Measured in px/sec.*/
    bool is_moving; /*Whether the entity is currently moving between grid cells.*/

    RRPG_Vector2Grid xxprv_previous_position;
    float xxprv_move_progress;
    Vector2 xxprv_next_sprite_position;
} RRPG_EntityController;


void RRPG_set_grid_side_length(int g); //Sets the grid side length as a global variable. Other functions will need to use that, so remember to call this before doing anything.
void RRPG_DEBUG_draw_grid(int grid_side_length, int extent_x, int extent_y); //Draws the grid semitransparently
RRPG_Vector2Grid RRPG_raw_pos_to_grid(Vector2 raw_pos, int grid_side_length); //Converts a raw WORLD position to GRID position.
Vector2 RRPG_grid_pos_to_raw(RRPG_Vector2Grid grid_pos, int grid_side_length); //Converts a GRID position to raw WORLD position (where new position is perfectly centered on the indicated grid cell).
void RRPG_position_camera_on_grid(Camera2D *camera, RRPG_Vector2Grid grid_pos, int grid_side_length); //Center the camera onto a certain grid cell.


/*PLAYER MOVEMENT FUNCTIONS*/

void RRPG_entity_controller_constructor(                   //Initializes the player controller with the proper default values.
    RRPG_EntityController *player,
    RRPG_Vector2Grid starting_grid_position,
    float speed,
    RRPG_CollisionGrid col_grid
);


int RRPG_PLAYER_sense_movement_control();       // Senses keyboard input (WASD). Returns the last input recorded in a static buffer, as a DIRECTION (integer).
void RRPG_PLAYER_position_camera_on_player(     //Centers the camera onto the player.
    Camera2D *camera,
    RRPG_EntityController *player
); 
void RRPG_walk_entity(                   //Moves an entity in that RPG-like way. direction_to_move only needs to be set while the entity is staying still: the parameter will then be ignored until the entity reaches the tile it is moving towards.
    RRPG_EntityController *player,
    int direction_to_move,
    RRPG_CollisionGrid collision_grid
); 

void RRPG_position_entity(
    RRPG_EntityController *entity,
    RRPG_Vector2Grid position,
    RRPG_CollisionGrid col_grid
);

void RRPG_PLAYER_DEBUG_dispay_player_info(      //Displays some player information onto the screen.
    RRPG_EntityController *player
); 


/*COLLISION FUNCTIONS*/



RRPG_CollisionGrid RRPG_initialize_collision_grid(
    int extent_x, //In # of grid cells, not px.
    int extent_y,//In # of grid cells, not px.
    RRPG_Vector2Grid *occupied, //testing
    int num_occupied
);
void RRPG_destroy_collision_grid(
    RRPG_CollisionGrid *grid
);

void RRPG_DEBUG_draw_collision_grid(        //Draws the extent of the grid, and colors in black all occupied cells.
    const RRPG_CollisionGrid *grid
);

int RRPG_return_collision_state_at_position(    //Returns the collision-state, expressed as an integer, of the current cell. If the cell is outside the collision grid's extents, it returns a very low integer (-9999).
    RRPG_CollisionGrid grid,
    RRPG_Vector2Grid cell
);

void RRPG_add_to_collision_state(
    RRPG_CollisionGrid *grid,
    RRPG_Vector2Grid cell
);

void RRPG_subtract_from_collision_state(
    RRPG_CollisionGrid *grid,
    RRPG_Vector2Grid cell
);