#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include <string.h>

#define screenwidth 1080
#define screenheight 810
#define blocksize 45
#define maxblocks 300
#define maxspeedx 300
#define radius 25
#define jumpspeed 600
#define Gravity 1200

float currentradius = radius;

typedef struct 
{
    char name[100];
    int score;
} Player;

// Save player info
void savescore(char name[], int score) {
    Player players[1000];
    int count = 0;

    FILE *file = fopen("leaderboard.txt", "r");
    if (file != NULL) {
        while (count < 100 && fscanf(file, "%99[^|]|%d\n", players[count].name, &players[count].score) == 2) {
            count++;
        }
        fclose(file);
    }

    strcpy(players[count].name, name);
    players[count].score = score;
    count++;

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (players[j].score > players[i].score) {
                Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }

    if (count > 10) count = 10;

    file = fopen("leaderboard.txt", "w");
    if (file == NULL) return;
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s|%d\n", players[i].name, players[i].score);
    }
    fclose(file);
}

void drawleaderboard() {
    Player players[10];
    int count = 0;

    FILE *file = fopen("leaderboard.txt", "r");
    if (file != NULL) {
        while (count < 10 && fscanf(file, "%99[^|]|%d\n", players[count].name, &players[count].score) == 2) {
            count++;
        }
        fclose(file);
    }

    ClearBackground(LIGHTGRAY);
    DrawText("LEADERBOARD", 400, 40, 50, DARKBLUE);
    DrawText("SL no. \t\t\tPLAYER \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tSCORE", 70, 170, 30, BLACK);

    DrawRectangle(0, 220, screenwidth, 50, MAROON);
    DrawRectangle(0, 270, screenwidth, 50, RED);
    DrawRectangle(0, 320, screenwidth, 50, ORANGE);

    for (int i = 0; i < count; i++) {
        DrawText(TextFormat("%d", i + 1), 90, 230 + i * 50, 30, i < 3 ? RAYWHITE : BLACK);
        DrawText(players[i].name, 250, 230 + i * 50, 30, i < 3 ? RAYWHITE : BLACK);
        DrawText(TextFormat("%d", players[i].score), 800, 230 + i * 50, 30, i < 3 ? RAYWHITE : BLACK);
    }
}

Vector2 position1 = {radius + blocksize, 500};
Vector2 position2 = {100, blocksize + radius};
Vector2 position3;
Vector2 position;
    
float ballRotation = 0;
Vector2 speed;
Vector2 gravity;

Vector2 enemy1position;
Vector2 enemy2position;
Vector2 enemy3position;
Vector2 enemy1position1 = {14 * blocksize, 13 * blocksize};
Vector2 enemy1speed1 = {0, 200};
Vector2 enemy1speed;
Vector2 enemy2speed;
Vector2 enemy3speed;
float enemy1size;
int enemycol;

Vector2 ringbackposition;
Vector2 ringbackposition1 = {11 * blocksize, 11 * blocksize};
Vector2 ringbackposition2;
Vector2 ringbackposition3;

Vector2 ringfrontposition;
Vector2 ringfrontposition1 = {11 * blocksize, 11 * blocksize};
Vector2 ringfrontposition2;
Vector2 ringfrontposition3;

Vector2 ring1backposition = {3 * blocksize, 8 * blocksize};
Vector2 ring1frontposition = {3 * blocksize, 8 * blocksize};
float ring1size = 0.35f;
float ring1radius = 24;

Vector2 ring2backposition = {23 * blocksize, 8 * blocksize};
Vector2 ring2frontposition = {23 * blocksize, 8 * blocksize};
float ring2size = 0.3f;
float ring2radius = 20;

Vector2 ring3backposition = {14.4f * blocksize, 11 * blocksize};
Vector2 ring3frontposition = {14.4f * blocksize, 11 * blocksize};
float ring3size = 0.22f;
float ring3radius = 21;

float ringsize;
float ringradius;
int ringcount;

Vector2 gemposition;
Vector2 gemposition1 = {17 * blocksize, 10 * blocksize};
Vector2 gemposition2;
Vector2 gemposition3;
int gempass;
int gemcount;
float gemsize;

Vector2 flagpos;
Vector2 flagpos1 = {12 * blocksize, 6 * blocksize};
Vector2 flagpos2;
Vector2 flagpos3;

float spike1size = 40;
Vector2 spike1position2 = {10.83f * blocksize, 7.6f * blocksize};
float spike2size = 40;
Vector2 spike2position2 = {11.3f * blocksize, 7.6f * blocksize};
float spike3size = 40;
Vector2 spike3position2 = {11.77f * blocksize, 7.6f * blocksize};
float spike4size = 40;
Vector2 spike4position2 = {12.24f * blocksize, 7.6f * blocksize};
float spike5size = 40;
Vector2 spike5position2 = {13.83f * blocksize, 7.6f * blocksize};
float spike6size = 40;
Vector2 spike6position2 = {14.3f * blocksize, 7.6f * blocksize};
float spike7size = 40;
Vector2 spike7position2 = {14.77f * blocksize, 7.6f * blocksize};
float spike8size = 40;
Vector2 spike8position2 = {6.7f * blocksize, 15.6f * blocksize};
float spike9size = 40;
Vector2 spike9position2 = {7.17f * blocksize, 15.6f * blocksize};

float enemy1size2 = 80;
Vector2 enemy1position2 = {8 * blocksize, 1 * blocksize};
Vector2 enemy1speed2 = {0, 200};

float enemy2size2 = 45;
Vector2 enemy2position2 = {3 * blocksize, 10 * blocksize};
Vector2 enemy2speed2 = {0, 100};

float enemy3size2 = 80;
Vector2 enemy3position2 = {10.7f * blocksize, 10 * blocksize};
Vector2 enemy3speed2 = {0, 200};

// Level 2 power-up
float powerupsize = 30;
Vector2 powerupposition = {16 * blocksize, 15.6f * blocksize};
bool poweruppass = false;
float smallradius = 18;

// Level 2 ring passed state
bool ring1passed = false;
bool ring2passed = false;
bool ring3passed = false;

// Level 2 checkpoint
bool checkpoint1 = false;
Vector2 checkpointposition = {2 * blocksize, 100};

// Level 2 gems
Vector2 gem1position = {13 * blocksize, 16 * blocksize};
Vector2 gem2position = {21 * blocksize, 16 * blocksize};
Vector2 gem3position = {21 * blocksize, 3 * blocksize};
bool gem1pass = false;
bool gem2pass = false;
bool gem3pass = false;

Rectangle ringrecup1;
Rectangle ringrecup2;
Rectangle ringrecup3;
Rectangle ringrecup4;
Rectangle ring1recup1 = {10 * blocksize + 43, 10 * blocksize + 5, 15, 8};
Rectangle ring1recdown1 = {10 * blocksize + 43, 12 * blocksize - 6, 15, 12};
Rectangle ringrecdown1;

int score = 0;

typedef struct {
    Rectangle rect;
} block;

block blocks[maxblocks];
int blockcount = 0;

void addblock(int gridx, int gridy) {
    if (blockcount >= maxblocks) return;
    blocks[blockcount].rect = (Rectangle){gridx * blocksize, gridy * blocksize, blocksize, blocksize};
    blockcount++;
}

void addhorizontalplatform(int x, int y, int width) {
    for (int i = 0; i < width; i++) addblock(x + i, y);
}

void addrevhorizontalplatform(int x, int y, int width) {
    for (int i = 0; i < width; i++) addblock(x - i, y);
}

void addrevverticalplatform(int x, int y, int height) {
    for (int i = 0; i < height; i++) addblock(x, y - i);
}

void addverticalplatform(int x, int y, int height) {
    for (int i = 0; i < height; i++) addblock(x, y + i);
}

void levelgeneration1() {
    for (int x = 0; x < (screenwidth / blocksize); x++) addblock(x, 17);
    for (int x = 0; x < (screenwidth / blocksize); x++) addblock(x, 0);
    for (int y = 0; y < (screenheight / blocksize); y++) addblock(23, y);
    for (int y = 0; y < (screenheight / blocksize); y++) addblock(0, y);

    addhorizontalplatform(1, 13, 7);
    addverticalplatform(7, 13, 5);
    addverticalplatform(10, 0, 7);
    addhorizontalplatform(10, 7, 5);
    addverticalplatform(15, 7, 5);
    addrevhorizontalplatform(15, 12, 5);
    addhorizontalplatform(15, 11, 4);
    addhorizontalplatform(20, 8, 3);
    for (int x = 18; x < 24; x++) {
        for (int y = 15; y <= 17; y++) addblock(x, y);
    }
    for (int x = 21; x < 24; x++) {
        for (int y = 13; y <= 16; y++) addblock(x, y);
    }
}

void levelgeneration2() {
    for (int x = 0; x < (screenwidth / blocksize); x++) addblock(x, 17);
    for (int x = 0; x < (screenwidth / blocksize); x++) addblock(x, 0);
    for (int y = 0; y < (screenheight / blocksize); y++) addblock(23, y);
    for (int y = 0; y < (screenheight / blocksize); y++) addblock(0, y);

    addhorizontalplatform(0, 3, 5);
    addhorizontalplatform(1, 9, 19);
    addhorizontalplatform(20, 12, 4);

    addverticalplatform(13, 7, 2);
    addverticalplatform(10, 6, 3);
    addverticalplatform(7, 1, 6);
    addverticalplatform(16, 1, 4);
    addverticalplatform(19, 7, 2);
    addverticalplatform(14, 12, 5);
    addverticalplatform(18, 15, 3);

    addrevhorizontalplatform(13, 14, 4);
    addrevhorizontalplatform(6, 6, 4);
    
    for (int x = 1; x < 5; x++) {
        for (int y = 13; y <= 17; y++) addblock(x, y);
    }
}

void drawlevel() {
    for (int i = 0; i < blockcount; i++) {
        Rectangle r = blocks[i].rect;
        DrawRectangleRec(r, MAROON);
        DrawRectangleLinesEx(r, 3, RED);
        DrawLine(r.x + 5, r.y + 5, r.x + r.width - 5, r.y + 5, ORANGE);
    }
}

bool resolveCircleBlock(Vector2 *position, Vector2 *speed, Rectangle r) {
    float closestX = Clamp(position->x, r.x, r.x + r.width);
    float closestY = Clamp(position->y, r.y, r.y + r.height);
    Vector2 closestPoint = {closestX, closestY};
    Vector2 difference = Vector2Subtract(*position, closestPoint);
    float distance = Vector2Length(difference);

    if (distance > currentradius || distance == 0.0f)
        return false;

    Vector2 normal = Vector2Scale(difference, 1.0f / distance);
    position->x = closestPoint.x + normal.x * currentradius;
    position->y = closestPoint.y + normal.y * currentradius;

    float velocityIntoBlock = Vector2DotProduct(*speed, normal);
    if (velocityIntoBlock < 0.0f)
        *speed = Vector2Subtract(*speed, Vector2Scale(normal, velocityIntoBlock));
    
    return normal.y < -0.5f;
}

bool checkcollision(Vector2 *position, Vector2 *speed) {
    bool onplatform = false;
    for (int i = 0; i < blockcount; i++) {
        if (resolveCircleBlock(position, speed, blocks[i].rect))
            onplatform = true;
    }
    return onplatform;
}

void startLevel(int level) {
    blockcount = 0;

    if (level == 1) {
        levelgeneration1();
        position = position1;
        enemy1position = enemy1position1;
        enemy1speed = enemy1speed1;
        ringbackposition = ringbackposition1;
        ringfrontposition = ringfrontposition1;
        gemposition = gemposition1;
        flagpos = flagpos1;
        ringrecup1 = ring1recup1;
        ringrecdown1 = ring1recdown1;
        currentradius = radius;
    } else if (level == 2) {
        levelgeneration2();
        position = position2;
        enemy1position = enemy1position2;
        enemy1speed = enemy1speed2;
        enemy2position = enemy2position2;
        enemy2speed = enemy2speed2;
        enemy3position = enemy3position2;
        enemy3speed = enemy3speed2;

        ringbackposition = ringbackposition2;
        ringfrontposition = ringfrontposition2;
        gemposition = gemposition2;
        flagpos = flagpos2;

        ring1backposition = (Vector2){3 * blocksize, 8 * blocksize};
        ring1frontposition = (Vector2){3 * blocksize, 8 * blocksize};
        ring2backposition = (Vector2){23 * blocksize, 8 * blocksize};
        ring2frontposition = (Vector2){23 * blocksize, 8 * blocksize};
        ring3backposition = (Vector2){14.4f * blocksize, 11 * blocksize};
        ring3frontposition = (Vector2){14.4f * blocksize, 11 * blocksize};

        ring1passed = false;
        ring2passed = false;
        ring3passed = false;
        checkpoint1 = false;
        checkpointposition = (Vector2){2 * blocksize, 100};
        poweruppass = false;
        currentradius = radius; // Reset to regular radius initially
        gem1pass = false;
        gem2pass = false;
        gem3pass = false;
    }

    speed = Vector2Zero();
    gravity = (Vector2){0, Gravity};
    enemy1size = 80;
    gempass = 0;
    gemcount = 0;
    ringcount = 0;
    enemycol = 0;
    gemsize = 0.05f;
    ringsize = 0.4f;
    ringradius = 25;
}

int main() {
    InitWindow(screenwidth, screenheight, "game");
    InitAudioDevice();
    SetTargetFPS(60);

    int loading = 0;
    int play = 0;
    int rules = 0;
    int leaderboard = 0;

    char name[100] = "";
    int namelength = 0;
    int entername = 1;
    int score = 0;
    int startgame = 0;
    int menu = 0;
    int scoresaved = 0;

    Texture2D first_background = LoadTexture("assets/firstpage.png");
    Texture2D menubar = LoadTexture("assets/menubar.png");
    Texture2D playbutton = LoadTexture("assets/playbutton.png");
    Texture2D playglowing = LoadTexture("assets/playglowing.png");
    Texture2D rulesbutton = LoadTexture("assets/rulesbutton.png");
    Texture2D rulesglowing = LoadTexture("assets/rulesglowing.png");
    Texture2D leaderboardbutton = LoadTexture("assets/leaderboardbutton.png");
    Texture2D leaderboardglowing = LoadTexture("assets/leaderboardglowing.png");
    Texture2D rulesbg = LoadTexture("assets/rulesbg.png");
    Texture2D rulesbackglowing = LoadTexture("assets/rulesbackglowing.png");
    Texture2D startgamebutton = LoadTexture("assets/startgamebutton.png");
    Texture2D startgameglowing = LoadTexture("assets/startgameglowing.png");

    int levelcount = 1;
    startLevel(levelcount);

    Texture2D ball = LoadTexture("assets/ball.png");
    Texture2D enemy1 = LoadTexture("assets/enemy1.png");
    Texture2D enemy2 = LoadTexture("assets/enemy1.png");
    Texture2D enemy3 = LoadTexture("assets/enemy1.png");
    
    Sound bounce = LoadSound("assets/bouncesound.mp3");
    Sound ringpasssound = LoadSound("assets/ring pass.mp3");

    Texture2D explosiontext = LoadTexture("assets/explosion.png");
    float explosionwidth = (float)explosiontext.width / 5;
    float explosionheight = (float)explosiontext.height;
    int currentframe = 0;
    int framescounter = 0;
    bool active = false;
    Rectangle explosionrec = {0, 0, explosionwidth, explosionheight};
    Vector2 explosionposition = {0.0f, 0.0f};
    Sound explosion = LoadSound("assets/explosion.mp3");

    float respawntimer = 0.0f;
    bool respawn = false;

    Texture2D gem = LoadTexture("assets/gem.png");
    Sound gemsound = LoadSound("assets/gemsound.mp3");

    Texture2D powerup = LoadTexture("assets/powerup.png");
    Sound powerupsound = LoadSound("assets/powerupsound.mp3");

    int gameover = 0;
    Texture2D button = LoadTexture("assets/buttonoriginal.png");
    Texture2D hoveredbutton = LoadTexture("assets/restart hovered.png");
    Sound buttonsound = LoadSound("assets/buttonsound.mp3");

    Texture2D life = LoadTexture("assets/life.png");
    int lifecount = 3;
    Sound lastlife = LoadSound("assets/lastlife.mp3");
    Sound lifeover = LoadSound("assets/lifeover.mp3");

    Texture2D flag = LoadTexture("assets/finish line.png");
    
    Music menumusic = LoadMusicStream("assets/menumusic.mp3");
    Music gamemusic = LoadMusicStream("assets/gamemusic.mp3");
    menumusic.looping = true;
    gamemusic.looping = true;
    SetMusicVolume(menumusic, 0.2f);
    SetMusicVolume(gamemusic, 0.2f);
    PlayMusicStream(menumusic);

    float timer = 0;

    while (!WindowShouldClose()) {
        UpdateMusicStream(menumusic);
        UpdateMusicStream(gamemusic);

        float dt = GetFrameTime();
        timer += dt;
    
        if (startgame == 0) {
            if (!loading) {
                if (timer < 1.0f) {
                    BeginDrawing();
                    DrawTexturePro(first_background, (Rectangle){0, 0, first_background.width, first_background.height}, (Rectangle){0, 0, screenwidth, screenheight}, (Vector2){0, 0}, 0.0f, WHITE);
                    DrawText("LOADING", 420, 650, 50, BLACK);
                    EndDrawing();
                } else if (timer < 2.0f) {
                    BeginDrawing();
                    DrawTexturePro(first_background, (Rectangle){0, 0, first_background.width, first_background.height}, (Rectangle){0, 0, screenwidth, screenheight}, (Vector2){0, 0}, 0.0f, WHITE);
                    DrawText("LOADING.", 420, 650, 50, BLACK);
                    EndDrawing();
                } else if (timer < 3.0f) {
                    BeginDrawing();
                    DrawTexturePro(first_background, (Rectangle){0, 0, first_background.width, first_background.height}, (Rectangle){0, 0, screenwidth, screenheight}, (Vector2){0, 0}, 0.0f, WHITE);
                    DrawText("LOADING. .", 420, 650, 50, BLACK);
                    EndDrawing();
                } else if (timer < 3.5f) {
                    BeginDrawing();
                    DrawTexturePro(first_background, (Rectangle){0, 0, first_background.width, first_background.height}, (Rectangle){0, 0, screenwidth, screenheight}, (Vector2){0, 0}, 0.0f, WHITE);
                    DrawText("LOADING. . .", 420, 650, 50, BLACK);
                    EndDrawing();
                } else {
                    loading = 1;
                    menu = 1;
                    timer = 0;
                }
            } else {
                Vector2 mousepos = GetMousePosition();
                Rectangle playrec = {420, 100, (float)playbutton.width, (float)playbutton.height};
                Rectangle rulesrec = {420, 300, (float)rulesbutton.width, (float)rulesbutton.height};
                Rectangle leaderboardrec = {420, 500, (float)leaderboardbutton.width, (float)leaderboardbutton.height};
                Rectangle rulesbackrec = {385, 670, 300, 88};
                Rectangle startgamerec = {390, 700, (float)startgamebutton.width / 3.0f, (float)startgamebutton.height / 3.0f};

                int rulesbackpressed = 0;

                if (menu == 1 && play != 2) {
                    if (CheckCollisionPointRec(mousepos, playrec)) {
                        play = 1;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            PlaySound(buttonsound);
                            play = 2;
                            entername = 1;
                            namelength = 0;
                            menu = 0;
                            name[0] = '\0';
                        }
                    } else {
                        play = 0;
                    }
                } else if (play == 2 && entername == 1) {
                    int key = GetCharPressed();
                    while (key > 0) {
                        if ((key >= 32) && (key <= 125) && namelength < 99) {
                            name[namelength] = (char)key;
                            namelength++;
                            name[namelength] = '\0';
                        }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (namelength > 0) {
                            namelength--;
                            name[namelength] = '\0';
                        }
                    }
                }

                if (menu == 1 && rules != 2) {
                    if (CheckCollisionPointRec(mousepos, rulesrec)) {
                        rules = 1;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            rules = 2;
                            menu = 0;
                            PlaySound(buttonsound);
                        }
                    } else {
                        rules = 0;
                    }
                } else if (rules == 2) {
                    if (CheckCollisionPointRec(mousepos, rulesbackrec)) {
                        rulesbackpressed = 1;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            PlaySound(buttonsound);
                            rules = 0;
                            menu = 1;
                        }
                    } else {
                        rulesbackpressed = 0;
                    }
                }

                if (menu == 1 && leaderboard != 2) {
                    if (CheckCollisionPointRec(mousepos, leaderboardrec)) {
                        leaderboard = 1;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            PlaySound(buttonsound);
                            leaderboard = 2;
                            menu = 0;
                        }
                    } else {
                        leaderboard = 0;
                    }
                }

                BeginDrawing();

                if (play == 2 && entername == 1) {
                    ClearBackground(SKYBLUE);
                    DrawText("ENTER YOUR NAME", 350, 200, 40, MAROON);
                    DrawRectangle(300, 300, 480, 70, LIGHTGRAY);
                    DrawRectangleLines(300, 300, 480, 70, BLACK);
                    DrawText(name, 320, 320, 30, BLACK);
                    DrawTexturePro(startgamebutton, (Rectangle){0, 0, (float)startgamebutton.width, (float)startgamebutton.height}, startgamerec, (Vector2){0, 0}, 0.0f, WHITE);
                    DrawText("PRESS 'left arrow key' TO GO BACK", 400, 400, 40, BLUE);

                    if (IsKeyPressed(KEY_LEFT)) {
                        menu = 1;
                        startgame = 0;
                        play = 0;
                        leaderboard = 0;
                        rules = 0;
                    }

                    if (CheckCollisionPointRec(mousepos, startgamerec)) {
                        DrawTexturePro(startgameglowing, (Rectangle){0, 0, (float)startgamebutton.width, (float)startgamebutton.height}, startgamerec, (Vector2){0, 0}, 0.0f, WHITE);
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            PlaySound(buttonsound);
                            gameover = 0;
                            enemycol = 0;
                            lifecount = 3;
                            ringcount = 0;
                            gemcount = 0;
                            gempass = 0;
                            respawn = false;
                            respawntimer = 0.0f;
                            speed = Vector2Zero();
                            score = 0;
                            scoresaved = 0;
                            startgame = 1;

                            StopMusicStream(menumusic);
                            PlayMusicStream(gamemusic);

                            if (namelength == 0) {
                                strcpy(name, "not given");
                            }
                            entername = 0;
                        }
                    }
                } else if (rules == 2) {
                    DrawTexturePro(rulesbg, (Rectangle){0, 0, (float)rulesbg.width, (float)rulesbg.height}, (Rectangle){0, 0, screenwidth, screenheight}, (Vector2){0, 0}, 0.0f, WHITE);
                    if (rulesbackpressed == 1) {
                        DrawTexturePro(rulesbackglowing, (Rectangle){0, 0, (float)rulesbackglowing.width, (float)rulesbackglowing.height}, rulesbackrec, (Vector2){0, 0}, 0.0f, WHITE);
                    }
                } else if (leaderboard == 2) {
                    drawleaderboard();
                    DrawText("PRESS 'B' TO GO BACK", 500, 760, 40, PURPLE);
                    if (IsKeyPressed(KEY_B)) {
                        leaderboard = 0;
                        menu = 1;
                    }
                } else if (menu == 1) {
                    DrawTexturePro(menubar, (Rectangle){0, 0, (float)menubar.width, (float)menubar.height}, (Rectangle){0, 0, screenwidth, screenheight}, (Vector2){0, 0}, 0.0f, WHITE);
                    if (rules == 0) {
                        DrawTexturePro(rulesbutton, (Rectangle){0, 0, (float)rulesbutton.width, (float)rulesbutton.height}, (Rectangle){420, 300, (float)rulesbutton.width, (float)rulesbutton.height}, (Vector2){0, 0}, 0.0f, WHITE);
                    }
                }
                EndDrawing();
            }
        } else {
            // Gameplay Loop
            if (IsKeyPressed(KEY_SPACE) && checkcollision(&position, &speed)) {
                speed.y = -jumpspeed;
                PlaySound(bounce);
            }

            if (IsKeyDown(KEY_RIGHT)) {
                speed.x = 200;
                ballRotation += 200 * dt;
            } else if (IsKeyDown(KEY_LEFT)) {
                speed.x = -200;
                ballRotation -= 200 * dt;
            } else {
                speed.x = 0;
            }

            speed.y += gravity.y * dt;
            position.x += speed.x * dt;
            position.y += speed.y * dt;

            checkcollision(&position, &speed);

            // Level 2 Power-up Logic Fixed
            if (levelcount == 2 && !poweruppass) {
                if (CheckCollisionCircles(position, currentradius, powerupposition, powerupsize)) {
                    poweruppass = true;
                    currentradius = smallradius;
                    PlaySound(powerupsound);
                }
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);
            drawlevel();

            // Draw Powerup if Level 2 and not passed
            if (levelcount == 2 && !poweruppass) {
                DrawTextureV(powerup, powerupposition, WHITE);
            }

            // Draw Ball
            Rectangle source = {0, 0, (float)ball.width, (float)ball.height};
            Rectangle destination = {position.x, position.y, 68, 68};
            Vector2 origin = {destination.width / 2.0f, destination.height / 2.0f};
            DrawTexturePro(ball, source, destination, origin, ballRotation, WHITE);

            EndDrawing();
        }
    }

    // Unload textures and sounds
    UnloadTexture(first_background);
    UnloadTexture(menubar);
    UnloadTexture(ball);
    UnloadTexture(powerup);
    UnloadSound(bounce);
    UnloadSound(powerupsound);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}