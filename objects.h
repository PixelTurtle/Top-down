#ifndef OBJECTS_H_INCLUDED
#define OBJECTS_H_INCLUDED
#include <cstring>
#include <string>

using namespace std;

class character;
extern SDL_Renderer* render;
void do_damage(int how_much, character* target);







//cout << walmart-> x << " " << walmart->y << " " << walmart->h << " " << walmart->w << endl;


class grid;

extern grid* grasshill;

class object
{
    public:
        bool del = false;
        bool dead = false;
        bool solid = true;
        int respawn = 0;
  //  SDL_Rect* rect = new SDL_Rect;
    //SDL_Texture* sprite;
    Animation* sheet = nullptr;
  //  int spriteW, spriteH; //ints that hold the dimensions of the sprite
    grid* location;
    object(int x, int y, int w, int h,int portionWidth, int portionHeight,  grid* where, string sprite_location, int AnimationSpeed); // declared in arrays.h
    object(int x, int y, int w, int h, grid* where, string sprite_location, int AnimationSpeed); // declared in arrays.h
    virtual void collision(object* collider)
    {

    }
    virtual void everyTick()
    {

    }
    ~object()
    {
        delete sheet;
    }
};





#endif // OBJECTS_H_INCLUDED
