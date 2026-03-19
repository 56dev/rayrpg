#include "rayRPG_tiles.h"

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
RRPG_TileSet RRPGTM_load_tileset(const char *tileset_path, bool *success) {
    FILE *fptr = fopen(tileset_path, "r");
    if(!fptr) {
        *success = false;
        return (RRPG_TileSet){0};
    }
    int cur_work_idx = -1;
    char buf[100];
    RRPG_TileSet tileset = (RRPG_TileSet){0};
    tileset.tileset_path = tileset_path;
    while(fgets(buf, 100, fptr)) {
        if(ferror(fptr)) {
            *success = false;
            return (RRPG_TileSet){0};
        }
        char *tok = strtok(buf, " ");
        if(strcmp(tok, "TILE") == 0) {
            ++tileset.count;
            ++cur_work_idx;
            continue;
        }
        void *o = NULL;
        enum {
            TILE_SIZE,
            PATH,
            T_POS,
            T_CAN_PASS
        };
        int token_type = -1;
        int token_idx = -1;
        while(tok != NULL) {
           ++token_idx;
           if(token_type < 0) {
               if(cur_work_idx < 0) {
                   if(strcmp(tok, "TILE_SIZE") == 0) {
                        token_type = TILE_SIZE;
                   } else if (strcmp(tok, "PATH") == 0) {
                        token_type = PATH; 
                   }
               } else {
                  if(strcmp(tok, "POS") == 0) {
                    token_type = T_POS;
                  } else if(strcmp(tok, "CAN_PASS") == 0) {
                    token_type = T_CAN_PASS;
                  }                    
               }
           } else {
                switch (token_type) {
                    case TILE_SIZE:
                        tileset.tile_size = 
                    break;
                    case PATH:
                    break;
                    case T_POS:
                    break;
                    case T_CAN_PASS:
                    break;
                }
           
           }
           tok = strtok(NULL, " ");
        }
    }
    *success = true;
}
bool RRPGTM_save_tileset(RRPG_TileSet tileset){
    FILE *fptr = fopen(tileset.tileset_path, "w");
    if(!fptr) {
        return false;
    }
    fprintf(fptr, "TILE_SIZE %i\n", tileset.tile_size);
    fprintf(fptr, "COUNT %i\n", tileset.count);
    fprintf(fptr, "PATH %s\n", tileset.atlas_path);
    for(int i = 0; i < tileset.count; ++i) {
        RRPG_Tile *tile = &(tileset.tiles[i]);
        fprintf(fptr, "TILE");
        fprintf(fptr, "POS %i %i\n", tile->tile_position.x, tile->tile_position.y);
        fprintf(fptr, "CAN_PASS %i\n", (tile->entities_can_pass ? 1 : 0));        
    }
    fclose(fptr);
    return true;
}
void RRPGTM_display_tile_texture(RRPG_Tile tile, Rectangle dest_rect) {
    Texture2D atlas = tile.tileset->atlas;
    int ts = tile.tileset->tile_size;
    Rectangle source = (Rectangle){tile.tile_position.x * ts, 
                tile.tile_position.y * ts, 
                ts, 
                ts};
    if(dest_rect.height <= 0 || dest_rect.width <= 0) {
        DrawTextureRec(atlas, 
           source, 
           (Vector2){dest_rect.x, dest_rect.y},
        WHITE);
    } else {
        DrawTexturePro(atlas, source, dest_rect, Vector2Zero(), 0.0f, WHITE);
        
    }
    
}


void RRPGTM_add_tile_to_tileset(RRPG_TileSet *tileset, RRPG_Vector2Grid tile_position) {

    if(tileset->xxprv_capacity / sizeof(RRPG_Tile) <= tileset->count) {
        tileset->xxprv_capacity *= 2;
        tileset->tiles = MemRealloc(tileset->tiles, tileset->xxprv_capacity);

    }
    tileset->tiles[tileset->count] = (RRPG_Tile) {
        .tileset = tileset,
        .tile_position = tile_position,
        .entities_can_pass = false,

    };
    ++tileset->count;

}


void RRPGTM_remove_tile_from_tileset(RRPG_TileSet *tileset, RRPG_Vector2Grid tile_position) {
    int i = RRPGTM_find_position_in_tileset(*tileset, tile_position);
    if(i < 0) {
        return;
    }
    for(; i < tileset->count - 1; ++i) {
        tileset->tiles[i] = tileset->tiles[i + 1];
    }
    --(tileset->count);
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


