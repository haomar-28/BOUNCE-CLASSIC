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
void drawlevel()
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

    /* BALL*/
    // for generating the movement of the ball
    Vector2 position = {2 * blocksize, 100};
    Vector2 speed = Vector2Zero();
    Vector2 gravity = {0, Gravity};

    // for spikes
    
    Texture2D spike1 = LoadTexture("assets/spike.png");

    if (spike1.id == 0)
    {
        TraceLog(LOG_ERROR, "Spike texture NOT loaded!");
    }
    else
    {
        TraceLog(LOG_INFO, "Spike texture loaded successfully!");
    }
    float spike1size = 250;
    // for spikes rectangle
    Vector2 spike1position = {12 * blocksize, 8 * blocksize};
    Rectangle spike1rect = {spike1position.x, spike1position.y, spike1size, spike1size};

    /*ENEMYS*/
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

    /*  RING  */
    // Vertical Ring1//
    // defining the position of the ring
    Vector2 ring1backposition = {3 * blocksize, 8 * blocksize};
    Vector2 ring1frontposition = {3 * blocksize, 8 * blocksize};
    // defining rings structure
    float ring1size = 0.35f;
    float ring1radius = 24;

    // Horizontal Ring2//
    // defining the position of the ring
    Vector2 ring2backposition = {23 * blocksize, 10 * blocksize};
    Vector2 ring2frontposition = {23 * blocksize, 10 * blocksize};
    // defining rings structure
    float ring2size = 0.3f;
    float ring2radius = 20;

    // loading rings textures of front and back
    Texture2D ring1backtexture = LoadTexture("assets/ring back.png");
    Texture2D ring1fronttexture = LoadTexture("assets/ring front.png");

    //*2parts will not need*//
    Texture2D ring2backtexture = LoadTexture("assets/ring back.png");
    Texture2D ring2fronttexture = LoadTexture("assets/ring front.png");
    //*2parts will not need*//

    // to load sound of bouncing
    Sound bounce = LoadSound("assets/bouncesound.mp3");

    /*  EXPLOSION */
    // for generating explosion after collision with enemy1
    Texture2D explosiontext = LoadTexture("assets/explosion.png");

    // for explosion effect
    float explosionwidth = (float)explosiontext.width / 5;
    float explosionheight = (float)explosiontext.height;
    int currentframe = 0;
    int currentline = 0;
    int framescounter = 0;
    bool active = false;

    Rectangle explosionrec = {explosionwidth * currentframe, 0, explosionwidth, explosionheight};
    Vector2 explosionposition = {0.0f, 0.0f};

    // loading explosion sound
    Sound explosion = LoadSound("assets/explosion.mp3");

    // for respawning the ball and explosion
    float respawntimer = 0.0f;
    bool respawn = false;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // for the speed of the ball when the collision happened once
        if (!respawn)
        {
            speed = Vector2Add(speed, Vector2Scale(gravity, dt));
            position = Vector2Add(position, Vector2Scale(speed, dt));
        }

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

        /*MOVE THE ENEMY*/

        // for enemy1s movement
        enemy1position = Vector2Add(enemy1position, Vector2Scale(enemy1speed, dt));
        // for enemys movement restriction
        if (enemy1position.y < 1 * blocksize)
        {
            enemy1speed.y = 160;
        }
        else if (enemy1position.y > 7 * blocksize - enemy1size)
        {
            enemy1speed.y = -125;
        }

        // for enemy2s movement
        enemy2position = Vector2Add(enemy2position, Vector2Scale(enemy2speed, dt));
        // for enemys movement restriction
        if (enemy2position.y < 10 * blocksize)
        {
            enemy2speed.y = 100;
        }
        else if (enemy2position.y > 13 * blocksize - enemy2size)
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
        else if (enemy3position.y > 14 * blocksize - enemy3size)
        {
            enemy3speed.y = -150;
        }

        // UPDATE COLLISION RECTANGLE
        enemy1rect.x = enemy1position.x;
        enemy1rect.y = enemy1position.y;

        enemy2rect.x = enemy2position.x;
        enemy2rect.y = enemy2position.y;

        enemy3rect.x = enemy3position.x;
        enemy3rect.y = enemy3position.y;

        /*  COLLISION  */
        // for enemy and ball collision
        // for enemy1
        if (!respawn && CheckCollisionCircleRec(position, radius, enemy1rect))
        {
            PlaySound(explosion);
            explosionposition = position;
            active = true;
            explosionposition.x = position.x - explosionwidth / 2.0f;
            explosionposition.y = position.y - explosionheight / 2.0f;

            respawn = true;
            respawntimer = 0.3f;
        }

        if (respawn)
        {
            respawntimer -= dt;
            if (respawntimer <= 0.0f)
            {
                position.x = 2 * blocksize;
                position.y = 100;
                respawntimer = 0.0f;
                respawn = false;
            }
        }

        // for enemy 2 and 3
        if (!respawn && CheckCollisionCircleRec(position, radius, enemy2rect))
        {
            PlaySound(explosion);
            explosionposition = position;
            active = true;
            explosionposition.x = position.x - explosionwidth / 2.0f;
            explosionposition.y = position.y - explosionheight / 2.0f;

            respawn = true;
            respawntimer = 0.3f;
        }

        if (respawn)
        {
            respawntimer -= dt;
            if (respawntimer <= 0.0f)
            {
                position.x = 2 * blocksize;
                position.y = 100;
                respawntimer = 0.0f;
                respawn = false;
            }
        }

        if (!respawn && CheckCollisionCircleRec(position, radius, enemy3rect))
        {
            PlaySound(explosion);
            explosionposition = position;
            active = true;
            explosionposition.x = position.x - explosionwidth / 2.0f;
            explosionposition.y = position.y - explosionheight / 2.0f;

            respawn = true;
            respawntimer = 0.3f;
        }

        if (respawn)
        {
            respawntimer -= dt;
            if (respawntimer <= 0.0f)
            {
                position.x = 2 * blocksize;
                position.y = 100;
                respawntimer = 0.0f;
                respawn = false;
            }
        }

        // explosion animation calculation
        if (active)
        {
            framescounter++;
            if (framescounter > 2)
            {
                currentframe++;

                if (currentframe >= 5)
                {
                    currentframe = 0;
                    active = false;
                }
                framescounter = 0;
            }
        }

        // new explosion position
        explosionrec.x = explosionwidth * currentframe;
        explosionrec.y = explosionheight * currentline;

        BeginDrawing();
        ClearBackground(SKYBLUE);

        // draw spike
        DrawTextureEx(spike1, spike1position, 0.0f, spike1size / spike1.width, WHITE);

        // to draw the blocks (level 1)
        drawlevel();

        // drawing front rings1 first
        DrawTextureEx(
            ring1fronttexture,
            (Vector2){
                ring1frontposition.x - ring1fronttexture.width * ring1size / 2, ring1frontposition.y - ring1fronttexture.height * ring1size / 2},
            0.0f,
            ring1size,
            WHITE);

        // drawing front rings2 first
        DrawTextureEx(
            ring2fronttexture,
            (Vector2){
                ring2frontposition.x - ring2fronttexture.width * ring2size / 2, ring2frontposition.y - ring2fronttexture.height * ring2size / 2},
            90.0f,
            ring2size,
            WHITE);

        // draw ball
        DrawCircleV(position, radius, RED);

        // after ball, drawing back rings1
        DrawTextureEx(
            ring1backtexture,
            (Vector2){
                ring1backposition.x - ring1backtexture.width * ring1size / 2, ring1backposition.y - ring1backtexture.height * ring1size / 2},
            0.0f,
            ring1size,
            WHITE);

        DrawTextureEx(
            ring2backtexture,
            (Vector2){
                ring2backposition.x - ring2backtexture.width * ring2size / 2, ring2backposition.y - ring2backtexture.height * ring2size / 2},
            90.0f,
            ring2size,
            WHITE);

        // draw enemy
        DrawTextureEx(enemy1, enemy1position, 0.0f, enemy1size / enemy1.width, WHITE);
        DrawTextureEx(enemy2, enemy2position, 0.0f, enemy2size / enemy2.width, WHITE);
        DrawTextureEx(enemy3, enemy3position, 0.0f, enemy3size / enemy3.width, WHITE);

        // draw explosion effect
        if (active)
        {
            DrawTextureRec(explosiontext, explosionrec, explosionposition, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(explosiontext);
    UnloadSound(explosion);
    UnloadSound(bounce);
    UnloadTexture(enemy1);
    UnloadTexture(spike1);
    UnloadTexture(ring1fronttexture);
    UnloadTexture(ring2backtexture);
    CloseWindow();
    return 0;
}