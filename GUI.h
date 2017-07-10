#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED



 paper coin;
 paper lockSprite;
Effect* invSprite;
Effect* weaponSelect;
button* selector = nullptr;
const int invSize = 3;
Effect* hurtBorder;
weaponButton* inventory[invSize] ;
Effect* announcement = nullptr;
paper* healthCount = new paper();
paper* killCount = new paper();
paper* objectiveDisplay = new paper();
paper moneyCount ;
#endif // GUI_H_INCLUDED
