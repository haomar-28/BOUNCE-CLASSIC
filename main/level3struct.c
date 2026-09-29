#include "raylib.h"

#define screenwidth 1080
#define screenheight 810
#define blocksize 30
#define maxblocks 300

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
    addhorizontalplatform(18, 5, 3);

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
    addverticalplatform(25, 1, 3);
    addverticalplatform(25, 6, 3);

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

int main()
{
    InitWindow(screenwidth, screenheight, "game");

    SetTargetFPS(60);

    // to generate blocks of level 3
    levelgeneration3();

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(SKYBLUE);

        // to draw the blocks (level 3)
        drawlevel();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}