#include<stdio.h>
#include<stdlib.h>
#include"raylib.h"
#include"raymath.h"
#include<string.h>


#define screenwidth 1080
#define screenheight 810
#define blocksize 45
#define maxblocks 300
#define maxspeedx 300
#define radius 30
#define jumpspeed 600
#define Gravity 1200

float currentradius = radius;

typedef struct 
{
    char name[100];
    int score;
}Player;


//save the info of the player

void savescore(char name[], int score){
    Player players[1000];
    int count =0;

    FILE *file = fopen ("leaderboard.txt", "r");
    //read old scores

    if(file != NULL) {

    while(count < 100 && fscanf(file, "%99[^|]|%d\n",players[count].name, &players[count].score) ==2){
        count++;
    }
    fclose(file);
}

    // adding current player
    strcpy(players[count].name, name);
    players[count].score = score;
    count ++;

    //sort the scores

    for (int i=0; i<count-1; i++){
        for(int j=i+1; j<count ;j++){
            if(players[j].score >players[i].score){
                Player temp = players[i];
                players[i] = players[j];
                players[j]= temp;

            }
        }
    }

    //only keep top 10
    if(count >10)count =10;

    //rewrite the file
    file = fopen("leaderboard.txt", "w");
    
    if(file == NULL){
        return;
    }
    
    for(int i=0; i<count ; i++){
        fprintf(file,"%s|%d\n",players[i].name, players[i].score);
    }

    fclose(file);
}


void drawleaderboard(){
    Player players[10];
    int count =0;

    FILE *file = fopen("leaderboard.txt", "r");

    if(file != NULL){
        while(count <10 && fscanf(file, "%99[^|]|%d\n",players[count].name, &players[count].score)==2){
            count++;
        }
        fclose(file);
    }

    ClearBackground(LIGHTGRAY);

    DrawText("LEADERBOARD", 400, 40, 50,DARKBLUE);

    DrawText(TextFormat("SL no. \t\t\tPLAYER \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tSCORE"),70,170,30, BLACK);

    DrawRectangle(0,220,screenwidth, 50,MAROON);
    DrawRectangle(0,270,screenwidth, 50,RED);
    DrawRectangle(0,320,screenwidth, 50,ORANGE);

   

    for(int i=0; i<count; i++){

    DrawText(TextFormat("%d", i+1),90, 230+i*50, 30, i < 3 ? RAYWHITE : BLACK);

    DrawText(players[i].name,250, 230+i*50, 30, i < 3 ? RAYWHITE : BLACK);

    DrawText(TextFormat("%d", players[i].score),800, 230+i*50, 30, i < 3 ? RAYWHITE : BLACK);
}
}





// dev

    Vector2 position1= {radius+blocksize, 500};
    Vector2 position2 = {100,blocksize +radius};
    Vector2 position3;
    Vector2 position;

    
float ballRotation =0;

    
    Vector2 speed;
    Vector2 gravity;

    Vector2 enemy1position;
    Vector2 enemy2position;
    Vector2 enemy3position;
    Vector2 enemy1position1 = {14*blocksize, 13*blocksize};
    Vector2 enemy1position2;
    Vector2 enemy1position3;
    Vector2 enemy1speed;
    Vector2 enemy2speed;
    Vector2 enemy3speed;
    Vector2 enemy1speed1 = {0,200};
    Vector2 enemy1speed2;
    float enemy1size;
    int enemycol;

    Vector2 ringbackposition;

    Vector2 ringbackposition1 = {11*blocksize,11*blocksize};
    Vector2 ringbackposition2;
    Vector2 ringbackposition3;

    Vector2 ringfrontposition;
    Vector2 ringfrontposition1= {11*blocksize, 11*blocksize};
    Vector2 ringfrontposition2;
    Vector2 ringfrontposition3;

    Vector2 ring1backposition = {3 * blocksize, 8 * blocksize};
    Vector2 ring1frontposition = {3 * blocksize, 8 * blocksize};
    float ring1size = 0.35f;
    float ring1radius = 24;

    Vector2 ring2backposition = {23 * blocksize, 8 * blocksize};
    Vector2 ring2frontposition = {23 * blocksize, 8 * blocksize};
    // defining rings structure
    float ring2size = 0.3f;
    float ring2radius = 20;

    Vector2 ring3backposition = {14.4 * blocksize, 11 * blocksize};
    Vector2 ring3frontposition = {14.4 * blocksize, 11 * blocksize};
    float ring3size = 0.22f;
    float ring3radius = 21;


    float ringsize;
    float ringradius;
    int ringcount;

    Vector2 gemposition;

    Vector2 gemposition1= {17*blocksize, 10* blocksize};
    Vector2 gemposition2;
    Vector2 gemposition3;
    int gempass;
    int gemcount;
    float gemsize ;

    Vector2 flagpos;
    Vector2 flagpos1= {12*blocksize, 6*blocksize};
    Vector2 flagpos2;
    Vector2 flagpos3;

    float spike1size = 40;
    // for spike rectangle
    Vector2 spike1position2 = {10.83 * blocksize, 7.6 * blocksize};
    

    float spike2size = 40;
    // for spike rectangle
    Vector2 spike2position2 = {11.3 * blocksize, 7.6 * blocksize};
    

    float spike3size = 40;
    // for spikes rectangle
    Vector2 spike3position2 = {11.77 * blocksize, 7.6 * blocksize};
    
    float spike4size = 40;
    // for spikes rectangle
    Vector2 spike4position2 = {12.24 * blocksize, 7.6 * blocksize};
    

    float spike5size = 40;
    // for spikes rectangle
    Vector2 spike5position2 = {13.83 * blocksize, 7.6 * blocksize};
    

    float spike6size = 40;
    // for spikes rectangle
    Vector2 spike6position2 = {14.3 * blocksize, 7.6 * blocksize};
   

    float spike7size = 40;
    // for spikes rectangle
    Vector2 spike7position2= {14.77 * blocksize, 7.6 * blocksize};
    

    float spike8size = 40;
    // for spikes rectangle
    Vector2 spike8position2 = {6.7 * blocksize, 15.6 * blocksize};
    

    float spike9size = 40;
    // for spikes rectangle
    Vector2 spike9position2 = {7.17 * blocksize, 15.6 * blocksize};

    float enemy1size2 = 80;
    // for enemys rectangle
    Vector2 enemy1position2 = {8 * blocksize, 1 * blocksize};
    Vector2 enemy1speed2 = {0, 200};

    float enemy2size2 = 45;
    // for enemys rectangle
    Vector2 enemy2position2 = {3 * blocksize, 10 * blocksize};
    Vector2 enemy2speed2 = {0, 100};

    float enemy3size2 = 80;
    // for enemys rectangle
    Vector2 enemy3position2 = {10.7 * blocksize, 10 * blocksize};
    Vector2 enemy3speed2 = {0, 200};

    // level 2 power up
    float powerupsize = 30;
    Vector2 powerupposition = {16 * blocksize, 15.6 * blocksize};
    bool poweruppass = false;
    float smallradius = 18;

    // level 2 ring passed state
    bool ring1passed = false;
    bool ring2passed = false;
    bool ring3passed = false;

    // level 2 checkpoint
    bool checkpoint1 = false;
    Vector2 checkpointposition = {2 * blocksize, 100};

    // level 2 gems
    Vector2 gem1position = {13 * blocksize, 16 * blocksize};
    Vector2 gem2position = {21 * blocksize, 16 * blocksize};
    Vector2 gem3position = {21 * blocksize, 3 * blocksize};
    bool gem1pass = false;
    bool gem2pass = false;
    bool gem3pass = false;

// creating upper and lower boundaries for ring
//hdsfds//

    
    Rectangle ringrecup1;
    Rectangle ringrecup2;
    Rectangle ringrecup3;
    Rectangle ringrecup4;

    Rectangle ring1recup2;
    Rectangle ringrecup3;
    Rectangle ring1recup1 = {10*blocksize + 43, 10*blocksize+5, 15, 8 };
    

    Rectangle ring1recdown2;
    Rectangle ring2recdown2;
    Rectangle ring3recdown2;
    Rectangle ring4recdown;
    Rectangle ring5recdown;

    Rectangle ring1recdown1 = {10 * blocksize + 43, 12* blocksize - 6, 15, 12};

    Rectangle ringrecdown1;
    Rectangle ringrecdown2;
    Rectangle ringrecdown3;

    //explosion facts
    int currentframe = 0;
    int framescounter = 0;
    bool active = false;

    Vector2 explosionposition;
    Rectangle explosionrec;
    

    int score =0;

    


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
    
    //rectangle blocks
    for (int x = 1; x < 6; x++)
    {
        for (int y = 13; y <= 17; y++)
        {
           addblock(x, y);
        }
    }
}


//draw blocks
void drawlevel(){
    for (int i=0;i<blockcount; i++){
        Rectangle r= blocks[i]. rect;

        //main block
        DrawRectangleRec(r,MAROON);
        //dark outline
        DrawRectangleLinesEx(r,3, RED);
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
    //find closest point of the block to the center of the ball
    float closestX = Clamp(position->x, r.x, r.x + r.width);
    float closestY = Clamp(position->y, r.y, r.y + r.height);

    Vector2 closestPoint = { closestX, closestY };

    //after subtraction , the direction will be perpendicular to surface
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
        if (resolveCircleBlock(position, speed, ringrecup1))
            onplatform = true;

    return onplatform;
}

//for rederecting downwards collision of ball with ring
bool checkcollisionringdown ( Vector2 *position, Vector2 *speed){
    bool onplatform = false;

        //if standing on a surface
        if (resolveCircleBlock(position, speed, ringrecdown1))
            onplatform = true;

    return onplatform;
}

// VERTICAL RING COLLISION FOR LEVEL 2
bool checkVerticalRingCollision(Vector2 *position, Vector2 *speed, Rectangle ringTop, Rectangle ringBottom)
{
    bool onring = false;
    resolveCircleBlock(position, speed, ringTop);
    if (resolveCircleBlock(position, speed, ringBottom))
        onring = true;
    return onring;
}

// HORIZONTAL RING COLLISION FOR LEVEL 2
bool checkHorizontalRingCollision(Vector2 *position, Vector2 *speed, Rectangle ringLeft, Rectangle ringRight)
{
    bool onring = false;
    if (resolveCircleBlock(position, speed, ringLeft))
        onring = true;
    if (resolveCircleBlock(position, speed, ringRight))
        onring = true;
    return onring;
}

void startLevel(int level)
{
    blockcount = 0;

    if(level == 1)
    {
        levelgeneration1();

        position = position1;

        enemy1position = enemy1position1;
        enemy1speed = enemy1speed1;

        ringbackposition = ringbackposition1;
        ringfrontposition = ringfrontposition1;

        gemposition = gemposition1;
        flagpos = flagpos1;
        

        ringrecup1= ring1recup1;
        ringrecdown1= ring1recdown1;
    }
// error
    else if(level == 2)
    {
        levelgeneration2();

        position = position2;

        enemy1position = enemy1position2;
        enemy1speed = enemy1speed2;

        ringbackposition = ringbackposition2;
        ringfrontposition = ringfrontposition2;

        gemposition = gemposition2;
        flagpos = flagpos2;

        ringrecup1= ring1recup2;
        ringrecdown1= ring1recdown2;

        position = position2;
        enemy1position = enemy1position2;
        enemy1speed = enemy1speed2;
        enemy2position = enemy2position2;
        enemy2speed = enemy2speed2;
        enemy3position = enemy3position2;
        enemy3speed = enemy3speed2;

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
        currentradius = radius;
        gem1pass = false;
        gem2pass = false;
        gem3pass = false;
        
    }

    speed = Vector2Zero();
    gravity = (Vector2){0, Gravity};

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


int main(){
    InitWindow(screenwidth,screenheight, "game");
    //to activate device;
    InitAudioDevice();

    SetTargetFPS(60);



    int loading =0;
    int play=0;
    int rules =0;
    int leaderboard =0;

    char name[100] = "";
    int namelength =0;
    int entername= 1;//to determine when the name input ends
    int score =0;
    int startgame =0;
    int namecount=0;

    int menu=0;
    int scoresaved =0;

    
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

    Texture2D startgamebutton =LoadTexture("assets/startgamebutton.png");
    Texture2D startgameglowing =LoadTexture("assets/startgameglowing.png");



    int levelcount =1;
                    
                       /* ====================== FOR LEVEL 1, SET POSITIONS OF EVERYTHING ================= */
    
    startLevel(levelcount);

    //ball
    Texture2D ball= LoadTexture("assets/ball.png");
    Rectangle source = {
    0,
    0,
    ball.width,
    ball.height
};

Rectangle destination = {
    position.x,
    position.y,
    68,
    68
};
Vector2 origin = {
    destination.width / 2.0f,
    destination.height / 2.0f
};


    // if(levelcount ==2){
    //     levelgeneration2();
    //     position = position2;
        
    //     speed= Vector2Zero();
    //     gravity = (Vector2){0, Gravity};
    // }



    Texture2D enemy1 = LoadTexture("assets/enemy1.png");
    Texture2D enemy2 = LoadTexture("assets/enemy1.png");
    Texture2D enemy3 = LoadTexture("assets/enemy1.png");
    
//enemy1 rectangle declaration, useful for collsiion checking
    Rectangle enemy1rect= {enemy1position.x, enemy1position.y, enemy1size, enemy1size};
    Rectangle enemy2rect = {enemy2position.x, enemy2position.y, enemy2size2, enemy2size2};
    Rectangle enemy3rect = {enemy3position.x, enemy3position.y, enemy3size2, enemy3size2};


    //loading rings textures of front and back
    Texture2D ringbacktexture = LoadTexture("assets/ring back.png");
    Texture2D ringfronttexture = LoadTexture("assets/ring front.png");
    Texture2D ringfulltexture = LoadTexture("assets/ring full.png");

    Texture2D ringbackbwtexture = LoadTexture("assets/ring back b&w.png");
    Texture2D ringfrontbwtexture = LoadTexture("assets/ring front b&w.png");

    //for second ring

    Texture2D ringback2texture = LoadTexture("assets/ring back.png");
    Texture2D ringfront2texture = LoadTexture("assets/ring front.png");
    Texture2D ringfull2texture = LoadTexture("assets/ring full.png");


    Texture2D ringbackbw2texture = LoadTexture("assets/ring back b&w.png");
    Texture2D ringfrontbw2texture = LoadTexture("assets/ring front b&w.png");

    // for 3rd ring

    Texture2D ringback3texture = LoadTexture("assets/ring back.png");
    Texture2D ringfront3texture = LoadTexture("assets/ring front.png");
    Texture2D ringfull3texture = LoadTexture("assets/ring full.png");

    Texture2D ringbackbw3texture = LoadTexture("assets/ring back b&w.png");
    Texture2D ringfrontbw3texture = LoadTexture("assets/ring front b&w.png");

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
    Texture2D gem= LoadTexture("assets/gem.png");

    Texture2D gem2= LoadTexture("assets/gem.png");

    Texture2D gem3= LoadTexture("assets/gem.png");
    Sound gemsound = LoadSound("assets/gemsound.mp3");

    /* LEVEL 2 ASSETS */
    Texture2D powerup = LoadTexture("assets/powerup.png");
    Sound powerupsound = LoadSound("assets/powerupsound.mp3");
    Texture2D spike1 = LoadTexture("assets/spike.png");
    Texture2D spike2 = LoadTexture("assets/spike.png");
    Texture2D spike3 = LoadTexture("assets/spike.png");
    Texture2D spike4 = LoadTexture("assets/spike.png");
    Texture2D spike5 = LoadTexture("assets/spike.png");
    Texture2D spike6 = LoadTexture("assets/spike.png");
    Texture2D spike7 = LoadTexture("assets/spike.png");
    Texture2D spike8 = LoadTexture("assets/spike.png");
    Texture2D spike9 = LoadTexture("assets/spike.png");


                     /* Gameover window*/
    int gameover = 0;
    Texture2D button= LoadTexture("assets/buttonoriginal.png");
    Texture2D hoveredbutton = LoadTexture("assets/restart hovered.png");

    float buttonscale = 0.25f;
    float buttonradius= button.height/2 * (buttonscale);
    Vector2 buttonpos = {500, 520};
    Vector2 buttoncenter = {buttonpos.x +buttonradius, buttonpos.y +buttonradius};
    int btnstate =0;
    bool btnaction;
    Vector2 mousepoint = {0.0f, 0.0f};

    Sound buttonsound = LoadSound("assets/buttonsound.mp3");

    Texture2D life = LoadTexture("assets/life.png");
    float lifescale = 0.05f;
    Vector2 lifepos = {920, 50};
    float lifesize = life.height/2 *lifescale;
    int lifecount = 3;
    Sound lastlife= LoadSound("assets/lastlife.mp3");
    Sound lifeover = LoadSound("assets/lifeover.mp3");

                       /*finish line*/
    Texture2D flag= LoadTexture("assets/finish line.png");
    float flagscale = 0.1f;
    Rectangle flagrect= {flagpos.x,
    flagpos.y+20,
    flag.width * flagscale,
    flag.height * flagscale
};

    
    //spikerectangles

    Rectangle spike1rect2 = {spike1position2.x, spike1position2.y, spike1size, spike1size};
    Rectangle spike2rect2 = {spike2position2.x, spike2position2.y, spike2size, spike2size};
    
    Rectangle spike3rect2 = {spike3position2.x, spike3position2.y, spike3size, spike3size};

    Rectangle spike4rect2 = {spike4position2.x, spike4position2.y, spike4size, spike4size};
    Rectangle spike5rect2 = {spike5position2.x, spike5position2.y, spike5size, spike5size};
    Rectangle spike6rect2 = {spike6position2.x, spike6position2.y, spike6size, spike6size};
    Rectangle spike7rect2 = {spike7position2.x, spike7position2.y, spike4size, spike4size};
    Rectangle spike8rect2 = {spike8position2.x, spike8position2.y, spike4size, spike4size};

    Rectangle spike9rect2 = {spike9position2.x, spike9position2.y, spike4size, spike4size};

    
    // array for spikerects
    Rectangle spikerects[] = {
        spike1rect2, spike2rect2, spike3rect2, spike4rect2, spike5rect2,
        spike6rect2, spike7rect2, spike8rect2, spike9rect2};
    int spikecount = 9;

                       /*MUSIC*/

    Music menumusic = LoadMusicStream("assets/menumusic.mp3");
    Music gamemusic = LoadMusicStream("assets/gamemusic.mp3");

    /* Ring 1 collision areas */
    Rectangle ring1top = {ring1frontposition.x - 5, ring1frontposition.y - ring1radius - 16, 12, 8};
    Rectangle ring1bottom = {ring1frontposition.x - 4.7, ring1frontposition.y + ring1radius + 15, 12, 8};

    /* Ring 2 collision areas */
    Rectangle ring2left = {ring2frontposition.x - 93, ring2frontposition.y - 15, 4, 10};
    Rectangle ring2right = {ring2frontposition.x - 33, ring2frontposition.y - 15, 4, 10};
    /* Ring 3 collision areas */
    Rectangle ring3top = {ring3frontposition.x - 1.8, ring3frontposition.y - ring3radius - 6, 7, 6};
    Rectangle ring3bottom = {ring3frontposition.x - 1.8, ring3frontposition.y + ring3radius, 7, 6};

    float timer =0;



    menumusic.looping = true;
    gamemusic.looping = true;

    SetMusicVolume(menumusic, 0.2f);  // 20% volume
    SetMusicVolume(gamemusic, 0.2f);  // 20% volume

    PlayMusicStream(menumusic);


    Sound levelpass = LoadSound("assets/levelpass.mp3");
    int levelpasscount= 0;


    while(!WindowShouldClose()){

        UpdateMusicStream(menumusic);
        UpdateMusicStream(gamemusic);

        float dt = GetFrameTime();
        timer += dt;
    
        if(startgame==0){

        

        if(!loading){
        

        if (timer <1.0f)
{
    
    BeginDrawing();
       

    DrawTexturePro(first_background, (Rectangle){0, 0, first_background.width, first_background.height},(Rectangle){0, 0,screenwidth, screenheight},(Vector2){0, 0}, 0.0f, WHITE);

    DrawText("LOADING", 420, 650, 50, BLACK);

    EndDrawing();
    
}
        else if(timer<2.0f){
            BeginDrawing();

    DrawTexturePro(first_background,(Rectangle){0, 0, first_background.width, first_background.height},(Rectangle){0, 0, screenwidth, screenheight},(Vector2){0, 0}, 0.0f, WHITE);

    DrawText("LOADING.", 420, 650, 50, BLACK);

    EndDrawing();
        }

        else if(timer <3.0f){
            BeginDrawing();

    DrawTexturePro(first_background,(Rectangle){0, 0, first_background.width, first_background.height},(Rectangle){0, 0,screenwidth, screenheight},(Vector2){0, 0}, 0.0f, WHITE);

    DrawText("LOADING. .", 420, 650, 50, BLACK);

    EndDrawing();
        }

        else if(timer <3.5){
            BeginDrawing();

    DrawTexturePro(first_background,(Rectangle){0, 0, first_background.width, first_background.height},(Rectangle){0, 0,screenwidth, screenheight}, (Vector2){0, 0}, 0.0f, WHITE);

    DrawText("LOADING. . .", 420, 650, 50, BLACK);

    EndDrawing();
        }

    else
    {
        loading = 1;
        menu = 1;
        timer = 0;
    }

    }
    else
        {
        Vector2 mousepos = GetMousePosition();

        Rectangle playrec = {420, 100, playbutton.width, playbutton.height};
        Rectangle rulesrec = {420, 300, rulesbutton.width, rulesbutton.height};
        Rectangle leaderboardrec = {420, 500, leaderboardbutton.width, leaderboardbutton.height};
        Rectangle rulesbackrec = {385, 670, 300,88};

        Rectangle startgamerec = {390, 700, startgamebutton.width/3, startgamebutton.height/3};


        int rulesbackpressed= 0;



        if (menu == 1 && play != 2) {

        if (CheckCollisionPointRec(mousepos, playrec)) {

        play = 1;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlaySound(buttonsound);

            play = 2;
            entername = 1;
            namelength = 0;
            menu =0;
            name[0] = '\0';
        }

    } else {
        play = 0;
    }
}


    else if(play==2 && entername ==1){
        
        int key = GetCharPressed();
        while(key>0){
            if((key>= 32) && (key<=125) && namelength <99){
                name[namelength] = (char)key;
                namelength++;
                name[namelength] ='\0';
            }

            key= GetCharPressed();
        }
            if(IsKeyPressed(KEY_BACKSPACE)){
                if(namelength>0){
                    namelength--;
                    name[namelength] = '\0';
                }
            }
        }

        
    

        if (menu == 1 && rules != 2) {
        if (CheckCollisionPointRec(mousepos, rulesrec)) {
        rules = 1;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            rules =2;
            menu =0;
            PlaySound(buttonsound);
        }
        } else {
            rules = 0;
    }
}
        else if (rules == 2) {
    if (CheckCollisionPointRec(mousepos, rulesbackrec)) {
        rulesbackpressed = 1;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlaySound(buttonsound);
            rules = 0;
            menu =1 ;
        }
    }
    else {
        rulesbackpressed = 0;
    }
}

        if (menu == 1 && leaderboard != 2) {
        if (CheckCollisionPointRec(mousepos, leaderboardrec)) {
        leaderboard = 1;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlaySound(buttonsound);
            leaderboard=2;
            menu =0;
            
        }
        } else {
            leaderboard = 0;
    }
}
        
        // =====================
        // NEXT WINDOW
        // =====================
        //dev 

        BeginDrawing();

        if(play==2 && entername ==1){
        ClearBackground(SKYBLUE);
        DrawText("ENTER YOUR NAME", 350, 200, 40, MAROON);
        DrawRectangle(300,300, 480, 70, LIGHTGRAY);
        DrawRectangleLines(300,300, 480, 70, BLACK);

        DrawText(name, 320, 320 ,30, BLACK);
        
       
        DrawTexturePro(startgamebutton, (Rectangle){0,0, startgamebutton.width, startgamebutton.height}, startgamerec, (Vector2){0,0}, 0.0f, WHITE);
        DrawText("PRESS 'left arrow key' TO GO BACK", 400, 400, 40, BLUE );

        if(IsKeyPressed(KEY_LEFT)){
            menu=1;
            startgame=0;
            play=0;
            leaderboard=0;
            rules=0;
        }

        
        if(CheckCollisionPointRec(mousepos, startgamerec)){
            DrawTexturePro(startgameglowing, (Rectangle){0,0, startgamebutton.width, startgamebutton.height}, startgamerec, (Vector2){0,0}, 0.0f, WHITE);
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                PlaySound(buttonsound);
                gameover = 0;
            enemycol = 0;
            lifecount = 3;
            ringcount = 0;
            gemcount = 0;
            gempass = 0;
            checkringcolor = 0;
            respawn = false;
            respawntimer = 0.0f;
            speed = Vector2Zero();

            score = 0;
            scoresaved = 0;

            startgame = 1;

                

                StopMusicStream(menumusic);
                PlayMusicStream(gamemusic);


                if(namelength ==0){
                    strcpy(name, "not given");
                }
                entername =0;
            }
        }
            
    }

        else if(rules==2){
                DrawTexturePro(rulesbg, (Rectangle){0, 0, rulesbg.width, rulesbg.height},(Rectangle){0, 0,screenwidth, screenheight},(Vector2){0, 0}, 0.0f, WHITE);
            
                if(rulesbackpressed==1){

                    DrawTexturePro(rulesbackglowing,(Rectangle){0,0, rulesbackglowing.width, rulesbackglowing.height},rulesbackrec,(Vector2){0,0}, 0.0f, WHITE);

                }
                
            }
        
           else if(leaderboard==2){
                drawleaderboard();

                DrawText("PRESS 'B' TO GO BACK",500, 760, 40, PURPLE);
                if(IsKeyPressed(KEY_B)){
                leaderboard=0;
                menu =1;
        }
            }

        else if(menu==1){
        //drawbuttons
        
        DrawTexturePro(menubar, (Rectangle){0, 0, menubar.width, menubar.height},(Rectangle){0, 0,screenwidth, screenheight},(Vector2){0, 0}, 0.0f, WHITE);

            if(rules ==0){
        DrawTexturePro(rulesbutton, (Rectangle){0, 0, rulesbutton.width, rulesbutton.height},(Rectangle){420, 300,rulesbutton.width, rulesbutton.height},(Vector2){0, 0}, 0.0f, WHITE);
            }

            else if(rules==1){
                DrawTexturePro(rulesglowing, (Rectangle){0, 0, rulesglowing.width, rulesglowing.height},(Rectangle){420, 300,rulesglowing.width, rulesglowing.height},(Vector2){0, 0}, 0.0f, WHITE);
            }
            


            if(play ==0){
        DrawTexturePro(playbutton, (Rectangle){0, 0, playbutton.width, playbutton.height},(Rectangle){420, 100,playbutton.width, playbutton.height},(Vector2){0, 0}, 0.0f, WHITE);
            }
            else if(play==1){
                DrawTexturePro(playglowing, (Rectangle){0, 0, playglowing.width, playglowing.height},(Rectangle){420, 100,playglowing.width, playglowing.height},(Vector2){0, 0}, 0.0f, WHITE);
            }

            if(leaderboard ==0){
        DrawTexturePro(leaderboardbutton, (Rectangle){0, 0, leaderboardbutton.width, leaderboardbutton.height},(Rectangle){420, 500,leaderboardbutton.width, leaderboardbutton.height},(Vector2){0, 0}, 0.0f, WHITE);
            }
            else if(leaderboard==1){
                DrawTexturePro(leaderboardglowing, (Rectangle){0, 0, leaderboardglowing.width, leaderboardglowing.height},(Rectangle){420, 500,leaderboardglowing.width, leaderboardglowing.height},(Vector2){0, 0}, 0.0f, WHITE);
            }
         
        }
    

        EndDrawing();
    }
}

    else if(startgame ==1){
        
        

        if(!gameover){

        Vector2 prevposition ;

        //for the speed of the ball when the collision happened once
        if(!respawn){
        prevposition = (Vector2) position;
        speed = Vector2Add(speed, Vector2Scale(gravity,dt));
        position = Vector2Add(position, Vector2Scale(speed,dt));

        ballRotation += (speed.x * dt / radius) * RAD2DEG;

        destination.x = position.x;
        destination.y = position.y;

    }

    //for checking if the ball is on the platform or not
         bool onplatform = checkcollision(&position, &speed);

    //for movement and jumping
        if (IsKeyDown(KEY_RIGHT))
            speed.x = maxspeedx;

        else if (IsKeyDown(KEY_LEFT))
            speed.x = -maxspeedx;
        else
            speed.x = 0;
        if (IsKeyPressed(KEY_UP) && (onplatform )){
            speed.y= - jumpspeed;
            PlaySound(bounce);
            ballRotation += (speed.x * dt / radius) * RAD2DEG;
        }


    if(levelcount==1){

    //for the ball passing the ring
        if (!checkringcolor && ((prevposition.x+radius > ringbackposition.x &&
      position.x +radius <= ringbackposition.x) ||
     (prevposition.x -radius < ringbackposition.x &&
      position.x- radius >= ringbackposition.x)) &&
 position.y > 10 * blocksize && position.y < 12 * blocksize)
    {
        checkringcolor = true;
        ringcount++;
        PlaySound(ringpasssound);
    }

     //for the ball passing the gem
        if (!gempass &&
            ((prevposition.x+radius < gemposition.x && position.x+radius >= gemposition.x) || (prevposition.x -radius > gemposition.x && position.x -radius<= gemposition.x)) && position.y + radius > gemposition.y &&  position.y - radius < gemposition.y + gem.height * gemsize)
        {
            gempass = 1;
            gemcount++;
            PlaySound(gemsound);
        }


        //for checking ringup collision
        bool onringup = checkcollisionringup(&position, &speed);

        //for checking ringdown collision
        bool onringdown = checkcollisionringdown(&position, &speed);

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
            
            enemycol++;
            lifecount --;
            if(lifecount ==1){
                PlaySound(lastlife);
                
            }

        }
        if(enemycol == 3){
            gameover = 1;
            lifecount= 0;
            if(lifecount ==0){
                
                PlaySound(lifeover);
            }
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

        if(active)
{
    framescounter++;

    if(framescounter >= 3)
    {
        framescounter = 0;
        currentframe++;

        if(currentframe >= 5)
        {
            currentframe = 0;
            active = false;
        }
    }
}

        

        //level passed
        if(position.x + radius >= flagrect.x &&
    position.x - radius <= flagrect.x + flagrect.width &&
    position.y + radius >= flagrect.y &&
    position.y - radius <= flagrect.y + flagrect.height){

        PlaySound(levelpass);

    levelcount++;

    startLevel(levelcount);

    flagrect.x = flagpos.x;
    flagrect.y = flagpos.y + 20;

    }

}
    else if(levelcount==2){

        Vector2 prevposition = (Vector2) position;

        // update level 2 ring collision areas
        ring1top = (Rectangle){ring1frontposition.x - 5, ring1frontposition.y - ring1radius - 16, 12, 8};
        ring1bottom = (Rectangle){ring1frontposition.x - 4.7, ring1frontposition.y + ring1radius + 15, 12, 8};
        ring2left = (Rectangle){ring2frontposition.x - 93, ring2frontposition.y - 15, 4, 10};
        ring2right = (Rectangle){ring2frontposition.x - 33, ring2frontposition.y - 15, 4, 10};
        ring3top = (Rectangle){ring3frontposition.x - 1.8, ring3frontposition.y - ring3radius - 6, 7, 6};
        ring3bottom = (Rectangle){ring3frontposition.x - 1.8, ring3frontposition.y + ring3radius, 7, 6};

        if(!respawn){

        speed = Vector2Add(speed, Vector2Scale(gravity,dt));
        position = Vector2Add(position, Vector2Scale(speed,dt));
        ballRotation += (speed.x * dt / currentradius) * RAD2DEG;
        destination.x = position.x;
        destination.y = position.y;
        }

        bool onplatform2 = checkcollision(&position, &speed);
        bool onring1 = checkVerticalRingCollision(&position, &speed, ring1top, ring1bottom);
        bool onring2 = checkHorizontalRingCollision(&position, &speed, ring2left, ring2right);
        bool onring3 = checkVerticalRingCollision(&position, &speed, ring3top, ring3bottom);

        if (IsKeyDown(KEY_RIGHT))
            speed.x = maxspeedx;
        else if (IsKeyDown(KEY_LEFT))
            speed.x = -maxspeedx;
        else
            speed.x = 0;

        if (IsKeyPressed(KEY_UP) && (onplatform2 || onring1 || onring2 || onring3))
        {
            speed.y = -jumpspeed;
            PlaySound(bounce);
        }

        if (!gem1pass &&
            ((prevposition.x + currentradius < gem1position.x && position.x + currentradius >= gem1position.x) ||
             (prevposition.x - currentradius > gem1position.x && position.x - currentradius <= gem1position.x)) &&
            position.y + currentradius > gem1position.y &&
            position.y - currentradius < gem1position.y + gem.height * gemsize)
        {
            gem1pass = true;
            gemcount++;
            PlaySound(gemsound);
        }

        if (!gem2pass &&
            ((prevposition.x + currentradius < gem2position.x && position.x + currentradius >= gem2position.x) ||
             (prevposition.x - currentradius > gem2position.x && position.x - currentradius <= gem2position.x)) &&
            position.y + currentradius > gem2position.y &&
            position.y - currentradius < gem2position.y + gem2.height * gemsize)
        {
            gem2pass = true;
            gemcount++;
            PlaySound(gemsound);
        }

        if (!gem3pass &&
            ((prevposition.x + currentradius < gem3position.x && position.x + currentradius >= gem3position.x) ||
             (prevposition.x - currentradius > gem3position.x && position.x - currentradius <= gem3position.x)) &&
            position.y + currentradius > gem3position.y &&
            position.y - currentradius < gem3position.y + gem3.height * gemsize)
        {
            gem3pass = true;
            gemcount++;
            PlaySound(gemsound);
        }

        if (!ring1passed &&
            ((prevposition.x + currentradius < ring1frontposition.x + ring1radius &&
              position.x + currentradius >= ring1frontposition.x + ring1radius) ||
             (prevposition.x - currentradius > ring1frontposition.x - ring1radius &&
              position.x - currentradius <= ring1frontposition.x - ring1radius)) &&
            position.y + currentradius > ring1frontposition.y - ring1radius &&
            position.y - currentradius < ring1frontposition.y + ring1radius)
        {
            ring1passed = true;
            ringcount++;
            PlaySound(ringpasssound);
        }

        if (!ring2passed &&
            ((prevposition.y < ring2frontposition.y && position.y >= ring2frontposition.y) ||
             (prevposition.y > ring2frontposition.y && position.y <= ring2frontposition.y)) &&
            position.x + currentradius > ring2left.x &&
            position.x - currentradius < ring2right.x + ring2right.width)
        {
            ring2passed = true;
            ringcount++;
            checkpoint1 = true;
            checkpointposition = (Vector2){21.63f * blocksize, 6.7f * blocksize};
            PlaySound(ringpasssound);
        }

        if (!ring3passed &&
            ((prevposition.x + currentradius < ring3frontposition.x + ring3radius &&
              position.x + currentradius >= ring3frontposition.x + ring3radius) ||
             (prevposition.x - currentradius > ring3frontposition.x - ring3radius &&
              position.x - currentradius <= ring3frontposition.x - ring3radius)) &&
            position.y + currentradius > ring3frontposition.y - ring3radius &&
            position.y - currentradius < ring3frontposition.y + ring3radius)
        {
            ring3passed = true;
            ringcount++;
            PlaySound(ringpasssound);
        }

        if (!poweruppass &&
            CheckCollisionCircles(position, currentradius,
            (Vector2){powerupposition.x + powerupsize / 2, powerupposition.y + powerupsize / 2},
            powerupsize / 2))
        {
            poweruppass = true;
            currentradius = smallradius;
            PlaySound(powerupsound);
        }

        enemy1position = Vector2Add(enemy1position, Vector2Scale(enemy1speed, dt));
        if (enemy1position.y < 1 * blocksize)
            enemy1speed.y = 160;
        else if (enemy1position.y > 7 * blocksize - enemy1size)
            enemy1speed.y = -125;

        enemy2position = Vector2Add(enemy2position, Vector2Scale(enemy2speed, dt));
        if (enemy2position.y < 10 * blocksize)
            enemy2speed.y = 100;
        else if (enemy2position.y > 13 * blocksize - enemy2size2)
            enemy2speed.y = -100;

        enemy3position = Vector2Add(enemy3position, Vector2Scale(enemy3speed, dt));
        if (enemy3position.y < 10 * blocksize)
            enemy3speed.y = 150;
        else if (enemy3position.y > 14 * blocksize - enemy3size2)
            enemy3speed.y = -150;

        enemy1rect.x = enemy1position.x;
        enemy1rect.y = enemy1position.y;
        enemy2rect.x = enemy2position.x;
        enemy2rect.y = enemy2position.y;
        enemy3rect.x = enemy3position.x;
        enemy3rect.y = enemy3position.y;

        for (int i = 0; i < spikecount; i++)
        {
            if (!respawn && CheckCollisionCircleRec(position, currentradius, spikerects[i]))
            {
                PlaySound(explosion);
                explosionposition = position;
                active = true;
                explosionposition.x = position.x - explosionwidth / 2.0f;
                explosionposition.y = position.y - explosionheight / 2.0f;
                respawn = true;
                respawntimer = 0.3f;
            }
        }

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
                    position = checkpointposition;
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
    }
    }
    else{
        mousepoint = GetMousePosition();
        btnaction = false;
        if(CheckCollisionPointCircle(mousepoint, buttoncenter, buttonradius)){
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnstate = 2;
            else btnstate = 1;
            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))btnaction = true;
        }
        else btnstate = 0;
        if(btnstate==2){
            PlaySound(buttonsound);
        }

        score = ringcount*5 +gemcount*10;
        if(scoresaved==0){
        savescore(name,score);
        scoresaved=1;
        }
    }



        BeginDrawing();
        ClearBackground(SKYBLUE);

        if(levelcount ==1){

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
        if(active)
{
        explosionrec.x = explosionwidth * currentframe;
        explosionrec.y = 0;
        explosionrec.width = explosionwidth;
        explosionrec.height = explosionheight;

        DrawTextureRec(explosiontext, explosionrec, explosionposition, WHITE);
}

        //draw life
        DrawTextureEx(life, lifepos, 0.0f, lifescale, WHITE);
        DrawText(TextFormat("%d", lifecount), 970,55,30, MAROON);

        //draw finish line
        DrawTextureEx(flag, flagpos,0.0f, flagscale, WHITE);
    
    if(gameover){

        DrawRectangle(0,0, screenwidth, screenheight, Fade(BLACK, 0.55f));


        DrawRectangle(190, 205, 700, 400, BLACK);
        DrawRectangleLinesEx( (Rectangle){190, 205, 700, 400}, 5, RED);


        DrawText("GAME OVER", 380, 280, 50, RED);
        DrawText("CLICK BUTTON TO RESTART",330, 450, 30, RED);
        DrawText("PRESS 'M' TO GO TO MAIN MENU",280, 490, 30, LIGHTGRAY);

        if(IsKeyPressed(KEY_M)){
    gameover = 0;

    // reset game state
    enemycol = 0;
    lifecount = 3;
    ringcount = 0;
    gemcount = 0;
    gempass = 0;
    checkringcolor = 0;
    respawn = false;
    respawntimer = 0.0f;
    speed = Vector2Zero();

    scoresaved = 0;
    score = 0;

    menu = 1;
    startgame = 0;
    play = 0;
    leaderboard = 0;
    rules = 0;

    // switch back to MENU music
    StopMusicStream(gamemusic);
    PlayMusicStream(menumusic);
}

        
        DrawTextureEx(button, buttonpos, 0.0f, buttonscale, WHITE);
        if(btnstate ==1){
            DrawTextureEx(hoveredbutton, buttonpos, 0.0f, buttonscale, WHITE);
        }

        //draw ring point
        DrawText(TextFormat("%d", ringcount), 500, 360, 30, WHITE);
        DrawTextureEx(ringfulltexture, (Vector2){450, 350}, 0.0f, ringsize/2.5, WHITE);

        //draw gem point
        DrawText(TextFormat("%d", gemcount), 620, 360, 30, WHITE);
        DrawTextureEx(gem, (Vector2){580, 360}, 0.0f, gemsize, WHITE);

        //draw score
        DrawText(TextFormat("Your Score: %d", score), 400, 400, 40, GRAY);


        

        if(btnstate==2){
            gameover=0;
            gempass= 0;
            gemcount=0;
            enemycol= 0;
            checkringcolor= 0;
            ringcount= 0;
            position.x= blocksize+radius;
            position.y= 500;
            respawn = false;
            respawntimer = 0.0f;
            speed = Vector2Zero();

            enemy1position.x= 14*blocksize;
            enemy1position.y = 13* blocksize;
            
            enemy1speed.y= 200;
            active = false;
            currentframe = 0;
            framescounter= 0;

            lifecount = 3;

            btnstate=0;

            score =0;
            scoresaved=0;
        }
    }
}

else if(levelcount==2){

    drawlevel();

    if(!poweruppass)
        DrawTextureEx(powerup, powerupposition, 0.0f, powerupsize / powerup.width, WHITE);

    if(!gem1pass)
        DrawTextureEx(gem, gem1position, 0.0f, gemsize, WHITE);
    if(!gem2pass)
        DrawTextureEx(gem2, gem2position, 0.0f, gemsize, WHITE);
    if(!gem3pass)
        DrawTextureEx(gem3, gem3position, 0.0f, gemsize, WHITE);

    DrawText(TextFormat("%d", ringcount), 3*blocksize, blocksize, 30, WHITE);
    DrawTextureEx(ringfulltexture, (Vector2){2*blocksize+10, blocksize-8}, 0.0f, ringsize/2.5, WHITE);
    DrawText(TextFormat("%d", gemcount), 5*blocksize, blocksize, 30, WHITE);
    DrawTextureEx(gem, (Vector2){4*blocksize+10, blocksize}, 0.0f, gemsize, WHITE);

    DrawTextureEx(spike1, spike1position2, 0.0f, spike1size / spike1.width, WHITE);
    DrawTextureEx(spike2, spike2position2, 0.0f, spike2size / spike2.width, WHITE);
    DrawTextureEx(spike3, spike3position2, 0.0f, spike3size / spike3.width, WHITE);
    DrawTextureEx(spike4, spike4position2, 0.0f, spike4size / spike4.width, WHITE);
    DrawTextureEx(spike5, spike5position2, 0.0f, spike5size / spike5.width, WHITE);
    DrawTextureEx(spike6, spike6position2, 0.0f, spike6size / spike6.width, WHITE);
    DrawTextureEx(spike7, spike7position2, 0.0f, spike7size / spike7.width, WHITE);
    DrawTextureEx(spike8, spike8position2, 0.0f, spike8size / spike8.width, WHITE);
    DrawTextureEx(spike9, spike9position2, 0.0f, spike9size / spike9.width, WHITE);

    DrawTextureEx(
        ring1passed ? ringfrontbwtexture : ringfronttexture,
        (Vector2){ring1frontposition.x - ringfronttexture.width * ring1size / 2, ring1frontposition.y - ringfronttexture.height * ring1size / 2},
        0.0f, ring1size, WHITE);

    DrawTextureEx(
        ring3passed ? ringfrontbw3texture : ringfront3texture,
        (Vector2){ring3frontposition.x - ringfront3texture.width * ring3size / 2, ring3frontposition.y - ringfront3texture.height * ring3size / 2},
        0.0f, ring3size, WHITE);

    DrawTextureEx(
        ring2passed ? ringfrontbw2texture : ringfront2texture,
        (Vector2){ring2frontposition.x - ringfront2texture.width * ring2size / 2, ring2frontposition.y - ringfront2texture.height * ring2size / 2},
        90.0f, ring2size, WHITE);

    DrawTexturePro(ball, source, destination, origin, ballRotation, WHITE);

    DrawTextureEx(
        ring1passed ? ringbackbwtexture : ringbacktexture,
        (Vector2){ring1backposition.x - ringbacktexture.width * ring1size / 2, ring1backposition.y - ringbacktexture.height * ring1size / 2},
        0.0f, ring1size, WHITE);

    DrawTextureEx(
        ring3passed ? ringbackbw3texture : ringback3texture,
        (Vector2){ring3backposition.x - ringback3texture.width * ring3size / 2, ring3backposition.y - ringback3texture.height * ring3size / 2},
        0.0f, ring3size, WHITE);

    DrawTextureEx(
        ring2passed ? ringbackbw2texture : ringback2texture,
        (Vector2){ring2backposition.x - ringback2texture.width * ring2size / 2, ring2backposition.y - ringback2texture.height * ring2size / 2},
        90.0f, ring2size, WHITE);

    DrawTextureEx(enemy1, enemy1position, 0.0f, enemy1size / enemy1.width, WHITE);
    DrawTextureEx(enemy2, enemy2position, 0.0f, enemy2size2 / enemy2.width, WHITE);
    DrawTextureEx(enemy3, enemy3position, 0.0f, enemy3size2 / enemy3.width, WHITE);

    if(active)
    {
        explosionrec.x = explosionwidth * currentframe;
        explosionrec.y = 0;
        explosionrec.width = explosionwidth;
        explosionrec.height = explosionheight;
        DrawTextureRec(explosiontext, explosionrec, explosionposition, WHITE);
    }
}

        EndDrawing();
}
    }

    UnloadTexture(explosiontext);
    UnloadSound(explosion);
    UnloadTexture(powerup);
    UnloadSound(powerupsound);
    UnloadTexture(spike1);
    UnloadTexture(spike2);
    UnloadTexture(spike3);
    UnloadTexture(spike4);
    UnloadTexture(spike5);
    UnloadTexture(spike6);
    UnloadTexture(spike7);
    UnloadTexture(spike8);
    UnloadTexture(spike9);
    UnloadSound(bounce);
    UnloadTexture(enemy1);
    UnloadTexture(ringfronttexture);
    UnloadTexture(ringbacktexture);
    UnloadTexture(ringbackbwtexture);
    UnloadTexture(ringfrontbwtexture);
    UnloadTexture(ringfulltexture);
    UnloadSound(ringpasssound);
    
    UnloadTexture(hoveredbutton);
    UnloadTexture(button);
    UnloadTexture(life);
    UnloadTexture(flag);
    UnloadTexture(gem);
    UnloadSound(gemsound);
    UnloadSound(levelpass);
    UnloadSound(lastlife);
    UnloadSound(lifeover);
    UnloadTexture(first_background);
    UnloadTexture(menubar);
    UnloadTexture(playbutton);
    UnloadTexture(playglowing);
    UnloadTexture(rulesbutton);
    UnloadTexture(rulesglowing);
    UnloadTexture(leaderboardbutton);
    UnloadTexture(leaderboardglowing);
    UnloadTexture(rulesbg);
    UnloadTexture(rulesbackglowing);
    UnloadSound(buttonsound);
    UnloadTexture(startgamebutton);
    UnloadTexture(startgameglowing);
    UnloadMusicStream(menumusic);
    UnloadMusicStream(gamemusic);
    CloseWindow();
    CloseAudioDevice();

    return 0;


}