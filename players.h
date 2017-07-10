#ifndef PLAYERS_H_INCLUDED
#define PLAYERS_H_INCLUDED

#include "objects.h"
#include "arrays.h"


using namespace std;

class projectile;
class grid;
class quest;
extern SDL_Renderer* render;
extern SDL_Renderer* quests;
extern grid* grasshill;
extern list<unique_ptr<projectile>> projectiles;
extern SDL_Event e;
extern int last_pressed;
extern string shot;
extern stage* level;

/*bool line_intersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
    int slopex = 0;
    int slopey = 0;
    vector<int>total_points;
    for (k = 0; k < y4; k++)
    {
        slopex =
    }


}*/



enum direction
{
    east,
    west,
    up,
    down
};

class player;


string RectIntersect(SDL_Rect* a, SDL_Rect* b, int diff)
{
    SDL_Rect* c = a;
    SDL_Rect* d = b;
   if (( (c->x -diff <= d->x + d->w && c->x -diff >= d->x)|| (c->x + c->w +diff<= d->x + d->w && c->x+ c->w + diff >= d->x)  ) && ((c->y <= d->y + d->h &&c->y >= d->y)  || (c->y + c->h <= d->y + d->h && c->y + c->h >= d->y)) )
   {
       return "horiz";
   }
   else if (( (c->x  <= d->x + d->w && c->x  >= d->x)|| (c->x + c->w <= d->x + d->w && c->x+ c->w  >= d->x)  ) && ((c->y  - diff <= d->y + d->h &&c->y-diff>= d->y)  || (c->y + c->h  + diff<= d->y + d->h && c->y + c->h + diff>= d->y)) )
    {
    return "vert";
    }
    swap(c, d);
    if (( (c->x -diff <= d->x + d->w && c->x -diff >= d->x)|| (c->x + c->w +diff<= d->x + d->w && c->x+ c->w + diff >= d->x)  ) && ((c->y <= d->y + d->h &&c->y >= d->y)  || (c->y + c->h <= d->y + d->h && c->y + c->h >= d->y)) )
   {
       return "horiz";
   }
   else if (( (c->x  <= d->x + d->w && c->x  >= d->x)|| (c->x + c->w <= d->x + d->w && c->x+ c->w  >= d->x)  ) && ((c->y  - diff <= d->y + d->h &&c->y-diff>= d->y)  || (c->y + c->h  + diff<= d->y + d->h && c->y + c->h + diff>= d->y)) )
    {
    return "vert";
    }
   else
   {
       return "none";
   }
   return "none";
}
string RectIntersect(const SDL_Rect& a, const SDL_Rect& b, int xDir , int yDir)
{
    SDL_Rect* c = new SDL_Rect(a);
    SDL_Rect* d = new SDL_Rect(b);
    string val = "";
    bool X0 = (xDir != 0);
    bool Y0 = (yDir != 0);
    for (int i = 0; i < 2; i ++)
    {

        //cout << d->y<< endl;
       // cout << X0 << endl;
        bool horizCollision = (X0&& ( (c->x + xDir <= d->x + d->w && c->x +xDir >= d->x)|| (c->x + c->w +xDir<= d->x + d->w && c->x+ c->w +xDir >= d->x)  ) && ((c->y <= d->y + d->h &&c->y >= d->y)  || (c->y + c->h <= d->y + d->h && c->y + c->h >= d->y)) );
        bool vertCollision = (Y0&&( (c->x  <= d->x + d->w && c->x  >= d->x)|| (c->x + c->w <= d->x + d->w && c->x+ c->w  >= d->x)  ) && ((c->y  + yDir <= d->y + d->h &&c->y+yDir>= d->y)  || (c->y + c->h  + yDir<= d->y + d->h && c->y + c->h + yDir>= d->y)) );
     //   cout << ((c->y <= d->y + d->h &&c->y >= d->y)  || (c->y + c->h <= d->y + d->h && c->y + c->h >= d->y)) << endl;


       /*if (horizCollision&& vertCollision)
        {
            val = "diag";
        }*/

       if (horizCollision)
       {
        val = "horiz";
       }
       else if (vertCollision)
       {

        val = "vert";
        }
         if (i == 1)
        {
            val = "none";
        }
        if (val != "")
        {
           break;
        }
        swap(c,d);

    }
    delete c;
    delete d;
    return val;
}
class character : public object
{
    public:
       float speed;
       int base_speed;
    double xspeed = 0;
    double yspeed  = 0;
    int xDir = 0;
    int yDir = 0;
    double rectX = 0;
    double rectY = 0;
  //  int sprite_angle = 0;
    int health;
    int max_health;
   /* int face_up = 1;
    int face_side = 0;*/
    //vector<unique_ptr<effect>> effects;
     int killCount = 0;
     character(int heal,double  x, double y, int w, int h, int portionWidth,int portionHeight,float veloc,  grid* where, const char* sprite_location): object(x,y,w,h, portionWidth, portionHeight,where, sprite_location)
    {
        solid = false;
        max_health = heal;
        health = max_health;
        speed = veloc;
        base_speed = speed;
        rectX = x;
        rectY = y;
      //  cout << rectX << endl;
       // spawn(*where);

    };
    virtual int walk(double x_dir, double y_dir); //declared in NPCS.h
      virtual void every_tick() // a function that is aticvated every millisecond
    {

    }
    virtual string serialize()
    {
        return convert(sheet->getRect()->x) +  " " + convert(sheet->getRect()->y)  +   " " +convert(health) ;
    }
    virtual void takeDamage(int damage, character& killer)
    {
        health -= damage;

    }
    virtual void spawn(grid& where)
    {
          int x = rand()% (level->width - 64);
        int y = rand() % (level->height - 64);
       // int x = 608; int y = 608;
/*int x = 600;
int y = 295;*/
        sheet->getRect()->x = x;
        sheet->getRect()->y = y;
       // cout << user->location->plot.size() << endl;
            for (int h = 0; h< where.plot.size();h ++)
            {
       //         cout << x << " "  << y <<  " " << endl;
                if (RectIntersect(sheet->getRect(), where.plot[h]->sheet->getRect(), 1 ) != "none")
    //   if (SDL_HasIntersection(rect, where.plot[h]->rect))
                {
                //    cout << " nope: "<<h <<  endl;
                    x = rand()% (level->width - 64);
                    y = rand() % (level->height - 64);
                   sheet->getRect()->x = x;
                    sheet->getRect()->y = y;
                    h = 0;
                }
            }
            rectX = x;
            rectY = y;
    }
    void setSpeeds()
    {
      //  cout << xDir << " " << yDir << endl;
           if (yDir != 0)
           {
           xspeed = fabs(speed*((sin(atan(xDir/yDir)))))*xDir;

          //  cout << "Speeds: " << fabs((((sin(atan(xDir/yDir))))*speed))  << endl;
           }
            else
            xspeed = speed *xDir;
           if (xDir != 0)
           {
            yspeed = fabs(speed*((sin(atan(yDir/xDir)))))*yDir;
           // cout << yspeed << endl;
           }
           else
            yspeed = speed*yDir;

    }
};

void do_damage(int how_much, character* target)
{
  /* // cout << "ASdf" << endl;

    for (unsigned int j = 0 ; j < target->effects.size(); j ++)
    {
            if (dynamic_cast<invincible*>(target->effects[j].get()) != 0)
            {
               // cout << " SDf" << endl;
                return;
            }
    }
        target->health -= how_much;
//        target->walk(-10*target->face_side, -10*target->face_up);

        target->effects.push_back((unique_ptr<effect>(new invincible(target))));
*/

}

class weapon;
//const int invSize  = 3;
class Effect;
extern list<Effect*> effectShelf;
extern const std::string defaultFont;
class Effect : public TempAnimation
{
    bool respectToCamera = false; // False if this effect just stays in one spot. True if the object is rendered with respect to the camera.
public:
       Effect (std::string spriteSheetLocation, const SDL_Rect& portionOfSpriteSheet, const SDL_Rect& blitTo, int frameT, int howLong, bool isStable ) : TempAnimation(spriteSheetLocation, portionOfSpriteSheet,blitTo , frameT, howLong)
       {
           respectToCamera = isStable;
       }
        Effect (std::string spriteSheetLocation, const SDL_Rect& blitTo, int frameT, int howLong, bool isStable ) :Effect(spriteSheetLocation, {0,0,0,0},blitTo , frameT, howLong, isStable)
       {
           respectToCamera = isStable;
            changePortion({0,0,spriteW, spriteH});
       }
       Effect(std::string toBeWritten, const SDL_Rect& blitTo, int howLong, bool isNotStable) : TempAnimation("", {0,0, 0,0}, blitTo, 0, howLong)
       {
            respectToCamera = isNotStable;
           changeText(defaultFont, toBeWritten);
           message = toBeWritten;

       }
       Effect() : TempAnimation("", {0,0,0,0}, {0,0,0,0}, 0,0)
       {

       }
       void init(int x, int y)
       {
           changeRect(x,y);
           if (SDL_GetTicks() - start >= duration)
           {
           start = SDL_GetTicks();
           effectShelf.push_back(this);
           }
       }
       void init()
       {
           init(getRect()->x, getRect()->y);
       }
       void addTime(int milliseconds)
       {
           start-=milliseconds;
       }
       bool isPlaying()
       {
           return SDL_GetTicks() - start < duration;
       }
       virtual void show(SDL_Renderer* renderer,SDL_Rect* part ,SDL_Rect* blit  ,double angleInDegrees =0 ,SDL_Point* center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE)
       {
           if (message == "")
           {
               TempAnimation::show(renderer, part, blit, angleInDegrees, center, flip);
           }
           else
           {
             //  cout << blit->x << endl;
               Paper::show(renderer,NULL,blit,angleInDegrees,center,flip);
           }
       }
       bool getRespectToCamera()
       {
           return respectToCamera;
       }


};
 //static Animation* playerWalkingSprite;
// static Effect* bloodSprite;
extern const int invSize;
//int big = 3;
 void print(string command);
 extern SDL_Rect* camera;
//Effect* hurtBorder;
class player : public character
{
    public:
    weapon* hand = NULL;
    Shelf<shared_ptr<weapon>> weapons = Shelf<shared_ptr<weapon>>(invSize);
    //SDL_Thread* thread;
    string name  = "name";
    int port;
    bool tookDamage = false;
    double angle = 0; // the angle the player is facing in RADIANS
    string spriteLocation = "";
    bool isDead = false;
    int locationIndex = 0;
   Paper healthBar;
    Effect* hurtEffect =  new Effect("sprites/players/blood(2).png", {0,0,494,512}, {0,0,64,64}, 50, 300, true );
    character* lastDamager =nullptr;
    player(int portNumber,  string iName, int heal, int x, int y,  const char* sprite_location, weapon* weap);
    player(int portNumber, string iName);
    player(string* arr);
    constexpr static double baseSpeed = 1.2;
    const static string defaultSpriteLocation ;
    //  both declared in weapons.h

    string serialize(); // declared in weapons.h

    virtual void update(int newHealth, double x, double y, double radian, weapon* newWeapon); //declared in weapons.h;
    virtual void update(string* arr);
    void equip(weapon* w); // declared in weapons.h
    virtual void die(); //declared in weapons.h
    void takeDamage(int amount, character& killer)
    {

        if (!isDead && health >0)
        {
        health-= amount;
        lastDamager = &killer;
        }
      //  cout << health << endl;
      tookDamage = true;
       print("health");
       SDL_Rect* rect = sheet->getRect();
       hurtEffect->init(rect->x, rect->y);

  //     cout << rect->x << rect->y << endl;
    }
    void show();// declared in weapons.h

    virtual int walk(float x_dir, float y_dir)
    {
        if (!isDead)
        {
        character::walk(x_dir, y_dir);
        if ((x_dir != 0 || y_dir != 0) )
        {

           ((Animation*)( sheet))->changeStopped(false);
            //sheet = playerWalkingSprite;
        }
        else
        {
            sheet->changePortion({0,0,sheet->getPortion()->w, sheet->getPortion()->h});
            ((Animation*)( sheet))->changeStopped(true);
        }
        }
    }


};

const string player::defaultSpriteLocation = "sprites/players/soldier.png";


enum explosions
{
    FIREROCKETEXPLOSION
};

struct explosion: public object
{
    int duration = 0;
    int damage =0;
    int start = 0;
   explosion(int x, int y, int w, int h, int howLong ,  int howMuchDamage, grid* where, const char* sprite_location):object(x, y, w, h, 64,64, where, sprite_location, 60 )
    {
         duration = howLong;
       //  delete sheet;
        //sheet = new Animation("sprites/explosion.png", {0, 0, 64, 64}, {x,y,w,h}, 1500/23);
        start = SDL_GetTicks();
        solid = false;
        damage = howMuchDamage;
        sheet->changeStopped(false);
        //cout << sheet->getRect()->w << endl;

    }
    virtual void collision(character* target)
    {
        target->health -= damage;
    }
    virtual void everyTick()
    {
        if ( SDL_GetTicks() - start >= 1500)
        {
          del = true;
       //   std::cout << "Del = true" << std::endl;
        }
    }
};

enum shots
{
    BULLET,
    LASER,
    FIREROCKET
};
static int projID = -1;
class projectile : public object
{

    int ID;
public:
  int   damage;
    double x_dir, y_dir, rectX, rectY, speed;
    shots name = BULLET ;
    character* owner = NULL;
    projectile(int x, int y, int w, int h,int portionWidth, int portionHeight, grid* where, const char* sprite_location, double x_po, double y_po, double velocity, int dam, character* shooter, int id = -2) : object(x, y, w, h, portionWidth,  portionHeight,where, sprite_location)
    {
           projectile( x, y,  w,  h,  where,  sprite_location, x_po,  y_po, velocity,  dam,  shooter, id) ;
       /* if (ID < -1000)
        {
            cout << "";
        }*/
    }
        projectile(int x, int y, int w, int h, grid* where, const char* sprite_location, double x_po, double y_po, double velocity, int dam, character* shooter, int id = -2) : object(x, y, w, h,where, sprite_location)
    {
        speed = velocity;
        x_dir = x_po;
        y_dir = y_po;
        damage = dam;
        rectX = x;
        rectY = y;
        owner = shooter;
        if (id == -2) //setting ID to -2 gives the bullet a new ID
        {
            projID ++;
            ID = projID ;
       //     cout << "ID: " << ID << endl;
        }
        else
        {
            ID = id;
        }
        }

     virtual void movement(); // declared in NPCS.h
    virtual void collision(character* target); // declared in NPCS.h

    virtual ~projectile()
    {

    }
    virtual string serialize()
    {
      //  player* ptr = dynamic_cast<player*>(owner);
        return convert(ID) + " " + convert(((player*)owner)->port) + " "+ convert(name) + " " + convert(sheet->getRect()->x) + " " + convert(sheet->getRect()->y) + " " + convert(x_dir) + " " + convert(y_dir) ;
    }
    virtual void update(int x, int y, double x_po, double y_po)
    {
        sheet->changeRect(x,y);
        x_dir = x_po;
        y_dir = y_po;
    }
    int getID()
    {
        return ID;
    }

};

struct bullet : public projectile
{
    bullet(int x, int y, grid* where, double x_po, double y_po, character* shooter, int id = -2) : projectile (x ,y , 16, 16, where, "sprites/projectiles/fireball.jpg", x_po, y_po, 3, 1, shooter,id)
    {
        name = BULLET;
    }
};

struct laser : public projectile
{
    laser(int x, int y, grid* where, double x_po, double y_po, character* shooter, int id = -2) : projectile (x, y, 16, 48,  where, "sprites/projectiles/laser.png", x_po, y_po, 4, 1, shooter,id)
    {
        name = LASER;
    }
};

struct fireRocket: public projectile
{
    fireRocket(int x, int y, grid* where, double x_po, double y_po, character* shooter, int id = -2) : projectile(x ,y , 24, 64, where, "sprites/projectiles/fireRocket.png", x_po, y_po, .05, 30, shooter,id)
    {
        name = FIREROCKET;
    }
    void movement()
    {
        speed *=1.1;
        projectile::movement();
    }
    void collision(character* target)
    {
      //  cout << "HE" << endl;
        projectile::collision(target);
        location->plot.push_back(shared_ptr<object>(new explosion(sheet->getRect()->x-64, sheet->getRect()->y-64, 128, 128, 120,  10, location, "sprites/explosion.png")));
    //   location->plot.push_back(unique_ptr<object>(new solid(rect->x, rect->y, 64, 64, location, "sprites/explosion.png")));
    }
};

class fireball : public projectile
{
    public:
        int radius;

    fireball(int x, int y, grid* where, int x_po, int y_po, int how_big, character* shooter, int id = -2) : projectile(x, y, how_big*2, how_big*2, where, "sprites/fireball.jpg", x_po, y_po, 2, 1, shooter,id)
    {
        radius = how_big;
    }
    void collision(character* target)
    {

        do_damage(damage + radius/2, target);
      //  delete this;
    }
};

/*int which_way(player* target, int& a, int& b)
{
    a = 0;
    b = 0;
    switch (target->facing)
    {
    case east:
        a = 1;
        break;
    case west:
        a = -1;
        break;
    case up:
        b = -1;
        break;
    case down:
        b = 1;
        break;
    }
    return a, b;
}*/
/*
void get_fast::use()
{
    owner->effects.push_back(unique_ptr<effect>(new sprint(owner)));
}

class survivalist : public player
{
    public:
    survivalist( int x, int y, grid* where): player(0,20, x, y, 64, 64, 1, where, "sprites/players/survivalist.png")
    {

//        hand = unique_ptr<weapon>(new shovel(this));
        basic_attack = new get_fast(this);
    }



};
    void sprint::use()
    {
        owner->speed = owner->base_speed*2;
    }
    void invincible::use()
    {
        SDL_SetTextureColorMod(owner->sprite, 225, 225, 225);
        if (duration%2 == 0)
        SDL_SetTextureColorMod(owner->sprite, 255, 255, 255);


    }


class hulk;

void hulking::use()
{
   // cout << "DID" << endl;
    owner->effects.push_back(unique_ptr<effect>(new hulk(owner)));
}

class demon_born : public player
{
public:
    demon_born( int x, int y, grid* where) : player(1, 30, x, y, 64, 64, 1, where, "sprites/players/goat.jpg")
    {
        basic_attack = new hulking(this);
    }




};
 hulk::hulk(character* what) : effect(what, 12000)
{
SDL_Surface* surf = IMG_Load("sprites/players/sprote.png");
SDL_Texture* text = SDL_CreateTextureFromSurface(render, surf );

what->sprite = text;

}

void shoot_fireball::use()
{
fireballSize ++;
    if(fireballSize >= 4)
    {
        fireballSize = 4;
    }
}

void shoot_fireball::release()
{
   // cout << last_pressed << endl;
     //             projectiles.push_back(unique_ptr<fireball>(new fireball(owner->rect->x + owner->rect->w/2 - fireballSize*8 , owner->rect->y + owner->rect->h/2 - fireballSize*8, owner->location, owner->face_side, owner->face_up, fireballSize*8 )));
       // cout<< "Size of loop: " << projectiles.size() << endl;
        fireballSize = 0;
       countdown = base_countdown;
}
class mage : public player
{
    public:
    mage(int x, int y, grid* where) : player(1, 20, x, y, 64, 64, 1, where, "sprites/players/sprote.png")
    {
        basic_attack = new shoot_fireball(this);
    }

};*/



#endif // PLAYERS_H_INCLUDED
