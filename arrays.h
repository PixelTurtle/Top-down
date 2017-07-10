#ifndef ARRAYS_H_INCLUDED
#define ARRAYS_H_INCLUDED

#include "objects.h"

//#include "NPCS.h"
#include <vector>

class NPC;
class player;

class grid
{
    public:
        int w, h,maximum = 10, x, y;
   vector<shared_ptr<object>> plot;
   grid* nodes[4];
    grid(int xCoord, int yCoord, int width = 640, int height = 640)
    {
        x = xCoord;
        y = yCoord;
        w = width;
        h = height;
        nodes[0] = NULL;
        nodes[1] = NULL;
        nodes[2] = NULL;
        nodes[3] = NULL;
    //    cout << plot.size() << endl;
    }
    int getIndex(object& obj)
    {
        if (nodes[0] != NULL)
        {
            int vertMid = x + w/2;
            int horizMid = y + h/2;
            SDL_Rect* rect = obj.sheet->getRect();
            if (rect->y <= horizMid)
            {
                if (rect->x <= vertMid)
                    return 0;
                else
                    return 1;
            }
            else
            {
                if (rect->x <= vertMid)
                    return 2;
                else
                    return 3;
            }
        }
        return -1;
    }
    void split()
    {
        nodes[0] = new grid(x, y, w/2, h/2);
        nodes[1] = new grid(x + w/2, y , w/2, h/2);
        nodes[2] = new grid(x, y + h/2, w/2, h/2);
        nodes[3] = new grid(x + w/2, y + h/2, w/2, h/2);
                int howBig = plot.size();
        for (int i = 0; i < howBig; ++i)
        {
            SDL_Rect* ptr = plot[i]->sheet->getRect();
            bool left = ( ptr->x <= x+ w/2);
             bool right=  (ptr->x + ptr->w>= x + w/2);
            if (ptr->y <= y + h/2)
            {
               // if (!(left && right))
                {
                if(left);
                nodes[0]->plot.push_back(plot[i]);
                if (right)
                    nodes[1]->plot.push_back(plot[i]);
                }
            }
          else if (ptr->y + ptr->h  >= y + h/2)
           {

               if (left)
                nodes[2]->plot.push_back(plot[i]);
                if (right)
                nodes[3]->plot.push_back(plot[i]);

           }
        }
    }
    vector<shared_ptr<object>>* retrieve(object& target) //returns a vector that contains all the objects that can be collided with
    {
        vector<shared_ptr<object>>* found =&this->plot;
        if (getIndex(target) != -1 )
        {
           found = nodes[getIndex(target)]->retrieve(target);
        }
        return found;
    }
    void insert(object& obj)
    {
        if (nodes[0] != NULL)
        {
            int index = getIndex(obj);
        if (index != -1)
        {
            nodes[index]->insert(obj);
                    return;
        }
        }
        plot.push_back(shared_ptr<object>(&obj));
        if (plot.size() > maximum)
        {
            split();
        }
    }
    void clear()
    {

    }

    void reset()
    {
            cout << "deleting grid..." << endl;
       plot.clear();
        if (nodes[0] != NULL)
        {
            delete nodes[0];
            nodes[0] = NULL;
            delete nodes[1];
            nodes[1] = NULL;
            delete nodes[2];
            nodes[2] = NULL;
            delete nodes[3];
            nodes[3] = NULL;
        }
    }
    ~grid()
    {
        cout << "deleting grid..." << endl;
        plot.clear();
        if (nodes[0] != NULL)
        {
            delete nodes[0];
            nodes[0] = NULL;
            delete nodes[1];
            nodes[1] = NULL;
            delete nodes[2];
            nodes[2] = NULL;
            delete nodes[3];
            nodes[3] = NULL;
        }
        cout << "Plot size: " << plot.size() << endl;

    }
};
struct stage;
bool load(string where, stage* location);
struct stage
{
private:
   SDL_Texture*  bg = NULL;
    public:
    grid parent {0,0};
    vector <player*> players;
    int width = 1280, height = 1280;
    string levelLocation = "";
      stage(string where)
    {
         //  areas.push_back(unique_ptr<grid>(new grid));
        load(where, this);

    }
    stage()
    {

    }
    ~stage()
    {
        SDL_DestroyTexture(bg);
    }
    bool loadLevel(string where)
    {
        levelLocation = where;
      return load(where, this);
    }
    void resetPlayers()
    {
        players.clear();
    }
    void resetLevel()
    {
        parent.reset();
    }
    void reset()
    {
        resetLevel();
        resetPlayers();
       // cout << players.size() << endl;
    }





        SDL_Texture* getBg()
    {
        return bg;
    }
    void changeBg(string sprite_location)
    {
      //  cout << sprite_location << endl;
        SDL_Surface* s = IMG_Load(sprite_location.c_str());
       //SDL_Surface* s = IMG_Load("sprites/bg/debug.png");
        if (s == NULL)
        {
            cout << "error changing bg: " + sprite_location + " " + IMG_GetError() << endl;
         }
         else
         {

             bg = SDL_CreateTextureFromSurface(render, s);
             // bg = SDL_CreateTextureFromSurface(render, s);
         }
         SDL_FreeSurface(s);
    }

} ;

object::object(int x, int y, int w, int h,int portionWidth, int portionHeight,  grid* where, string sprite_location, int AnimationSpeed = 100) : object(x,y,w,h, where, sprite_location, AnimationSpeed)
    {

     sheet->changePortion({0,0,portionWidth, portionHeight});
    }

    object::object(int x, int y, int w, int h, grid* where, string sprite_location, int AnimationSpeed = 100) // declared in arrays.h
    {
        //cout << sprite_location << endl;
   //  sheet = new Animation();
            location = where;
        del = false;
sheet = new Animation(sprite_location, {0,0,0,0}, {x,y,w,h}, AnimationSpeed);
sheet->changePortion({0,0,sheet->spriteW, sheet->spriteH});
     ((Animation*)(sheet))->changeStopped(true);

     //   cout << sheet->getPortion()->w << endl;
    }



#endif // ARRAYS_H_INCLUDED
