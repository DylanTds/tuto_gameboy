#include <gb/gb.h>
#include <stdio.h>
//
#include "ScorescreenMap.c"
#include "SavescreenMap.c"
#include "BienvenueMap.c"
//
#include "keyboardMap.c"
#include "keyboardTiles.c"
//
#include "CursorTiles.c"
// splashscreen
#include "map/SplashscreenMap.c"
#include "map/SplashscreenTiles.c"
// titlescreen
#include "map/TitlescreenMap.c"
#include "map/TitlescreenTiles.c"
// beat them all
//#include "map/beatThemUpTitleScreen_Map.c"
//#include "map/beatThemUpTitleScreen_Tuiles.c"
// puzzle game 02
//#include "map/puzzleGame2TitleScreen_Map.c"
//#include "map/puzzleGame2TitleScreen_Tuiles.c"
// Shoot em up
//#include "map/ShootEmUpTitleScreen_Map.c"
//#include "map/ShootEmUpTitleScreen_Tuiles.c"

extern UINT8 txt[12];   // txt saisi par l'utilisateur [12]-> nbr de char

UINT8 i,ii;
UINT8 souris[4];        // curseur pour le clavier
UBYTE keydown;          // utilisateur appuie encore sur un bouton ou non
UBYTE txtConf;          // fin de la saisie


void perfDelay(UINT8 numloops){
    UINT8 l;
    for(l=0; l<numloops; l++){
        wait_vbl_done();
    }
}

void fadeOut(){ 
    for(i=0; i<3; i++){ 
        switch (i){ 
            case 0: BGP_REG = 0xF9; break;
            case 1: BGP_REG = 0xFE; break;
            case 2: BGP_REG = 0xFF; break;
        }
        perfDelay(5);
    }
}

void fadeIn(){ 
    for(i=0; i<3; i++){
        switch (i){
            case 0: BGP_REG = 0xFE; break;
            case 1: BGP_REG = 0xF9; break;
            case 2: BGP_REG = 0xE4; break;
        }
        perfDelay(5);
    }
}

/*----------------------------------------  CLAVIER  --------------------------------------------------*/
// verification pour savoir si on sort du clavier ou non
UBYTE dansClavier( UINT8 x, UINT8 y){ return (14 <= x) && (x <= 142 ) && (54 <= y) && (y <= 118 ); }

// met fin a la saisie de text, si il y a au moins une lettre
void TextConf(){
    if(txt[12]>=1){
        txtConf = 1;
    }
}

// affiche, via les tuiles de background, le text saisi dans "txt[]"
void TextVisualOutput(UINT8 x, UINT8 y){
    // si txt[] est vide on ne fait rien
    if(txt[12] <= 0){
        return;
    }
    // coords (tuiles) : (1;4) | dimension (tuiles) : 11*1 | les valeur de "txt[]"
    set_bkg_tiles(x,y, txt[12],1, txt);
}

// supprime la derniere lettre saisie 
void TextDel(){
    // si txt[] est vide on ne fait rien
    if(txt[12] <= 0){
        return;
    }

    // efface la derniere lettre saisie (numero "txt[12]") dans "txt[]"
    txt[txt[12]] = 0;
    //actualise le nombre de lettres saisies
    txt[12] --;

    // replace la ligne en pointille
    for(i=0; i<11; i++){
        set_bkg_tiles((i+8),2, 1,1, keyboardMap);
    }

    //actualise l'affichage des lettres saisies
    TextVisualOutput(8,2);
}

// saisi une lettre via l'id de la tuile actuellement sous la souris
void TextAdd(){
    UINT8 keyboardIndex;
    // si txt[] est plein (0-11) on ne fait rien
    if(txt[12] >= 11){
        return;
    }

    // position de la souris en tuiles
    keyboardIndex = (souris[2] * 9) + ((souris[3])+1);

    // sauvegarde la valeur de la tuile dans "txt[]" par ordre de saisie
    txt[txt[12]] = keyboardIndex;

    //actualise le nombre de lettres saisies
    txt[12] ++;

    //actualise l'affichage des lettres saisies
    TextVisualOutput(8,2);
}

// verifie sur quelle tuile se trouve la souris et change l'action en accord
void TextInput(){
    if((souris[3] == 8) && (souris[2] == 4)){
        // valide la saisie dans txt[]
        TextConf();
    }
    else if((souris[3] == 7) && (souris[2] == 4)){
        // supprimer une lettre
        TextDel();
    }
    else if((souris[3] == 6) && (souris[2] == 4)){
        // rien
    }
    else{
        // ajouter la lettre
        TextAdd();
    }
}

// affiche un clavier pour sauvegarder une info
void ecranClavier(){
    // affiche le clavier
    set_bkg_data(0, 64, keyboardTiles);
    set_bkg_tiles(0, 0, 21, 18, keyboardMap);
    scroll_bkg(8, 0);
    fadeIn();

    // parcours le tableau et le remplis d'espace
    for(i=0; i<11; i++){ txt[i] = 0x00; }
    txt[12] = 0;
    
    // defini la position de base de la souris
    souris[0] = 14;
    souris[1] = 54;
    souris[2] = 0;
    souris[3] = 0;
    // charge les tuiles de la souris
    set_sprite_data(0, 4, cursorTiles);
    // defini les 4 tuiles de la souris
    for(i=0; i<4 ;i++){ 
        set_sprite_tile(i, i); 
    }
    // positionne les 4 tuiles de la souris
    move_sprite(0, souris[0], souris[1]);
    move_sprite(1, souris[0], souris[1]+4);
    move_sprite(2, souris[0]+4, souris[1]);
    move_sprite(3, souris[0]+4, souris[1]+4);

    // continue tant que la saisie n'est pas finie
    while (!txtConf){

        if(keydown==1){ waitpadup(); keydown = 0; }

        // commandes
        switch(joypad()){
            case J_UP:
                if(dansClavier(souris[0], souris[1]-16)){// 54 <= y <= 118
                    souris[1] -= 16;
                    souris[2] --;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 0, -16); }
                }else{
                    souris[1] = 118;
                    souris[2] = 4;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 0, (4*16)); }
                }
                keydown = 1;
                break;

            case J_DOWN:
                if(dansClavier(souris[0], souris[1]+16)){// 54 <= y <= 118
                    souris[1] += 16;
                    souris[2] ++;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 0, 16); }
                }else{
                    souris[1] = 54;
                    souris[2] = 0;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 0, (4*-16)); }
                }
                keydown = 1;
                break;

            case J_LEFT:
                if(dansClavier(souris[0]-16, souris[1])){// 14 <= x <= 142
                    souris[0] -= 16;
                    souris[3] --;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, -16, 0); }
                }else{
                    souris[0] = 142;
                    souris[3] = 8;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, (8*16), 0); }
                }
                keydown = 1;
                break;

            case J_RIGHT:
                if(dansClavier(souris[0]+16, souris[1])){// 14 <= x <= 142
                    souris[0] += 16;
                    souris[3] ++;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 16, 0); }
                }else{
                    souris[0] = 14;
                    souris[3] = 0;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, (8*-16), 0); }
                }
                keydown = 1;
                break;

            case J_A:
                TextInput();
                keydown = 1;
                break;

            case J_B:
                TextDel();
                keydown = 1;
                break;

            case J_START:
                TextConf();
                keydown = 1;
                break;
        }
    }
    fadeOut();
    HIDE_SPRITES;
    scroll_bkg(-8, 0);  // reset la position du background
    return;
}

/*-----------------------------------------------------------------------------------------------------*/


/*----------------------------------------   SCORE   --------------------------------------------------*/
void ecranScore(){
    set_bkg_data(0, 64, keyboardTiles);
    set_bkg_tiles(0, 0, 20, 18, ScorescreenMap);
    TextVisualOutput(3,3);
    fadeIn();
    while(1){ if(joypad()&J_B){ fadeOut();return; } }
}
/*-----------------------------------------------------------------------------------------------------*/


/*----------------------------------------   SAVE   --------------------------------------------------*/
void ecranSave(){
    // ecran et tuiles de l'ecran de sauvegarde
    set_bkg_data(0, 64, keyboardTiles);
    set_bkg_tiles(0,0, 20, 18, SavescreenMap);

    // tuiles et la position de la souris
    souris[0] = 144;    // x
    souris[1]= 16+16;      // y
    souris[2] = 0;      // col
    souris[3] = 0;      // row
    set_sprite_data(0, 44, keyboardTiles);
    set_sprite_tile(0, 43);
    move_sprite(0, souris[0], souris[1]);

    // verification si il y a deja des donnee ou non (43 -> 0x2B)
    if( (txt[12]<1) || (0x2B<txt[12]) ){
        // si donnee trops petite ou trop grande on afficher le clavier pour sauver des infos
        fadeIn();
        if(keydown==1){ waitpadup(); keydown = 0; }
        waitpad(J_A);
        keydown = 1;
        fadeOut();
        ecranClavier();
    }
    
    // affiche les donnees sauvegardees
    TextVisualOutput(6,0);
    fadeIn();

    while (1){
        if(keydown==1){ waitpadup(); keydown = 0; }
        switch(joypad()){   // commandes
            case J_UP:
                keydown = 1;
                if((souris[0] == 144 ) && (16 <= souris[1]-16) && (souris[1]-16 <= 108 )){// 16 <= y <= 108
                    souris[1] -= 16;
                    souris[2] --;
                    scroll_sprite(0, 0, -16);
                }else{
                    souris[1] = 108;
                    souris[2] = 2;
                    scroll_sprite(0, 0, souris[2]*16);
                }
                break;

            case J_DOWN:
                keydown = 1;
                if((souris[0] == 144 ) && (16 <= souris[1]+16) && (souris[1]+16 <= 108 )){// 16 <= y <= 108
                    souris[1] += 16;
                    souris[2] ++;
                    scroll_sprite(0, 0, +16);
                }else{
                    souris[1] = 16;
                    souris[2] = 0;
                    scroll_sprite(0, 0, souris[2]*-16);
                }
                break;

            case J_A:
                keydown = 1;
                if(souris[2] == 0){
                    fadeOut();
                    HIDE_SPRITES;
                    ecranSave();
                }
                else if(souris[2] == 1){
                    fadeOut();
                    HIDE_SPRITES;
                    return;
                    break;
                }
                else if(souris[2] == 2){
                    fadeOut();
                    HIDE_SPRITES;
                    ecranScore();
                    return;
                }
                break;

            case J_B:
                keydown = 1;
                fadeOut();
                HIDE_SPRITES;
                return;
                break;
        }
    }









    
}
/*-----------------------------------------------------------------------------------------------------*/



/*----------------------------------------   LOOP   --------------------------------------------------*/
void MAINLoop(){
    // ecran titre
    set_bkg_data(0, 89, TitlescreenTiles);
    set_bkg_tiles(0, 0, 20, 18, TitlescreenMap);
    fadeIn();

    if(keydown==1){ waitpadup(); keydown = 0; }

    // commande
    waitpad(J_A|J_START);
    keydown = 1;
    fadeOut();

    ecranSave();

    // jeu
    set_bkg_data(0, 64, keyboardTiles);
    set_bkg_tiles(0,0, 20, 18, BienvenueMap);
    TextVisualOutput(11,1);
    fadeIn();

    MAINLoop();
}
/*-----------------------------------------------------------------------------------------------------*/


void main(){
    // active la memory bank 01, sur laquelle on peut sauvegarder
    ENABLE_RAM_MBC1;
    
    SHOW_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;

    // splash
    set_bkg_data(0, 101, SplashscreenTiles);    
    set_bkg_tiles(0, 0, 20, 18, SplashscreenMap);

    // commande
    waitpad(J_A|J_START);
    keydown = 1;
    fadeOut();

    MAINLoop();
    
    DISABLE_RAM_MBC1;
}