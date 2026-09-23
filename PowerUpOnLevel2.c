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

// needed for power up
float currentradius = radius;

// define block data type
typedef struct
{
    Rectangle rect;
} block;

block blocks[maxblocks];
int blockcount = 0;

// add one block
void addblock(int gridx, int gridy)
{
    if (blockcount >= maxblocks)
        return;

    blocks[blockcount].rect = (Rectangle){
        gridx * blocksize, gridy * blocksize, blocksize, blocksize};
    blockcount++;
}

// add a horizontal platform
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
    for (int x = 1; x < 5; x++)
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

    if (distance > currentradius || distance == 0.0f)
        return false;

    // Normal is a direction only and points away from the block toward the ball.
    Vector2 normal = Vector2Scale(difference, 1.0f / distance);

    // Move ball outside the block,in the direcction to normal (perpendicular to the previous movement of the ball)
    position->x = closestPoint.x + normal.x * currentradius;
    position->y = closestPoint.y + normal.y * currentradius;

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

// VERTICAL RING COLLISION //
// Ring 1 and Ring 3

bool checkVerticalRingCollision(
    Vector2 *position, Vector2 *speed, Rectangle ringTop, Rectangle ringBottom)
{
    bool onring = false;

    /* top collision */
    resolveCircleBlock(position, speed, ringTop);

    /* bottom collision */
    if (resolveCircleBlock(position, speed, ringBottom))
    {
        onring = true;
    }
    return onring;
}

// HORIZONTAL RING COLLISION//
// ring 2
void checkHorizontalRingCollision(Vector2 *position, Vector2 *speed, Rectangle ringLeft, Rectangle ringRight)
{
    // for left and right collision //
    resolveCircleBlock(position, speed, ringLeft);
    resolveCircleBlock(position, speed, ringRight);
}

int main()
{
    InitWindow(screenwidth, screenheight, " bounce classic");
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

    // powerup //
    Texture2D powerup = LoadTexture("assets/epowerup.png");
    float powerupsize = 30;
    Vector2 powerupposition = {16 * blocksize, 15.6 * blocksize};

    bool poweruppass = false;
    float smallradius = 18;

    if (!poweruppass)
    {
        DrawTextureEx(powerup, powerupposition, 0.0f, powerupsize / powerup.width, WHITE);
    }

    //*SERIES OF SPIKES *//
    //*FOR SERIES 1 OF SPIKES *//
    Texture2D spike1 = LoadTexture("assets/spike.png");
    float spike1size = 40;
    // for spike rectangle
    Vector2 spike1position = {10.83 * blocksize, 7.6 * blocksize};
    Rectangle spike1rect = {spike1position.x, spike1position.y, spike1size, spike1size};

    Texture2D spike2 = LoadTexture("assets/spike.png");
    float spike2size = 40;
    // for spike rectangle
    Vector2 spike2position = {11.3 * blocksize, 7.6 * blocksize};
    Rectangle spike2rect = {spike2position.x, spike2position.y, spike2size, spike2size};

    Texture2D spike3 = LoadTexture("assets/spike.png");
    float spike3size = 40;
    // for spikes rectangle
    Vector2 spike3position = {11.77 * blocksize, 7.6 * blocksize};
    Rectangle spike3rect = {spike3position.x, spike3position.y, spike3size, spike3size};

    Texture2D spike4 = LoadTexture("assets/spike.png");
    float spike4size = 40;
    // for spikes rectangle
    Vector2 spike4position = {12.24 * blocksize, 7.6 * blocksize};
    Rectangle spike4rect = {spike4position.x, spike4position.y, spike4size, spike4size};

    //*FOR SERIES 2 OF SPIKES *//
    Texture2D spike5 = LoadTexture("assets/spike.png");
    float spike5size = 40;
    // for spikes rectangle
    Vector2 spike5position = {13.83 * blocksize, 7.6 * blocksize};
    Rectangle spike5rect = {spike5position.x, spike5position.y, spike5size, spike5size};

    Texture2D spike6 = LoadTexture("assets/spike.png");
    float spike6size = 40;
    // for spikes rectangle
    Vector2 spike6position = {14.3 * blocksize, 7.6 * blocksize};
    Rectangle spike6rect = {spike6position.x, spike6position.y, spike6size, spike6size};

    Texture2D spike7 = LoadTexture("assets/spike.png");
    float spike7size = 40;
    // for spikes rectangle
    Vector2 spike7position = {14.77 * blocksize, 7.6 * blocksize};
    Rectangle spike7rect = {spike7position.x, spike7position.y, spike4size, spike4size};

    //*FOR SERIES 3 OF SPIKES *//
    Texture2D spike8 = LoadTexture("assets/spike.png");
    float spike8size = 40;
    // for spikes rectangle
    Vector2 spike8position = {7.7 * blocksize, 15.6 * blocksize};
    Rectangle spike8rect = {spike8position.x, spike8position.y, spike4size, spike4size};

    Texture2D spike9 = LoadTexture("assets/spike.png");
    float spike9size = 40;
    // for spikes rectangle
    Vector2 spike9position = {6.7 * blocksize, 15.6 * blocksize};
    Rectangle spike9rect = {spike9position.x, spike9position.y, spike4size, spike4size};

    // array for spikerects
    Rectangle spikerects[] = {
        spike1rect, spike2rect, spike3rect, spike4rect, spike5rect,
        spike6rect, spike7rect, spike8rect, spike9rect};
    int spikecount = 9;

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
    Vector2 enemy3position = {10.7 * blocksize, 10 * blocksize};
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

    /* Ring 1 collision areas */
    Rectangle ring1top = {ring1frontposition.x - 5, ring1frontposition.y - ring1radius - 16, 12, 8};
    Rectangle ring1bottom = {ring1frontposition.x - 4.7, ring1frontposition.y + ring1radius + 15, 12, 8};

    // Vertical Ring3//
    // defining the position of the ring
    Vector2 ring3backposition = {14.4 * blocksize, 11 * blocksize};
    Vector2 ring3frontposition = {14.4 * blocksize, 11 * blocksize};
    // defining rings structure
    float ring3size = 0.22f;
    float ring3radius = 22;

    /* Ring 3 collision areas */
    Rectangle ring3top = {ring3frontposition.x - 1.8, ring3frontposition.y - ring3radius - 6, 7, 6};
    Rectangle ring3bottom = {ring3frontposition.x - 1.8, ring3frontposition.y + ring3radius, 7, 6};

    // Horizontal Ring2//
    // defining the position of the ring
    Vector2 ring2backposition = {23 * blocksize, 10 * blocksize};
    Vector2 ring2frontposition = {23 * blocksize, 10 * blocksize};
    // defining rings structure
    float ring2size = 0.3f;
    float ring2radius = 20;

    /* Ring 2 collision areas */
    Rectangle ring2left = {ring2frontposition.x - 93, ring2frontposition.y - 15, 4, 10};
    Rectangle ring2right = {ring2frontposition.x - 33, ring2frontposition.y - 15, 4, 10};

    // loading rings textures of front and back
    Texture2D ring1backtexture = LoadTexture("assets/ring back.png");
    Texture2D ring1fronttexture = LoadTexture("assets/ring front.png");

    Texture2D ring2backtexture = LoadTexture("assets/ring back.png");
    Texture2D ring2fronttexture = LoadTexture("assets/ring front.png");

    Texture2D ring3backtexture = LoadTexture("assets/ring back.png");
    Texture2D ring3fronttexture = LoadTexture("assets/ring front.png");

    // loading rings textures of front and back for black&white
    Texture2D ring1backbwtexture = LoadTexture("assets/ring back b&w.png");
    Texture2D ring1frontbwtexture = LoadTexture("assets/ring front b&w.png");

    Texture2D ring2backbwtexture = LoadTexture("assets/ring back b&w.png");
    Texture2D ring2frontbwtexture = LoadTexture("assets/ring front b&w.png");

    Texture2D ring3backbwtexture = LoadTexture("assets/ring back b&w.png");
    Texture2D ring3frontbwtexture = LoadTexture("assets/ring front b&w.png");

    // ring passing sound
    Sound ringpasssound = LoadSound("assets/ring pass.mp3");

    // ring passed state //
    bool ring1passed = false;
    bool ring2passed = false;
    bool ring3passed = false;

    int ringcount = 0;

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
        float dt = GetFrameTime() / 3;

        // for the speed of the ball when the collision happened once
        Vector2 prevposition = position;

        if (!respawn)
        {
            speed = Vector2Add(speed, Vector2Scale(gravity, dt));
            position = Vector2Add(position, Vector2Scale(speed, dt));
        }

        // for checking if the ball is on the platform or not //
        bool onplatform = checkcollision(&position, &speed);

        // for checking ring collision //
        bool onring1 = checkVerticalRingCollision(&position, &speed, ring1top, ring1bottom);

        checkHorizontalRingCollision(&position, &speed, ring2left, ring2right);

        bool onring3 = checkVerticalRingCollision(&position, &speed, ring3top, ring3bottom);

        // for movement and jumping
        if (IsKeyDown(KEY_RIGHT))
            speed.x = maxspeedx;

        else if (IsKeyDown(KEY_LEFT))
            speed.x = -maxspeedx;
        else
            speed.x = 0;
        if (IsKeyPressed(KEY_UP) && (onplatform || onring1 || onring3))
        {
            speed.y = -jumpspeed;
            PlaySound(bounce);
        }

        // RING 1  Vertical ring//

        if (!ring1passed &&
            (
                // Left to Right
                (
                    prevposition.x + currentradius < ring1frontposition.x + ring1radius &&
                    position.x + currentradius >= ring1frontposition.x + ring1radius)

                ||
                // Right to Left
                (
                    prevposition.x - currentradius > ring1frontposition.x - ring1radius &&
                    position.x - currentradius <= ring1frontposition.x - ring1radius)) &&

            position.y + currentradius > ring1frontposition.y - ring1radius &&
            position.y - currentradius < ring1frontposition.y + ring1radius)
        {
            ring1passed = true;
            ringcount++;

            PlaySound(ringpasssound);
        }

        /* Ring 2 Pass Detection */

        if (!ring2passed &&
            (
                // Top to Bottom
                (
                    prevposition.y < ring2frontposition.y &&
                    position.y >= ring2frontposition.y)

                ||

                // Bottom to Top
                (
                    prevposition.y > ring2frontposition.y &&
                    position.y <= ring2frontposition.y)) &&
            position.x + currentradius > ring2left.x &&
            position.x - currentradius < ring2right.x + ring2right.width)
        {
            ring2passed = true;
            ringcount++;

            PlaySound(ringpasssound);
        }
        // RING  3 Vertical ring//

        if (!ring3passed &&
            (
                // Left to Right
                (
                    prevposition.x + currentradius < ring3frontposition.x + ring3radius &&
                    position.x + currentradius >= ring3frontposition.x + ring3radius)

                ||
                // Right to Left
                (
                    prevposition.x - currentradius > ring3frontposition.x - ring3radius &&
                    position.x - currentradius <= ring3frontposition.x - ring3radius)) &&

            position.y + currentradius > ring3frontposition.y - ring3radius &&
            position.y - currentradius < ring3frontposition.y + ring3radius)
        {
            ring3passed = true;
            ringcount++;

            PlaySound(ringpasssound);
        }

        /* POWER UP COLLISION */
        if (!poweruppass &&
            CheckCollisionCircles(
                position,
                currentradius,
                (Vector2){
                    powerupposition.x + powerupsize / 2,
                    powerupposition.y + powerupsize / 2},
                powerupsize / 2))
        {
            poweruppass = true;
            currentradius = smallradius;
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

        // COLLISION //

        // for spike and ball collision
        for (int i = 0; i < spikecount; i++)
        {
            if (!respawn && CheckCollisionCircleRec(position, radius, spikerects[i]))
            {
                // explosion
                PlaySound(explosion);

                explosionposition = position;
                active = true;

                explosionposition.x = position.x - explosionwidth / 2.0f;
                explosionposition.y = position.y - explosionheight / 2.0f;

                // respawn
                respawn = true;
                respawntimer = 0.3f;
            }
        }

        // for enemy and ball collision
        // for enemy1
        if (!respawn && CheckCollisionCircleRec(position, currentradius, enemy1rect))
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
        if (!respawn && CheckCollisionCircleRec(position, currentradius, enemy2rect))
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

        if (!respawn && CheckCollisionCircleRec(position, currentradius, enemy3rect))
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

        // to draw the blocks (level 2)
        drawlevel();

        if (!poweruppass)
        {
            DrawTextureEx(
                powerup, powerupposition, 0.0f, powerupsize / powerup.width, WHITE);
        }

        // draw series of spikes
        DrawTextureEx(spike1, spike1position, 0.0f, spike1size / spike1.width, WHITE);
        DrawTextureEx(spike2, spike2position, 0.0f, spike2size / spike2.width, WHITE);
        DrawTextureEx(spike3, spike3position, 0.0f, spike3size / spike3.width, WHITE);
        DrawTextureEx(spike4, spike4position, 0.0f, spike4size / spike4.width, WHITE);

        DrawTextureEx(spike5, spike5position, 0.0f, spike5size / spike5.width, WHITE);
        DrawTextureEx(spike6, spike6position, 0.0f, spike6size / spike6.width, WHITE);
        DrawTextureEx(spike7, spike7position, 0.0f, spike7size / spike7.width, WHITE);

        DrawTextureEx(spike8, spike8position, 0.0f, spike8size / spike8.width, WHITE);
        DrawTextureEx(spike9, spike9position, 0.0f, spike9size / spike9.width, WHITE);

        // drawing front rings1 first
        DrawTextureEx(
            ring1passed ? ring1frontbwtexture : ring1fronttexture,
            (Vector2){
                ring1frontposition.x - ring1fronttexture.width * ring1size / 2, ring1frontposition.y - ring1fronttexture.height * ring1size / 2},
            0.0f,
            ring1size,
            WHITE);

        DrawTextureEx(
            ring3passed ? ring3frontbwtexture : ring3fronttexture,
            (Vector2){
                ring3frontposition.x - ring3fronttexture.width * ring3size / 2, ring3frontposition.y - ring3fronttexture.height * ring3size / 2},
            0.0f,
            ring3size,
            WHITE);

        // drawing front rings2 first
        DrawTextureEx(
            ring2passed ? ring2frontbwtexture : ring2fronttexture,
            (Vector2){
                ring2frontposition.x - ring2fronttexture.width * ring2size / 2, ring2frontposition.y - ring2fronttexture.height * ring2size / 2},
            90.0f,
            ring2size,
            WHITE);

        // draw ball
        DrawCircleV(position, currentradius, RED);

        /* Rect for Ring Collision
        DrawRectangleLinesEx(ring1top, 3, BLACK);
        DrawRectangleLinesEx(ring1bottom, 3, BLUE);
        DrawRectangleLinesEx(ring2right, 3, BLACK);
        DrawRectangleLinesEx(ring2left, 3, BLUE);
        DrawRectangleLinesEx(ring3top, 3, BLACK);
        DrawRectangleLinesEx(ring3bottom, 3, BLUE);
         */

         
        // after ball, drawing back rings1
        DrawTextureEx(
            ring1passed ? ring1backbwtexture : ring1backtexture,
            (Vector2){
                ring1backposition.x - ring1backtexture.width * ring1size / 2, ring1backposition.y - ring1backtexture.height * ring1size / 2},
            0.0f,
            ring1size,
            WHITE);

        DrawTextureEx(
            ring3passed ? ring3backbwtexture : ring3backtexture,
            (Vector2){
                ring3backposition.x - ring3backtexture.width * ring3size / 2, ring3backposition.y - ring3backtexture.height * ring3size / 2},
            0.0f,
            ring3size,
            WHITE);

        DrawTextureEx(
            ring2passed ? ring2backbwtexture : ring2backtexture,
            (Vector2){
                ring2backposition.x - ring2backtexture.width * ring2size / 2, ring2backposition.y - ring2backtexture.height * ring2size / 2},
            90.0f,
            ring2size,
            WHITE);

        // draw all enemy
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
    UnloadSound(ringpasssound);
    UnloadTexture(enemy1);
    UnloadTexture(spike1);
    UnloadTexture(powerup);
    UnloadTexture(ring1fronttexture);
    UnloadTexture(ring1backtexture);
    UnloadTexture(ring1frontbwtexture);
    UnloadTexture(ring1backbwtexture);
    UnloadTexture(ring2frontbwtexture);
    UnloadTexture(ring2backbwtexture);
    UnloadTexture(ring3frontbwtexture);
    UnloadTexture(ring3backbwtexture);

    CloseWindow();

    return 0;
}
