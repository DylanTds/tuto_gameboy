#include <gb/gb.h>
#include <stdio.h>

// charge
#include "SplashscreenMap13.c"
#include "SplashscreenTiles13.c"

void perfDelay(UINT8 numloops){ UINT8 i; for( i = 0; i < numloops; i++){ wait_vbl_done(); } }

void main(){

    set_bkg_data(0, 101, SplashscreenTiles13);
    set_bkg_tiles(0, 0, 20, 18, SplashscreenMap13);

    SHOW_BKG;
    DISPLAY_ON;

    // ne continue pas tant que le joueur n'appui pas sur le bouton
    // mieux que "if(joypad())"
    waitpad(J_START);

    printf("\n \n nouvelle partie");
}