#include <gb/gb.h>
#include <stdio.h>
#include "HeroSprite.c"

void main(){
    UINT8 currTileIndex = 0;

    set_sprite_data(0, 2, Hero);
    set_sprite_tile(0, 0);
    move_sprite(0, 84, 80);
    SHOW_SPRITES;

    while(1){
        switch(joypad()){               // joypad() est fonction qui recupere l'input du joueur et retourne une constante (    J_A J_B J_START J_SELECT)
            case J_UP:
                scroll_sprite(0, 0, -8);
                break;
            case J_DOWN:
                scroll_sprite(0, 0, 8);
                break;
            case J_LEFT:
                scroll_sprite(0, -8, 0);
                break;
            case J_RIGHT:
                scroll_sprite(0, 8, 0);
                break;
            
        }
        delay(100);


    }
}