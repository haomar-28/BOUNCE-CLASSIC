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
    Vector2 enemy1position1 = {14*blocksize, 13*blocksize};
    Vector2 enemy1position2;
    Vector2 enemy1position3;
    Vector2 enemy1speed;
    Vector2 enemy1speed1 = {0,200};
    Vector2 enemy1speed2;
    float enemy1size;
    int enemycol;

    Vector2 ringbackposition1;
    Vector2 ringbackposition2;
    Vector2 ringbackposition3;
    

    Vector2 ring1backposition1 = {11*blocksize,11*blocksize};
    Vector2 ring1backposition2={3 * blocksize, 8 * blocksize};
    Vector2 ring2backposition2= {23 * blocksize, 8 * blocksize};
    Vector2 ring3backposition2 = {14.4 * blocksize, 11 * blocksize};


    Vector2 ringfrontposition1;
    Vector2 ringfrontposition2;
    Vector2 ringfrontposition3;
    
    Vector2 ring1frontposition1= {11*blocksize, 11*blocksize};
     Vector2 ring1frontposition2 = {3 * blocksize, 8 * blocksize};
    Vector2 ring2frontposition2 = {23 * blocksize, 8 * blocksize};
    Vector2 ring3frontposition2 = {14.4 * blocksize, 11 * blocksize};

    Vector2 ring1backposition2 = {3 * blocksize, 8 * blocksize};
    Vector2 ring1frontposition2 = {3 * blocksize, 8 * blocksize};
    float ring1size2 = 0.35f;
    float ring1radius2 = 24;

    Vector2 ring2backposition2 = {23 * blocksize, 8 * blocksize};
    Vector2 ring2frontposition2 = {23 * blocksize, 8 * blocksize};
    // defining rings structure
    float ring2size2 = 0.3f;
    float ring2radius2 = 20;

    Vector2 ring3backposition2 = {14.4 * blocksize, 11 * blocksize};
    Vector2 ring3frontposition2 = {14.4 * blocksize, 11 * blocksize};
    float ring3size2 = 0.22f;
    float ring3radius2 = 21;


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
    


// creating upper and lower boundaries for ring
//hdsfds//

    
    Rectangle ringrecup1;
    Rectangle ringrecup2;
    Rectangle ringrecup3;
    Rectangle ringrecup4;

    
    Rectangle ring2recup2;
    Rectangle ring3recup2;
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


    //spikerectangles

    Rectangle spike1rect2 = {10.83 * blocksize, 7.6*blocksize, 40, 40};
    Rectangle spike2rect2 = {11.3 * blocksize, 7.6 * blocksize, 40, 40};
    
    Rectangle spike3rect2 = {11.77 * blocksize, 7.6 * blocksize, 40, 40};

    Rectangle spike4rect2 = {12.24 * blocksize, 7.6 * blocksize, 40, 40};
    Rectangle spike5rect2 = {13.83 * blocksize, 7.6 * blocksize, 40, 40};
    Rectangle spike6rect2 = {14.3 * blocksize, 7.6 * blocksize, 40, 40};
    Rectangle spike7rect2 = {14.77 * blocksize, 7.6 * blocksize, 40, 40};
    Rectangle spike8rect2 = {6.7 * blocksize, 15.6 * blocksize, 40, 40};

    Rectangle spike9rect2 = {7.17 * blocksize, 15.6 * blocksize, 40, 40};

    
    // array for spikerects
    Rectangle spikerects[] = {
        {10.83 * blocksize, 7.6*blocksize, 40, 40}, {11.3 * blocksize, 7.6 * blocksize, 40, 40},{11.77 * blocksize, 7.6 * blocksize, 40, 40}, {12.24 * blocksize, 7.6 * blocksize, 40, 40}, {13.83 * blocksize, 7.6 * blocksize, 40, 40},
        {14.3 * blocksize, 7.6 * blocksize, 40, 40}, {14.77 * blocksize, 7.6 * blocksize, 40, 40}, {6.7 * blocksize, 15.6 * blocksize, 40, 40}, {7.17 * blocksize, 15.6 * blocksize, 40, 40}};
    int spikecount = 9;

    Rectangle enemy1rect = {8 * blocksize, 1 * blocksize, 80,80};
    Rectangle enemy2rect = {3 * blocksize, 10 * blocksize, 45,45};
    Rectangle enemy3rect = {10.7 * blocksize, 10 * blocksize, 80,80};
          

    /* Ring 1 collision areas */
    Rectangle ring1recup2 = {3 * blocksize - 5, 8 * blocksize - 024 - 16, 12, 8};
    Rectangle ring1recdown2 = {3 * blocksize - 4.7, 8 * blocksize + 24 + 15, 12, 8};

    /* Ring 2 collision areas */
    Rectangle ring2left2 = {23 * blocksize - 93, 8 * blocksize - 15, 4, 10};
    Rectangle ring2right2 = {23 * blocksize - 33, 8 * blocksize - 15, 4, 10};
    /* Ring 3 collision areas */
    Rectangle ring3recup2 = {14.4 * blocksize - 1.8, 11 * blocksize - 22 - 6, 7, 6};
    Rectangle ring3recdown2 = {14.4 * blocksize - 1.8, 11 * blocksize + 22, 7, 6};

    
    Rectangle ring2recup2;
    Rectangle ring3recup2;
    

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

void startLevel(int level)
{
    blockcount = 0;

    if(level == 1)
    {
        levelgeneration1();

        position = position1;

        enemy1position = enemy1position1;
        enemy1speed = enemy1speed1;

        ringbackposition1 = ring1backposition1;
        ringfrontposition1 = ring1frontposition1;

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

        ringbackposition1 = ring1backposition2;
        ringfrontposition2 = ring1frontposition2;

        gemposition = gemposition2;
        flagpos = flagpos2;

        ringrecup1= ring1recup2;
        ringrecdown1= ring1recdown2;
        
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

    
    
                       /*MUSIC*/

    Music menumusic = LoadMusicStream("assets/menumusic.mp3");
    Music gamemusic = LoadMusicStream("assets/gamemusic.mp3");

    
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
        if (!checkringcolor && ((prevposition.x+radius > ringbackposition1.x &&
      position.x +radius <= ringbackposition1.x) ||
     (prevposition.x -radius < ringbackposition1.x &&
      position.x- radius >= ringbackposition1.x)) &&
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



            startLevel(levelcount);
        
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
            ringfrontposition1.x - ringfronttexture.width * ringsize / 2, ringfrontposition1.y - ringfronttexture.height * ringsize / 2},
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
            ringbackposition1.x - ringbacktexture.width * ringsize / 2, ringbackposition1.y - ringbacktexture.height * ringsize / 2},
            0.0f,
            ringsize,
            WHITE);
        }

        else {
        //drawing front ring first
        DrawTextureEx(
        ringfrontbwtexture,
        (Vector2){
            ringfrontposition1.x - ringfronttexture.width * ringsize / 2, ringfrontposition1.y - ringfronttexture.height * ringsize / 2},
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
            ringbackposition1.x - ringbacktexture.width * ringsize / 2, ringbackposition1.y - ringbacktexture.height * ringsize / 2},
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
    //draw ball
        DrawTexturePro(
    ball,
    source,
    destination,
    origin,
    ballRotation,
    WHITE
);

}

        EndDrawing();
}
    }

    UnloadTexture(explosiontext);
    UnloadSound(explosion);
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