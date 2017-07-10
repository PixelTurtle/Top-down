#ifndef WINS_H_INCLUDED
#define WINS_H_INCLUDED

extern player* winner;
typedef bool(*winPtr)(int);
extern winPtr win;
extern const string defaultFont;
bool pause = false;

bool kills(int howMany)
{
    int size = level->players.size();
    for(unsigned int i = 0; i < size;i ++)
    {
        if (level->players[i]->killCount >= howMany)
        {
         //   cout << level->players[i]->killCount << endl;
            winner = level->players[i];
            return true;
        }
    }


    return false;
}
extern int SCREEN_HEIGHT, SCREEN_WIDTH;
void enterKills()
{

 announcement->changeDisplay(true);
    if (e.type == SDL_KEYDOWN && !e.key.repeat)
    {
     //   cout << maxKills<< endl;

        SDL_Keycode sym= e.key.keysym.sym;
        if ( sym<= SDLK_9 && sym >= SDLK_0)
        {
            int num = sym- SDLK_0;
            if (maxKills*10 + num <= 100)
            {
            maxKills = maxKills*10 + num;
          //  cout << s << endl;
            write(defaultFont, convert(maxKills), NULL, BLACK,announcement, 128, 490, 65, 64*findIntLength(maxKills), 64);
            }
           // SDL_RenderClear(render);
        }
        else if (sym == SDLK_BACKSPACE && !e.key.repeat)
        {
            maxKills = maxKills/10;
           // cout << maxKills << endl;
               write(defaultFont, convert(maxKills), NULL, BLACK,announcement, 128, 490, 65, 64*findIntLength(maxKills), 64);
        }
        else if (sym == SDLK_RETURN)
        {
            //cout<< " done" << endl;
            win = &kills;
            menu = choosing;
        }
    }
}
extern string name;

void enterName()
{

    if (name != "")
 write(defaultFont, name, NULL,BLACK, announcement, 64, SCREEN_WIDTH/2 - 32*name.length(), SCREEN_HEIGHT - 128, 64*name.length(), 64);
 announcement->changeDisplay(true);
                {
                    if (e.type == SDL_TEXTINPUT && !e.key.repeat )
                    {
                       // cout << e.text.text << endl;
                        name += *e.text.text;
                        write(defaultFont, name, NULL,BLACK, announcement, 64, SCREEN_WIDTH/2 - 32*name.length(), SCREEN_HEIGHT - 128, 64*name.length(), 64);
                    }

                    else if (e.type == SDL_KEYDOWN )
                    {
                        if (e.key.keysym.sym == SDLK_RETURN )
                        {
                        menu = title;
                        announcement->clear();
                        }
                        if (e.key.keysym.sym == SDLK_BACKSPACE && !e.key.repeat)
                        {
                            name =name.substr(0, name.length()-1);
                           // if (name!="")
                            write(defaultFont, name, NULL, BLACK,announcement, 64, SCREEN_WIDTH/2 - 32*name.length(), SCREEN_HEIGHT - 128, 64*name.length(), 64);
                        }
                    }

                }

}

struct winCondition
{

        winPtr howWin = &kills;
        void win(int x)
        {
            if (howWin(x))
            {
                pause = true;

            }
        }
};

#endif // WINS_H_INCLUDED
