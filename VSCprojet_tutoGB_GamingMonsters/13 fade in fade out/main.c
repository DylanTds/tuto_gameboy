#include <gb/gb.h>
#include <stdio.h>

// charge
#include "SplashscreenMap13.c"
#include "SplashscreenTiles13.c"
#include "hommageMap.c"
#include "hommageTiles.c"

// variable pour fadein ou fadeout
UINT8 i;

void perfDelay(UINT8 numloops){ UINT8 i; for( i = 0; i < numloops; i++){ wait_vbl_done(); } }

// background -> ecran noir
// on modifie background palette en passant, petit a petit, les 4 couleurs au noir
void fadeOut(){
    for(i=0; i<3; i++){
        switch (i){
            case 0:
                BGP_REG = 0xF9; // palette 0xF9 -> | noir  | noir  | grisF | grisC |
                break;
            case 1:
                BGP_REG = 0xFE; // palette 0xFE -> | noir  | noir  | noir  | grisF |
                break;
            case 2:
                BGP_REG = 0xFF; // palette 0xFF -> | noir  | noir  | noir  | noir  |
                break;
        }
        perfDelay(25);
    }
}

//  ecran noir -> background
// on modifie background palette du noir vers les 4 nuances de gris
void fadeIn(){
    for(i=0; i<3; i++){
        switch (i){
            case 0:
                BGP_REG = 0xFE; // palette 0xFE -> | noir  | noir  | noir  | grisF |
                break;
            case 1:
                BGP_REG = 0xF9; // palette 0xF9 -> | noir  | noir  | grisF | grisC |
                break;
            case 2:
                BGP_REG = 0xE4; // palette 0xE4 -> | noir  | grisF | grisC | blanc |
                break;
        }
        perfDelay(25);
    }
}

void main(){

    // premier splash
    set_bkg_data(0, 101, SplashscreenTiles13);
    set_bkg_tiles(0, 0, 20, 18, SplashscreenMap13);

    SHOW_BKG;
    DISPLAY_ON;

    // ne continue pas tant que le joueur n'appui pas sur le bouton
    // mieux que "if(joypad())"
    waitpad(J_START);

    // fade to black
    fadeOut();
    // changement de splash
    set_bkg_data(0, 245, hommageTiles);
    set_bkg_tiles(0, 0, 20, 18, hommageMap);
    // fade to normal
    fadeIn();

    while (1){
        // jeu 
    }
}