#include "raylib.h"
#include "raymath.h"

#define screenwidth 1080
#define screenheight 810
#define blocksize 30
#define maxblocks 300
#define maxspeedx 300
#define radius 17
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

// add one block(later needed)

void addblock(int gridx, int gridy)
{
    if (blockcount >= maxblocks)
        return;

    blocks[blockcount].rect = (Rectangle){
        gridx * blocksize, gridy * blocksize, blocksize, blocksize};
    blockcount++;
}

// for adding tunnel blocks
void addrectangle(int x, int y, int width, int height)
{
    if (blockcount >= maxblocks)
        return;

    blocks[blockcount].rect = (Rectangle){x, y, width, height};

    blockcount++;
}

// add a horizontal platform (using addblock) (used later in level generation)

void addhorizontalplatform(float x, float y, int width)
{
    for (int i = 0; i < width; i++)
    {
        addblock(x + i, y);
    }
}

void addverticalplatform(float x, float y, int height)
{
    for (int i = 0; i < height; i++)
    {
        addblock(x, y + i);
    }
}

// now the level generating , where we will create left, right, top , bottom wall and random blocks around the screen

void levelgeneration3()
{
    // bottom wall

    for (int x = 0; x < (screenwidth / blocksize); x++)
    {
        addblock(x, 26);
    }

    // top wall

    for (int x = 0; x < (screenwidth / blocksize); x++)
    {
        addblock(x, 0);
    }

    // right wall
    for (int y = 0; y < (screenheight / blocksize); y++)
    {
        addblock(35, y);
    }

    // left wall
    for (int y = 0; y < (screenheight / blocksize); y++)
    {
        addblock(0, y);
    }

    // start design
    // platform1,2
    addhorizontalplatform(1, 8, 29);
    addhorizontalplatform(6, 18, 29);
    addhorizontalplatform(1, 4, 4);
    addhorizontalplatform(18, 4, 3);

    // platform3
    // single blocks
    addhorizontalplatform(5, 22, 1);
    addhorizontalplatform(7, 23, 1);
    addhorizontalplatform(9, 24, 1);

    addhorizontalplatform(6, 13, 1);

    addhorizontalplatform(13, 21, 3);
    addhorizontalplatform(13, 24, 3);

    // platform1
    addverticalplatform(15, 1, 3);
    addverticalplatform(25, 1, 2);
    addverticalplatform(25, 6, 3);

    addverticalplatform(34, 14, 4);

    for (int x = 1; x < 4; x++)
    {
        for (int y = 14; y <= 18; y++)
        {
            addblock(x, y);
        }
    }

    for (int x = 9; x < 13; x++)
    {
        for (int y = 5; y <= 8; y++)
        {
            addblock(x, y);
        }
    }

    // for tunnel
    addrectangle(230, 314, 670, 15);
    addrectangle(230, 484, 670, 15);
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
        *speed = Vector2Subtract(
            *speed,
            Vector2Scale(normal, velocityIntoBlock));

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

bool checkVerticalRingCollision(Vector2 *position, Vector2 *speed, Rectangle ringTop, Rectangle ringBottom)
{
    bool onring = false;

    // Top rect collision
    if (resolveCircleBlock(position, speed, ringTop))
    {
        onring = true;
    }

    // Bottom rect collision
    if (resolveCircleBlock(position, speed, ringBottom))
    {
        onring = true;
    }

    return onring;
}

int main()
{
    InitWindow(screenwidth, screenheight, "Bounce Classic");

    // to activate device;
    InitAudioDevice();

    SetTargetFPS(60);

    // to generate blocks of level 3
    levelgeneration3();

    /* BALL*/
    // for generating the movement of the ball
    Vector2 position = {3 * blocksize, 2 * blocksize};
    Vector2 speed = Vector2Zero();
    Vector2 gravity = {0, Gravity};

    // for win window
    bool gamewin = false;

    // first powerup  - large //
    Texture2D powerup = LoadTexture("assets/epowerup.png");
    float powerupsize = 25;
    Vector2 powerupposition = {33 * blocksize, 11.6 * blocksize};

    bool poweruppass = false;
    float largeradius = 22;

    // second powerup - small //
    Texture2D smallpowerup = LoadTexture("assets/epowerup.png");
    float smallpowerupsize = 25;
    Vector2 smallpowerupposition = {4.65 * blocksize, 17.8 * blocksize};

    bool smallpoweruppass = false;
    float smallradius = 17;

    // to load the sound of power up
    Sound powerupsound = LoadSound("assets/powerupsound.mp3");

    if (!poweruppass)
    {
        DrawTextureEx(powerup, powerupposition, 0.0f, powerupsize / powerup.width, WHITE);
    }

    if (!smallpoweruppass)
    {
        DrawTextureEx(smallpowerup, smallpowerupposition, 0.0f,
                      smallpowerupsize / smallpowerup.width, WHITE);
    }

    //*SERIES OF SPIKES *//
    Texture2D spike1 = LoadTexture("assets/spike.png");
    float spike1size = 25;
    // for spike rectangle
    Vector2 spike1position = {18.4 * blocksize, 6.7 * blocksize};
    Rectangle spike1rect = {spike1position.x, spike1position.y, spike1size, spike1size};

    Texture2D spike2 = LoadTexture("assets/spike.png");
    float spike2size = 25;
    // for spike rectangle
    Vector2 spike2position = {19 * blocksize, 6.7 * blocksize};
    Rectangle spike2rect = {spike2position.x, spike2position.y, spike2size, spike2size};

    Texture2D spike3 = LoadTexture("assets/spike.png");
    float spike3size = 25;
    // for spikes rectangle
    Vector2 spike3position = {26 * blocksize, 6.7 * blocksize};
    Rectangle spike3rect = {spike3position.x, spike3position.y, spike3size, spike3size};

    Texture2D spike4 = LoadTexture("assets/spike.png");
    float spike4size = 25;
    // for spikes rectangle
    Vector2 spike4position = {26.5 * blocksize, 6.7 * blocksize};
    Rectangle spike4rect = {spike4position.x, spike4position.y, spike4size, spike4size};

    Texture2D spike5 = LoadTexture("assets/spike.png");
    float spike5size = 27;
    // for spikes rectangle
    Vector2 spike5position = {6 * blocksize, 24.6 * blocksize};
    Rectangle spike5rect = {spike5position.x, spike5position.y, spike5size, spike5size};

    Texture2D spike6 = LoadTexture("assets/spike.png");
    float spike6size = 27;
    // for spikes rectangle
    Vector2 spike6position = {5.5 * blocksize, 24.6 * blocksize};
    Rectangle spike6rect = {spike6position.x, spike6position.y, spike6size, spike6size};

    Texture2D spike7 = LoadTexture("assets/spike.png");
    float spike7size = 25;
    // for spikes rectangle
    Vector2 spike7position = {31 * blocksize, 24.7 * blocksize};
    Rectangle spike7rect = {spike7position.x, spike7position.y, spike4size, spike4size};

    Texture2D spike8 = LoadTexture("assets/spike.png");
    float spike8size = 25;
    // for spikes rectangle
    Vector2 spike8position = {30.5 * blocksize, 24.7 * blocksize};
    Rectangle spike8rect = {spike8position.x, spike8position.y, spike4size, spike4size};

    Texture2D spike9 = LoadTexture("assets/spike.png");
    float spike9size = 25;
    // for spikes rectangle
    Vector2 spike9position = {28 * blocksize, 9.2 * blocksize};
    Rectangle spike9rect = {spike9position.x, spike9position.y, spike9size, spike9size};

    Texture2D spike10 = LoadTexture("assets/spike.png");
    float spike10size = 25;
    // for spikes rectangle
    Vector2 spike10position = {28 * blocksize, 16.7 * blocksize};
    Rectangle spike10rect = {spike10position.x, spike10position.y, spike10size, spike10size};

    // array for spikerects
    Rectangle spikerects[] = {
        spike1rect, spike2rect, spike3rect, spike4rect, spike5rect,
        spike6rect, spike7rect, spike8rect, spike9rect, spike10rect};

    int spikecount = 10;

    /*ENEMYS*/
    // for the enemy1
    Texture2D enemy1 = LoadTexture("assets/enemy1.png");
    float enemy1size = 40;
    // for enemys rectangle
    Vector2 enemy1position = {22 * blocksize, 3 * blocksize};
    Vector2 enemy1speed = {0, 200};
    Rectangle enemy1rect = {enemy1position.x, enemy1position.y, enemy1size, enemy1size};

    // for the enemy2
    Texture2D enemy2 = LoadTexture("assets/enemy1.png");
    float enemy2size = 40;
    // for enemys rectangle
    Vector2 enemy2position = {17 * blocksize, 20 * blocksize};
    Vector2 enemy2speed = {0, 200};
    Rectangle enemy2rect = {enemy2position.x, enemy2position.y, enemy2size, enemy2size};

    // for the enemy3
    Texture2D enemy3 = LoadTexture("assets/enemy1.png");
    float enemy3size = 40;
    // for enemys rectangle
    Vector2 enemy3position = {20 * blocksize, 20 * blocksize};
    Vector2 enemy3speed = {0, 200};
    Rectangle enemy3rect = {enemy3position.x, enemy3position.y, enemy3size, enemy3size};

    // for the enemy4
    Texture2D enemy4 = LoadTexture("assets/enemy1.png");
    float enemy4size = 40;
    // for enemys rectangle
    Vector2 enemy4position = {23 * blocksize, 20 * blocksize};
    Vector2 enemy4speed = {0, 200};
    Rectangle enemy4rect = {enemy4position.x, enemy4position.y, enemy4size, enemy4size};

    // for the enemy5
    Texture2D enemy5 = LoadTexture("assets/enemy1.png");
    float enemy5size = 40;
    // for enemys rectangle
    Vector2 enemy5position = {3 * blocksize, 10 * blocksize};
    Vector2 enemy5speed = {0, 200};
    Rectangle enemy5rect = {enemy5position.x, enemy5position.y, enemy5size, enemy5size};

    // for the enemy9
    Texture2D enemy9 = LoadTexture("assets/enemy1.png");
    float enemy9size = 40;
    // for enemys rectangle
    Vector2 enemy9position = {3.1 * blocksize, 18.4 * blocksize};
    Vector2 enemy9speed = {0, 200};
    Rectangle enemy9rect = {enemy9position.x, enemy9position.y, enemy9size, enemy9size};

    // not moving enemy //
    // for the enemy6
    Texture2D enemy6 = LoadTexture("assets/enemy1.png");
    float enemy6size = 40;
    // for enemys rectangle
    Vector2 enemy6position = {31.6 * blocksize, 16.9 * blocksize};
    Rectangle enemy6rect = {enemy6position.x, enemy6position.y, enemy6size, enemy6size};

    // for the enemy7
    Texture2D enemy7 = LoadTexture("assets/enemy1.png");
    float enemy7size = 40;
    // for enemys rectangle
    Vector2 enemy7position = {31.6 * blocksize, 14 * blocksize};
    Rectangle enemy7rect = {enemy7position.x, enemy7position.y, enemy7size, enemy7size};

    // for the enemy8
    Texture2D enemy8 = LoadTexture("assets/enemy1.png");
    float enemy8size = 40;
    // for enemys rectangle
    Vector2 enemy8position = {31.6 * blocksize, 15.4 * blocksize};
    Rectangle enemy8rect = {enemy8position.x, enemy8position.y, enemy8size, enemy8size};

    // for the enemy10
    Texture2D enemy10 = LoadTexture("assets/enemy1.png");
    float enemy10size = 40;
    // for enemys rectangle
    Vector2 enemy10position = {4.5 * blocksize, 5.4 * blocksize};
    Rectangle enemy10rect = {enemy10position.x, enemy10position.y, enemy10size, enemy10size};

    /*  RING  */
    // Vertical Ring1//
    // defining the position of the ring
    Vector2 ring1backposition = {25 * blocksize, 4.6 * blocksize};
    Vector2 ring1frontposition = {25 * blocksize, 4.6 * blocksize};
    // defining rings structure
    float ring1size = 0.23f;
    float ring1radius = 18;

    /* Ring 1 collision areas */
    Rectangle ring1top = {ring1frontposition.x - 1.8, ring1frontposition.y - ring1radius - 7, 7, 6};
    Rectangle ring1bottom = {ring1frontposition.x - 1.2, ring1frontposition.y + ring1radius + 1.8, 7, 6};

    // Vertical Ring2//
    // defining the position of the ring
    Vector2 ring2backposition = {30 * blocksize, 23 * blocksize};
    Vector2 ring2frontposition = {30 * blocksize, 23 * blocksize};
    // defining rings structure
    float ring2size = 0.22f;
    float ring2radius = 21;

    /* Ring 2 collision areas */
    Rectangle ring2top = {ring2frontposition.x - 1.8, ring2frontposition.y - ring2radius - 6, 7, 6};
    Rectangle ring2bottom = {ring2frontposition.x - 1.8, ring2frontposition.y + ring2radius, 7, 6};

    // Vertical Ring3//
    // defining the position of the ring
    Vector2 ring3backposition = {29 * blocksize, 25 * blocksize};
    Vector2 ring3frontposition = {29 * blocksize, 25 * blocksize};
    // defining rings structure
    float ring3size = 0.22f;
    float ring3radius = 21;

    /* Ring 3 collision areas */
    Rectangle ring3top = {ring3frontposition.x - 1.8, ring3frontposition.y - ring3radius - 6, 7, 6};
    Rectangle ring3bottom = {ring3frontposition.x - 1.8, ring3frontposition.y + ring3radius, 7, 6};

    // Vertical Ring4//
    // defining the position of the ring
    Vector2 ring4backposition = {31 * blocksize, 21 * blocksize};
    Vector2 ring4frontposition = {31 * blocksize, 21 * blocksize};
    // defining rings structure
    float ring4size = 0.22f;
    float ring4radius = 21;

    /* Ring 4 collision areas */
    Rectangle ring4top = {ring4frontposition.x - 1.8, ring4frontposition.y - ring4radius - 6, 7, 6};
    Rectangle ring4bottom = {ring4frontposition.x - 1.8, ring4frontposition.y + ring4radius, 7, 6};

    // loading rings textures of front and back
    Texture2D ring1backtexture = LoadTexture("assets/ring back.png");
    Texture2D ring1fronttexture = LoadTexture("assets/ring front.png");

    Texture2D ring2backtexture = LoadTexture("assets/ring back.png");
    Texture2D ring2fronttexture = LoadTexture("assets/ring front.png");

    Texture2D ring3backtexture = LoadTexture("assets/ring back.png");
    Texture2D ring3fronttexture = LoadTexture("assets/ring front.png");

    Texture2D ring4backtexture = LoadTexture("assets/ring back.png");
    Texture2D ring4fronttexture = LoadTexture("assets/ring front.png");

    // loading rings textures of front and back for black&white
    Texture2D ring1backbwtexture = LoadTexture("assets/ring back b&w.png");
    Texture2D ring1frontbwtexture = LoadTexture("assets/ring front b&w.png");

    Texture2D ring2backbwtexture = LoadTexture("assets/ring back b&w.png");
    Texture2D ring2frontbwtexture = LoadTexture("assets/ring front b&w.png");

    Texture2D ring3backbwtexture = LoadTexture("assets/ring back b&w.png");
    Texture2D ring3frontbwtexture = LoadTexture("assets/ring front b&w.png");

    Texture2D ring4backbwtexture = LoadTexture("assets/ring back b&w.png");
    Texture2D ring4frontbwtexture = LoadTexture("assets/ring front b&w.png");

    // ring passing sound
    Sound ringpasssound = LoadSound("assets/ring pass.mp3");

    // ring passed state //
    bool ring1passed = false;
    bool ring2passed = false;
    bool ring3passed = false;
    bool ring4passed = false;

    int ringcount = 0;

    // sublevel
    bool checkpoint1 = false;
    Vector2 checkpointposition = {3 * blocksize, 2 * blocksize};

    /*  GEMS */
    Vector2 gem1position = {1 * blocksize, 12 * blocksize};
    Vector2 gem2position = {14 * blocksize, 22.5 * blocksize};
    Vector2 gem3position = {16 * blocksize, 23 * blocksize};
    Vector2 gem4position = {18 * blocksize, 23.5 * blocksize};
    Vector2 gem5position = {15 * blocksize, 17 * blocksize};
    Vector2 gem6position = {28 * blocksize, 3 * blocksize};

    Texture2D gem1 = LoadTexture("assets/gem.png");
    Texture2D gem2 = LoadTexture("assets/gem.png");
    Texture2D gem3 = LoadTexture("assets/gem.png");
    Texture2D gem4 = LoadTexture("assets/gem.png");
    Texture2D gem5 = LoadTexture("assets/gem.png");
    Texture2D gem6 = LoadTexture("assets/gem.png");

    bool gem1pass = false;
    bool gem2pass = false;
    bool gem3pass = false;
    bool gem4pass = false;
    bool gem5pass = false;
    bool gem6pass = false;

    int gemcount = 0;
    Sound gemsound = LoadSound("assets/gemsound.mp3");
    float gemsize = 0.05f; // for all of the gems

    /*finish line*/
    Texture2D flag = LoadTexture("assets/finish line.png");
    Vector2 flagpos = {33.4 * blocksize, 24.5 * blocksize};
    float flagscale = 0.1f;
    Rectangle flagrect = {flagpos.x,
                          flagpos.y,
                          flag.width * flagscale,
                          flag.height * flagscale

    };

    // to load sound of bouncing
    Sound bounce = LoadSound("assets/bouncesound.mp3");

    /*  EXPLOSION */
    // for generating explosion after collision with enemy
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

        float dt = GetFrameTime() / 2;

        // for the speed of the ball when the collision happened once
        Vector2 prevposition = position;

        // for the speed of the ball when the collision happened once
        if (!respawn)
        {
            speed = Vector2Add(speed, Vector2Scale(gravity, dt));
            position = Vector2Add(position, Vector2Scale(speed, dt));
        }

        // BALL AND THE GEM
        // for the ball passing the gems

        if (!gem1pass &&
            ((prevposition.x + currentradius < gem1position.x &&
              position.x + currentradius >= gem1position.x) ||

             (prevposition.x - currentradius > gem1position.x &&
              position.x - currentradius <= gem1position.x)) &&

            position.y + currentradius > gem1position.y &&
            position.y - currentradius <
                gem1position.y + gem1.height * gemsize)
        {
            gem1pass = true;
            gemcount++;

            PlaySound(gemsound);
        }

        if (!gem2pass &&
            ((prevposition.x + currentradius < gem2position.x &&
              position.x + currentradius >= gem2position.x) ||

             (prevposition.x - currentradius > gem2position.x &&
              position.x - currentradius <= gem2position.x)) &&

            position.y + currentradius > gem2position.y &&
            position.y - currentradius <
                gem2position.y + gem2.height * gemsize)
        {
            gem2pass = true;
            gemcount++;

            PlaySound(gemsound);
        }

        if (!gem3pass &&
            ((prevposition.x + currentradius < gem3position.x &&
              position.x + currentradius >= gem3position.x) ||

             (prevposition.x - currentradius > gem3position.x &&
              position.x - currentradius <= gem3position.x)) &&

            position.y + currentradius > gem3position.y &&
            position.y - currentradius <
                gem3position.y + gem3.height * gemsize)
        {
            gem3pass = true;
            gemcount++;

            PlaySound(gemsound);
        }

        if (!gem4pass &&
            ((prevposition.x + currentradius < gem4position.x &&
              position.x + currentradius >= gem4position.x) ||

             (prevposition.x - currentradius > gem4position.x &&
              position.x - currentradius <= gem4position.x)) &&

            position.y + currentradius > gem4position.y &&
            position.y - currentradius <
                gem4position.y + gem4.height * gemsize)
        {
            gem4pass = true;
            gemcount++;

            PlaySound(gemsound);
        }

        if (!gem5pass &&
            ((prevposition.x + currentradius < gem5position.x &&
              position.x + currentradius >= gem5position.x) ||

             (prevposition.x - currentradius > gem5position.x &&
              position.x - currentradius <= gem5position.x)) &&

            position.y + currentradius > gem5position.y &&
            position.y - currentradius <
                gem5position.y + gem5.height * gemsize)
        {
            gem5pass = true;
            gemcount++;

            PlaySound(gemsound);
        }

        if (!gem6pass &&
            ((prevposition.x + currentradius < gem6position.x &&
              position.x + currentradius >= gem6position.x) ||

             (prevposition.x - currentradius > gem6position.x &&
              position.x - currentradius <= gem6position.x)) &&

            position.y + currentradius > gem6position.y &&
            position.y - currentradius <
                gem6position.y + gem6.height * gemsize)
        {
            gem6pass = true;
            gemcount++;

            PlaySound(gemsound);
        }

        // for checking if the ball is on the platform or not
        bool onplatform = checkcollision(&position, &speed);

        // for checking ring collision //
        bool onring1 = checkVerticalRingCollision(&position, &speed, ring1top, ring1bottom);
        bool onring2 = checkVerticalRingCollision(&position, &speed, ring2top, ring2bottom);
        bool onring3 = checkVerticalRingCollision(&position, &speed, ring3top, ring3bottom);
        bool onring4 = checkVerticalRingCollision(&position, &speed, ring4top, ring4bottom);

        // for movement and jumping
        if (IsKeyDown(KEY_RIGHT))
            speed.x = maxspeedx;

        else if (IsKeyDown(KEY_LEFT))
            speed.x = -maxspeedx;
        else
            speed.x = 0;
        if (IsKeyPressed(KEY_UP) && (onplatform || onring1 || onring2 || onring3 || onring4))
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

            checkpoint1 = true;
            checkpointposition = (Vector2){29.63 * blocksize, 5.7 * blocksize};

            PlaySound(ringpasssound);
        }

        // RING 2  Vertical ring//

        if (!ring2passed &&
            (
                // Left to Right
                (
                    prevposition.x + currentradius < ring2frontposition.x + ring2radius &&
                    position.x + currentradius >= ring2frontposition.x + ring2radius)

                ||
                // Right to Left
                (
                    prevposition.x - currentradius > ring2frontposition.x - ring2radius &&
                    position.x - currentradius <= ring2frontposition.x - ring2radius)) &&

            position.y + currentradius > ring2frontposition.y - ring2radius &&
            position.y - currentradius < ring2frontposition.y + ring2radius)
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

        // RING 4  Vertical ring//

        if (!ring4passed &&
            (
                // Left to Right
                (
                    prevposition.x + currentradius < ring4frontposition.x + ring4radius &&
                    position.x + currentradius >= ring4frontposition.x + ring4radius)

                ||
                // Right to Left
                (
                    prevposition.x - currentradius > ring4frontposition.x - ring4radius &&
                    position.x - currentradius <= ring4frontposition.x - ring4radius)) &&

            position.y + currentradius > ring4frontposition.y - ring4radius &&
            position.y - currentradius < ring4frontposition.y + ring4radius)
        {
            ring4passed = true;
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
            currentradius = largeradius;

            PlaySound(powerupsound);
        }

        /* SMALL POWER UP COLLISION */
        if (!smallpoweruppass &&
            CheckCollisionCircles(
                position,
                currentradius,
                (Vector2){
                    smallpowerupposition.x + smallpowerupsize / 2,
                    smallpowerupposition.y + smallpowerupsize / 2},
                smallpowerupsize / 2))
        {
            smallpoweruppass = true;

            // activate checkpoint 2
            checkpoint1 = true;
            checkpointposition = (Vector2){6.8 * blocksize, 17 * blocksize};

            currentradius = smallradius;

            PlaySound(powerupsound);
        }

        /*MOVE THE ENEMY*/
        // for enemy1s movement
        enemy1position = Vector2Add(enemy1position, Vector2Scale(enemy1speed, dt));
        // for enemys movement restriction
        if (enemy1position.y < 3 * blocksize)
        {
            enemy1speed.y = 140;
        }
        else if (enemy1position.y > 8 * blocksize - enemy1size)
        {
            enemy1speed.y = -140;
        }

        // for enemy2s movement
        enemy2position = Vector2Add(enemy2position, Vector2Scale(enemy2speed, dt));
        // for enemys movement restriction
        if (enemy2position.y < 19 * blocksize)
        {
            enemy2speed.y = 150;
        }
        else if (enemy2position.y > 23 * blocksize - enemy2size)
        {
            enemy2speed.y = -150;
        }

        // for enemy3s movement
        enemy3position = Vector2Add(enemy3position, Vector2Scale(enemy3speed, dt));
        // for enemys movement restriction
        if (enemy3position.y < 19 * blocksize)
        {
            enemy3speed.y = 150;
        }
        else if (enemy3position.y > 23 * blocksize - enemy3size)
        {
            enemy3speed.y = -150;
        }

        // for enemy4s movement
        enemy4position = Vector2Add(enemy4position, Vector2Scale(enemy4speed, dt));
        // for enemys movement restriction
        if (enemy4position.y < 19 * blocksize)
        {
            enemy4speed.y = 150;
        }
        else if (enemy4position.y > 23 * blocksize - enemy4size)
        {
            enemy4speed.y = -150;
        }

        // for enemy5s movement
        enemy5position = Vector2Add(enemy5position, Vector2Scale(enemy5speed, dt));
        // for enemys movement restriction
        if (enemy5position.y < 9 * blocksize)
        {
            enemy5speed.y = 150;
        }
        else if (enemy5position.y > 14 * blocksize - enemy5size)
        {
            enemy5speed.y = -150;
        }

        // for enemy9s movement
        enemy9position = Vector2Add(enemy9position, Vector2Scale(enemy9speed, dt));
        // for enemys movement restriction
        if (enemy9position.y < 19 * blocksize)
        {
            enemy9speed.y = 150;
        }
        else if (enemy9position.y > 23 * blocksize - enemy9size)
        {
            enemy9speed.y = -150;
        }

        // UPDATE COLLISION RECTANGLE
        enemy1rect.x = enemy1position.x;
        enemy1rect.y = enemy1position.y;

        enemy2rect.x = enemy2position.x;
        enemy2rect.y = enemy2position.y;

        enemy3rect.x = enemy3position.x;
        enemy3rect.y = enemy3position.y;

        enemy4rect.x = enemy4position.x;
        enemy4rect.y = enemy4position.y;

        enemy5rect.x = enemy5position.x;
        enemy5rect.y = enemy5position.y;

        enemy6rect.x = enemy6position.x;
        enemy6rect.y = enemy6position.y;

        enemy7rect.x = enemy7position.x;
        enemy7rect.y = enemy7position.y;

        enemy8rect.x = enemy8position.x;
        enemy8rect.y = enemy8position.y;

        enemy9rect.x = enemy9position.x;
        enemy9rect.y = enemy9position.y;

        enemy10rect.x = enemy10position.x;
        enemy10rect.y = enemy10position.y;

        // COLLISION //

        // for spike and ball collision
        for (int i = 0; i < spikecount; i++)
        {
            if (!respawn && CheckCollisionCircleRec(position, currentradius, spikerects[i]))
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
                position = checkpointposition;
                respawntimer = 0.0f;
                respawn = false;
            }
        }

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
                position = checkpointposition;
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
                position = checkpointposition;
                respawntimer = 0.0f;
                respawn = false;
            }
        }

        if (!respawn && CheckCollisionCircleRec(position, currentradius, enemy4rect))
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
                position = checkpointposition;
                respawntimer = 0.0f;
                respawn = false;
            }
        }

        if (!respawn && CheckCollisionCircleRec(position, currentradius, enemy5rect))
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
                position = checkpointposition;
                respawntimer = 0.0f;
                respawn = false;
            }
        }

        if (!respawn && CheckCollisionCircleRec(position, currentradius, enemy6rect))
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
                position = checkpointposition;
                respawntimer = 0.0f;
                respawn = false;
            }
        }

        if (!respawn && CheckCollisionCircleRec(position, currentradius, enemy7rect))
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
                position = checkpointposition;
                respawntimer = 0.0f;
                respawn = false;
            }
        }

        if (!respawn && CheckCollisionCircleRec(position, currentradius, enemy8rect))
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
                position = checkpointposition;
                respawntimer = 0.0f;
                respawn = false;
            }
        }

        if (!respawn && CheckCollisionCircleRec(position, currentradius, enemy9rect))
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
                position = checkpointposition;
                respawntimer = 0.0f;
                respawn = false;
            }
        }

        if (!respawn && CheckCollisionCircleRec(position, currentradius, enemy10rect))
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
                position = checkpointposition;
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

        if (respawn)
        {
            respawntimer -= dt;

            if (respawntimer <= 0.0f)
            {
                if (checkpoint1)
                {
                    position = checkpointposition;
                }

                else
                {
                    position.x = 2 * blocksize;
                    position.y = 100;
                }

                speed = Vector2Zero();

                respawntimer = 0.0f;
                respawn = false;
            }
        }

        // new explosion position
        explosionrec.x = explosionwidth * currentframe;
        explosionrec.y = explosionheight * currentline;

        // WIN WINDOW
        if (!gamewin && !respawn &&
            CheckCollisionCircleRec(position, currentradius, flagrect))
        {
            gamewin = true;
            speed = Vector2Zero();
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);

        // WIN WINDOW
        if (gamewin)
        {
            ClearBackground(SKYBLUE);
            Rectangle panel = {250, 150, 580, 500};

            // Shadow
            DrawRectangleRounded(
                (Rectangle){panel.x + 8, panel.y + 8, panel.width, panel.height},
                0.08f, 20, GREEN);

            // Main panel
            DrawRectangleRounded(panel, 0.08f, 20, BLUE);

            Rectangle button = {360, 450, 360, 60};
            DrawRectangleRounded(button, 0.2f, 15, DARKBLUE);

            //Texts 
            DrawText("CONGRATULATIONS!", 300, 200, 48, GOLD);
            DrawText("YOU HAVE CRACKED", 335, 260, 40, YELLOW);
            DrawText("BERNA-BOUNCE!", 300, 350, 60, RED);

            DrawText("PRESS ENTER TO CONTINUE", 395, 468, 20, WHITE);
            DrawText("Great job! Keep going!", 415, 540, 20, SKYBLUE);

            // DECORATION
            DrawCircle(290, 190, 6, YELLOW);
            DrawCircle(790, 190, 6, YELLOW);
            DrawCircle(310, 570, 5, GREEN);
            DrawCircle(770, 570, 5, SKYBLUE);

            EndDrawing();
            continue;
        }

        // to draw the blocks (level 3)
        drawlevel();

        //powerups

        if (!poweruppass)
        {
            DrawTextureEx(powerup, powerupposition, 0.0f, powerupsize / powerup.width, WHITE);
        }

        if (!smallpoweruppass)
        {
            DrawTextureEx(smallpowerup, smallpowerupposition, 0.0f, smallpowerupsize / smallpowerup.width, WHITE);
        }

        // draw ball
        DrawCircleV(position, currentradius, RED);

        // draw gem
        if (!gem1pass)
        {
            DrawTextureEx(gem1, gem1position, 0.0f, gemsize, WHITE);
        }

        if (!gem2pass)
        {
            DrawTextureEx(gem2, gem2position, 0.0f, gemsize, WHITE);
        }

        if (!gem3pass)
        {
            DrawTextureEx(gem3, gem3position, 0.0f, gemsize, WHITE);
        }

        if (!gem4pass)
        {
            DrawTextureEx(gem4, gem4position, 0.0f, gemsize, WHITE);
        }

        if (!gem5pass)
        {
            DrawTextureEx(gem5, gem5position, 0.0f, gemsize, WHITE);
        }

        if (!gem6pass)
        {
            DrawTextureEx(gem6, gem6position, 0.0f, gemsize, WHITE);
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
        DrawTextureEx(spike10, spike10position, 0.0f, spike10size / spike10.width, WHITE);

        // draw all enemy
        DrawTextureEx(enemy1, enemy1position, 0.0f, enemy1size / enemy1.width, WHITE);
        DrawTextureEx(enemy2, enemy2position, 0.0f, enemy2size / enemy2.width, WHITE);
        DrawTextureEx(enemy3, enemy3position, 0.0f, enemy3size / enemy3.width, WHITE);
        DrawTextureEx(enemy4, enemy4position, 0.0f, enemy4size / enemy4.width, WHITE);
        DrawTextureEx(enemy5, enemy5position, 0.0f, enemy5size / enemy5.width, WHITE);
        DrawTextureEx(enemy6, enemy6position, 0.0f, enemy6size / enemy6.width, WHITE);
        DrawTextureEx(enemy7, enemy7position, 0.0f, enemy7size / enemy7.width, WHITE);
        DrawTextureEx(enemy8, enemy8position, 0.0f, enemy8size / enemy8.width, WHITE);
        DrawTextureEx(enemy9, enemy9position, 0.0f, enemy9size / enemy9.width, WHITE);
        DrawTextureEx(enemy10, enemy10position, 0.0f, enemy10size / enemy10.width, WHITE);

        // draw finish line
        DrawTextureEx(flag, flagpos, 0.0f, flagscale, WHITE);

        // drawing front ring first
        DrawTextureEx(
            ring1passed ? ring1frontbwtexture : ring1fronttexture,
            (Vector2){
                ring1frontposition.x - ring1fronttexture.width * ring1size / 2, ring1frontposition.y - ring1fronttexture.height * ring1size / 2},
            0.0f,
            ring1size,
            WHITE);

        DrawTextureEx(
            ring2passed ? ring2frontbwtexture : ring2fronttexture,
            (Vector2){
                ring2frontposition.x - ring2fronttexture.width * ring2size / 2, ring2frontposition.y - ring2fronttexture.height * ring2size / 2},
            0.0f,
            ring2size,
            WHITE);

        DrawTextureEx(
            ring3passed ? ring3frontbwtexture : ring3fronttexture,
            (Vector2){
                ring3frontposition.x - ring3fronttexture.width * ring3size / 2, ring3frontposition.y - ring3fronttexture.height * ring3size / 2},
            0.0f,
            ring3size,
            WHITE);

        DrawTextureEx(
            ring4passed ? ring4frontbwtexture : ring4fronttexture,
            (Vector2){
                ring4frontposition.x - ring4fronttexture.width * ring4size / 2, ring4frontposition.y - ring4fronttexture.height * ring4size / 2},
            0.0f,
            ring4size,
            WHITE);

        // after ball, drawing back rings
        DrawTextureEx(
            ring1passed ? ring1backbwtexture : ring1backtexture,
            (Vector2){
                ring1backposition.x - ring1backtexture.width * ring1size / 2, ring1backposition.y - ring1backtexture.height * ring1size / 2},
            0.0f,
            ring1size,
            WHITE);

        DrawTextureEx(
            ring2passed ? ring2backbwtexture : ring2backtexture,
            (Vector2){
                ring2backposition.x - ring2backtexture.width * ring2size / 2, ring2backposition.y - ring2backtexture.height * ring2size / 2},
            0.0f,
            ring2size,
            WHITE);

        DrawTextureEx(
            ring3passed ? ring3backbwtexture : ring3backtexture,
            (Vector2){
                ring3backposition.x - ring3backtexture.width * ring3size / 2, ring3backposition.y - ring3backtexture.height * ring3size / 2},
            0.0f,
            ring3size,
            WHITE);

        DrawTextureEx(
            ring4passed ? ring4backbwtexture : ring4backtexture,
            (Vector2){
                ring4backposition.x - ring4backtexture.width * ring4size / 2, ring4backposition.y - ring4backtexture.height * ring4size / 2},
            0.0f,
            ring4size,
            WHITE);

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
    UnloadTexture(powerup);
    UnloadTexture(smallpowerup);
    UnloadSound(powerupsound);
    UnloadTexture(ring1fronttexture);
    UnloadTexture(ring1backtexture);
    UnloadTexture(ring1frontbwtexture);
    UnloadTexture(ring1backbwtexture);
    UnloadTexture(ring2frontbwtexture);
    UnloadTexture(ring2backbwtexture);
    UnloadTexture(ring3frontbwtexture);
    UnloadTexture(ring3backbwtexture);
    UnloadTexture(ring4frontbwtexture);
    UnloadTexture(ring4backbwtexture);
    UnloadTexture(gem1);
    UnloadTexture(gem2);
    UnloadTexture(gem3);
    UnloadTexture(gem4);
    UnloadTexture(gem5);
    UnloadSound(gemsound);
    UnloadTexture(flag);

    CloseWindow();
    return 0;
}
