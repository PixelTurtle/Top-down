#ifndef SDL_ttf_H_INCLUDED
#define SDL_ttf_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <functional>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <vector>

static SDL_Renderer* render = nullptr;

typedef void (*functionPtr)();
typedef std::function<void()>* functionWrapper;
typedef std::function<void()> actualFunction;
const SDL_Color BLACK = {0,0,0};

struct Paper;

SDL_Texture* write(std::string how_write, std::string type, SDL_Color* backColor,const SDL_Color& fontColor,int how_big,SDL_Renderer* renderer );
void write(std::string how_write, std::string type, SDL_Color* backColor, const SDL_Color& fontColor,Paper* sheet, int how_big, int x , int y , int w , int h , SDL_Renderer* renderer); //how_write is a font. type is the string to be written.
bool changeTexture(SDL_Texture*& text, std::string sprite_location, std::string delimiter = "absolutely nothing", SDL_Renderer* renderer = render) // delimiter allows one to pass a blank string without an error message
{

if (sprite_location == delimiter)
{
    text = NULL;
    return true;
}
    SDL_Surface* surf = IMG_Load(sprite_location.c_str());
    text = SDL_CreateTextureFromSurface(renderer, surf );
    SDL_FreeSurface(surf);
    if (!(text))
    {
        std::cout << "Warning: error changing texture to: " << sprite_location << " " << IMG_GetError() << std::endl;
        return false;
    }
    return text;
}
SDL_Texture* changeTexture(std::string sprite_location,  std::string delimiter = "absolutely nothing", SDL_Renderer* renderer = render)
{
    SDL_Texture* t = nullptr;
   if (changeTexture(t, sprite_location, delimiter, renderer))
   {
       return t;
   }
   return nullptr;
}
bool renderTexture(SDL_Texture* text,SDL_Rect* rect,SDL_Rect* drect,  double angleInDegrees = 0,SDL_Point* center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Renderer* renderer = render)
{
    if (!text)
    {
       // std::cout <<  << std::endl;
        std::cout << ("Error: error rendering texture: texture is NULL.") << std::endl;
        throw std::logic_error("");
        return false;
    }
    else if (drect&&  drect->w*drect->h <= 0)
    {
       // std::cout << "Error: Passed drect has area of 0." << std::endl;
        std::cout << "Error in RenderTexture function: Width is: " << drect->w << " and Height is: " << drect->h << std::endl;
        return false;
    }

   SDL_RenderCopyEx(renderer, text, rect, drect, angleInDegrees, center, flip  );
}

class Paper
{

    SDL_Rect* rect = NULL;
    SDL_Texture* text = NULL;
    SDL_Rect* portion = NULL;
    protected:
    std::string spriteLocation = "";
    std::string message = ""; // if this paper holds a message, this string records it
              bool display = true;
    public:
           int spriteW = 0, spriteH = 0;
        Paper(const SDL_Rect& r , std::string sprite_location, const SDL_Rect& p = {0,0,0,0})
    {
        rect = new SDL_Rect(r);
        portion = new SDL_Rect(p);
        changeText(sprite_location);
      //  SDL_QueryTexture(text, NULL, NULL, &spriteW, &spriteH);
    }
    Paper(const SDL_Rect& r, std::string font_location, std::string toBePrint,  SDL_Color* backColor = NULL, const SDL_Color& fontColor = BLACK,int howBig = 64)
    {
        rect = new SDL_Rect(r);
        changeText(font_location, toBePrint, backColor, fontColor,howBig);
        //  SDL_QueryTexture(text, NULL, NULL, &spriteW, &spriteH);
    }
    Paper()
    {
         rect = new SDL_Rect{0, 0, 0, 0};
         portion = new SDL_Rect{0,0,0,0};
         text = SDL_CreateTexture(render, SDL_PIXELFORMAT_UNKNOWN,SDL_TEXTUREACCESS_TARGET, 0,0);
    }
    void clear()//erases all contents of the paper
    {
         SDL_DestroyTexture(text);
    }
           SDL_Texture* getText()
       {
           return text;
       }
    void changeDisplay()
    {
        if (display)
            display = false;
        else
            display = true;
    }
    void changeDisplay(bool newDisplay)
    {
        display = newDisplay;
    }
    bool getDisplay()
    {
        return display;
    }
    virtual void show(SDL_Renderer* r = render)
    {
        if (display)
        {
            renderTexture( text, NULL, rect, 0, nullptr, SDL_FLIP_NONE,r);
        }
    }
    void show(const SDL_Rect& r,SDL_Rect* portion = nullptr)
    {
        SDL_Rect* blit = new SDL_Rect(r);
         show(render, portion, blit);
         delete blit;
    }
   virtual void show(int x, int y, SDL_Renderer* renderer = render)
    {
        if (!renderer)
        {
            std::cout << "Error: error rendering paper: renderer is null" << std::endl;
        }
        changeRect(x,y);
    show(renderer, NULL, rect);
    }
    virtual void show(SDL_Renderer* renderer,SDL_Rect* part ,SDL_Rect* blit ,double angleInDegrees =0 ,SDL_Point* center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE)
    {
   //     std::cout << blit->x << std::endl;
   if (!text)
   {
       throw std::logic_error("Text is null");
   }
        if (display)
             renderTexture(text, part, blit,angleInDegrees, center, flip,renderer);
    }
    bool changeText(std::string font_location, std::string toBePrint, SDL_Color* backColor = NULL, const SDL_Color& fontColor = BLACK,int howBig = 64, SDL_Renderer* renderer = render)
    {

        SDL_Texture* t  = write(font_location, toBePrint, backColor, fontColor,howBig, renderer);
        changeText(*t);
        message = toBePrint;
          SDL_QueryTexture(text, NULL, NULL, &spriteW, &spriteH);
          return true;
    }
    bool changeText(std::string sprite_location, SDL_Renderer* renderer = render)
    {
        if(!changeTexture(text, sprite_location, "absolutely nothing", renderer))
        {
            std::cout << "Error changing paper texture to: " << sprite_location << std::endl;
            return false;
        }
        else
        spriteLocation = sprite_location;
        SDL_QueryTexture(text, NULL, NULL, &spriteW, &spriteH);
        return true;
    }
    bool changeText( SDL_Texture& newText )
    {
        if (text)
            SDL_DestroyTexture(text);
        text = &newText;
          SDL_QueryTexture(text, NULL, NULL, &spriteW, &spriteH);
          return text;
    }
    void changeTextColor(const SDL_Color& col)
    {
         SDL_SetTextureColorMod(text, col.r, col.g, col.b);
    }
    bool isTextValid()
    {
        return text;
    }
    SDL_Rect* getRect()
    {
       // std::cout << rect->x << std::endl;
        return rect;
    }
    void changeRect(int x, int y, int w, int h)
    {
        changeRect(x,y);
        rect->w = w;
        rect->h = h;
      //  if (rect->w > 1000)
     //   std::cout << "New Width: " << rect->w << std::endl;
    }
    void changeRect(int x, int y)
    {
          if (!rect)
        {
            rect = new SDL_Rect;
        }
        rect->x = x;
        rect->y = y;
    }
    void changeRect(const SDL_Rect& newRect)
    {
        changeRect(newRect.x,newRect.y,newRect.w,newRect.h);
    }
        SDL_Rect* getPortion ()
    {
        return portion;
    }
    void changePortion(const SDL_Rect& newRect)
    {
        if (portion)
        {
            portion->x = newRect.x;
            portion->y = newRect.y;
            portion->w = newRect.w;
            portion->h = newRect.h;
        }
    }
    void changePortion(int x, int y)
    {
        changePortion({x,y,portion->w,portion->h});
    }
    std::string getMessage()
    {
        return message;
    }
    std::string getSpriteLocation()
    {
        return spriteLocation;
    }
 virtual  ~Paper()
    {

        SDL_DestroyTexture(text);
        if (rect != NULL)
        delete rect;
        if (portion!= NULL)
            delete portion;
    }


};

class Animation : public Paper // a class that allows for an animated paper. The text and rect variables in this case represent the current frame of animation and where to blit that current frame
{
    protected:
    int frameTime = 0; //the amount of time of each frame in milliseconds.
    int start = 0; // the time at which the animation began
    bool stopped = false;
    public:
    Animation(std::string spriteSheetLocation, const SDL_Rect& portionOfSpriteSheet, const SDL_Rect& blitTo, int frameT ) : Paper(blitTo, spriteSheetLocation, portionOfSpriteSheet)
    {
     //   std::cout << portion->w << std::endl;
        frameTime = frameT;
        start = SDL_GetTicks();
    }
    Animation()
    {
        Paper();
    }
    using Paper::show;
    virtual void show(SDL_Renderer* renderer,SDL_Rect* part ,SDL_Rect* blit ,double angleInDegrees =0 ,SDL_Point* center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE)
    {
          //  std::cout << spriteW/portion.get()->w<< std::endl;
           SDL_Rect* thisPortion = getPortion();
        if (getDisplay() )
        {
    //    std::cout << portion->w << std::endl;
      if (!stopped)
      {
          int ticks = SDL_GetTicks()-start;
        //   std::cout << portion->w << std::endl;

     changePortion(    {   (ticks/frameTime)%(spriteW/thisPortion->w)*thisPortion->w,
    (ticks/(spriteW/thisPortion->w*frameTime))%((spriteH/thisPortion->h))*thisPortion->h , thisPortion->w, thisPortion->h}  );
      }
         //   std::cout << portionOf->y << " " << ticks  << std::endl;
            Paper::show(renderer,thisPortion,blit,angleInDegrees, center, flip);
         //   if (portion.get()->x == 0)
         //   std::cout << portion.get()->x << std::endl;
        }
    }
    void restart()
    {
        changePortion({0,0});
    }
    void changeStopped(bool newStop)
    {
        stopped = newStop;
    }


};

class TempAnimation : public Animation // a class that represents an animation that plays then disappears
{
    protected:
    int duration = 0;
public:
    TempAnimation(std::string spriteSheetLocation, const SDL_Rect& portionOfSpriteSheet, const SDL_Rect& blitTo, int frameT, int howLong ) : Animation(spriteSheetLocation, portionOfSpriteSheet,blitTo , frameT)
    {
        duration = howLong;
    }
    void changeDuration(int newDuration)
    {
        duration = newDuration;
    }
   virtual void show(SDL_Renderer* renderer,SDL_Rect* part ,SDL_Rect* blit ,double angleInDegrees =0 ,SDL_Point* center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE)
   {
       if (getDisplay())
       {
           if (SDL_GetTicks() - start < duration)
           {
               Animation::show(renderer, part, blit, angleInDegrees, center, flip);
           }
           else
           {
            //   std::cout << "Done" << std::endl;
         changeDisplay(false);
         changePortion(0,0);
       //     start = SDL_GetTicks();
           }
       }
       else
       {
           start = SDL_GetTicks();
           changeDisplay(true);
           Animation::show(renderer, part, blit, angleInDegrees, center, flip);
       }
   }
   virtual void show(int x, int y, SDL_Renderer* renderer = render)
   {

        show(renderer, getPortion(), getRect());
   }
};



class Button
{
    protected:
        bool selected = false;
std::function<void()>* use = {};
public:
   Paper* sheet = new Paper();
   std::string spriteLocation = ""; // a variable that holds where the sprite is located

    Button(int x, int y, int w, int h, std::string sprite_location, std::function<void()>* func,SDL_Renderer* see = render)
    {
    //    std::cout << sprite_location << std::endl;
        sheet->changeRect({x, y, w, h});
        if (sprite_location!= "")
        {

        if (!sheet->changeText(sprite_location, see ))
        {
            std::cout << "Error loading button " << sprite_location<< "  : " << IMG_GetError() << std::endl;
        }

       spriteLocation = sprite_location;
        }
        use = func;
    }
    Button(int x, int y, int w, int h, std::string font, std::string text, SDL_Color* c,const SDL_Color& fontColor,std::function<void()>* func,SDL_Renderer* see = render)
    {
        sheet->changeRect({x, y, w, h});
        sheet->changeText( font, text, c, fontColor, 64);
       if (func != NULL)
       {
       use= func;
       }
    }
       Button(int x, int y, int w, int h, std::string font, std::string text, int r, int g, int b,const SDL_Color& fontColor,functionWrapper func,SDL_Renderer* see = render)
    {
        SDL_Color c;
        if (r >= 0 && g>= 0 && b>=0)
        {
            c = {r,g,b};
        }
        sheet->changeRect({x, y, w, h});
        sheet->changeText(font, text, &c, fontColor,64, see);
       if (func != NULL)
       {
       use= func;
       }
    }
    Button()
    {

    }
    bool getSelected()
    {
        return selected;
    }
    void changeSelected(bool newChosen)
    {

        selected = newChosen;
    }
    virtual void function()
    {
       // std::cout << "A" << std::endl;
        if (use != NULL)
        (*use)();
    }
    virtual void during()
    {

    }
    virtual void release()
    {

    }
};



struct Screen
{
std::vector<Button*>* buttons  = NULL;
    std::unique_ptr<SDL_Texture,std::function<void(SDL_Texture*)>> bg ;
   std::vector< std::shared_ptr<Paper> >sheets;
    std::function<void()>* first = NULL;
    std::function<void()>* last = NULL;
    std::function<void()>* use = NULL;
    Screen(const char* background,  functionWrapper func = NULL, functionWrapper f = NULL, functionWrapper la = NULL, Paper* message = NULL,std::vector<Button*>* l = NULL,SDL_Renderer* renderer = render)
    {
            buttons = l;
         bg =std::unique_ptr<SDL_Texture,std::function<void(SDL_Texture*)>> (nullptr, &SDL_DestroyTexture);
         if ((std::string)background != "" )
         {
            SDL_Surface* surf = IMG_Load(background);
            if (surf == NULL)
            {
                std::cout << "Error loading background: " + (std::string)background + " " + IMG_GetError() << std::endl;
             }
            bg.reset(SDL_CreateTextureFromSurface(renderer, surf));
            SDL_FreeSurface(surf);
            if ( bg == NULL )
            {
                std::cout << "Error loading background: " + (std::string)background + " " + IMG_GetError() << std::endl;
            }
         }
        if (func != NULL)
        {
            use =  func;
        }
        if (f)
        {
           first= f;
        }
        if (la)
        {
            last = la;
        }
        if (message)
        {
            sheets.push_back(std::unique_ptr<Paper>(message));
            message->changeDisplay(true);
        }

    }
        Screen(std::string background, std::function<void()>*  func = NULL, std::function<void()>*  f = NULL, std::function<void()>* la = NULL,std::vector<Paper*>* message = NULL,std::vector<Button*>* l = NULL, SDL_Renderer* renderer = render)
    {

        Screen(background.c_str(), func, f, la,  NULL, l,render);
        if (message)
        {
            int size = message->size();
            for (int i = 0; i < size; i++)
            {
                     sheets.push_back(std::unique_ptr<Paper>(message->at(i)));
                     message->at(i)->changeDisplay(true);
            }
            delete message;
        }
    }
    virtual void function()
    {
        if (use != NULL)
        (*use)();
    }
    virtual void useFirst()
    {
        if (first)
        {
            (*first)();
        }
    }
    virtual void useLast()
    {
        if (last)
        {
            (*last)();
        }
    }
    ~Screen()
    {
        if (buttons)
    buttons->clear();

    }
};





SDL_Texture* write(std::string how_write, std::string type, SDL_Color* backColor,int r, int g, int b, int how_big,SDL_Renderer* renderer = render) //returns an SDL_Texture with the text written
{
    if ((how_write.length() >= 4 && how_write.substr(how_write.length()-4, 4) != ".ttf") || how_write.length() <= 4)
    {
        how_write += ".ttf";
    }
    TTF_Font* font = TTF_OpenFont(how_write.c_str(), how_big);
if (font == NULL)
{
   std:: cout << "Error in Write function: " << TTF_GetError() << std::endl;
   return NULL;
}
   SDL_Color foregroundColor = { r,g,b };
   //SDL_Color backgroundColor = backColor;
char *text = new char[type.length() +1 ];
strcpy(text, type.c_str());
//cout << "y: " << y << endl;
SDL_Surface* textSurface = NULL;
        if (backColor == NULL )
           textSurface = TTF_RenderText_Blended(font, text, foregroundColor);
        else
            textSurface = TTF_RenderText_Shaded(font, text, foregroundColor, *backColor);

   if ( textSurface == NULL)
   {
       std::cout << SDL_GetError() << std::endl;
      std:: cout << "Error in Write function: " << TTF_GetError() << std::endl;
  //    return nullptr;
   }
   delete[] text;
return SDL_CreateTextureFromSurface(renderer, textSurface);
//SDL_SetTextureBlendMode(sheet->text, SDL_BLENDMODE_BLEND);
//SDL_SetTextureAlphaMod(sheet->text, 32);

//SDL_RenderCopy(render, gh, NULL, text);

}

SDL_Texture* write(std::string how_write, std::string type, SDL_Color* backColor,const SDL_Color& fontColor,  int how_big,SDL_Renderer* renderer = render) //how_write is a font. type is the string to be written.
{
  return  write(how_write, type, backColor, fontColor.r,fontColor.g, fontColor.b,  how_big, renderer);
}

void write(std::string how_write, std::string type, SDL_Color* backColor,const SDL_Color& fontColor, Paper* sheet, int how_big,SDL_Renderer* renderer = render) //how_write is a font. type is the string to be written.
{
    SDL_Texture* texture = write(how_write, type, backColor, fontColor.r,fontColor.g, fontColor.b, how_big, renderer  );
    if (texture)
        sheet->changeText(*texture);

}

void write(std::string how_write, std::string type, SDL_Color* backColor, const SDL_Color& fontColor, Paper* sheet, int how_big, int x , int y , int w , int h , SDL_Renderer* renderer = render) //how_write is a font. type is the string to be written.
{
     if (sheet == NULL)
    {
        std::cout << "WARNING: Paper passed to write function is null. Nothing was done to modify it." << std::endl;
        return;
    }
    write(how_write, type, backColor,{fontColor.r,fontColor.g, fontColor.b}, sheet, how_big, renderer);
    sheet->changeRect(x, y, w, h);

}





#endif // SDL_ttf_H_INCLUDED
