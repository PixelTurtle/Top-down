#ifndef ATTACKS_H_INCLUDED
#define ATTACKS_H_INCLUDED

class character;

struct attack
{
    int countdown, base_countdown;
    character* owner;
    SDL_Keycode key;
    attack(character* who)
    {
        owner = who;
       // key = which;
    }
    virtual void use()
    {

    }
    virtual void release()
    {

    }
};


struct shoot_fireball : public attack
{
    shoot_fireball(character* who) : attack(who)
    {
        countdown = 100;
        base_countdown = 100;
    }
    int fireballSize;
    void use();

    void release();

};

struct get_fast : public attack
{
    get_fast(character* who) : attack(who)
    {
        countdown = 1000;
        base_countdown = 1000;
    }
    void use();
    //void release();
};

struct hulking : public attack
{
    hulking(character* who) : attack(who)
    {
        countdown = 1000;
        base_countdown  = 1000;
    }
    void use();
};
#endif // ATTACKS_H_INCLUDED
