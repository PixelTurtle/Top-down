#ifndef MODES_H_INCLUDED
#define MODES_H_INCLUDED

//class projectile;
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 640;
//= SDL_CreateWindow("RPG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
SDL_Window* game = SDL_CreateWindow("RPG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);


//SDL_Renderer* quests = SDL_CreateRenderer(game, -1, SDL_RENDERER_ACCELERATED);
extern int scoreBegins;
extern bool quit ;
extern SDL_Rect* camera ;
extern SDL_Event e;
extern std::vector<paper> shelf;
extern std::list <std::unique_ptr<projectile>> projectiles;
extern menus menu ;
extern stage* level;// the almighty stage pointer. This should be the only instance of a stage in the entire project.
extern string shot;
  string name = "";
extern screen* currentScreen;
SDL_Texture* connectingTexture;
screen* titleScreen = NULL;
screen* mapScreen = NULL;
screen* killAmount = NULL;
screen* modifyProfile = NULL;
screen* connectingScreen = NULL;
screen* gameScreen = NULL;
screen* weaponsScreen = NULL;
extern player* me;
extern server* s ;
extern client* c ;
extern bool isHost;
extern bool mouseDown ;
extern player* winner ;
extern Effect* announcement ;
extern Effect* invSprite;
extern Effect* weaponSelect;
extern Effect* hurtBorder;
extern bool pause;

typeButton* currentType = nullptr;
//SDL_Texture* pic = nullptr;
int xMid, yMid, a, b;
extern bool armory[maximumWeapons];
class Effect;
list<Effect*> effectShelf;

void loadSave()
{
    ifstream f("player.txt");
    if (!f.is_open())
    {
        f.close();
        ofstream open("player.txt");
        int size = sizeof(armory)/sizeof(bool);
        for (unsigned int i = 0; i < size; i ++)
        {
            if (i == 0)
            {
                open.write("true", 4);
            }
            else
            {
                open.write("false", 5);
            }
            open.write(" ", 1);
        }
        open.close();
    }
    else
    {
        string buffer = "";
        int it = 0;
         while (getline(f, buffer, ' '))
         {
             if (buffer == "true")
             {
                 armory[it] = true;
             }
             else if (buffer == "false")
             {
                 armory[it] = false;
             }
             it++;
         }
         f.close();
    }


}

void loadVectors() // a method that is used in the beginning to fill the mapScreen and shop vectors
{
     DIR* dir = opendir("maps");
int itr = 0;
int buttonPerCol = 4; //represents how many buttons per column
int space = SCREEN_HEIGHT/(buttonPerCol+1);
dirent* d = NULL;
while (d = readdir(dir))
{
        name = (string)d->d_name;
        if (name != "." && name != ".." )
        {
       // cout << name << endl;
        mapScreen->buttons->push_back( (new levelButton(itr/(buttonPerCol) + 32, (itr%(buttonPerCol+1))*64 + 16, 256, 128, "sprites/maps/" + name.substr(0, name.length() - 3) + "png")));
        itr++;
        }
}
delete d;
closedir(dir);
/*
int size = sizeof(armory)/(sizeof(bool)) - 1;
SDL_Color col ;
for (int i = 0; i < size; ++i)// so the pistol won't appear in the shop
{
   // cout << i << endl;
    if (!armory[i + 1])
    {
        col = {0, 190, 87};
    }
    else
    {
        col = {0, 0, 0};
    }
    shop->buttons->push_back(new weaponItemButton( 10, (SCREEN_WIDTH/4 )*(i/buttonPerCol + 1) -64, 16*(i + 1) + 64*(i), 128, 64, "sprites/items/pack.png"));
}*/

}

void check()
{
    if (s != NULL)
       {
       //    cout << "Listening " << endl;

         s->notify();
          //  s->hear();
       }
       else
       {
           c->listen();
       }
}
string serializedBullets = "-1"; //a string that represents all current bullets serialized
   actions keyLog = noKey; //an enum that tracks the current event.
void update()
{
    if (s!= NULL)
    {
        if (charCount(shot, '@') != projectiles.size()) //if new bullets have since been added to the projectiles vector since the last frame
        {
            shot = "";
            int projSize = projectiles.size();
            for (std::list<unique_ptr<projectile>>::const_iterator i = projectiles.begin(); i != projectiles.end(); ++i)
            {
                shot += (*i)->serialize() + "@";
            }
          //  cout << serializedBullets << endl;
        }
        // std::cout << "update " + me->serialize() + "@" + shot << endl;
        s->serverSend("update " + me->serialize() + "@" + shot);
    }
    else
    {
    //   std::cout << "update " + me->serialize() + " " + convert(keyLog) +"@" + shot << endl;
        send(c->pack, c->sock, 24, "255.255.255.255", "update " + me->serialize() + " " + convert(keyLog) +"@" + shot);
    }
}
void updateCamera()
{
    if (me)
    {
        if (camera)
        {
           camera->x =  me->sheet->getRect()->x + me->sheet->getRect()->w/2- SCREEN_WIDTH/2;
            camera->y = me->sheet->getRect()->y + me->sheet->getRect()->h/2 - SCREEN_HEIGHT/2;
        }
        else
        {
            camera = new SDL_Rect{me->sheet->getRect()->x + me->sheet->getRect()->w/2- SCREEN_WIDTH/2, me->sheet->getRect()->y + me->sheet->getRect()->h/2 - SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT};
        }
            if (camera->x < 0)
        {
           // cout << me->sheet->getRect()->x << endl;
            camera->x = 0;
        }
        else if (camera->x + camera->w> level->width)
        {
            camera->x = -camera->w + level->width;
        }
        if (camera->y < 0)
            camera->y  =0;
        else if (camera->y + camera->h > level->height)
            camera->y  = - camera->h + level->height;
    }
}
void show();

extern string objective;
extern int money;
extern paper moneyCount;
extern paper* healthCount, *killCount,*objectiveDisplay;
void print(string command)
{
    if (command == "health")
    {
                 write(defaultFont, "Health: " + convert(me->health),NULL,BLACK, healthCount, 64, 0, scoreBegins, 120, 120);

    }
    else if (command == "kills")
    {
        write(defaultFont, "Kills: " + convert(me->killCount), NULL,  BLACK,killCount, 64 , 0, scoreBegins + 120, 120, 120);
    }
    else if (command == "goal")
    {
       // announcement = &shelf[2];
            write(defaultFont, objective,NULL, BLACK,objectiveDisplay, 64, 64, 10, SCREEN_WIDTH-64, 64 );
        //    write(defaultFont, "otato",NULL, announcement.get(), 64, 64, 10, SCREEN_WIDTH-64, 64 );

    }
    else if (command == "money")
    {
//        cout << "A" << endl;
        write(defaultFont, "Money: " + convert(money) , NULL,BLACK, &moneyCount, 64, SCREEN_WIDTH -128, 10, 128, 64);
    }
}


bool payUp(int cost)
{
    if (money >= cost)
    {
        money-= cost;
        print("money");
        return true;
    }
    return false;
}


void reset(bool announceClear = true, bool levelClear = true, bool playerClear = true, bool serverClear = true, bool clientClear = true, bool projectileClear = true) // a method that completely resets the level for the client
{
                //menu = title;
                cout << "Resetting..." << endl;
                if (announceClear)
            announcement->clear();
            if (levelClear)
            level->resetLevel();
            if (playerClear)
            level->resetPlayers();
            if (serverClear)
            {
            if (s)
            {
                delete s;
            }
           s = NULL;
            }
            if (clientClear)
           if (c->user != NULL)
            c->reset();
            if (projectileClear)
            {
                projectiles.clear();
            }
}

screen* currentScreen;
extern string name;
bool isHost;
//bool first = false;
void setUp()
{

      //if (menu != playing)
        {

           // cout << menu << endl;
              switch(menu)
              {
                  case title:
                    changeScreen(*titleScreen);
                    break;
                  case choosing:
                    changeScreen(*mapScreen);
                    break;
                  case chooseKills:
                   changeScreen(*killAmount);
                    break;
                  case connecting:
                    changeScreen(*connectingScreen);
                    break;
                  case modSelf:
                    changeScreen(*modifyProfile);
                    break;
                    break;
                  case choosingWeapons:
                      changeScreen(*weaponsScreen);
                    break;
              }
              renderTexture(currentScreen->bg.get(), NULL, NULL);
              if (currentScreen->buttons != NULL)
              for (unsigned int i = 0; i < currentScreen->buttons->size(); ++i)
              {
                  button* b = currentScreen->buttons->at(i);
                   b->sheet->show();
                    b->during();
                  if (e.motion.type == SDL_MOUSEMOTION || e.button.type == SDL_MOUSEBUTTONDOWN )
                  {
                      int a , c;
                      SDL_GetMouseState(&a, &c);
                      SDL_Point p = {a, c};
                          if (SDL_PointInRect(&p, b->sheet->getRect()))
                          {
                              if (e.button.type == SDL_MOUSEBUTTONDOWN && !mouseDown)
                              {
                                  if (selector)
                                  {
                                selector->changeSelected(false);
                               selector->release();
                                  }
                                  selector = b;
                                  selector->changeSelected(true);
                                  b->function();
                                  e.type = SDL_MOUSEBUTTONUP;
                              }
                              else
                              b->sheet->changeTextColor({ 0, 100, 25});
                          }
                          else
                          {
                              b->sheet->changeTextColor( {255,255,255});
                          }
                  }
              }



              if (menu == connecting)
              {
                  if (c->connect())
                  {
                      changeMenu(playing);
                  }

              }
             if (menu == playing )
              {
               //   if (isHost)
               //     cout << level->players.size() << endl;
                    reset(true, true, isHost, isHost,isHost);
                //    cout << level->players.size() << endl;
                    if (!level->loadLevel(level->levelLocation))
                  {
                      throw logic_error("Error loading map...");
                  }
                    if (isHost)
                    {
                        s = new server();
                        s->clientPtr = c;
                          c->reset(new player(c->port, name));
                          level->players.push_back(c->user.get());
                    }
                      //
                  pause = false;
            me = c->user.get();
            if (me == NULL)
            {
                throw std::logic_error("me is null") ;
            }
            for (int i = 0; i < invSize; i ++)
            {
                if (inventory[i])
                {
                    //cout << inventory[i]->getWeaponName() << endl;
        //            arrPushBack<shared_ptr<weapon>, invSize>(me->inventory,shared_ptr<weapon>(nullptr),(shared_ptr<weapon>(loadGun(inventory[i]->getWeaponName(), me))));
                   me->weapons.replace(i, std::shared_ptr<weapon>(loadGun(inventory[i]->getWeaponName(), me)));
                  // cout << inventory[i]->getWeaponName() << endl;
                }
            }
            me->equip(me->weapons.get(0).get());
          //  level->players.push_back(me);
           // cout << me->spriteLocation << endl;
            //me->name = name;

           //  int a, b, xMid,yMid; // used to detemine the player's angle
             //send(c->pack, c->sock, 24, "255.255.255.255", "update " + convert( c->port)+ " "+ c->user->serialize() +"@"+ shot );
             print("health");
             print("kills");
             SDL_Color col = {255,255,255};
             objective =  "Objective: kill " + convert(maxKills) + " people.";
             print("goal");
               updateCamera();
            changeScreen(*gameScreen);
            if (!isHost)
            {
                me->health = 2;
            }
                 return;
              }
               currentScreen->function();
               int size = currentScreen->sheets.size();
           for (int i = 0; i < size; i ++)
           {
               currentScreen->sheets[i].get()->show();
           }
            //  announcement->show();
           //   currentScreen->sheet->show();

           /*   SDL_RenderPresent(render);
              SDL_RenderClear(render);*/
        }
}

double prevAngle = 0;
extern const int invSpriteBorder;

extern SDL_Texture* transparent;
void show()//a function that updates and renders everything
{
  //  SDL_RenderCopy(render, me->sprite, NULL, &invRect);
 //cout << "Number of players: " << level->players.size() << endl;

       renderTexture( level->getBg(), camera, NULL);
      int size = me->location->plot.size();
     // cout << size << endl;
            for ( int x = size-1; x >= 0; x-- )
        {
            object* obj = me->location->plot[x].get();
            if ( obj != NULL && obj->dead == false )
            {
          //      cout << x << endl;
                if ( obj->del == true )
                {
                    me->location->plot.erase(me->location->plot.begin() + x);
                }
                else
                {
                    obj->everyTick();
                  //  std::cout << size << std::endl;
                                          //  ptr->sheet->show(render, ptr->sheet->getPortion(),&r, ptr->angle*(180/M_PI));
                      SDL_Rect r = {obj->sheet->getRect()->x - camera->x, obj->sheet->getRect()->y - camera->y, obj->sheet->getRect()->w, obj->sheet->getRect()->h};
                     // std::cout << obj->sheet->getRect()->w << std::endl;
                      obj->sheet->show(render, obj->sheet->getPortion(),&r);
                }
            }
        }

       // int projSize = projectiles.size();
        //cout << projSize << endl;
      //  cout <<projectiles.size();
                    //cout << projectiles.size() << endl;
        for ( std::list<unique_ptr<projectile>>::iterator v= projectiles.begin(); v != projectiles.end(); ++v)// renders and updates projectiles
{
   // cout << 1 << endl;
   projectile* current = (v->get());
           character* owner = (*v)->owner;
        (*v)->movement();
              SDL_Rect* rect = current->sheet->getRect();
                      int xDirection = convertTo1(current->x_dir);
                      int yDirection = convertTo1(current->y_dir);
                      int p = level->players.size();
            for ( int k= 0; k < p; ++k )
    {
        player* play = level->players[k];
        SDL_Rect* playRect = play->sheet->getRect();

         if ( play != owner && !play->isDead && (RectIntersect(playRect, rect, current->speed) != "none" || (playRect->x*(xDirection) <= xDirection*current->rectX &&playRect->x*xDirection >= xDirection*(current->rectX + rect->w) && playRect->y*(yDirection) <= yDirection*current->rectY &&playRect->y*yDirection >= yDirection*(current->rectY + rect->h) ) ))
        {

            current->collision(play);
           break;

        }

    }
int pl = current->location->plot.size();
    for ( int i= 0; i < pl; ++i )
    {
    object* obj =  current->location->plot[i].get();
        if ( obj != NULL)
        {
                 SDL_Rect* objRect = obj->sheet->getRect();
               if ( obj->solid && (RectIntersect(objRect, rect, current->speed) != "none" || (objRect->x*(xDirection) <= xDirection*current->rectX &&objRect->x*xDirection >= xDirection*(current->rectX + rect->w) && objRect->y*(yDirection) <= yDirection*current->rectY &&objRect->y*yDirection >= yDirection*(current->rectY + rect->h) ) ))
            {
               // cout << i << endl;
                // cout << pl << endl;
                //cout << location->citizens[k]->health << endl;;
               current->collision(NULL);
                //ptr->del = true;
                break;
            }
        }
    }
       if ( (*v)->sheet->getRect()->y > level->height || (*v)->sheet->getRect()->y < 0 || (*v)->sheet->getRect()->x < 0 || (*v)->sheet->getRect()->x > level->width || (*v)->del == true)
       {
     v =  projectiles.erase(v);
     //   projSize--;
       }
        else
        {
    SDL_Rect r = {(*v)->sheet->getRect()->x - camera->x, (*v)->sheet->getRect()->y - camera->y, (*v)->sheet->getRect()->w, (*v)->sheet->getRect()->h};
  //  cout <<  (*v)->sheet->getRect()->w<< " " <<  (*v)->sheet->getRect()->h << endl;
//cout << projectiles[v]->x_dir << " "  << projectiles[v]->y_dir << endl;
   (*v)->sheet->show(render,  NULL,&r, 180/M_PI*(atan((*v)->y_dir / (*v)->x_dir)) + 90 + 180*((*v)->x_dir < 0), NULL, SDL_FLIP_NONE);
// SDL_RenderCopy(render, projectiles[v]->sprite, NULL, &r);
        }

}
 //cout << projectiles.size() << endl;
        int much = level->players.size();
   //     cout << much << endl;
         for (unsigned int k = 0; k < much ; ++k )
        {
                check();
                  player* ptr = level->players[k];
                  if (ptr->health <= 0 && !ptr->isDead)
                  {
                         ptr->isDead = true;
                  //     cout << ptr->name << ": " << ptr->health << endl;
                        if (ptr->lastDamager != NULL)
                        {
                           // cout << k << endl;
                      ptr->lastDamager->killCount++;
                        }
                      if (ptr->lastDamager == me)
                      {
                        //  cout << "A " << endl;

                          // cout << "A" << endl;
                          std::string str = "You killed: " + ptr->name;
                        announcement->changeText(defaultFont,str,NULL, {78,144,188}  );
                         announcement->changeDuration(1000);
                         announcement->init(SCREEN_WIDTH/2 - str.length() ,SCREEN_HEIGHT/4);

                        //* ( new Effect("Good Job", {0,0,64,64}, 10000, false))->init(0,0);
                        //cout << "Size: " << effectShelf.size() << endl;
                          print("kills");
                      }
                     else if (ptr == me)
                      {
                     //   SDL_Color* col = new SDL_Color{255,255,255};
                        announcement->changeText(defaultFont, "You died!");
                        announcement->changeDuration(1000);
                        announcement->init(SCREEN_WIDTH/2, SCREEN_HEIGHT/4);
                        pause = true;
                       // delete col;

                      }
                  }
                  else
                  {
                      if (ptr->health > 0)
                      ptr->isDead = false;
                        ptr->show();
                        if (ptr == me)
                        {

                        }

                  }
        }
        if (me->tookDamage)
        {
            me->tookDamage = false;
             hurtBorder->init(0,0);
        }
//invSprite->show();

     for (list<Effect*>::iterator i = effectShelf.begin(); i != effectShelf.end(); ++i )
     {

        //cout << i << endl;
         if (!(*i)->isPlaying())
         {
             i = effectShelf.erase(i);
         }
         else
         {

            bool respect = (*i)->getRespectToCamera();
             SDL_Rect r = { (*i)->getRect()->x - camera->x*respect,  (*i)->getRect()->y-camera->y*respect,  (*i)->getRect()->w,  (*i)->getRect()->h};
            (*i)->show(render,(*i)->getPortion(),&r);
          // cout <<  (*i)->getRect()->w << " " <<   (*i)->getRect()->h << endl;

         }
     }
         if (invSprite->isPlaying())
    {
    //    cout << " A " << endl;
    int invSize = me->weapons.getSize();
    SDL_Rect* invRect = invSprite->getRect();
   // cout << invSize << endl;
    //cout << invRect->y << endl;
         for(int i = 0; i < invSize; i++)
         {
             if (me->weapons.get(i).get())
             {
               //  cout << i << endl;
             //SDL_Rect* rect = new SDL_Rect{64*i , SCREEN_HEIGHT - 64, 64, 64};
           me->weapons.get(i)->show( invRect->x + invRect->w/invSize*i + invSpriteBorder, invRect->y + invSpriteBorder, 0);
             }
         }
            weaponSelect->show(render,weaponSelect->getPortion(),weaponSelect->getRect());
    }
     SDL_SetRenderDrawColor(render, 244, 70, 255, 00);
  //         SDL_RenderDrawLine(render, xMid, yMid, a, b);
          SDL_SetRenderDrawColor(render, 0,0, 0, 0 );

}
const int mouseScrollDelay = 150;
int scrollDelay = 0;

void gameLoop()
{

//keyLog = "noKey";

/*if (s!= NULL)
{
    s->notify();
}*/
shot = "-1";


      // cout << "Much: " << much<< endl;
//cout << menu << endl;

    if (e.type == SDL_QUIT)
    {
        quit = true;
    }
//cout << menu << endl;
  /*  if (pause)
    {
       // cout << menu << endl;
        if (me->health <= 0)
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {

              print("goal");
               announcement->changeDisplay(false);
               menu = playing;
               pause = false;
               me->die();
               update();
                            updateCamera();
                            print("health");
        }

    }*/
    else
    {


     if (e.type == SDL_KEYDOWN )
    {
       // cout << "keyPressed  " << endl;
            if (!e.key.repeat)
            {
                int sym = e.key.keysym.sym ;
                if (sym -1  < SDLK_0 + invSize  && sym - 1>= SDLK_0 )
                {
                    //cout << sym-SDLK_0 << endl;
                    if (me->weapons.get(sym - SDLK_0 - 1).get())
                    me->equip(me->weapons.get(sym - SDLK_0 - 1 ).get());
                }
                else
                {
                    switch(e.key.keysym.sym)
                    {
                    case SDLK_d:
                        keyLog = moveRight;
                        me->xDir = 1;
                        break;
                    case SDLK_a:
                        keyLog = moveLeft;
                       me->xDir = -1;
                        break;
                    case SDLK_w:
                        keyLog = moveUp;
                        me->yDir = -1;
                        break;
                    case SDLK_s:
                        keyLog = moveDown;
                        me->yDir = 1;
                        break;
                    case SDLK_BACKQUOTE:
                        cout << "Coordinates of me: " << me->sheet->getRect()->x << " " << me->sheet->getRect()->y << endl;
                        cout << "Number of players: " << level->players.size() << endl;
                      announcement->changeDisplay();
                            break;
                    case SDLK_F1:
                        {
                            int length = currentScreen->sheets.size();
                            for (int i = 0; i < length; i ++)
                            {
                                gameScreen->sheets[i].get()->changeDisplay();
                            }
                        //   ( new Effect("Good Job", {0,0,64,64}, 10000, false))->init(camera->x,camera->y);
                      //      me->hurtEffect->init(camera->x,camera->y);
                            break;
                        }
                    }
                }
            }
        }
               else if ( e.type == SDL_KEYUP)
        {
          //  keyLog = keyRelease;
             switch(e.key.keysym.sym)
            {
            case SDLK_a:
            case SDLK_d:
                me->xDir = 0;
                keyLog = horizRelease;
        //        me->yspeed = 0;
                break;
            case SDLK_s:
            case SDLK_w:
                me->yDir = 0;
                keyLog = vertRelease;
                break;
            }
             // cout << last_pressed << endl;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN && mouseDown == false)
        {
            mouseDown = true;
            if (e.button.button == SDL_BUTTON_LEFT)
            {

         //    int hypotenuse = sqrt(pow((a-xMid),2) + pow(b-yMid, 2));
                if (invSprite->isPlaying())
                {
                    int index = (weaponSelect->getRect()->x - invSprite->getRect()->x)/weaponSelect->getRect()->w;
                    if (me->weapons.get(index).get())
                    {
                        me->equip(me->weapons.get(index).get());
                        invSprite->addTime(-99999999);
                        weaponSelect->addTime(-9999999);
                    }
              //      cout << "A" << endl;
                }
                else
                {
                                    if (me->isDead)
                          me->die();
                    else if (me->hand->shootable())
                      shot =  me->hand->use();
                }
            }
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            mouseDown = false;
        }
         else if (e.type == SDL_MOUSEMOTION)
        {
          //  movedMouse = true;
          //cout << "move" << endl;
          if (!me->isDead)
          {

            SDL_GetMouseState(&a, &b);
              yMid = me->sheet->getRect()->y + me->sheet->getRect()->h/2 - camera->y;
            xMid = me->sheet->getRect()->x + me->sheet->getRect()->w/2 - camera->x;
                if (a-xMid < 0)
                {
                    double angle =  ((atan(((double)(b-(yMid))) /(a-xMid)))) ;
                    me->angle = M_PI + angle;
                }
                else
           me->angle = (atan(((double)(b-(yMid))) /(a-xMid)));
           keyLog = mouseMotion;
          // cout << me->angle << endl;
        }
        }
        else if (e.type == SDL_MOUSEWHEEL)
        {
        //    if(  || !invSprite->isPlaying()  )
            {
               //     bool up = e.wheel.y > 0;
                if ((e.wheel.timestamp - scrollDelay)>= mouseScrollDelay)
                {
                     scrollDelay = SDL_GetTicks();
                SDL_Rect* rect = weaponSelect->getRect();
                SDL_Rect* iRect = invSprite->getRect();
                int scrollDirection = convertTo1(e.wheel.y);
                    weaponSelect->changeRect(rect->x + (scrollDirection*(rect->x + rect->w*(.5+.5*scrollDirection)) < scrollDirection*(iRect->x + (.5+.5*scrollDirection)*(iRect->w )) )*scrollDirection*rect->w, rect->y);
                //    cout << e.wheel.y << endl;
                invSprite->addTime(1000);
              //  weaponSelect->addTime(1000);
                }
                else
                {
                    weaponSelect->init();
                }
                  invSprite->init();
            }

        }
        else
        {
            keyLog = noKey;
        }



 //std::thread showThread(show);
 show();
//if (isHost)


bool doneAnything = ((me->xspeed != 0 || me->yspeed != 0 || shot != "-1" || e.type==SDL_MOUSEMOTION));



//cout << keyLog << endl;
        // if (doneAnything)
     //   cout << me->angle << endl;
                // if (isHost)

   //     cout << c->prevX << endl;
   // cout << me->xDir << " " << me->yDir << endl;
  //cout <<"1. " <<  me->sheet->getRect()->x  << endl;
            me->setSpeeds();
if (!isHost )
{
    if (c->received)
{

   c->origX = me->rectX;
   c->origY = me->rectY;
   update();
}


}
else
    update();
      me->walk(me->xspeed, me->yspeed);
if (c->received)
{
         c->received = false;
         c->prevX = me->rectX;
       c->prevY = me->rectY;
}





                      //      cout <<"2. " <<  me->sheet->getRect()->x << endl;
                    //  cout << c->received << endl;



//me->hand->resetRect();
updateCamera();
//showThread.join();
//cout << me->sheet->getRect()->y << endl;
//cout << c.user->serialize() << endl;

}
}


#endif // MODES_H_INCLUDED
