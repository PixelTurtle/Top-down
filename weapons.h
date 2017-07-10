#ifndef WEAPONS_H_INCLUDED
#define WEAPONS_H_INCLUDED
#include "players.h"
#include <type_traits>
//extern SDL_Rect* camera;

enum weaponNames
{
    GUN,
    LASERGUN,
    SPREADER,
    SHARK,
    PERSECUTED,
    maximumWeapons
};



extern vector<paper> shelf;

extern int SCREEN_HEIGHT;
struct weapon : public object
{

     // the sheet->getRect() the sprite is blitted to
    SDL_Rect* hit_box = new SDL_Rect; // the actual hitbox
    int base_w, base_h; // the default width and height

    bool sheathed = false;
    bool isOnGround = true, infinite = true;

    protected:
    int how_long;
    int damage;
    int cost = 10;

    player* owner = nullptr;
    string whereSprite = "";
    SDL_Point shootFrom; //the point where bullets are shot from
    weaponNames name = GUN;


    public:
    weapon( int x, int y, int w, int h,   int ouch, player* wielder,  grid* where,char* sprite_location ,bool ground = true, bool lasts = true) : object(x, y, w, h, where, sprite_location)
    {
        solid = false;
        whereSprite = sprite_location;
        owner = wielder;
        if (owner != NULL)
        {
            resetRect();
        }
       // cout << sheet->getRect()->x << endl;
        hit_box->w = w;
        hit_box->h = h;

        if  (where == NULL)
        {
            if (owner != NULL)
            location = owner->location;
            else
                location = NULL;
        }
        else
             location = where;
        base_w = w;
        base_h = h;
        damage = ouch;
     //   ammoCount.reset(new Paper({0,4*SCREEN_HEIGHT/5,128,64}, defaultFont,"Ammo: " + convert(ammo) ));
 //       ammoCount->changeText(*write(defaultFont, "Ammo: " + convert(ammo), NULL, {0,0,0}, 64, render));
    }
    virtual void show(int x, int y, double degrees)
    {
        SDL_Rect blit = {x, y, sheet->getRect()->w, sheet->getRect()->h};
        sheet->show(render, NULL, &blit, degrees);

       // ammoCount->show();
    }
    void resetRect()
    {
      // std::cout << owner->angle << std::endl;
      if (owner != nullptr)
      {
              SDL_Rect* ownerRect = owner->sheet->getRect();
            SDL_Rect* rect = sheet->getRect();

        //    int orient = rect->w * cos(owner->angle) + rect->h * sin(owner->angle); //an int that renders the weapon's position at the edge of the owner sprite.
        sheet->changeRect( ownerRect->x + ownerRect->w * max((cos(owner->angle)),0.0) + rect->w*min(cos(owner->angle), 0.0) , ownerRect->y + ownerRect->h * std::max(sin(owner->angle),0.0) + rect->h*std::min(sin(owner->angle), 0.0));
                shootFrom = {sheet->getRect()->x + sheet->getRect()->w*cos(owner->angle), sheet->getRect()->y +sheet->getRect()->h*sin(owner->angle)};

       //cout << rect->x << ownerRect->x << endl;
       // sheet->changeRect(rect->x + ownerRect->w/2 - rect->w/2 + ownerRect->w/2 * cos(owner->angle),ownerRect->y + ownerRect->h/2 - rect->h/2 + ownerRect->w/2 * sin(owner->angle), ownerRect->w/2 , ownerRect->h/2);
      }
    }
   virtual string  use()
    {

        return "";
    }
    virtual weaponNames serialize()
    {
        return name;
    }
   virtual void collision(object* collider)
   {
      player* j=  dynamic_cast<player*>(collider);
      if (owner == NULL && j )
      {
    /*  if (infinite = false)
      {
        j->hand.reset(this);
        duration = maxDuration;
      }
      else
      {
          if (j->hand.get()->name != name)
        j->hand.reset(this);
      }*/


      j->equip(this);

      }
   }
   virtual bool shootable()
   {
       return true;
   }
   std::string getSpriteLocation()
   {
       return whereSprite;
   }
   int getCost()
   {
       return cost;
   }
   weaponNames getName()
   {
       return name;
   }
   bool equals(weapon& other)
   {
       return other.name == name;
   }
   bool equals(weaponNames otherName)
   {
       return name = otherName;
   }
   player* getOwner()
   {
       return owner;
   }
   void changeOwner(player* newOwner)
   {
       owner = newOwner;
   }
};

class RangedWeapon : public weapon
{
private:
            int ammo = 0;
        unique_ptr<Paper> ammoCount = nullptr;
    virtual void removeAmmo(int howMuch = 1)
    {
        ammo -= howMuch;
    }
    virtual void showAmmo()
    {
        ammoCount->changeText(defaultFont, "Ammo: " + convert(ammo), NULL);
        ammoCount->show();
    }

public:
    RangedWeapon(int x, int y, int w, int h, int howMuchAmmo, int ouch, player* wielder,  grid* where,char* sprite_location, bool ground = true, bool lasts = true) : weapon(x,y,w,h,ouch, wielder, where, sprite_location, ground, lasts)
    {
        ammo = howMuchAmmo;
        ammoCount.reset(new Paper({0, 5*SCREEN_HEIGHT/6, 128, 64}, defaultFont, "Ammo : " + convert(ammo)));
    }
    virtual void show(int x, int y, double degrees)
    {
        weapon::show(x, y, degrees);
    //    showAmmo();
    }
    void show(double degrees)
    {

    }

};

template <typename T>
struct weaponBlock : public object //holds weapons to get
{
  weapon* pic = dynamic_cast<weapon*>(new T(NULL));
  object* last = NULL;
    weaponBlock(int x, int y, grid* where) : object(x, y, 64, 64, where, "none" )
    {
        solid = false;


        if (!sheet->changeText( pic->getSpriteLocation().c_str()))
        {
            cout << "Error loading WeaponBlock sprite(" << pic->getSpriteLocation() <<"): " << IMG_GetError() << endl;
        }
    }
    void collision(object* target)
    {
        if (last != target)
        {
            last = target;
        player* p = static_cast<player*>(target);
       // if (p)
        {
          //  if (p->hand.get()->name != pic->name )
            p->equip(new T(NULL));
        }
        }
    }
};

struct hand_weapon : public weapon
{
    hand_weapon(int x, int y, int w, int h,  int ouch,player* wielder, grid* where, char* sprite_location ) : weapon(x, y , w, h,  ouch,wielder, where,sprite_location )
    {

    }
   /* void use()
    {
         /*  {
        how_long = SDL_GetTicks() + duration;
        sheathed = false;

            hit_box->w = base_h*(abs(owner->face_side)) + base_w*(abs(owner->face_up));
            hit_box->h = base_w*(abs(owner->face_side)) + base_h*(abs(owner->face_up)) ;
            //cout << sheet->getRect()->w ;
           sheet->getRect()->x = owner->sheet->getRect()->x + owner->sheet->getRect()->w*(.5+.5*owner->face_side )  + abs(owner->face_side)*((owner->face_side)*sheet->getRect()->h/2 - sheet->getRect()->w/2) - (abs(owner->face_up)*(sheet->getRect()->w/2)) ;
            //cout << sheet->getRect()->x <<  " " << sheet->getRect()->y  << " " << sheet->getRect()->x + sheet->getRect()->w << " "<< sheet->getRect()->y + sheet->getRect()->h << endl;
            sheet->getRect()->y = owner->sheet->getRect()->y -sheet->getRect()->h*(.5 + .5*-owner->face_up)*abs(owner->face_up) + owner->sheet->getRect()->h*(.5 - .5*-owner->face_up)*abs(owner->face_up) ;
            hit_box->x = owner->sheet->getRect()->x  + owner->sheet->getRect()->w*((.5+.5*owner->face_side)) - hit_box->w*(.5 - .5*owner->face_side) ;
            hit_box->y = owner->sheet->getRect()->y + owner->sheet->getRect()->h*(.5 - .5*-owner->face_up)  - hit_box->h*(.5 + .5*-owner->face_up);
        }
        for (int i = 0; i < owner->location->citizens.size(); i ++)
        {
            if ( owner->location->citizens[i] != NULL)
            {
               if (owner->location->citizens[i]->dead == false )
            {
                  if (SDL_HasIntersection(hit_box, owner->location->citizens[i]->sheet->getRect()) )
            {
                //cout << "DETECT" << endl;
                do_damage(damage, owner->location->citizens[i]);
            }
            }
            }


        }*/

};






struct gun : public RangedWeapon
{
    gun( player* wielder, int x = 0, int y = 0,grid* where = NULL): RangedWeapon( x, y, 32, 32, 100, 1, wielder, where, "sprites/weapon/gun.png")
    {
        name = GUN;


    }

   string use()
    {
     //   ammo --;
      //  cout << "a" << endl;
        projectiles.push_back(unique_ptr<projectile>( new bullet(shootFrom.x, shootFrom.y,  owner->location, cos(owner->angle), sin(owner->angle),owner)));
        return projectiles.back().get()->serialize();
    }
  /*  void collision(object* collider )
    {
        player* j = dynamic_cast<player*>(collider);
        if (j )
        {
            j->equip(new gun(j));
        }
    }*/

};
struct laserGun :public RangedWeapon
{
    laserGun( player* wielder, int x = 0, int y = 0,grid* where = NULL): RangedWeapon(x, y, 32, 20, 200, 1, wielder, where, "sprites/weapon/lasergun.png")
    {
        name = LASERGUN;

    }
    string use()
    {
               projectiles.push_back(unique_ptr<projectile>( new laser(shootFrom.x,shootFrom.y, owner->location, cos(owner->angle), sin(owner->angle), owner)));
               return projectiles.back().get()->serialize();
    }
};
struct spreader: public RangedWeapon
{
    spreader(player* wielder, int x = 0, int y = 0, grid* where = NULL): RangedWeapon(x, y, 64, 40, 100, 1, wielder, where, "sprites/weapon/spreader.png")
    {
        name = SPREADER;
    }
    string use()
    {
        bullet* a = new bullet(shootFrom.x, shootFrom.y, owner->location, cos(owner->angle + M_PI/12), sin(owner->angle + M_PI/12) , owner);
        bullet* b = new bullet(shootFrom.x, shootFrom.y, owner->location, cos(owner->angle), sin(owner->angle), owner);
        bullet* c = new bullet(shootFrom.x, shootFrom.y, owner->location, cos(owner->angle - M_PI/12), sin(owner->angle - M_PI/12), owner);
        projectiles.push_back(unique_ptr<projectile>(a));
        projectiles.push_back(unique_ptr<projectile>(b));
        projectiles.push_back(unique_ptr<projectile>(c));
        return a->serialize() + "@" + b->serialize() + "@" + c->serialize() ;
    }
};

struct fireShark : public RangedWeapon
{
    fireShark(player* wielder, int x = 0, int y = 0, grid* where = NULL): RangedWeapon(x, y, 64, 40, 5, 1, wielder, where, "sprites/weapon/bazooka.png" )
    {
        name = SHARK;
    }
    string use()
    {
        projectiles.push_back(unique_ptr<projectile>(new fireRocket(shootFrom.x, shootFrom.y, owner->location, cos(owner->angle), sin(owner->angle), owner)));
        return projectiles.back().get()->serialize();
    }
};

struct persecuted : public weapon
{
    persecuted(player* wielder, int x = 0, int y = 0, grid* where = NULL) : weapon(x, y, 64, 40, 1, wielder, where, "sprites/weapon/flamethrower.png")
    {
        name = PERSECUTED;
    }
    string use()
    {

    }
};

extern button* selector;
weapon* loadGun(int whichWeapon, player* wielder);

class weaponWrapper // a lightweight class used to represent weapons using only a sprite and a number
{
    protected:
    paper sprite;
    weaponNames weapName = GUN;
public:
    weaponWrapper(int weapN)
    {
        weapName = (weaponNames)weapN;
        weapon* w = loadGun(weapName, NULL);
        if (!w)
        {
            cout << "ERROR: error loading weapon in weaponWrapper: " << weapName << endl;
            return;
        }
      //  cout << w->whereSprite << endl;
        sprite.changeText(w->getSpriteLocation());
       // cout << sprite.getSpriteLocation() << endl;
        delete w;
    }
    paper& getSprite()
    {
        return sprite;
    }
    weaponNames getWeapName()
    {
        return weapName;
    }
};


class shopWeaponWrapper : public weaponWrapper
{
    int cost = 8000;
public:
    shopWeaponWrapper(int weapN) : weaponWrapper(0)
    {
         weapName = (weaponNames)weapN;
        weapon* w = loadGun(weapName, NULL);
        if (!w)
        {
            cout << "ERROR: error loading weapon in weaponWrapper: " << weapName << endl;
            return;
        }
      //  cout << w->whereSprite << endl;
        sprite.changeText(w->getSpriteLocation());
        cost = w->getCost();
       // cout << sprite.getSpriteLocation() << endl;
        delete w;
    }
    int getCost()
    {
        return cost;
    }

};

class weaponButton;
extern weaponButton* inventory[3];
extern screen* weaponsScreen;
extern const int invSize;
   extern  const string defaultFont;
   extern paper lockSprite;
   extern paper coin;
   bool payUp(int cost);
class weaponButton : public button
{
    bool selected;
        shopWeaponWrapper* weapPtr = nullptr;
        bool isLocked;
    public:

    bool inInventory = false;

    weaponButton(int x, int y,   shopWeaponWrapper* weap, bool in, bool locked) : button(x, y, 128, 64, "", NULL)
    {
                        isLocked = locked;
              inInventory = in;
        if (weap)
        {
        weapPtr = (weap);
  //    changeTexture(sheet->,weapPtr->getSprite().getSpriteLocation().c_str());
  sheet->changeText(weapPtr->getSprite().getSpriteLocation().c_str());

        }
        else
        {
           // changeTexture(sprite, "sprites/buttons/weaponSelect.png");
            sheet->changeText("sprites/buttons/weaponSelect.png");
        }
      if (!sheet->isTextValid())
      {
          cout << "SD" << SDL_GetError() << IMG_GetError() << endl;;
      }
    }
    void changeWeapPtr(shopWeaponWrapper* weap)
    {
          weapPtr = (weap);
        if (weap)
        {

       sheet->changeText(weapPtr->getSprite().getSpriteLocation().c_str());
//      inInventory = in;
        }
        else
        {
             sheet->changeText( "sprites/buttons/weaponSelect.png");
        }
    }
    void function()
    {
        selected = true;
      //  cout << inInventory << endl;
      if (weapPtr != nullptr)
      {
        if (!isLocked)
        {
            // cout << "A " << endl;
           if (inInventory)
           {

             //   cout << weapPtr->getWeapName() << endl;
               weaponButton* butPtr = reinterpret_cast<weaponButton*>(weaponsScreen->buttons->at(weapPtr->getWeapName() + 1));
              // if (butPtr)
           //   cout << weapPtr->getWeapName() << endl;
               {
                 //  weaponScreen->buttons->push_back
                   butPtr->changeWeapPtr( weapPtr);
                                changeWeapPtr(nullptr);
                   // inInventory = false;
               }

           }
           else
           {
               for (int i = 0; i < invSize; i ++)
               {
                    if ( inventory[i]->weapPtr == NULL)
                    {
                   //     cout << "yest" << endl;
                        inventory[i]->changeWeapPtr(weapPtr) ;
                        changeWeapPtr(NULL);
                       // inInventory = true;
                        break;
                    }
               }
           }
        }
        else
        {
            if(payUp(weapPtr->getCost()))
            {
                isLocked = false;
            }
        }
      }
    }
    void during()
    {
        if (isLocked)
        {
            const int heightBelowButton = (80- 32)/2; // 80 is half the vertical distance between each item. 32 is the height of each item
            lockSprite.changeRect({sheet->getRect()->x + sheet->getRect()->w/2, sheet->getRect()->y + sheet->getRect()->h/2, sheet->getRect()->w/2, sheet->getRect()->h/2});
            lockSprite.show();
            paper p ({0, 0, 32, 64}, defaultFont, convert(weapPtr->getCost()), NULL, {255,255,255} );
            p.show(sheet->getRect()->x + sheet->getRect()->w/2, sheet->getRect()->y + sheet->getRect()->h + heightBelowButton);
            coin.show(sheet->getRect()->x + sheet->getRect()->w/2 - 64, sheet->getRect()->y + sheet->getRect()->h+ heightBelowButton);
        }
    }
    weaponWrapper* getWeapPtr()
    {
        return weapPtr;
    }
    int getWeaponName()
    {
        if (weapPtr != nullptr)
        {
            return weapPtr->getWeapName();
        }
        else
        {
            return -1;
        }
    }
    ~weaponButton()
    {
        delete weapPtr;
    }
};


extern bool armory[maximumWeapons];

extern int pageNumber;

vector<button*>& showCollection()
{
    vector<button*>* vec = new vector<button*>;
    const int perRow = 3; //an int denoting how many weapons per row
    const int space = SCREEN_HEIGHT/10; //an int denoting the space between each weapon
    const int weaponsPerPage = 6;
    for (int i = 0; i < weaponsPerPage; i ++)
    {
        if (i >= maximumWeapons-1 )
        {
            break;
        }
        vec->push_back(new weaponButton(space*((i%perRow)*perRow) + space, SCREEN_HEIGHT/2 +SCREEN_HEIGHT/4*(i/perRow),   new shopWeaponWrapper(i+1 + pageNumber), false,!armory[i+1]));
     /*   if (i < invSize)
        {

        }*/

      /*  if (!armory[i])
        {
            SDL_SetTextureColorMod(vec->at(i)->sprite, 0, 0, 0);
        }*/
    }
    return *vec;
}

    player::player(int portNumber, string iName,int heal, int x, int y,  const char* sprite_location, weapon* weap =nullptr): character(heal, x,y,64,64,64,64, player::baseSpeed,&level->parent, sprite_location)
    {
        //cout << "Sprite: " << sprite_location << endl;
        name = iName;
      //  level->players.push_back(this);
      //   std::fill_n(inventory.begin(), invSize, shared_ptr<weapon>(nullptr));

            weapons.fill(nullptr);
        equip(weap);
      /*  if (hand == nullptr)
        {
            cout << "WNU" << endl;
            throw logic_error("hand is null");
        }*/
     //   hand.get()->owner = this;

        port = portNumber;
        spriteLocation = sprite_location;
       if ( sheet->getText() == nullptr)
        {
            throw std::logic_error("");
        }
         SDL_Rect* rect = sheet->getRect();
        healthBar.changeRect(rect->x, rect->y - 10, rect->w, 5 );
        healthBar.changeText("sprites/players/healthBar.png");
        if (healthBar.getRect()->w > 100 )
        {
            throw std::logic_error("");
        }
     //   equip(loadGun(inventory.get(0)->getWeaponName(),this));

    };

    player::player(int portNumber, string iName = "name") : player(portNumber, iName, 20, 0,0,player::defaultSpriteLocation.c_str() )
    {

       spawn(level->parent);

    }
    weapon* loadGun(int whichWeapon, player* wielder);
    extern const int serializedLength;
    player::player(string* arr) : character(20, 1280, 1280, 64, 64, 64,64,player::baseSpeed, &level->parent, player::defaultSpriteLocation.c_str()) // this constructor deserializes a player
    {
        port = convert(arr[0]);
        name = (arr[1]);
        health = convert(arr[2]);
        sheet->getRect()->x = convert(arr[3]);
        sheet->getRect()->y = convert(arr[4]);
        rectX = sheet->getRect()->x;
        rectY = sheet->getRect()->y;
        angle = convert(arr[5]);
        spriteLocation = arr[6];
        sheet->changeText(spriteLocation);
        equip(loadGun(convert(arr[7]), this));
     //   cout << "Hand name:" << hand->serialize() << endl;
            SDL_Rect* rect = sheet->getRect();
    healthBar.changeRect(rect->x, rect->y - 10, rect->w, 5 );
    healthBar.changeText("sprites/players/healthBar.png");
      //  level->players.push_back(this);
                if (level->players.size() > 2)
        {
            throw std::logic_error("");
        }
    }
        string player::serialize()
    {
     //   cout <<"Port: " << port << endl;
    // std::cout << convert(rectX) +  " " + convert(rectY);
       return  convert(port)+ " " +name + " " + convert(health) + " " + convert(rectX) +  " " + convert(rectY) +  " " +convert(angle) +   " " +  spriteLocation + " "  +convert(hand->serialize())+ " " + convert(locationIndex);
    }
       void player::update(int newHealth, double x, double y, double radian, weapon* newWeapon)
    {
        health = newHealth;
        sheet->getRect()->x = x;
        sheet->getRect()->y = y;
        rectX = x ;
        rectY = y ;
        angle = radian;
        if (newWeapon != NULL )
        equip(newWeapon);
     /*   weapon* ptr = hand;
        if(ptr != NULL)
        {
           ptr->resetRect();
        }*/

    }
    void player::update(string* arr)
    {
        port = convert(arr[0]);
   /*     name = (arr[1]);
        health = convert(arr[2]);
        SDL_Rect* rect = sheet->getRect();
        rect->x = convert(arr[3]);
        rect->y = convert(arr[4]);
        angle = convert(arr[5]);*/
        int newHealth = convert(arr[2]);
        double newX = convert(arr[3]);
        double newY = convert(arr[4]);
        double radian = convert(arr[5]);
        weapon* newWeapon = NULL;
        int weap = convert(arr[7]);
        if (weap != hand->getName())
        {
            newWeapon = loadGun(weap, this);
        }
        update(newHealth, newX, newY,radian, newWeapon);
     //   spriteLocation = arr[6];
        //SDL_DestroyTexture(sprite);
      //  sheet->changeText(spriteLocation.c_str());
     /*   if (hand)
        {
            int weap = convert(arr[7]);
            if (weap != hand->name)
            {
                 weapon* current = loadGun(weap, this);
                equip(current);
            }
               hand->resetRect();
          //  cout <<SDL_GetTicks()<< endl;
        }*/
    }
    void player::show()
    {
                        hand->resetRect();
                        SDL_Rect* rec= sheet->getRect();
                          SDL_Rect r = {rec->x - camera->x, rec->y - camera->y, rec->w, rec->h};
                       //    cout << sheet->getSpriteLocation()<< endl;
                     //  SDL_RenderCopy(render, sheet->getText(),NULL,sheet->getRect());
                        SDL_Rect* handRec = hand->sheet->getRect();
                        sheet->show(render, sheet->getPortion(),&r, angle*(180/M_PI));
                    //     hand->sheet->show(render,NULL, &g,angle*(180/M_PI) );
                         hand->show(handRec->x - camera->x, handRec->y - camera->y, angle*(180/M_PI));
                         if (health > 0)
                         {
                            SDL_Rect* healthRect = healthBar.getRect();
                            healthRect->x = rec->x;
                            healthRect->y = rec->y;
                           SDL_Rect h = {healthRect->x - camera->x, healthRect->y - camera->y, healthRect->w*(((double)health/max_health)), healthRect->h};
                             healthBar.show(render,NULL, &h);
                         }
    }
    void player::equip(weapon* w)
    {
 //       cout <<"x:" <<  w->sheet->getRect()->x << endl;
        if ((hand != NULL  && w->getName() == hand->getName()) ||  w == NULL)
                {
                    //cout << w->name << endl;
                    return;
                }

        int x = 0;
        hand = w;
        for ( x = 0; x < invSize; ++x)
        {
            if (weapons.get(x).get() && w->equals(*weapons.get(x).get()))
                {
                    return;
                }
        }

 //cout << w->name << endl;
      hand->changeOwner(this);

  //arrPushBack<shared_ptr<weapon>, invSize>(inventory, shared_ptr<weapon>(nullptr), shared_ptr<weapon>(w));
       // cout << "Name: " << hand.get()->name << endl;

    }
    void justDied();

     void player::die()
    {
        isDead = false;
       spawn(level->parent);
        health = max_health;
     //   int g;
    //    int
   /*     for (unsigned int i = 0; i < inventory.size(); i ++)
        {
            weapon* ptr = inventory[i].get();
            if (ptr->name != GUN)
            {
                inventory.erase(inventory.begin() + i);
            }
            else
                g = i;
        }
        equip(inventory[g].get());
       // justDied();*/
    }





#endif // WEAPONS_H_INCLUDED
