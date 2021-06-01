#include <gb/gb.h>
#include <stdio.h>
#include "GameCharacter.c"
#include "CharSprites.c"
/*
    pas besoin de paint, dans le gameboy tile designer
    on peut aller dans "view" > "tile size"
    et on choisir entre "8x8" / "8x16" / "16x16" / "32x32"

    c'est bien plus pratique, GamingMonsters devais pas etre au courant..
*/

UINT8 playerlocation[2];

// OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
struct GameCharacter ship;
struct GameCharacter alien;
UBYTE spriteSize = 8;


void perfDelay(UINT8 numloops){ UINT8 i; for( i = 0; i < numloops; i++){ wait_vbl_done(); } }
void checkScreenLimit(){
    if( (8  < playerlocation[0]) && (playerlocation[0] < 160) &&
        (16 < playerlocation[1]) && (playerlocation[1] < 152) ){
    }else{
        if(playerlocation[0] < 8){
            playerlocation[0] = 8;
        }
        if(playerlocation[0] > 160){
            playerlocation[0] = 160;
        }
        if(playerlocation[1] < 16){
            playerlocation[1] = 16;
        }
        if(playerlocation[1] > 152){
            playerlocation[1] = 152;
        }
    }
}


// fonction generale pour deplacer les 4 sprites du metasprite d'un coup
void moveGameCharacter(struct GameCharacter* character, UINT8 x, UINT8 y){
    // le " * " permet d'indiquer un emplacement memoire via un pointer, c'est comme un raccourcis
    move_sprite(character->spriteIds[0], x, y);
    move_sprite(character->spriteIds[1], x, y+spriteSize);
    move_sprite(character->spriteIds[2], x+spriteSize, y);
    move_sprite(character->spriteIds[3], x+spriteSize, y+spriteSize);
}


// ethode pour initier le Ship
void setupShip(){
    // coords et dimensions
    ship.x      = 80;
    ship.y      = 130;
    ship.width  = 16;
    ship.height = 16;
    // chargement des 4 sprites
    set_sprite_tile(0, 0);      // haut gauche
    set_sprite_tile(1, 1);      // haut droit
    set_sprite_tile(2, 2);      // bas  gauche
    set_sprite_tile(3, 3);      // bas  droit
    // ajout des ids dans le tableau
    ship.spriteIds[0] = 0;
    ship.spriteIds[1] = 1;
    ship.spriteIds[2] = 2;
    ship.spriteIds[3] = 3;
    // positionnement
    moveGameCharacter(&ship, ship.x, ship.y);
}


// ethode pour initier le Alien
void setupAlien(){
    // coords et dimensions
    alien.x      = 30;
    alien.y      = 0;
    alien.width  = 16;
    alien.height = 16;
    // chargement des 4 sprites
    set_sprite_tile(4, 4);      // haut gauche
    set_sprite_tile(5, 5);      // haut droit
    set_sprite_tile(6, 6);      // bas  gauche
    set_sprite_tile(7, 7);      // bas  droit
    // ajout des ids dans le tableau
    alien.spriteIds[0] = 4;
    alien.spriteIds[1] = 5;
    alien.spriteIds[2] = 6;
    alien.spriteIds[3] = 7;
    // positionnement
    moveGameCharacter(&alien, alien.x, alien.y);
}


void main(){
    // chargement des 80 sprites de CharSprite
    set_sprite_data(0, 80, CharSprites);
    // initie les deux metasprites
    setupShip();
    setupAlien();

    SHOW_SPRITES;
    DISPLAY_ON;

    while (1){

        // Commandes
        if(joypad() & J_UP){
            ship.y -= 4;
            moveGameCharacter(&ship, ship.x, ship.y);
        }
        if(joypad() & J_DOWN){
            ship.y += 4;
            moveGameCharacter(&ship, ship.x, ship.y);
        }
        if(joypad() & J_LEFT){
            ship.x -= 4;
            moveGameCharacter(&ship, ship.x, ship.y);
        }
        if(joypad() & J_RIGHT){
            ship.x += 4;
            moveGameCharacter(&ship, ship.x, ship.y);
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
        
        // Verification et 
        // checkScreenLimit();

        // mouvement de l'alien pour un semblant d'animation..
        alien.y += 8;
        // alien apparait haut ecran si sort en bas
        if(alien.y>=160){
            alien.y = 0;
            alien.x = ship.x;
        }
        //actualise la position d'alien
        moveGameCharacter(&alien, alien.x, alien.y);

        perfDelay(5);

    }
}