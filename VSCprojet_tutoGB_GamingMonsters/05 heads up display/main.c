#include <gb/gb.h>
#include "BasicBackground.c"
#include "BasicBackgroundMap.c"
#include <gb/font.h>                // librairie de polices de charactere
#include "WindowMap.c"

/*
    l'ecran de la gameboy est composé de trois couches :
    Window layer -> Sprite layer -> Background layer
*/

void main(){            
    font_t police_char;                     // variable "police_char" de type "font_t" pour stoquer la police choisie
    
    font_init();                            // initialiser la librairie de polices
    police_char = font_load(font_min);      // chargement de la police "font_min" dans la variable "police_char"
    
    font_set(police_char);                  // definition de la police de charactere a utiliser par la suite


    /*
        min_font est composée de 36 tiles allant de A a Z etc
        les font et le background partagent leurs tiles, il faut donc les décaler !
        sinon les tiles du background vont remplacer une partie des tiles de font

        on va donc changer dans la memoire de background "0" les 7 premiere tiles de "backgroundtiles"
        mais on va les stoquer a partir de la 37eme position pour laisser tranquille les 36 tiles de font

        attention, il faudra aussi re-exporter le basicBakgroundMap.c avec un "tile offset" de 37
    */

    set_bkg_data(37, 7, BackgroundTiles);       // on charge les 7 tiles mais on stoque a partir de la 37eme place
    set_bkg_tiles(0, 0, 40, 18, BackgroundMap);

    set_win_tiles(0, 0, 8, 1, WindowMap);       // positionne la window a (0;0), avec 8*1tiles affichee et son tilesmap "WindowMap"
    move_win(7, 120);                           // le window layer n'est pas transparent, il faut le decaler sinon plus de background

    SHOW_BKG;
    SHOW_WIN;
    DISPLAY_ON;

    while(1){
        scroll_bkg(1,0);
        delay(100);
    }

}