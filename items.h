#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED

extern SDL_Renderer* render;

class item
{
    protected:
     int price;
     bool bought = false;
     std::string spriteLocation = "";
    std::unique_ptr<SDL_Texture,std::function<void(SDL_Texture*)>> sprite = std::unique_ptr<SDL_Texture,std::function<void(SDL_Texture*)>> (nullptr, &SDL_DestroyTexture);
    public:
    item(int cost, std::string sprite_location)
    {
        price = cost;
        spriteLocation = sprite_location;
        sprite.reset( SDL_CreateTextureFromSurface(render, IMG_Load(sprite_location.c_str())));
    }
    std::string getSpriteLocation()
    {
        return spriteLocation;
    }
    int getPrice()
    {
        return price;
    }
    void buy()
    {
        bought = true;
    }
    bool getBought()
    {
        return bought;
    }
    virtual void use()
    {

    }
    ~item()
    {

    }
};

extern int money;
struct itemButton : public button
{
    item* stock = NULL;
    itemButton (item* i,int x, int y, int w, int h,std::string sprite_location, SDL_Renderer* see = render) : button(x, y ,w , h, sprite_location,NULL, see )
    {
        stock = i;
    }
    itemButton (item* i,int x, int y, int w, int h,std::string font, std::string text, int r, int g, int b,const SDL_Color& fontColor = BLACK, SDL_Renderer* see = render) : button(x, y ,w , h, font, text, r, g, b,fontColor,NULL, see )
    {
        stock = i;
    }
     itemButton (item* i,int x, int y, int w, int h,std::string font, std::string text, SDL_Color& col, const SDL_Color& fontColor = BLACK,SDL_Renderer* see = render) : button(x, y ,w , h, font, text,&col,fontColor,NULL, see )
    {
        stock = i;
    }
      itemButton (item* i,int x, int y, int w, int h,SDL_Renderer* see = render) : button(x, y ,w , h,i->getSpriteLocation(),NULL, see )
    {
        stock = i;

    }
    void function()
    {
        if ( stock != NULL && money >= stock->getPrice() && !stock->getBought())
        {
            stock->buy();
            money -= stock->getPrice();
        }
    }
};


extern bool armory[maximumWeapons];

struct weaponItemButton : public itemButton, item // a weird cross between an item button and an item that represents buyable weapons
{
    int weaponInt = 0;
    weaponItemButton(int c, int x, int y, int w, int h,std::string sprite_location, SDL_Renderer* see = render) : itemButton(NULL, x, y ,w , h, sprite_location, see ),item(c, sprite_location)
    {
        weaponInt = w;
    }
    void function()
    {
        if (money >= price && !bought)
        {
            cout << "yes" << endl;
            buy();
            armory[weaponInt] = true;
        }
    }

};



#endif // ITEMS_H_INCLUDED
