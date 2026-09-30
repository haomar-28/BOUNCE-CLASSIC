#include "raylib.h"
#include "raymath.h"

#define screenwidth 1080
#define screenheight 810
#define blocksize 45
#define maxblocks 300
#define maxspeedx 300
#define radius 25
#define jumpspeed 600
#define Gravity 1200

// define block data type
typedef struct
{
    Rectangle rect;
} block;

block blocks[maxblocks];
int blockcount = 0;

// add one block(later needed)

void addblock(int gridx, int gridy)
{
    if (blockcount >= maxblocks)
        return;

    blocks[blockcount].rect = (Rectangle){
        gridx * blocksize, gridy * blocksize, blocksize, blocksize};
    blockcount++;
}

// add a horizontal platform (using addblock) (used later in level generation)

void addhorizontalplatform(int x, int y, int width)
{
    for (int i = 0; i < width; i++)
    {
        addblock(x + i, y);
    }
}

void addrevhorizontalplatform(int x, int y, int width)
{
    for (int i = 0; i < width; i++)
    {
        addblock(x - i, y);
    }
}

void addrevverticalplatform(int x, int y, int height)
{
    for (int i = 0; i < height; i++)
    {
        addblock(x, y - i);
    }
}
void addverticalplatform(int x, int y, int height)
{
    for (int i = 0; i < height; i++)
    {
        addblock(x, y + i);
    }
}

// now the level generating , where we will create left, right, top , bottom wall and random blocks around the screen

void levelgeneration2()
{

    // bottom wall

    for (int x = 0; x < (screenwidth / blocksize); x++)
    {
        addblock(x, 17);
    }

    // top wall

    for (int x = 0; x < (screenwidth / blocksize); x++)
    {
        addblock(x, 0);
    }

    // right wall
    for (int y = 0; y < (screenheight / blocksize); y++)
    {
        addblock(23, y);
    }

    // left wall
    for (int y = 0; y < (screenheight / blocksize); y++)
    {
        addblock(0, y);
    }

    // start design

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

    // rectangle blocks
    for (int x = 1; x < 6; x++)
    {
        for (int y = 13; y <= 17; y++)
        {
            addblock(x, y);
        }
    }
}
// draw blocks
void drawlevel2()
{
    for (int i = 0; i < blockcount; i++)
    {
        Rectangle r = blocks[i].rect;

        // main block
        DrawRectangleRec(r, RED);
        // dark outline
        DrawRectangleLinesEx(r, 3, MAROON);
        // highlight
        DrawLine(
            r.x + 5,
            r.y + 5,
            r.x + r.width - 5,
            r.y + 5, ORANGE);
    }
}

// to change the position and speed after collision
bool resolveCircleBlock(Vector2 *position, Vector2 *speed, Rectangle r)
{
    // find closest point of the block to the center of the ball
    float closestX = Clamp(position->x, r.x, r.x + r.width);
    float closestY = Clamp(position->y, r.y, r.y + r.height);

    Vector2 closestPoint = {closestX, closestY};

    // after subtraction , the direction will be perpendicular to surface
    Vector2 difference = Vector2Subtract(*position, closestPoint);

    float distance = Vector2Length(difference);

    if (distance > radius || distance == 0.0f)
        return false;

    // Normal is a direction only and points away from the block toward the ball.
    Vector2 normal = Vector2Scale(difference, 1.0f / distance);

    // Move ball outside the block,in the direcction to normal (perpendicular to the previous movement of the ball)
    position->x = closestPoint.x + normal.x * radius;
    position->y = closestPoint.y + normal.y * radius;

    // Remove only velocity going into the block.
    float velocityIntoBlock = Vector2DotProduct(*speed, normal);

    if (velocityIntoBlock < 0.0f)
        *speed = Vector2Subtract(*speed, Vector2Scale(normal, velocityIntoBlock));

    // True when standing on a surface.
    return normal.y < -0.5f;
}

bool checkcollision(Vector2 *position, Vector2 *speed)
{
    bool onplatform = false;

    for (int i = 0; i < blockcount; i++)
    {
        // if standing on a surface
        if (resolveCircleBlock(position, speed, blocks[i].rect))
            onplatform = true;
    }

    return onplatform;
}

int main()
{
    InitWindow(screenwidth, screenheight, "game");
    // to activate device;
    InitAudioDevice();

    SetTargetFPS(60);

    // to generate blocks of level 2
    levelgeneration2();

    // for generating the movement of the ball
    Vector2 position = {2 * blocksize, 100};

    Vector2 speed = Vector2Zero();
    Vector2 gravity = {0, Gravity};

    // for the enemy1
    Texture2D enemy1 = LoadTexture("assets/enemy1.png");
    float enemy1size = 80;
    // for enemys rectangle
    Vector2 enemy1position = {8 * blocksize, 1 * blocksize};
    Vector2 enemy1speed = {0, 200};
    Rectangle enemy1rect = {enemy1position.x, enemy1position.y, enemy1size, enemy1size};

    // for the enemy2
    Texture2D enemy2 = LoadTexture("assets/enemy1.png");
    float enemy2size = 45;
    // for enemys rectangle
    Vector2 enemy2position = {3 * blocksize, 10 * blocksize};
    Vector2 enemy2speed = {0, 100};
    Rectangle enemy2rect = {enemy2position.x, enemy2position.y, enemy2size, enemy2size};

    // for the enemy3
    Texture2D enemy3 = LoadTexture("assets/enemy1.png");
    float enemy3size = 80;
    // for enemys rectangle
    Vector2 enemy3position = {11 * blocksize, 10 * blocksize};
    Vector2 enemy3speed = {0, 200};
    Rectangle enemy3rect = {enemy3position.x, enemy3position.y, enemy3size, enemy3size};

    // to load sound of bouncing
    Sound bounce = LoadSound("assets/bouncesound.mp3");

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // for the speed of the ball
        speed = Vector2Add(speed, Vector2Scale(gravity, dt));
        position = Vector2Add(position, Vector2Scale(speed, dt));

        // for checking if the ball is on the platform or not
        bool onplatform = checkcollision(&position, &speed);

        // for movement and jumping
        if (IsKeyDown(KEY_RIGHT))
            speed.x = maxspeedx;

        else if (IsKeyDown(KEY_LEFT))
            speed.x = -maxspeedx;
        else
            speed.x = 0;
        if (IsKeyPressed(KEY_UP) && onplatform)
        {
            speed.y = -jumpspeed;
            PlaySound(bounce);
        }

        // for enemy1s movement
        enemy1position = Vector2Add(enemy1position, Vector2Scale(enemy1speed, dt));
        // for enemys movement restriction
        if (enemy1position.y < 1 * blocksize)
        {
            enemy1speed.y = 200;
        }
        else if (enemy1position.y > 5 * blocksize - enemy1size)
        {
            enemy1speed.y = -200;
        }

        // for enemy2s movement
        enemy2position = Vector2Add(enemy2position, Vector2Scale(enemy2speed, dt));
        // for enemys movement restriction
        if (enemy2position.y < 10 * blocksize)
        {
            enemy2speed.y = 100;
        }
        else if (enemy2position.y > 14 * blocksize - enemy1size)
        {
            enemy2speed.y = -100;
        }

        // for enemy3s movement
        enemy3position = Vector2Add(enemy3position, Vector2Scale(enemy3speed, dt));
        // for enemys movement restriction
        if (enemy3position.y < 10 * blocksize)
        {
            enemy3speed.y = 150;
        }
        else if (enemy3position.y > 14 * blocksize - enemy1size)
        {
            enemy3speed.y = -150;
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);

        // to draw the blocks (level 2)
        drawlevel();

        // draw ball
        DrawCircleV(position, radius, RED);

        // draw all enemys
        DrawTextureEx(enemy1, enemy1position, 0.0f, enemy1size / enemy1.width, WHITE);
        DrawTextureEx(enemy2, enemy2position, 0.0f, enemy2size / enemy2.width, WHITE);
        DrawTextureEx(enemy3, enemy3position, 0.0f, enemy3size / enemy3.width, WHITE);

        EndDrawing();
    }
    UnloadSound(bounce);
    UnloadTexture(enemy1);
    UnloadTexture(enemy2);
    UnloadTexture(enemy3);
    CloseWindow();
    return 0;
}
