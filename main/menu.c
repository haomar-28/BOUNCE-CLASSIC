#include<stdio.h>
#include"raylib.h"
#include"raymath.h"


#define screenwidth 1080
#define screenheight 810

int main(){

    InitWindow(screenwidth, screenheight, "Menu");

    int loading =0;
    int play=0;
    int rules =0;
    int leaderboard =0;
    
    Texture2D first_background = LoadTexture("assets/firstpage.png");
    Texture2D menubar = LoadTexture("assets/menubar.png");

    Texture2D playbutton = LoadTexture("assets/playbutton.png");
    Texture2D playglowing = LoadTexture("assets/playglowing.png");
    Texture2D rulesbutton = LoadTexture("assets/rulesbutton.png");
    Texture2D rulesglowing = LoadTexture("assets/rulesglowing.png");
    Texture2D leaderboardbutton = LoadTexture("assets/leaderboardbutton.png");
    Texture2D leaderboardglowing = LoadTexture("assets/leaderboardglowing.png");
    


    while(!WindowShouldClose()){
        if(!loading){
        BeginDrawing();
       

    DrawTexturePro(first_background, (Rectangle){0, 0, first_background.width, first_background.height},(Rectangle){0, 0,screenwidth, screenheight},(Vector2){0, 0}, 0.0f, WHITE);

    DrawText("LOADING", 420, 650, 50, BLACK);

    EndDrawing();

    WaitTime(1.0);

    BeginDrawing();

    DrawTexturePro(first_background,(Rectangle){0, 0, first_background.width, first_background.height},(Rectangle){0, 0, screenwidth, screenheight},(Vector2){0, 0}, 0.0f, WHITE);

    DrawText("LOADING.", 420, 650, 50, BLACK);

    EndDrawing();

    WaitTime(1.0);

    BeginDrawing();

    DrawTexturePro(first_background,(Rectangle){0, 0, first_background.width, first_background.height},(Rectangle){0, 0,screenwidth, screenheight},(Vector2){0, 0}, 0.0f, WHITE);

    DrawText("LOADING. .", 420, 650, 50, BLACK);

    EndDrawing();
    WaitTime(1.0);

    BeginDrawing();

    DrawTexturePro(first_background,(Rectangle){0, 0, first_background.width, first_background.height},(Rectangle){0, 0,screenwidth, screenheight}, (Vector2){0, 0}, 0.0f, WHITE);

    DrawText("LOADING. . .", 420, 650, 50, BLACK);

    EndDrawing();
    WaitTime(0.5f);
    loading =1;}
    else
        {
        Vector2 mousepos = GetMousePosition();

        Rectangle playrec = {420, 100, playbutton.width, playbutton.height};
        Rectangle rulesrec = {420, 300, rulesbutton.width, rulesbutton.height};
        Rectangle leaderboardrec = {420, 500, leaderboardbutton.width, leaderboardbutton.height};

        if(CheckCollisionPointRec(mousepos, playrec)){
            play=1;
        }
        else play =0;
        if(CheckCollisionPointRec(mousepos, rulesrec)){
            rules=1;
        }
        else rules=0;
        if(CheckCollisionPointRec(mousepos, leaderboardrec)){
            leaderboard=1;
        }
        else leaderboard =0; 
        
        // =====================
        // NEXT WINDOW
        // =====================

        BeginDrawing();

        //drawbuttons
        DrawTexturePro(menubar, (Rectangle){0, 0, menubar.width, menubar.height},(Rectangle){0, 0,screenwidth, screenheight},(Vector2){0, 0}, 0.0f, WHITE);

            if(rules ==0){
        DrawTexturePro(rulesbutton, (Rectangle){0, 0, rulesbutton.width, rulesbutton.height},(Rectangle){420, 300,rulesbutton.width, rulesbutton.height},(Vector2){0, 0}, 0.0f, WHITE);
            }
            else{
                DrawTexturePro(rulesglowing, (Rectangle){0, 0, rulesglowing.width, rulesglowing.height},(Rectangle){420, 300,rulesglowing.width, rulesglowing.height},(Vector2){0, 0}, 0.0f, WHITE);
            }

            if(play ==0){
        DrawTexturePro(playbutton, (Rectangle){0, 0, playbutton.width, playbutton.height},(Rectangle){420, 100,playbutton.width, playbutton.height},(Vector2){0, 0}, 0.0f, WHITE);
            }
            else{
                DrawTexturePro(playglowing, (Rectangle){0, 0, playglowing.width, playglowing.height},(Rectangle){420, 100,playglowing.width, playglowing.height},(Vector2){0, 0}, 0.0f, WHITE);
            }

            if(leaderboard ==0){
        DrawTexturePro(leaderboardbutton, (Rectangle){0, 0, leaderboardbutton.width, leaderboardbutton.height},(Rectangle){420, 500,leaderboardbutton.width, leaderboardbutton.height},(Vector2){0, 0}, 0.0f, WHITE);
            }
            else{
                DrawTexturePro(leaderboardglowing, (Rectangle){0, 0, leaderboardglowing.width, leaderboardglowing.height},(Rectangle){420, 500,leaderboardglowing.width, leaderboardglowing.height},(Vector2){0, 0}, 0.0f, WHITE);
            }
        


        EndDrawing();
    }


    }
    UnloadTexture(first_background);
    UnloadTexture(menubar);
    UnloadTexture(playbutton);
    UnloadTexture(playglowing);
    UnloadTexture(rulesbutton);
    UnloadTexture(rulesglowing);
    UnloadTexture(leaderboardbutton);
    UnloadTexture(leaderboardglowing);
    
}

