#include <gb/gb.h>
#include <stdio.h>

#include "MetaSprite.c"     
#include "CharSprites.c"

struct MetaSprite ship;
struct MetaSprite alien;

void perfDelay(UINT8 numloops){ UINT8 i; for( i = 0; i < numloops; i++){ wait_vbl_done(); } }

void checkScreenLimit(){
    if(ship.x < 8){     ship.x = 8;   }
    if(ship.x > 152){   ship.x = 152; }
    if(ship.y < 16){    ship.y = 16;  }
    if(ship.y > 152){   ship.y = 152; }
    moveMetaSprite(&ship, ship.x, ship.y);
}

// verifie si il y a pas un overlap de deux metasprites
UBYTE CheckCollision(struct MetaSprite* one, struct MetaSprite* two){
    // verification si le coin en haut a droite de "one" n'est pas dans "two"
    // et si le coin en haut a droite de "two" n'est pas dans "one"
    return ( (one->x >= two->x) && (one->x <= (two->x + two->width)) ) && ( (one->y >= two->y) && (one->y <= (two->y + two->height)) ) || ( (two->x >= one->x) && (two->x <= (one->x + one->width)) ) && ( (two->y >= one->y) && (two->y <= (one->y + one->height)) );
}

void main(){
    set_sprite_data(0, 8, CharSprites);
    setupMetaSprite(&ship,  80,130, 16,16, 0,1,2,3, 0);
    setupMetaSprite(&alien, 30,0,   16,16, 4,5,6,7, 1);

    SHOW_SPRITES;
    DISPLAY_ON;

    // la detection de collision arretera la boucle au moindre accident spacial
    while (!CheckCollision(&ship, &alien)){
        if(joypad()){
            if(joypad() & J_UP){
                ship.y -= 4;
            }
            if(joypad() & J_DOWN){
                ship.y += 4;
            }
            if(joypad() & J_LEFT){
                ship.x -= 4;
            }
            if(joypad() & J_RIGHT){
                ship.x += 4;
            }
            checkScreenLimit();
        }

        alien.y += 8;
        if(alien.y>=160){
            alien.y = 0;
            alien.x = ship.x;
        }
        moveMetaSprite(&alien, alien.x, alien.y);
        
        perfDelay(5);
    }
    // fin de la boucle, on affiche le message de fin
    printf("\n \n \n \n \n \n \n \n \n==== GAME  OVER ====");
}