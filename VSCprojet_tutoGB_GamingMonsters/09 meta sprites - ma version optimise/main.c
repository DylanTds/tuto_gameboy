#include <gb/gb.h>
#include <stdio.h>
#include "MetaSprite.c"     
#include "CharSprites.c"
/*
    gameboy tile designer > "view" > "tile size"
    et on peut choisir entre "8x8px" / "8x16px" / "16x16px" / "32x32px"
*/

// on cree nos deux metasprites
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

void main(){
    // chargement de 8/80 sprites de CharSprite
    set_sprite_data(0, 8, CharSprites);
    // initie les deux metasprites
    setupMetaSprite(&ship,  80,130, 16,16, 0,1,2,3, 0);
    setupMetaSprite(&alien, 30,0,   16,16, 4,5,6,7, 1);

    SHOW_SPRITES;
    DISPLAY_ON;

    while (1){
        // Commandes
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
            if(joypad() & J_A){
                // rien.
            }
            if(joypad() & J_B){
                // rien.
            }
            if(joypad() & J_START){
                // rien.
            }
            if(joypad() & J_SELECT){
                // rien.
            }
            checkScreenLimit();
        }

        // mouvement de l'alien, pour un semblant de fun
        alien.y += 8;
        // alien apparait haut ecran si sort en bas et sur le meme axe que le joueur
        if(alien.y>=160){
            alien.y = 0;
            alien.x = ship.x;
        }
        // actualise la position d'alien
        moveMetaSprite(&alien, alien.x, alien.y);

        // attends 5 rafraichissements
        perfDelay(5);
    }
}