#include <gb/gb.h>
#include <stdio.h>
#include "BasicBackground.c"
#include "BasicBackgroundMap.c"

void main(){                                    // ecran de la gameboy : 160 par 144 pixels
    set_bkg_data(0, 7, BackgroundTiles);        // changer dans la memoire de background "0" les 7 premiere tiles de "backgroundtiles"
    set_bkg_tiles(0, 0, 40, 18, BackgroundMap); // définition de la position "(0;0)", de la taille du background "(40*18tiles)" et de sa disposition "BackgroundMap".

    SHOW_BKG;
    DISPLAY_ON;

    while(1){
        scroll_bkg(1,0);    // deplace de +x et/ou +y le background
        delay(100);
    }



}