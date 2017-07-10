#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED

class character;

struct effect
{
    int duration;
    character* owner;
    effect(character* what, int length)
    {
        owner = what;
        duration = length;
    }
    virtual void use()
    {

    }

};

struct sprint : public effect
{
    sprint(character* what) : effect(what, 1000)
    {

    }
    void use(); // defined in players.h

};

struct hulk : public effect
{
    hulk(character* what);

};

struct invincible : public effect
{
    invincible(character* what) : effect(what, 100)
    {

    }
    void use()
    {

    }



};

#endif // EFFECTS_H_INCLUDED
