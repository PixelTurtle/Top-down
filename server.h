#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED


#include "weapons.h"

extern stage* level;
//extern SDL_Renderer* render;


bool load(string where, stage* location)
{

    ifstream open(where);
    if (!open.is_open())
    {
        cout <<"Error loading map of location: " << where << endl;
        return false;
    }
    if (location->parent.plot.size() != 0) //if the level hasn't been cleared yet;
    {
        cout << "Clearing ... " <<endl;
        location->reset();
    }
    string  fullBuffer = "";
    int x , y , w , h , which = 0; // which points to the area to spawn in
  //       location->changeBg("sprites/bg/debug.png");

    getline(open, fullBuffer);
    location->changeBg(fullBuffer);
   while(getline(open, fullBuffer ))
   {
    //     cout << "Buffer: " << fullBuffer << endl;
    if (!(fullBuffer[0] == '/' && fullBuffer[1] == '/'))
    {
        string* breakUp = divideString(fullBuffer);
         int obj = convert(breakUp[0]);
         x = convert(breakUp[1]);
         y = convert(breakUp[2]);
         w = convert(breakUp[3]);
         h = convert(breakUp[4]);
        string sprite = breakUp[5];
        switch(obj)
        {
        case 0 :
            location->parent.insert(*(new object(x,y,w,h,&location->parent, sprite.c_str())));
            break;
        case 1: location->parent.insert(*(new weaponBlock<laserGun>( x, y, &location->parent) ));
        break;
        case 2: location->parent.insert(*(new weaponBlock<spreader>( x, y, &location->parent) ));
        break;
        case 3: location->parent.insert(*(new weaponBlock<fireShark>( x, y,&location->parent)));
        break;
        }
   }
   }
  // cout << location->players.size() << endl;

return true;

   }

   bool load(string where)
   {

   }

enum commands
{
    updatePlayers,
    introPlayers,
    updateProjectiles

};

              const  int serializedLength = 8; //represents a constant int that is the lenght of a player's serialized form -1
                          int totalLength = serializedLength + 1;
    void loadBullet(int proj, int x, int y, double x_dir, double y_dir, character* shooter, int id, grid* where  = &level->parent)
    {
        switch(proj)
        {
        case 0:
            projectiles.push_back(unique_ptr<projectile>(new bullet(x, y, where, x_dir, y_dir, shooter, id)));
            break;
        case 1:
            projectiles.push_back(unique_ptr<projectile>(new laser(x, y, where, x_dir, y_dir, shooter, id)));
            break;
        case 2: projectiles.push_back(unique_ptr<projectile>(new fireRocket(x , y, where, x_dir, y_dir, shooter, id)));
       // SDL_Delay(100);
      //  cout << "Errpr" << endl;
        break;
        default:
            return;
        }
    }
    weapon* loadGun(int whichWeapon, player* wielder)
    {
        switch(whichWeapon)
        {
            case -1:
            return nullptr;
            break;
            case GUN: return new gun(wielder);
            break;
            case LASERGUN: return new laserGun(wielder);
            break;
            case SPREADER: return new spreader(wielder);
            break;
            case SHARK: return new fireShark(wielder);
            break;
            case PERSECUTED: return new persecuted(wielder);
            break;
            default: return new gun(wielder);
        }
    }
    weapon* loadGun(weaponNames whichWeapon, player* wielder)
    {
        return loadGun((int)whichWeapon, wielder);
    }
extern int maxKills;
extern string name;
//extern bool first;
const int packetSize = 1024;
struct client
{
    IPaddress ip ;
    UDPpacket* pack = SDLNet_AllocPacket(packetSize);
    int port; //port is this client's port.
    UDPsocket sock;
    unique_ptr<player> user;
    int prevX = 0; //this and all prevVariables are essentially the character before sending data to the server. It is used to prevent cheating.
    int prevY = 0;
    Uint32 prevTime;
    bool received = false; // represents whether or not the client recieved this frame.
    client(int p)
    {
       // cout << "POrt <<" << p << endl;
        port = p;
        sock = SDLNet_UDP_Open(port);

       // cout << &sock << endl;
     /*           user = new player(port, 20, 200, 200, 64, 64,2, level->areas[0].get(),"sprites/players/sprote.bmp");
         int x = rand() % 640;
        int y = rand() % 400;
        user->rect->x = x;
        user->rect->y = y;*/


    }
    ~client()
{
    SDLNet_UDP_Close(sock);
}
    void reset( player* ptr = nullptr)//a funtion that resets the player
    {
           user.reset(ptr);
    }

        bool connect( const char* address = "255.255.255.255", int serverPort = 24) //function used to connect to server
    {
        if (SDL_GetTicks() % 1000 == 0)
        {
            string s = "join " + convert(port) +  " " +name;
            if (!send(pack, sock, serverPort,address, s))
            {
                cout << "Error sending: " << SDLNet_GetError() << endl;
            }
            cout << "Connecting..." << endl;
           // first =true;
        }
        if (SDLNet_UDP_Recv(sock, pack))
        {
        cout << "Recieved!" << endl;
        string toLoad (pack->data, pack->data +  pack->len);
       // cout << toLoad<< endl;
    //    cout << toLoad.substr(0, toLoad.find(" ")) <<"A"<< endl;
        maxKills = convert(toLoad.substr(0, toLoad.find(" ")));
       // cout << maxKills << endl;
        toLoad = toLoad.substr(toLoad.find(" ")+1, toLoad.length()); //cuts oof the part that contains how many kills are required to win
        level->levelLocation = (toLoad.substr(0, toLoad.find(" ")));
        toLoad = toLoad.substr(toLoad.find(" ")+1, toLoad.length());//cuts off the level
       cout << "toLoad: " << toLoad << endl;
        string* breakUp = divideString(toLoad);
        int count = charCount(toLoad, '@');
        cout << "Count: " << count << endl;
      //cout << "Number of players: " << level->players.size() << endl;
        for (int i = 0; i < count; ++i)
        {

            int reach = i*(serializedLength + 1 );
            level->players.push_back(new player(convert(breakUp[reach]),  breakUp[reach+1],convert(breakUp[reach+2]), convert(breakUp[reach+3]), convert(breakUp[reach+4]), breakUp[reach+6].c_str()));
            level->players[level->players.size() -1]->equip(loadGun(convert(breakUp[reach+7]), level->players[level->players.size() -1]));
      //     cout << breakUp[reach+6].c_str() << endl;

        }
  //      cout <<breakUp[count*serializedLength ] << endl;
        user.reset( new player(port, name, 20, convert(breakUp[count*serializedLength + 1]), convert(breakUp[count*serializedLength + 2]), player::defaultSpriteLocation.c_str()));
        if (user.get() ==NULL )
        {
            throw std::logic_error("user is NULL!!!");
        }
        level->players.push_back(user.get());
      //  cout << level->players.size() << endl;
         prevX = user.get()->sheet->getRect()->x;
        prevY = user.get()->sheet->getRect()->y;
    //      cout << "Number of players: " << level->players.size() << endl;
  //   cout << "Number of players: " << level->players.size() << endl;
   //     cout << user->serialize() << endl;
     //   user = new player(port, 20, convert(breakUp[2]), convert(breakUp[3]),  "sprites/players/sprote.bmp");
        //  cout << user->rect->x << endl;
        return true;
        }
      //  cout << port << endl;
        return false;
    }
     void listen()
    {
      //  int total = 0;

     //   while (total < level->players.size() - 1)
        {

        if(SDLNet_UDP_Recv(sock, pack))
    {

          // cout<< "Received: " << pack->data << endl;
            int playerSize = level->players.size();
            string s(pack->data, pack->data+pack->len);
           // cout << "Received: " << s << endl;
                            string command = s.substr(0,s.find(" "));
                            s =  s.substr(s.find(" ") + 1, s.length());//cuts off the part of s that has the command
                         int newPort = convert(s.substr(0, s.find(" ")));
                         string* breakUp = divideString(s);
                        /* int x = convert(breakUp[2]);
                         int y = convert(breakUp[3]);
                        double ang = convert(breakUp[4]);
                         int health = convert(breakUp[1]);
                         string picture = breakUp[5];*/

                         weapon* currentWeapon = NULL;

                       //  for (int ad = 0; ad < 9; ad++)
                        /* {
                             cout << breakUp[ad] << endl;
                             }*/
                           // cout << "Start Time: " << SDL_GetTicks() << endl;
                             if (convert(breakUp[serializedLength+1]) != -1)
                             {
                                 //cout << breakUp[6] << endl;
                             //     cout<< "Received: " << pack->data << endl;

                                  int counted = charCount(s.substr(s.find("@") , s.length()), '@') - 1;
                              //  cout <<s.substr(s.find("@") , s.length()) << endl;
                              //  cout << counted << endl;
                                   int constant = 7; //how long the bullet serialization is.
                                   player* ptr = level->players[0]; // the player that shot the bullet first

                                 for (int i = 0; i < counted; i ++)
                                 { // cout << "I: " << i << endl;
                              int shooterPort = convert(breakUp[serializedLength + i*constant + 2]);
                              int projectileID = convert(breakUp[serializedLength + i*constant + 1]);
                            int projSize = projectiles.size();
                           //    cout << "ID : " << projectileID << endl;
                                   //  string seg = breakUp[serializedLength+1 + i];
                                    if (ptr->port != shooterPort)
                                    {
                                            for (int b = 0; b < playerSize; b++)
                                         {
                                             ptr = level->players[b];
                                                if (ptr->port == shooterPort)
                                                {
                                                //    cout << "Port: " << ptr->port << endl;
                                                    break;
                                                }
                                         }
                                    }

                                    if (projSize == 0)
                                    {
                                      loadBullet(convert(breakUp[constant*i +serializedLength + 3]), convert(breakUp[constant*i+serializedLength + 4]), convert(breakUp[constant*i+serializedLength + 5]), convert(breakUp[constant*i+serializedLength+6]), convert(breakUp[constant*i+serializedLength+7]), ptr,projectileID, ptr->location);
                                    }
                                    else
                                    {

                                    int jellyJam = 0;
                                   for (std::list<unique_ptr<projectile>>::const_iterator p = projectiles.begin(); p != projectiles.end(); ++p )
                                   {
                                        //cout << projectiles[p].get()->getID() << endl;
                                       if ((*p)->getID() == projectileID && shooterPort == ((player*)((*p)->owner))->port)
                                       {

                                          // cout << "ID : " << projectileID << " "<< "shooterPort : " << shooterPort << endl;
                                        //projectiles[p].get()->update(convert(breakUp[constant*i+serializedLength + 3]),convert(breakUp[constant*i+serializedLength + 4]), convert(breakUp[constant*i+serializedLength+5]), convert(breakUp[constant*i+serializedLength+6]));
                                           break;
                                       }
                                     if (jellyJam == projectiles.size()-1)
                                      {
                                         // cout << "P: " << p << endl;
                                          // if (projectileID == 0 && shooterPort == 0)
                                     //    cout <<  "I: " << i << " " << s.substr(s.find("@") , s.length()) << endl;

                                   //  cout << "ID : " << projectileID << " "<< "shooterPort : " << shooterPort << endl;
                                      loadBullet(convert(breakUp[constant*i +serializedLength + 3]), convert(breakUp[constant*i+serializedLength + 4]), convert(breakUp[constant*i+serializedLength + 5]), convert(breakUp[constant*i+serializedLength+6]), convert(breakUp[constant*i+serializedLength+7]), ptr,  projectileID,ptr->location);
                                      }
                                      jellyJam++;
                                    }
                                    }
                                 }
                               //  cout << "Size: " << projectiles.size() << endl;
                             }
                           //  cout << "End Time: " << SDL_GetTicks() << endl;

                for (unsigned int u = 0; u< playerSize; ++u)
                {
                                {

                                        if (level->players[u]->port == newPort) //finds the player that sent the data
                                        {
                                             player* ptr = level->players[u];

                                             if (ptr == user.get())
                                             {
                                                  int newX = convert(breakUp[3]);
                                                int newY = convert(breakUp[4]);
                                                if (!(newX == prevX && newY == prevY))
                                                {
                                                  //  cout << "Old: " << newX <<" " << prevX << endl;
                                                       double radian = ptr->angle;
                                                       ptr->update(breakUp);
                                                        ptr->angle = radian;
                                                     if (ptr == user.get())
                                                     {
                                                            received = true;
                                                     }
                                                //    cout << "New: " << newX << " " << ptr->sheet->getRect()->x << endl;
                                                }

                                            //    if (prevX == newX && prev)
                                             }

                                            else
                                            {
                                              //  double radian = ptr->angle;
                                                       ptr->update(breakUp);
                                                     //   ptr->angle = radian;
                                               /*     string* arr = breakUp;
                                                       ptr->port = convert(arr[0]);
                                                        ptr->name = (arr[1]);
                                                        ptr->health = convert(arr[2]);
                                                        SDL_Rect* rect = ptr->sheet->getRect();
                                                        rect->x = convert(arr[3]);
                                                        rect->y = convert(arr[4]);
                                                        ptr->angle = convert(arr[5]);

                                                            weapon* newWeapon = NULL;
                                                    int weap = convert(arr[7]);
                                                    if (weap != ptr->hand->name)
                                                    {
                                                        newWeapon = loadGun(weap,ptr);
                                                    }*/
                                                        /*    string* arr = breakUp;
                                                            int newHealth = convert(arr[2]);
                                                            int newX = convert(arr[3]);
                                                            int newY = convert(arr[4]);
                                                            double newAngle = convert(arr[5]);
                                                            weapon* newWeapon = NULL;
                                                            int weap = convert(arr[7]);
                                                            if (weap != ptr->hand->name)
                                                            {
                                                                newWeapon = loadGun(weap, ptr);
                                                            }
                                                           ptr-> update(newHealth, newX, newY,newAngle, newWeapon);*/


                                             }
                                            /* int weap = convert(breakUp[6]);
                                             //cout << breakUp[6] << endl;
                                             if (weap != ptr->hand.get()->name)
                                                currentWeapon = loadGun(weap, ptr);*/


                                     //       total ++;
                                            break;
                                        }
                                       else   if (u == playerSize- 1)
                                        {
                                         //   new player(breakUp);
                                            break;
                                        }

                                }

                }
                                     delete [] breakUp;
        }
    }
    }
    };



struct server
{
//vector<unique_ptr<contact>> connections;
 UDPsocket baseSocket;
   const int port = 24;
    client* clientPtr = nullptr;
        UDPpacket* pack = SDLNet_AllocPacket(packetSize) ;

     //   stage* level = new stage();
    server(string whichLevel)
    {
   baseSocket = SDLNet_UDP_Open(port);

  level->loadLevel(whichLevel);
    }
    server()
    {
   baseSocket = SDLNet_UDP_Open(port);
     // cout << &baseSocket << endl;
    }
    ~server()
{
    SDLNet_UDP_Close(baseSocket);
}
void reset()
{

}

    void notify()
    {
      //  cout << "listening" << endl;
        if (SDLNet_UDP_Recv(baseSocket, pack))
        {
            string s(pack->data, pack->data + pack->len);
  // cout << "S:" << s << endl;
    string cop = s;
    int unsigned ad, x, y,face_up, health;
    string clientCommand = s.substr(0, s.find(" ")); // a string that stores the command from the client
         s = s.substr(s.find(" ") + 1, s.length()); // cuts off the command
     int port = convert(s.substr(0, s.find(" ")));
     string segment = s.substr(s.find(" ") + 1, s.length());
   //  cout << segment << endl;
     string name = segment.substr(0, s.find(" " ));
    // cout << "port: " << port << endl;
     int levelSize = level->players.size();
  //   cout << levelSize << endl;
     if (clientCommand == "join")
    {


     //   player* user = new player(port, 20, 175, 175, 64, 64, 2, &level->parent, "sprites/players/sprote.bmp");
       // user->spawn(*user->location);
       // level->players.push_back(user);
       string collection = "";
       for (int i = 0; i < levelSize; i++)
       {
         //  cout << level->players[i]->serialize() << endl;
           collection += level->players[i]->serialize() + "@";
       }


          player* user = new player(port, name);
      //  cout << user->serialize() << endl;
        send(pack, baseSocket, port, "255.255.255.255",  convert(maxKills)+ " " +(string)"maps/grasshill.txt " + collection + convert(user->sheet->getRect()->x) + " " + convert(user->sheet->getRect()->y )); // sends the level
        delete user;
   //     cout << level->players.size() << endl;
     //   user->sheet->changeRect(-100,-100);
      //  cout << " hand: " <<  user->hand.get()->rect->x << " " << user->rect->x << endl;
return;
    }
    else if (clientCommand == "broadcast")
    {
        cout <<"sned" << endl;
        send(pack, baseSocket, port, "255.255.255.255", (string)"maps/grasshill.txt" );
        return;
    }


//cout << "S:" << s << endl;
      string* breakUp  = divideString(s); // at this point, s should just be the serialized character+ porjectile
                     x = convert(breakUp[3]);
        //             cout << x << endl;
                    y = convert(breakUp[4]);
                    double radian = convert(breakUp[5]);
          //          cout << radian << endl;
                    health = convert(breakUp[2]);
                   // cout << health << endl;
                    string picture = breakUp[6];
                          int weap = convert(breakUp[7]);
                          int keyPressed = convert(breakUp[serializedLength + 1]);
                  //        cout << "KeyPressed: " << keyPressed << endl;
                   // cout << picture << endl;
                    weapon* currentWeapon =  NULL;


  //  else
    {

                //grid* lev = &level->parent;
                for (ad = 0; ad < levelSize; ++ad) //finds the player that sent it.
                {
                    if (port ==level->players[ad]->port)
                    {
                      /*  int weap = convert(breakUp[6]);
                        currentWeapon = loadGun(convert(breakUp[6]), lev->players[ad]);
                        lev->players[ad]->update(health, x, y, radian, currentWeapon);*/
                        break;
                    }
                    if (ad == levelSize-1)
                    {
                        // cout << level->players.size() << endl;
                        level->players.push_back(new player(breakUp));
                  //      cout << level->players[level->players.size()-1]->sheet->getRect()->x << endl;
                //       cout << level->players.size() << endl;
                        //ad++;
                      //  cout << ad << endl;
                    }
                }
                player* ptr = level->players[ad];
               // cout << ad << endl;

                switch(keyPressed)
                    {
           case moveRight:
           {
                ptr->xDir = 1;
                break;
           }
           case moveLeft:
           {
                ptr->xDir = -1;
                break;
           }
           case moveUp:
           {//cout << "moving up" << endl;
               ptr->yDir = -1;
                break;
           }
           case moveDown:
           {
                ptr->yDir = 1;
                break;
           }
           case horizRelease:
           {
               ptr->xDir = 0;
               //ptr->yspeed = 0;
               break;
           }
           case vertRelease:
            {
                ptr->yDir = 0;
            }
                    }
                            //cout << ptr->xDir << " " << ptr->yDir <<  endl;
                   //         cout << clientPtr->user.get()->sheet->getRect()->x << " " <<  clientPtr->user.get()->sheet->getRect()->y << endl;
                            ptr->setSpeeds();
         //   cout << ptr->sheet->getRect()->x << endl;
                                 ptr->walk(ptr->xspeed, ptr->yspeed);
                               //  cout << ptr->sheet->getRect()->x << endl;
                            ptr->update(health, ptr->sheet->getRect()->x , ptr->sheet->getRect()->y,radian, currentWeapon);
                               //              cout << breakUp[6] << endl;
                        if (ptr->hand->equals((weaponNames)weap))
                         currentWeapon = loadGun(weap, ptr);
                for (unsigned  int p = 0; p < levelSize;++p )
                {
                    int totalLength = serializedLength + 1; // an int representing the length of serializedLength + keyLog
                   // if (p != ad) //if the player is not the player that sent the packet
                    {

                       if (level->players[p]->port== clientPtr->port )// directly update everything for the host
                        {

                                        //    cout << "A: " << breakUp[serializedLength + 1]<< endl;

                                        //   int bulletLength = 5; // an int representing the length of a serialized bullet
                             if (convert(breakUp[totalLength + 1]) != -1)
                            {
                                int constant = 7; //how long the bullet serialization is.
                                 int c = charCount(s.substr(s.find("@") , s.length()), '@');
                               //  cout <<c << endl;
                               // cout << s.substr(s.find("@") + 1, s.length())<< endl;
                                    for (int i = 0; i < c; i++)
                                    {
                                          int projectileID = convert(breakUp[totalLength+ i*constant + 1]);
                                      loadBullet(convert(breakUp[constant*i +totalLength + 3]), convert(breakUp[constant*i+totalLength + 4]), convert(breakUp[constant*i+totalLength + 5]), convert(breakUp[constant*i+totalLength+6]), convert(breakUp[constant*i+totalLength+7]), ptr,  projectileID,ptr->location);
                                    }
                            }
                        }
                        else
                   send(pack, baseSocket,level->players[p]->port,"255.255.255.255", "update " + ptr->serialize() +"@" + cop.substr(totalLength, cop.size()));
                 // cout <<  "SEnding: "  << ptr->serialize() +"@" + cop.substr(serializedLength, cop.size()) << endl;
                  //cout << "S: " << cop << endl;
                //  cout <<  cop.substr(totalLength, cop.size()) << endl;
                    }

                  // delete[] breakUp ;
                }

    }
        }
    }
    void serverSend(string message)
    {
      //  delete pack;
        //pack = SDLNet_AllocPacket(sizeof(char)*message.length());
         int levelSize = level->players.size();
        for (int x = 1; x < levelSize; x++)
        {

            send(pack, baseSocket, level->players[x]->port, "255.255.255.255",message );
        }
    }


};

extern server* s;

void notifyLoop ()
{
if (s!= NULL)
    {
        s->notify();
    }
}


#endif // SERVER_H_INCLUDED
