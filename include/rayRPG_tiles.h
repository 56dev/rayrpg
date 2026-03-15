#pragma once

#include <raylib.h>
#include <rayRPG_grid_movement.h>

struct RRPG_Tile; 

typedef struct RRPG_TileSet {
//public:
    const char *tileset_path;
    const char *atlas_path;
    Texture2D atlas;
    int tile_size;
    int count;
    struct RRPG_Tile *tiles;
//private:
    int xxprv_capacity;
} RRPG_TileSet;

typedef struct RRPG_Tile {
    RRPG_TileSet *tileset;
    RRPG_Vector2Grid tile_position;
    bool entities_can_pass;
} RRPG_Tile;

typedef struct RRPG_TileMap {
    char *file_name;
    RRPG_Vector2Grid *map;
    int length;
    int width;
} RRPG_TileMap;

Texture2D RRPGTM_load_atlas(
    const char *path);
RRPG_TileSet RRPGTM_create_tileset_from_atlas(
    const char *atlas_path, 
    const char *tileset_path, 
    int tile_size);
void RRPGTM_add_tile_to_tileset(RRPG_TileSet *tileset, RRPG_Vector2Grid tile_position);
int RRPGTM_find_position_in_tileset(RRPG_TileSet tileset, RRPG_Vector2Grid tile_position);
bool RRPGTM_save_tileset(RRPG_TileSet tileset);
void RRPGTM_display_tile_texture(RRPG_Tile tile, Rectangle dest_rect); //passing in a display_rect with zero width/height makes the function display the texture with the default tile dimensions
void RRPGTM_remove_tile_from_tileset(RRPG_TileSet *tileset, RRPG_Vector2Grid tile_position);
