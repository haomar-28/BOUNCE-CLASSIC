#include"raylib.h"
#include"raymath.h"


#define screenwidth 1080
#define screenheight 810
#define blocksize 45
#define maxblocks 300
#define maxspeedx 300
#define radius 25
#define jumpspeed 600
#define Gravity 1200


//define block data type
typedef struct{
    Rectangle rect;
} block;

block blocks[maxblocks];
int blockcount =0;


//add one block(later needed)

void addblock(int gridx, int gridy){
    if (blockcount >= maxblocks)
    return;

    blocks[blockcount].rect = (Rectangle){
        gridx*blocksize, gridy*blocksize, blocksize, blocksize
    };
    blockcount++;
}

//add a horizontal platform (using addblock) (used later in level generation)

void addhorizontalplatform(int x, int y, int width){
    for (int i=0; i<width; i++){
        addblock(x+i, y);
    }
}

void addrevhorizontalplatform(int x, int y, int width){
    for (int i=0; i<width; i++){
        addblock(x-i, y);
    }
}

void addrevverticalplatform(int x, int y, int height){
    for (int i=0 ; i< height; i++){
        addblock (x, y-i);
    }
}
void addverticalplatform(int x, int y, int height){
    for (int i=0 ; i< height; i++){
        addblock (x, y+i);
    }
}


//now the level generating , where we will create left, right, top , bottom wall and random blocks around the screen

void levelgeneration1(){

    //bottom wall

    for (int x=0; x<(screenwidth/blocksize); x++){
        addblock(x,17);
    }

    //top wall

    for (int x=0; x< (screenwidth/blocksize); x++){
        addblock(x, 0);
    }

    //right wall
    for (int y=0; y<(screenheight/blocksize); y++){
        addblock(23,y);
    }

    //left wall
    for (int y=0; y<(screenheight/blocksize); y++){
        addblock(0,y);
    }

    //start design
    addhorizontalplatform(1,13,7);
    
    addverticalplatform(7,13,5);
    
    addverticalplatform(10,0,7);
    
    addhorizontalplatform(10,7,5);
    
    addverticalplatform(15,7,5);
    
    addrevhorizontalplatform(15,12,5);

    addhorizontalplatform(15,11,4);

    addhorizontalplatform(20,8,3);
    for (int x=18;x<24;x++){
        for (int y=15; y<=17;y++){
            addblock(x,y);
        }
    }
    for (int x=21;x<24;x++){
        for (int y=13; y<=16;y++){
            addblock(x,y);
        }
    }
}

//draw blocks
void drawlevel(){
    for (int i=0;i<blockcount; i++){
        Rectangle r= blocks[i]. rect;

        //main block
        DrawRectangleRec(r,RED);
        //dark outline
        DrawRectangleLinesEx(r,3, MAROON);
        //highlight
        DrawLine(
            r.x+5,
            r.y+5,
            r.x+ r.width-5,
            r.y+5, ORANGE
        );
    }
}

int main(){
    InitWindow(screenwidth,screenheight, "game");

    SetTargetFPS(60);

    //to generate blocks of level 1
    levelgeneration1();

    // for generating the movement of the ball
    Vector2 position = {radius+blocksize, 500};

    Vector2 speed= Vector2Zero();
    Vector2 gravity = {0, Gravity};



    while(!WindowShouldClose()){

        BeginDrawing();
        ClearBackground(SKYBLUE);

        //to draw the blocks (level 1)
        drawlevel();

        //draw ball
        DrawCircleV(position, radius, RED);

        EndDrawing();
    }
    CloseWindow();
    return 0;

}