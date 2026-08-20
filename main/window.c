#include"raylib.h"


#define screenwidth 1080
#define screenheight 810


int main(){
    InitWindow(screenwidth,screenheight, "game");

    SetTargetFPS(60);

    while(!WindowShouldClose()){

        BeginDrawing();
        ClearBackground(SKYBLUE);
        EndDrawing();
    }
    CloseWindow();
    return 0;

}