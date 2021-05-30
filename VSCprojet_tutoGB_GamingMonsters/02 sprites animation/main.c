#include <gb/gb.h>
#include <stdio.h>
#include "HeroSprite.c" // importation des sprites de "Hero"

void main(){

    UINT8 currTileIndex = 0;         // variable pour la valeur de la tile de "Hero" que l'on veut assigner au sprite "0"

    set_sprite_data(0, 2, Hero);
    set_sprite_tile(0, 0);
    move_sprite(0, 0, 78);
    SHOW_SPRITES;

    while(1){                       // boucle infinie
        if(currTileIndex==0){        // alterne automatiquement entre les deux valeurs pour la tile
            currTileIndex = 1;
        }
        else{
            currTileIndex = 0;
        }

        set_sprite_tile(0, currTileIndex); // assigne la nouvelle valeur de tile au sprite "0"

        delay(500);                // attends 500 milisecondes avant de passer a la prochaine commande

        scroll_sprite(0, 10, 0);    // positionne le sprite "0" 10 pixel plus à droite (que sa position precedante)
    }
}