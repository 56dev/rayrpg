#include "rayRPG_tiles.h"

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>


Texture2D RRPGTM_load_atlas(const char *path) {
    Texture2D tex = LoadTexture(path);
    if(tex.id == 0) {
        TraceLog(LOG_ERROR, "Failed to load atlas: %s", path);
    }
    return tex;
}

RRPG_TileSet RRPGTM_create_tileset_from_atlas(const char *atlas_path, const char *tileset_path, int tile_size) {
    return (RRPG_TileSet) {
        .atlas_path = atlas_path,
        .tileset_path = tileset_path,
        .tile_size = tile_size,
        .atlas = RRPGTM_load_atlas(atlas_path),
        .count = 0,
        .tiles = MemAlloc(sizeof(RRPG_Tile)),
        
        .xxprv_capacity = sizeof(RRPG_Tile),
    };
}

void RRPGTM_add_tile_to_tileset(RRPG_TileSet *tileset, RRPG_Vector2Grid tile_position) {

    if(tileset->xxprv_capacity / sizeof(RRPG_Tile) <= tileset->count) {
        tileset->xxprv_capacity *= 2;
        tileset->tiles = MemRealloc(tileset->tiles, tileset->xxprv_capacity);

    }
    tileset->tiles[tileset->count] = (RRPG_Tile) {
        .tileset = tileset,
        .tile_position = tile_position,
        .collision_state = 0,
    };
    ++tileset->count;

}

int RRPGTM_find_position_in_tileset(RRPG_TileSet tileset, RRPG_Vector2Grid tile_position) {
    for(int i = 0; i < tileset.count; i++) {
        if((tileset.tiles[i].tile_position.x == tile_position.x) && 
            (tileset.tiles[i].tile_position.y == tile_position.y)) {
            return i;
        }
    }
    return -1;
}

void RRPGTM_remove_tile_from_tileset(RRPG_TileSet tileset, RRPG_Vector2Grid tile_position) {
    int i = 0;
    for(; i < tileset.count; ++i) {
        if(tileset.tiles[i].tile_position.x == tile_position.x && 
            tileset.tiles[i].tile_position.y == tile_position.y) {
            break;
        }
    }
    for(; i < tileset.count - 1; ++i) {
        tileset.tiles[i] = tileset.tiles[i + 1];
    }
    --tileset.count;
}



void RRPGTM_save_tileset(const char *path) {

}