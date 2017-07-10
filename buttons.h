#ifndef BUTTONS_H_INCLUDED
#define BUTTONS_H_INCLUDED


extern stage* level;
extern server* s;
extern client* c;
extern Effect* announcement;
extern screen* currentScreen;
extern SDL_Event e;
extern bool isHost;
enum menus
{
    title,
    playing,
    joining,
    choosing,
    chooseKills,
    modSelf,
    connecting,
    buying,
    choosingWeapons
};

extern menus menu;
void changeMenu(menus m);


void join()
{
    isHost = false;
    menu = connecting;
}
extern int maxKills;
extern string objective;
extern const string defaultFont;

void host()
{

   // currentScreen = &killAmount;
isHost = true;
    changeMenu( chooseKills);
//     write(defaultFont, convert(maxKills), NULL, announcement, 128, 490, 65, 64*findIntLength(maxKills), 64);

}

void gotoMenu()
{
    changeMenu(title);
}

void changeSelf()
{
    changeMenu(modSelf);
}
void reset(bool announceClear , bool levelClear, bool playerClear, bool serverClear, bool clientClear, bool  projectilesClear );

struct levelButton : public button
{
    levelButton (int x, int y, int w, int h,std::string sprite_location, SDL_Renderer* see = render) : button(x, y ,w , h, sprite_location,NULL, see )
    {

    }
    void function()
    {
           string b =  this->spriteLocation;
            string j = "sprites/";
        int l = (j).length();
        string location =b.substr(l, b.length() - l - 4 ) + ".txt";
       // cout << location << endl;
       level->levelLocation = location;
        menu = playing;
    }
};

class typeButton;
extern typeButton* currentType;

class selectButton : public button // a class that represents a button that does something while selected
{
    protected:
    bool chosen = false;
    functionWrapper during = NULL;
    public:
        selectButton** ptr = NULL;
    selectButton(int x, int y, int w, int h, selectButton*& selector, std::string font, std::string toWrite, int r, int g, int b, const SDL_Color& fontColor, functionWrapper func, functionWrapper dur ) : button(x, y, w, h, font, toWrite,r, g, b,fontColor, func  )
    {
        ptr = &selector;
        during = dur;
    }
    virtual void function()
    {
        if (use)
        {
            *ptr = this;
        }
    }
    virtual void whileChosen()
    {
        if (during)
        {
            (*during)();
        }
    }
    bool getChosen()
    {
        return chosen;
    }
};


extern SDL_Event e;
class typeButton : public button //a button structure that lets you type things (ex: type in a name)
{
        protected:
    string* modify = NULL; //the string that this button mods
string permaBit = "";
    public:
    typeButton(int x, int y, int w, int h, string* modifier, string perma) : button(x, y, w, h, "", NULL)
    {
           modify = modifier;
           permaBit = perma;
            sheet->changeText(defaultFont, (permaBit + *modify), NULL, BLACK,64);
    }
    string getPermaBit()
    {
        return permaBit;
    }
    string getModify()
    {
        return *modify;
    }
    void function()
    {
            //cout << "a" << endl;
             SDL_StartTextInput();

    }
   virtual void change(string newStr, bool mod = true) //if mod the change *modify. Otherwise, just display newStr
    {
        if (mod)
        {
        *modify += newStr;
      sheet->changeText(defaultFont, (permaBit + *modify), nullptr);
      SDL_Rect* rectangle = sheet->getRect();
      sheet->changeRect(rectangle->x-11,rectangle->y, rectangle->w + 22, rectangle->h);
        }
        else
        {
                   sheet->changeText(defaultFont, (permaBit + *modify + newStr), nullptr);
        }

    }
  virtual void change(int howMuch, bool mod = true)
    {
        if (modify->length() > 0)
        {
                if (mod )
            {
               unsigned int cutOff = abs((int)(modify->length() - howMuch));
              // cout << cutOff << endl;
            *modify = modify->substr(0,cutOff );
           sheet->changeText(defaultFont, (permaBit + *modify), nullptr);
                 SDL_Rect* rectangle = sheet->getRect();
      sheet->changeRect(rectangle->x+11*howMuch, rectangle->y, rectangle->w - 22*howMuch, rectangle->h);

          // cout << howMuch << endl;
            }
            else
            {
                sheet->changeText(defaultFont, (permaBit + modify->substr(0, modify->length()- howMuch)), nullptr, BLACK,64);
            }
        }


    }
    virtual void during()
    {

        if (selected)
        {
        if (e.type == SDL_TEXTINPUT && !e.key.repeat)
        {
            change(e.text.text);
         //   e.key.repeat = true;
            //cout  << 1 << endl;
        }
        else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE && !e.key.repeat)
        {
            change(1);
        }
        int ticks = SDL_GetTicks()%2000;
              if (ticks >= 0 && ticks < 1000)
             {
                // currentType->change(" |", false);
                   SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
                 SDL_RenderDrawLine(render, sheet->getRect()->x + sheet->getRect()->w, sheet->getRect()->y , sheet->getRect()->x + sheet->getRect()->w, sheet->getRect()->y + sheet->getRect()->h);
             }
        }
    }
    virtual void release()
    {
        SDL_StopTextInput();
    }



};

extern int SCREEN_WIDTH;
class modIntButton : public typeButton
{
    int* modified = NULL;
    int maxValue = 0;
public:
    modIntButton(int x, int y, int w, int h, int& mod, int max = 99) : typeButton(x, y, w, h,new string(""), "Kills: " )
    {
        modified = &mod;
        maxValue = max;
        //*modify = convert(*modified);
    }
    void change(string newStr, bool mod = true)
    {
        if (*modified*10 + convert(newStr) <= maxValue)
        {
        typeButton::change(convert(convert(newStr)), mod);
        *modified = convert(*modify);
        }
    }
    void change(int howMuch = 1, bool mod = true)
    {
        typeButton::change(howMuch, mod);
        *modified = convert(*modify);
    }
    ~modIntButton()
    {
        delete modify;
    }
};

void changeMenu(menus m)
{
    menu = m;
    selector = nullptr;
    SDL_StopTextInput();
}
void changeScreen(screen& s)
{
    if (currentScreen != &s)
    {
        if (currentScreen)
        currentScreen->useLast();
        s.useFirst();
        currentScreen = &s;
    }

}

/*struct multiplayerScreen : public screen
{
    bool LAN = true;
    multiplayerScreen(const char* background, wrapperFunction<void,screen>* func = NULL, wrapperFunction<void,screen>* f = NULL,wrapperFunction<void, screen>* la = NULL, std::vector<button*>* l = NULL, SDL_Renderer* renderer = render, paper* message = NULL) : screen(background, func, f,la,renderer, message,l)
    {

    }
   void use()
    {
        if (LAN)
        {
            send(c->pack, c->sock, 24, "255.255.255.255", "broadcast");
        }
        if (SDLNet_UDP_Recv(c->sock, c->pack))
        {
            cout << c->pack->data << endl;
        }
    }

};*/
void play()
{



    //std::cout << "yes" << std::endl;
}




#endif // BUTTONS_H_INCLUDED
