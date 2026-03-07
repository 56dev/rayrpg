#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include "rayRPG_boilerplate.h"
#include "rayRPG_tiles.h"

#define RAYGUI_IMPLEMENTATION

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    #include "raygui.h" 
#pragma GCC diagnostic pop

#define CAMW 256*6
#define CAMH 144*6

void mouse_gestures(Camera2D *camera, RRPG_TileSet tileset);
void tile_panel(RRPG_TileSet tileset, RRPG_Vector2Grid position);
void RRPGTM_display_tileset(RRPG_TileSet *tileset, int tile_size, Camera2D camera, int mode, RRPG_Vector2Grid *OUT_tile_position_selected);

enum {
    MODE_CREATION,
    MODE_SELECTION
};

int main() {
    RenderTexture2D target;
    Camera2D camera = (Camera2D){
        .offset = (Vector2){CAMW / 2, CAMH / 2},
        .target = (Vector2){0, 0},
        .rotation = 0.0f,
        .zoom = 6.0f,
    };
    
    RRPG_initialize(
        "rayRPG tilemaker",
        CAMW,
        CAMH,
        CAMW,
        CAMH,
        TEXTURE_FILTER_POINT,
        &target
    );

    //Texture2D atlas = RRPGTM_load_atlas("sprites/atlases/Sprite-0001.png");
    RRPG_TileSet tileset = RRPGTM_create_tileset_from_atlas("sprites/atlases/Sprite-0001.png", "data/tilesets/test.rrpgatl", 16);
    int mode = MODE_SELECTION;
    RRPG_Vector2Grid tile_selected = (RRPG_Vector2Grid){-1, -1};
    
    while(!WindowShouldClose()) {
        RRPG_render_adjust_mouse(CAMW, CAMH);

        mouse_gestures(&camera, tileset);

        
        if(IsKeyPressed(KEY_ONE)) {
            mode = MODE_CREATION;
        } else if (IsKeyPressed(KEY_TWO)) {
            mode = MODE_SELECTION;
        }
        BeginTextureMode(target);
            GuiToggleGroup((Rectangle){CAMW - 250, 0, 100, 100}, "CREATE TILE;SELECT TILE", &mode);
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                RRPGTM_display_tileset(&tileset, 16, camera, mode, &tile_selected);
            EndMode2D();
            tile_panel(tileset, tile_selected);
        EndTextureMode();

        RRPG_draw_to_screen(CAMW, CAMH, &target);
    }
    RRPG_end(&target);
}

void mouse_gestures(Camera2D *camera, RRPG_TileSet tileset) {
    static bool is_dragging = false;
    static Vector2 mouse_pos_when_drag_started = (Vector2){-INFINITY, -INFINITY};
    Vector2 mouse_pos = GetMousePosition();
    Vector2 world_mouse_pos = GetScreenToWorld2D(mouse_pos, *camera);
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && 
        (world_mouse_pos.x < 0 || world_mouse_pos.x > tileset.atlas.width || 
        world_mouse_pos.y < 0 || world_mouse_pos.y > tileset.atlas.height)) {
            is_dragging = true;            
            mouse_pos_when_drag_started = GetScreenToWorld2D(mouse_pos, *camera);
        } 
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        is_dragging = false;
    }
    if(is_dragging) {
        camera->target = Vector2Subtract(camera->target, Vector2Subtract(GetScreenToWorld2D(mouse_pos, *camera), mouse_pos_when_drag_started));
    }
    camera->target = Vector2Clamp(
        camera->target, 
        (Vector2){0, 0},
        (Vector2){tileset.atlas.width, tileset.atlas.height});

    float wheel = GetMouseWheelMove();
    if(wheel < 0) {
        camera->zoom -= GetFrameTime() * 15.0f;
    } else if(wheel > 0) {
        camera->zoom += GetFrameTime() * 15.0f;
    }

    camera->zoom = Clamp(camera->zoom, 2.0f, 18.0f);
}
void tile_panel(RRPG_TileSet tileset, RRPG_Vector2Grid position) {
    
    Rectangle panel = (Rectangle) {
        0, 0, CAMW * 5/16, CAMH
    };
    DrawRectangleRec(panel, BEIGE);
    float line_thick = 10.0f;
    DrawLineEx((Vector2){panel.width + line_thick/2, 0}, (Vector2){panel.width + line_thick/2, panel.height}, line_thick, BLACK);

    float margin_thickness = 30.0f;
    //**DEBUG */
    //DrawRectangleLines(panel.x + margin_thickness, panel.y + margin_thickness, panel.width - margin_thickness*2, panel.height - margin_thickness*2, RED);
    Rectangle content_rect = (Rectangle) {panel.x + margin_thickness, panel.y + margin_thickness, panel.width - margin_thickness*2, panel.height - margin_thickness*2};
    float font_size = 50.0f;
    int tile_idx = RRPGTM_find_position_in_tileset(tileset, position);
    if(tile_idx < 0) {
        DrawText("No tile selected.", content_rect.x, content_rect.y + font_size*3, font_size, DARKGRAY);
        return;
    }
    RRPG_Tile *tile = &(tileset.tiles[tile_idx]);
    DrawText(TextFormat("x: %i, y: %i", tile->tile_position.x, tile->tile_position.y), content_rect.x, content_rect.y + font_size*0, font_size, DARKGRAY);
    int o = GuiGetStyle(DEFAULT, TEXT_SIZE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, font_size);
        GuiCheckBox((Rectangle){content_rect.x, content_rect.y + font_size*1, font_size, font_size}, "lol", &(tile->entities_can_pass));
    GuiSetStyle(DEFAULT, TEXT_SIZE, o);
    //DrawText(TextFormat("collision state: %i", tile->entities_can_pass), content_rect.x, content_rect.y + font_size*1, font_size, DARKGRAY);
    
}
void RRPGTM_display_tileset(RRPG_TileSet *tileset, int tile_size, Camera2D camera, int mode, RRPG_Vector2Grid *OUT_tile_position_selected) {
    DrawTextureEx(tileset->atlas, (Vector2){0, 0}, 0, 1.0f, WHITE);
    
    Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
    
    for(int x = 0; x < (int)floor(tileset->atlas.width / tile_size); ++x) {
        for(int y = 0; y < (int)floor(tileset->atlas.height / tile_size); ++y) {
            Color tile_color;
            if(RRPGTM_find_position_in_tileset(*tileset, (RRPG_Vector2Grid){x, y}) < 0) {
                tile_color = GRAY;
                tile_color.a = 40;                
            }
            else {
                tile_color = RED;
                tile_color.a = 0;
            }
            DrawRectangle(x * tile_size, y * tile_size, tile_size, tile_size, tile_color);
        }
    }    

    float line_thick = 0.5f;
    DrawRectangleLinesEx((Rectangle){-line_thick, -line_thick, tileset->atlas.width + line_thick*2, tileset->atlas.height + line_thick*2}, line_thick, BLACK);
    if(mouse_pos.x < 0 || mouse_pos.x > tileset->atlas.width) { return; }
    if(mouse_pos.y < 0 || mouse_pos.y > tileset->atlas.height) { return; }
    RRPG_Vector2Grid tile_pos = (RRPG_Vector2Grid){ 
        (int)floor(mouse_pos.x / tile_size),
        (int)floor(mouse_pos.y / tile_size)
    };
    Color selection_color = GRAY;
    selection_color.a = 120;

    DrawRectangle(tile_pos.x * tile_size, tile_pos.y * tile_size, tile_size, tile_size, selection_color);
    if(mode == MODE_CREATION) {
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if(RRPGTM_find_position_in_tileset(*tileset, (RRPG_Vector2Grid){tile_pos.x, tile_pos.y}) < 0) {
                RRPGTM_add_tile_to_tileset(tileset, tile_pos);
            }
        } 
    } else if(mode == MODE_SELECTION) {
        
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            
            int idx = RRPGTM_find_position_in_tileset(*tileset, (RRPG_Vector2Grid){tile_pos.x, tile_pos.y});
            if(idx >= 0) {
                *OUT_tile_position_selected = tileset->tiles[idx].tile_position;
            }
        } 
    }
}