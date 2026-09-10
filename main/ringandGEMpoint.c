#include"raylib.h"
#include"raymath.h"


#define screenwidth 1080
#define screenheight 810
#define blocksize 45
#define maxblocks 300
#define maxspeedx 300
#define radius 30
#define jumpspeed 600
#define Gravity 1200

// creating upper and lower boundaries for ring
    Rectangle ringrecup = {10*blocksize + 43, 10*blocksize+5, 15, 8 };
    Rectangle ringrecdown = {10 * blocksize + 43, 12* blocksize - 6, 15, 12};


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

// to change the position and speed after collision
    bool resolveCircleBlock(Vector2 *position, Vector2 *speed, Rectangle r)
{
    float closestX = Clamp(position->x, r.x, r.x + r.width);
    float closestY = Clamp(position->y, r.y, r.y + r.height);

    Vector2 closestPoint = {closestX, closestY};

    Vector2 difference = Vector2Subtract(*position, closestPoint);

    float distance = Vector2Length(difference);

    // Ball is outside the block
    if (distance > radius)
        return false;

    // Ball center is inside the block
    if (distance == 0.0f)
    {
        float left   = position->x - r.x;
        float right  = (r.x + r.width) - position->x;
        float top    = position->y - r.y;
        float bottom = (r.y + r.height) - position->y;

        float smallest = fminf(fminf(left, right), fminf(top, bottom));

        if (smallest == top)
        {
            position->y = r.y - radius;
            speed->y = 0;
            return true;
        }
        else if (smallest == bottom)
        {
            position->y = r.y + r.height + radius;
            speed->y = 0;
        }
        else if (smallest == left)
        {
            position->x = r.x - radius;
            speed->x = 0;
        }
        else
        {
            position->x = r.x + r.width + radius;
            speed->x = 0;
        }

        return false;
    }

    Vector2 normal = Vector2Scale(difference, 1.0f / distance);

    // Push ball outside block
    position->x = closestPoint.x + normal.x * radius;
    position->y = closestPoint.y + normal.y * radius;

    // Remove velocity going into block
    float velocityIntoBlock = Vector2DotProduct(*speed, normal);

    if (velocityIntoBlock < 0.0f)
    {
        *speed = Vector2Subtract(
            *speed,
            Vector2Scale(normal, velocityIntoBlock)
        );
    }

    return normal.y < -0.5f;
}



bool checkcollision(Vector2 *position, Vector2 *speed)
{
    bool onplatform = false;

    for (int i = 0; i < blockcount; i++)
    {
        //if standing on a surface
        if (resolveCircleBlock(position, speed, blocks[i].rect))
            onplatform = true;
    }

    return onplatform;
}

//for rederecting upwards collision of ball with ring
bool checkcollisionringup ( Vector2 *position, Vector2 *speed){
    bool onplatform = false;

        //if standing on a surface
        if (resolveCircleBlock(position, speed, ringrecup))
            onplatform = true;

    return onplatform;
}

//for rederecting downwards collision of ball with ring
bool checkcollisionringdown ( Vector2 *position, Vector2 *speed){
    bool onplatform = false;

        //if standing on a surface
        if (resolveCircleBlock(position, speed, ringrecdown))
            onplatform = true;

    return onplatform;
}


int main(){
    InitWindow(screenwidth,screenheight, "game");
    //to activate device;
    InitAudioDevice();

    SetTargetFPS(60);

    //to generate blocks of level 1
    levelgeneration1();

                    /* BALL*/
    Texture2D ball= LoadTexture("assets/ball.png");

    // for generating the movement of the ball
    Vector2 position = {radius+blocksize, 500};
    Vector2 speed= Vector2Zero();
    Vector2 gravity = {0, Gravity};

    Rectangle source = {
    0,
    0,
    ball.width,
    ball.height
};

Rectangle destination = {
    position.x,
    position.y,
    74,
    74
};

Vector2 origin = {
    37,37
};
float ballRotation =0;
                    /*ENEMY1*/

    //for the enemy1
    Texture2D enemy1 = LoadTexture("assets/enemy1.png");
    float enemy1size= 80;
    //for enemys rectangle 
    Vector2 enemy1position= {14*blocksize, 13*blocksize};
    Vector2 enemy1speed= {0,200};
    Rectangle enemy1rect= {enemy1position.x, enemy1position.y, enemy1size, enemy1size};

                    /*  RING  */

    //defining the position of the ring
    Vector2 ringbackposition = {11*blocksize,11*blocksize};
    Vector2 ringfrontposition= {11*blocksize, 11*blocksize};

    //defining rings structure
    float ringsize = 0.4f;
    float ringradius= 25;

    int ringcount =0;

    //loading rings textures of front and back
    Texture2D ringbacktexture = LoadTexture("assets/ring back.png");
    Texture2D ringfronttexture = LoadTexture("assets/ring front.png");
    Texture2D ringfulltexture = LoadTexture("assets/ring full.png");

    Texture2D ringbackbwtexture = LoadTexture("assets/ring back b&w.png");
    Texture2D ringfrontbwtexture = LoadTexture("assets/ring front b&w.png");

    //to load sound of bouncing 
    Sound bounce = LoadSound("assets/bouncesound.mp3");

    bool checkringcolor =0;

    //loading sound of ring passing
    Sound ringpasssound= LoadSound("assets/ring pass.mp3");

                    /*  EXPLOSION */

    //for generating explosion after collision with enemy1
    Texture2D explosiontext= LoadTexture("assets/explosion.png");

    //for explosion effects
    float explosionwidth = (float) explosiontext.width/5;
    float explosionheight = (float) explosiontext.height;
    int currentframe = 0;
    int currentline=0;
    int framescounter =0;
    bool active = false;

    Rectangle explosionrec= {explosionwidth* currentframe ,0, explosionwidth, explosionheight};
    Vector2 explosionposition= {0.0f, 0.0f};

    //loading explosion sound
    Sound explosion = LoadSound("assets/explosion.mp3");

    //for respawning the ball and explosion
    float respawntimer = 0.0f;
    bool respawn = false;

                    /*  GEM   */
    Vector2 gemposition = {17*blocksize, 10* blocksize};
    Texture2D gem= LoadTexture("assets/gem.png");
    int gempass = 0;
    int gemcount= 0;
    Sound gemsound = LoadSound("assets/gemsound.mp3");
    float gemsize = 0.05f;


    while(!WindowShouldClose()){
        float dt = GetFrameTime();


        //for the speed of the ball when the collision happened once
        if(!respawn){
        Vector2 prevposition = position;
        float dt = GetFrameTime();

    // =========================
    // MOVEMENT INPUT
    // =========================

    if (IsKeyDown(KEY_RIGHT))
        speed.x = maxspeedx;

    else if (IsKeyDown(KEY_LEFT))
        speed.x = -maxspeedx;

    else
        speed.x = 0;


    // =========================
    // PHYSICS
    // =========================

    speed = Vector2Add(speed, Vector2Scale(gravity, dt));

    position = Vector2Add(
        position,
        Vector2Scale(speed, dt)
    );


    // =========================
    // ROTATION
    // =========================

    ballRotation += (speed.x * dt / radius) * RAD2DEG;


    // =========================
    // UPDATE DRAW POSITION
    // =========================

    destination.x = position.x;
    destination.y = position.y;
        //for the ball passing the ring
        if (!checkringcolor && prevposition.x > ringbackposition.x && position.x <= ringbackposition.x && position.y > 10 * blocksize && position.y < 12 * blocksize)
    {
        checkringcolor = true;
        ringcount++;
        PlaySound(ringpasssound);
    }

        //for the ball passing the gem
        if (!gempass &&
            ((prevposition.x < gemposition.x && position.x >= gemposition.x) || (prevposition.x > gemposition.x && position.x <= gemposition.x)) && position.y + radius > gemposition.y &&  position.y - radius < gemposition.y + gem.height * gemsize)
        {
            gempass = 1;
            gemcount++;
            PlaySound(gemsound);
        }


    }

        //for checking if the ball is on the platform or not
         bool onplatform = checkcollision(&position, &speed);

        //for checking ringup collision
        bool onringup = checkcollisionringup(&position, &speed);

        //for checking ringdown collision
        bool onringdown = checkcollisionringdown(&position, &speed);

        
        if (IsKeyPressed(KEY_UP) && (onplatform || onringdown)){
            speed.y= - jumpspeed;
            PlaySound(bounce);
            ballRotation += (speed.x * dt / radius) * RAD2DEG;
        }

        //for enemy1s movement
        enemy1position = Vector2Add(enemy1position, Vector2Scale(enemy1speed,dt));
        //for enemys movement restriction
        if (enemy1position.y < 13 * blocksize)
        {
            enemy1speed.y = 200;
        }
        else if (enemy1position.y > 17 * blocksize- enemy1size)
        {
            enemy1speed.y = -200;
        }

        // UPDATE COLLISION RECTANGLE
        enemy1rect.x = enemy1position.x;
        enemy1rect.y = enemy1position.y;

                    /*  COLLISION  */

        //for enemy and ball collision
        if(!respawn && CheckCollisionCircleRec(position, radius, enemy1rect)){
            PlaySound(explosion);
            explosionposition = position;
            active = true;
            explosionposition.x = position.x -explosionwidth/ 2.0f;
            explosionposition.y = position.y- explosionheight/ 2.0f;

            respawn = true;
            respawntimer= 0.3f;    

        }

        if(respawn ){
            respawntimer -= dt;
            if(respawntimer<=0.0f){
            position.x = radius+ blocksize;
            position.y= 500;
            respawntimer = 0.0f;
            respawn = false;
            }
        }

        //explosion animation calculation
        if(active){
            framescounter++;
            if(framescounter> 2){
                currentframe ++;
                
                if(currentframe >= 5)
                {
                    currentframe = 0;
                    active = false;
                }
                framescounter = 0;
            }
        }

        //new explosion position
        explosionrec.x= explosionwidth * currentframe;
        explosionrec.y= explosionheight * currentline;

        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawText(TextFormat("Y = %.1f", position.y),
             200, 200, 20, BLACK);


        //draw gem
        if(!gempass){
            DrawTextureEx(gem, gemposition, 0.0f, gemsize, WHITE);
        }

        //draw ring point
        DrawText(TextFormat("%d", ringcount), 3*blocksize, blocksize, 30, WHITE);
        DrawTextureEx(ringfulltexture, (Vector2){2*blocksize+10, blocksize-8}, 0.0f, ringsize/2.5, WHITE);

        //draw gem point
        DrawText(TextFormat("%d", gemcount), 5*blocksize, blocksize, 30, WHITE);
        DrawTextureEx(gem, (Vector2){4*blocksize+10, blocksize}, 0.0f, gemsize, WHITE);

        //to draw the blocks (level 1)
        drawlevel();

        if(!checkringcolor){
        //drawing front ring first
        DrawTextureEx(
        ringfronttexture,
        (Vector2){
            ringfrontposition.x - ringfronttexture.width * ringsize / 2, ringfrontposition.y - ringfronttexture.height * ringsize / 2},
            0.0f,
            ringsize,
            WHITE);


        //draw ball
        DrawTexturePro(
    ball,
    source,
    destination,
    origin,
    ballRotation,
    WHITE
);
        
        //after ball, drawing back ring
        DrawTextureEx(
        ringbacktexture,
        (Vector2){
            ringbackposition.x - ringbacktexture.width * ringsize / 2, ringbackposition.y - ringbacktexture.height * ringsize / 2},
            0.0f,
            ringsize,
            WHITE);
        }

        else {
        //drawing front ring first
        DrawTextureEx(
        ringfrontbwtexture,
        (Vector2){
            ringfrontposition.x - ringfronttexture.width * ringsize / 2, ringfrontposition.y - ringfronttexture.height * ringsize / 2},
            0.0f,
            ringsize,
            WHITE);

        //draw ball
        //draw ball
        DrawTexturePro(
    ball,
    source,
    destination,
    origin,
    ballRotation,
    WHITE
);
        
        
        //after ball, drawing back ring
        DrawTextureEx(
        ringbackbwtexture,
        (Vector2){
            ringbackposition.x - ringbacktexture.width * ringsize / 2, ringbackposition.y - ringbacktexture.height * ringsize / 2},
            0.0f,
            ringsize,
            WHITE);
        }

        //draw enemy1
        DrawTextureEx(enemy1, enemy1position,0.0f, enemy1size/enemy1.width, WHITE );

        //draw explosion effect
        if(active){
            DrawTextureRec(explosiontext, explosionrec, explosionposition, WHITE);
        }

        EndDrawing();
    }
    UnloadTexture(explosiontext);
    UnloadSound(explosion);
    UnloadSound(bounce);
    UnloadTexture(enemy1);
    UnloadTexture(ringfronttexture);
    UnloadTexture(ringbacktexture);
    UnloadTexture(ringbackbwtexture);
    UnloadTexture(ringfrontbwtexture);
    UnloadSound(ringpasssound);
    CloseWindow();
    return 0;

}