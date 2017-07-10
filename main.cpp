#include <iostream>
#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>
#include <thread>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <memory>
#include <fstream>
#include <cstring>
#include <list>
#include <vector>
#include <array>
//#include <algorithm>
//#include <cmath>
#include <math.h>
#include <dirent.h>
//#include "resources.h"
#include "resources/SDL_ttf.h"
#include "resources/vanilla.h"
#include "resources/SDL_net.h"
typedef Button button;
typedef Screen screen;
typedef Paper paper;
#include "players.h"

#include "objects.h"
#include "NPCS.h"
#include "actions.h"
//#include "quests.h"
#include "server.h"
#include "weapons.h"
#include "items.h"
#include "buttons.h"
#include "wins.h"
#include "modes.h"
#include "GUI.h"
//#include "arrays.h"

using namespace std;

 int money = 0;
const string defaultFont = "fonts/Roboto-Black.ttf";

int last_pressed = 0;// an int that will record how long the last key pressed was held
extern SDL_Window*   game;//= SDL_CreateWindow("RPG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
extern SDL_Renderer* render;// = SDL_CreateRenderer(game, -1, SDL_RENDERER_ACCELERATED );
//SDL_Renderer* quests = SDL_CreateRenderer(game, -1, SDL_RENDERER_ACCELERATED);
int scoreBegins = 64;

bool quit = false;
SDL_Rect* camera ;
SDL_Event e;
vector<paper> shelf;
list <unique_ptr<projectile>> projectiles;
menus menu = modSelf;
stage* level = new stage();// the almighty stage pointer. This should be the only instance of a stage in the entire project.
//player* me = new mage(200, 200, level);
string shot = "-1";
//string serializedBullets = ""; //a string that represents all current bullets serialized
player* me = nullptr;
server* s = nullptr;
client* c = nullptr;
bool mouseDown = false;
player* winner = nullptr;

int maxKills = 0;
  winPtr win = &kills;
  string objective = "";
extern string name;
bool armory[maximumWeapons] = {true,false, false, false, false};


const int delay = 2;

const int invSpriteBorder = 4;
int pageNumber = 0; //the page number of the collection
SDL_Texture* transparent;
int main(int argc, char* args[])
{

  //  cout << sizeof(Shelf<shared_ptr<weapon>>) << endl;
    transparent = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET, 640,640);
    SDL_SetTextureAlphaMod(transparent, 0);
     render = SDL_CreateRenderer(game, -1, SDL_RENDERER_TARGETTEXTURE );// = SDL_CreateRenderer(game, -1, SDL_RENDERER_ACCELERATED );

hurtBorder = new Effect("sprites/players/hurtB.png",{0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, 17, 102, false);
//playerWalkingSprite = new Animation("sprites/players/walking.png", {0,0,64,64}, {0,0,0,0}, 100);
    //player::walkingSprite
         srand(time(NULL));
     SDL_StartTextInput();
     IMG_Init(IMG_INIT_PNG);
       SDLNet_Init();
          TTF_Init();

//changeTexture(pic,"sprites/misc/hurt.png");

invSprite = new Effect("sprites/misc/inventory.png", {SCREEN_WIDTH/2 - (192 + invSpriteBorder*2) - invSpriteBorder,9*SCREEN_HEIGHT/10 - 2*invSpriteBorder - 10, 192 + invSpriteBorder*2, 64 + 2*invSpriteBorder}, 1,1500, false  );
inventory [0] = new weaponButton(64, 16, NULL, true, false);
inventory[1] =  new weaponButton(192 + 16, 16, NULL, true,false);
inventory[2] =  new weaponButton(336 + 16, 16, NULL, true,false);

inventory[0]->changeWeapPtr(new shopWeaponWrapper(0));
weaponSelect = new Effect("sprites/misc/weaponSelect.png", {invSprite->getRect()->x,invSprite->getRect()->y, invSprite->getRect()->h, invSprite->getRect()->h}, 1,2500, false);

//changeTexture(lockSprite, );
paper copy({0,0,64,64}, "sprites/shop/coin.png");
coin = copy;
lockSprite.changeText("sprites/shop/locked.png");
lockSprite.changeRect({0,0, 32,32});
moneyCount.changeText(defaultFont,"Money: " + convert(money));
moneyCount.changeRect({SCREEN_WIDTH- 128, 10, 128, 64});
//inventory = {weaponButton(16, 16, weaponWrapper()), weaponbutton(), weaponButton()}

announcement = new Effect("", {0,0,128,128},1000, false);
Button leftScrollButton (SCREEN_WIDTH/40, SCREEN_HEIGHT - SCREEN_HEIGHT/40 - 64, 16, 64,"sprites/shop/leftScroll.png", new actualFunction([](){pageNumber = max(pageNumber-1,0); showCollection();})  );
Button rightScrollButton (SCREEN_WIDTH - SCREEN_WIDTH/40, SCREEN_HEIGHT - SCREEN_HEIGHT/40 - 64, 16, 64,"sprites/shop/rightScroll.png", new actualFunction([](){pageNumber = min(pageNumber+1,maximumWeapons/6 - 1); showCollection();}));

c = new client(rand());
connectingScreen = new screen("sprites/misc/loading.png");
titleScreen= new screen("sprites/bg/title.png",  NULL, new function<void()>([](){print("money");}), NULL, &moneyCount,new vector<button*>{
                        new button(10, SCREEN_HEIGHT - 64, 128, 64, defaultFont, "Shop", 255,255,255,BLACK, new function<void()> ([](){changeMenu(choosingWeapons);})),
                        new button(SCREEN_WIDTH- 144, SCREEN_HEIGHT - 64, 128, 64, "sprites/buttons/changeName.png", new function<void()>(&changeSelf)),
                        new button(SCREEN_WIDTH/2 - 128, SCREEN_HEIGHT/4 + 128, 256, 128, "sprites/buttons/host.png", new function<void()>(&host)),
                        new button( SCREEN_WIDTH/2 - 128, SCREEN_HEIGHT/4 - 64, 256, 128, "sprites/buttons/play.png",  new function<void()>(&play)),
                        new button (SCREEN_WIDTH/2 - 128,SCREEN_HEIGHT/4 + 320 , 256, 128, "sprites/buttons/join.png",  new function<void()>(&join) )});
mapScreen= new screen("sprites/bg/background.png",NULL, NULL,NULL, NULL, new vector<button*>);
killAmount= new screen ("sprites/bg/background.png",NULL, NULL, NULL,NULL, new vector<button*>{new button(SCREEN_WIDTH - 128 - 64, SCREEN_HEIGHT - 10 - 64, 128, 64, defaultFont, "OK", 134, 314, 56, BLACK,new function<void()> ([](){changeMenu(choosing);})),new modIntButton(SCREEN_WIDTH/2 - 64, 64, 132, 64, maxKills)} );
modifyProfile= new screen("sprites/bg/background.png", NULL, NULL,new actualFunction([](){if (name == ""){name = "Player";}}),NULL,new vector<button*>{new button(SCREEN_WIDTH/2 - 64, SCREEN_HEIGHT - 16-64, 128, 64, defaultFont, "Change Weapons", 134, 314, 56,BLACK, new actualFunction([](){changeMenu(choosingWeapons);})),new typeButton(SCREEN_WIDTH/2 - 64, 64, 132, 64, &name,"name: "), new button(SCREEN_WIDTH - 16 - 128, SCREEN_HEIGHT- 16 - 64, 128, 64,"sprites/buttons/menu.png", new function<void()> (&gotoMenu) )});
gameScreen = new screen("sprites/bg/background.png", new function<void()>([](){print("goal");healthCount->changeDisplay(true); killCount->changeDisplay(true);}), NULL, new function<void()>([](){}), new vector<paper*>{objectiveDisplay});
weaponsScreen = new screen("sprites/bg/background.png",NULL, new actualFunction([](){print("money");weaponsScreen->buttons = new vector<Button*>(addVector<Button*>((showCollection()), *(weaponsScreen->buttons)));}), new actualFunction([](){clearVector<Button>(*(weaponsScreen->buttons),2, maximumWeapons);}), &moneyCount, new vector<Button*>{&leftScrollButton, &rightScrollButton, inventory[0],inventory[1], inventory[2]});
      /* cout << "Enter a name!" << endl;
       string name;
cin >>name;*/
loadSave();
loadVectors();
name = "";
bool showFPS = false;
//changeMenu(choosingWeapons);
//cout << dir->wdirp->data;
//print("money");
//e.key.repeat = false;
int currentTime = 0; // an int used to calculate how much the game should delay.
    while (quit == false)
    {

      currentTime = SDL_GetTicks();
         //  if (SDL_PollEvent(&e))
                  SDL_PollEvent(&e);


            if (menu != playing )
        {
           setUp();
           if (menu == playing)
           {
               me->name = name;
               SDL_StopTextInput();
           }
        }
        else if (me)
        {

    if( win(maxKills))
    {
        show();
         SDL_Color col = {255,255,255};
         pause = true;
        write(defaultFont, "Game Over. " + winner->name + " won!",&col, BLACK,announcement,  64, 64, SCREEN_HEIGHT/2, SCREEN_WIDTH-64, 64);
        announcement->changeDisplay(true);
        announcement->show(render, NULL, announcement->getRect());
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
          reset();
            menu = title;
             if (winner == me)
            {
                money += me->killCount*10;
             //   cout << money;
            }
        }
    }
    else
    {
        gameLoop();
    }
    int nowTime = SDL_GetTicks() - currentTime;
            if (nowTime <=delay)
            {
           SDL_Delay(delay - (nowTime));
            }
            if (showFPS)
     cout << nowTime << endl;
    //e.type == NULL;
        }
          if (e.type == SDL_KEYDOWN || e.type == SDL_TEXTINPUT)
         {
             if (!e.key.repeat)
             {
                if (e.type == SDL_KEYDOWN)
                 {
                     switch (e.key.keysym.sym)
                     {
                     case SDLK_ESCAPE:
                        if (menu != title)
                        {
                            announcement->changeDisplay(false);
                            changeMenu(title);
                        }
                         else if (menu == title && c != NULL)
                         {
                             announcement->changeDisplay(true);
                             changeMenu(playing);
                         }
                         break;
                     case SDLK_F3:
                        showFPS = true;
                        break;
                    }
                 }
              //   cout << 2 << endl;
                e.key.repeat = true;
             }
         }
         else if (e.type == SDL_KEYUP)
         {
             e.key.repeat = false;
            // cout << 3 << endl;
         }
         else if (e.type == SDL_MOUSEBUTTONDOWN)
         {
             mouseDown = true;
         }
         else if (e.type == SDL_MOUSEBUTTONUP || e.key.repeat)
         {
             mouseDown = false;
         }
        /* int shelfSize = shelf.size();
         for(unsigned int i = 0; i < shelfSize; ++i)
         {
             shelf[i].show();
         }*/
         int size = currentScreen->sheets.size();
         for (int i = 0; i < size; i ++)
         {
             currentScreen->sheets[i].get()->show();
         }
      //   bloodSprite->show(0,0);
            SDL_RenderPresent(render);
        SDL_RenderClear(render);

    }


}






