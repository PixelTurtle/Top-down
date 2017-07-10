#ifndef NPCS_H_INCLUDED
#define NPCS_H_INCLUDED

#include "weapons.h"
#include "players.h"
#include <math.h>
//#include "quests.h"
using namespace std;

//class player;

extern SDL_Rect* camera;


/*class NPC : public  character
{

    public:
bool resurgetRect();
vector<quest*> quest_queue;
    NPC(bool respawn_or_not, int healt, int x, int y, int w, int h, int veloc, grid* where, const char* sprite_location):character(healt, x, y, w,  h, veloc, where, sprite_location)
    {


         //   respawn = 1000;
            location->insert(*this);
            resurgetRect() = respawn_or_not;


    }
    void give_quest(quest* dobject, player* target);

   virtual void hand_out_quest()
   {

   }

};*/

void projectile::collision(character* target)
    {
//     cout << "Asdf" << endl;
if (target != NULL  )
     target->takeDamage(damage, *owner);
//if (target!= owner)
//cout << "A " << endl;
        del = true;
    }

void projectile::movement()
{
            rectX += speed*x_dir;
            rectY += speed*y_dir;
           sheet->changeRect(rectX, rectY);


}

void flip(int &a, int &b) // switches the values of two ints
{
    int c = a;
    int d = b;
    a = d;
    b = c;
   // cout << *a << endl;
}





void Path(SDL_Rect* a, SDL_Rect* b, int diff)
{

}



 int character::walk(double x_dir, double y_dir)
    {

        bool xStick = false;
        bool yStick = false;
       // cout << location->getIndex(*this) << endl;
       vector<shared_ptr<object>>* temp = (location->retrieve(*this));
        vector<object*> here;
        int size = temp->size();
        for (unsigned int it = 0; it < size; it++)
        {
            here.push_back(temp->at(it).get());
        }
     //   cout << here.size() << endl;
int addX,  addY;
      addX = convertTo1(x_dir);
      addY = convertTo1(y_dir);


            SDL_Rect intersectX = {rectX + x_dir, rectY, sheet->getRect()->w, sheet->getRect()->h};
            SDL_Rect intersectY = {rectX , rectY + y_dir, sheet->getRect()->w, sheet->getRect()->h};
            SDL_Rect intersect = {rectX, rectY, sheet->getRect()->w, sheet->getRect()->h};
            bool outX = (intersectX.x < 0 || intersectX.x+ intersectX.w > level->width ); //if the destination is out of the level's bounds
            bool outY = (intersectY.y < 0 || intersectY.y + intersectY.h > level->height);
            if (outX || outY)
            {
                if (outX)
                {
            x_dir = 0;
            //cout << addX << endl;
            sheet->getRect()->x = level->width*(.5+.5*addX) - sheet->getRect()->w*(.5+.5*addX);
                }
                else
                {
                y_dir = 0;
                sheet->getRect()->y= level->height*(.5+.5*addY) - sheet->getRect()->h*(.5+.5*addY);
                }
              //  cout << getRect()->x << endl;
            }
            else
            {
                       int size = here.size();
        for ( int x = 0; x <size; x++)
        {
                SDL_Rect hereRectX = SDL_Rect{here[x]->sheet->getRect()->x - addX, here[x]->sheet->getRect()->y , here[x]->sheet->getRect()->w, here[x]->sheet->getRect()->h};
                SDL_Rect hereRectY = SDL_Rect{here[x]->sheet->getRect()->x , here[x]->sheet->getRect()->y - addY, here[x]->sheet->getRect()->w, here[x]->sheet->getRect()->h};
                     string resultX = RectIntersect(intersectX,hereRectX,addX, 0);
                     string resultY = RectIntersect(intersectY,hereRectY, 0,addY);
                     //cout << RectIntersect(&hereRectX, &intersectX, 1) << endl;
                   //  cout << resultX << " " << resultY << endl;
            if ( (resultX != "none" || resultY != "none"))
            {

                if (here[x]->solid)
                {
               // cout << x_dir << " " << y_dir << endl;

                //    cout << convertTo1(x_dir) << endl;
                  //  cout << "2. " + result << endl;
                  //  SDL_Rect* hereRect = here[x]->sheet->getRect();
                    if (resultX == "horiz")
                    {
                        x_dir= (abs(addX)*(hereRectX.x + (.5-.5*addX)*hereRectX.w - (.5+.5*addX)*sheet->getRect()->w - rectX ))/speed;
                        xStick = true;
                //        rectX = (hereRect->x + hereRect->w*(.5-.5*addX) - addX);
                    }
                   if (resultY == "vert")
                    {
                   //     cout << sheet->getRect()->y << endl;
                       // cout << sheet->getRect()->x << endl;
                        y_dir = (abs(addY)*(hereRectY.y + (.5-.5*addY)*hereRectY.h - (.5+.5*addY)*sheet->getRect()->h  - rectY))/speed;
                        yStick = true;
                      //  rectY = hereRect->y + hereRect->h*(.5-.5*addY);
                    }
                  /*  else if (result == "diag")
                    {

                                                x_dir= (abs(addX)*(hereRect->x + (.5-.5*addX)*hereRect->w - (.5+.5*addX)*sheet->getRect()->w - rectX))/speed;
                        xStick = true;
                        yStick = true;
                        y_dir = (abs(addY)*(hereRect->y + (.5-.5*addY)*hereRect->h - (.5+.5*addY)*sheet->getRect()->h  - rectY))/speed;

                    }*/
                  //   cout << "3. " + convert(sheet->getRect()->y) << endl;
                }
                here[x]->collision(this);
            }
        }
            }

           // if (!xStick)
            rectX += x_dir*speed;

            //if (!yStick)
            rectY += y_dir*speed;


             sheet->getRect()->x = rectX;
             sheet->getRect()->y = rectY;
/*if (getRect()->x < location->x)
{
    if (locationIndex > 0)
    location = level->areas[locationIndex--];
}
else if (getRect()->x > location->x + location->width)
{
    if (locationIndex < level->areas.size())
    {
        location = level->Areas[locationIndex ++];
    }
}
if (getRect()->y < location->y)
{
    if (locationIndex > 0)
        location = level->areas[]
}*/
//cout << getRect()->x << " " << camera->x << endl;
    }









#endif // NPCS_H_INCLUDED
