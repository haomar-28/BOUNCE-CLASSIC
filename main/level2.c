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

    Vector2 position1= {radius+blocksize, 500};
    Vector2 position2 = {100,blocksize +radius};
    Vector2 position3;
    Vector2 position;

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

    Vector2 ringbackposition;

    Vector2 ringbackposition1 = {11*blocksize,11*blocksize};
    Vector2 ringbackposition2;
    Vector2 ringbackposition3;

    Vector2 ringfrontposition;
    Vector2 ringfrontposition1= {11*blocksize, 11*blocksize};
    Vector2 ringfrontposition2;
    Vector2 ringfrontposition3;


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

// creating upper and lower boundaries for ring
//hdsfds//

    Rectangle ringrecup;
    Rectangle ringrecup2;
    Rectangle ringrecup3;
    Rectangle ringrecup1 = {10*blocksize + 43, 10*blocksize+5, 15, 8 };
    Rectangle ringrecdown;
    Rectangle ringrecdown1 = {10 * blocksize + 43, 12* blocksize - 6, 15, 12};
    Rectangle ringrecdown2;
    Rectangle ringrecdown3;

    //explosion facts
    int currentframe = 0;
    int framescounter = 0;
    bool active = false;

    Vector2 explosionposition;
    Rectangle explosionrec;


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
        

        ringrecup= ringrecup1;
        ringrecdown= ringrecdown1;
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

        ringrecup= ringrecup2;
        ringrecdown= ringrecdown2;
        
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




    int levelcount =1;
                    
                       /* ====================== FOR LEVEL 1, SET POSITIONS OF EVERYTHING ================= */
    
    startLevel(levelcount);


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
    Vector2 buttonpos = {500, 500};
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

    Sound levelpass = LoadSound("assets/levelpass.mp3");
    int levelpasscount= 0;


    while(!WindowShouldClose()){
        float dt = GetFrameTime();

        if(!gameover){

        Vector2 prevposition ;

        //for the speed of the ball when the collision happened once
        if(!respawn){
        prevposition = (Vector2) position;
        speed = Vector2Add(speed, Vector2Scale(gravity,dt));
        position = Vector2Add(position, Vector2Scale(speed,dt));


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
        DrawCircleV(position, radius, RED);
        
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
        DrawCircleV(position, radius, RED);
        
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
        }
    }
}

else if(levelcount==2){
    drawlevel();
    DrawCircleV(position, radius, RED);

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
    UnloadTexture(ringfulltexture);
    UnloadSound(ringpasssound);
    UnloadSound(buttonsound);
    UnloadTexture(hoveredbutton);
    UnloadTexture(button);
    UnloadTexture(life);
    UnloadTexture(flag);
    UnloadTexture(gem);
    UnloadSound(gemsound);
    UnloadSound(levelpass);
    UnloadSound(lastlife);
    UnloadSound(lifeover);
    CloseWindow();

    return 0;


}