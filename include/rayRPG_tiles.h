#pragma once

#include <raylib.h>
#include <rayRPG_grid_movement.h>

struct RRPG_Tile; 

typedef struct RRPG_TileSet {
//public:
    char *tileset_path;
    char *atlas_path;
    Texture2D atlas;
    int tile_size;
    int count;
    struct RRPG_Tile *tiles;
//private:
    int xxprv_capacity;
} RRPG_TileSet;

typedef struct RRPG_Tile {
    RRPG_TileSet tileset;
    RRPG_Vector2Grid tile_position;
    int collision_state;
} RRPG_Tile;

typedef struct RRPG_TileMap {
    char *file_name;
    RRPG_Vector2Grid *map;
    int length;
    int width;
} RRPG_TileMap;

Texture2D RRPGTM_load_atlas(
    const char *path);

void RRPGTM_display_tileset(
    RRPG_TileSet tileset, 
    int tile_size, 
    Camera2D camera);

RRPG_TileSet RRPGTM_create_tileset_from_atlas(
    const char *atlas_path, 
    const char *tileset_path, 
    int tile_size);

void RRPGTM_save_tileset();