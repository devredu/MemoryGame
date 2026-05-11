#include "raylib.h"

int main(){
    InitWindow(800, 600, "Okienko");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello World!", 800/2, 600/2, 32, BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}