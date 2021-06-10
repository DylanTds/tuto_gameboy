#include <gb/gb.h>
#include <stdio.h>

#include "maps/SplashscreenMap.c"
#include "maps/TitlescreenMap.c"
#include "maps/ScorescreenMap.c"
#include "maps/keyboardMap.c"

#include "tuiles/SplashscreenTiles.c"
#include "tuiles/TitlescreenTiles.c"
#include "tuiles/keyboardTiles.c"
#include "tuiles/CursorTiles.c"

#include "Cursor.c"

extern UINT8 txt[12];  // txt[0-11] -> txt saisi par l'utilisateur | txt[12]-> nbr de char
UINT8 i;
UBYTE keydown;         // utilisateur appuie encore sur un bouton ou non
UBYTE txtValid;         // fin de la saisie

UINT8 souris[4]; // curseur pour le clavier


void perfDelay(UINT8 numloops){ UINT8 ii; for(ii=0; ii<numloops; ii++){ wait_vbl_done(); } }
void fadeOut(){ for(i=0; i<3; i++){ switch (i){ case 0: BGP_REG = 0xF9; break; case 1: BGP_REG = 0xFE; break; case 2: BGP_REG = 0xFF; break; } perfDelay(5); } }
void fadeIn(){ for(i=0; i<3; i++){ switch (i){  case 0: BGP_REG = 0xFE; break; case 1: BGP_REG = 0xF9; break; case 2: BGP_REG = 0xE4; break; } perfDelay(5); } }

// met fin a la saisie de text, si il y a au moins une lettre
void TxtValidation(){ if(txt[12]>=1){ txtValid = 1; } }
// affiche, via les tuiles de background, le text saisi dans "txt[]"
void TextVisualOutput(UINT8 x, UINT8 y){
    if(txt[12] <= 0) return;
    // coords (tuiles) : (1;4) | dimension (tuiles) : 11*1 | les valeur de "txt[]"
    set_bkg_tiles(x,y, txt[12],1, txt);
}
// supprime la derniere lettre saisie 
void TextDel(){
    // mais seulement si il y a deja quelque chose dans txt[]
    if(txt[12] <= 0) return;
    // efface la derniere lettre saisie dans "txt[]"
    txt[txt[12]] = 0;
    // replace la ligne en pointille
    for(i=0; i<11; i++){ set_bkg_tiles((i+8),2, 1,1, keyboardMap); }
    //actualise le nombre de lettres saisies
    txt[12] --;
    //actualise l'affichage des lettres saisies
    TextVisualOutput(8,2);
}
// saisi une lettre via l'id de la tuile actuellement sous la souris
void TextAdd(){
    UINT8 keyboardIndex;
    // si le tableau "txt[]" n'est pas plein (0-11)
    if(txt[12] >= 11) return;
    // position de la souris en tuiles
    keyboardIndex = (souris[3] * 9) + (souris[2]+1);
    // sauvegarde la valeur de la tuile dans "txt[]" par ordre de saisie
    txt[txt[12]] = keyboardIndex;
    //actualise le nombre de lettres saisies
    txt[12] ++;
    //actualise l'affichage des lettres saisies
    TextVisualOutput(8,2);
}
// verifie sur quelle tuile se trouve la souris et change l'action en accord
void KeyboardInput(){
    if((souris[2] == 8) && (souris[3] == 4)){       // valider la saisie
        TxtValidation();
    }else if((souris[2] == 7) && (souris[3] == 4)){ // supprimer une lettre
        TextDel();
    }else if((souris[2] == 6) && (souris[3] == 4)){ // rien
    }else{                                              // ajouter une lettre
        TextAdd();
    }
}
// parcours le tableau et le remplis d'espace
void resetTxt(){ for(i=0; i<11; i++){ txt[i] = 0x00; } txt[12] = 0; }
// verification pour savoir si on sort du clavier ou non
UBYTE dansClavier( UINT8 x, UINT8 y){ return (14 <= x) && (x <= 142 ) && (54 <= y) && (y <= 118 ); }
// maintenant on peu l'appeller quand on veut pour taper un nom ou autre..
void ecranClavier(){
    // affiche le clavier
    set_bkg_data(0, 64, keyboardTiles);
    set_bkg_tiles(0, 0, 21, 18, keyboardMap);
    scroll_bkg(8, 0);
    fadeIn();
    // vide le tab txt[]
    resetTxt();

    // defini la position de base de la souris
    souris[0] = 14; // x
    souris[1] = 54; // y
    souris[2] = 0;  // col
    souris[3] = 0;  // row
    
    // charge les tuiles de la souris
    set_sprite_data(0, 4, cursorTiles);
    // defini les 4 tuiles de la souris
    for(i=0; i<4 ;i++){ set_sprite_tile(i, i); }
    // positionne les 4 tuiles de la souris
    move_sprite(0, souris[0], souris[1]);
    move_sprite(1, souris[0], souris[1]+4);
    move_sprite(2, souris[0]+4, souris[1]);
    move_sprite(3, souris[0]+4, souris[1]+4);

    // continue tant que la saisie n'est pas finie
    while (!txtValid){

        if(keydown){ waitpadup(); keydown = 0; }

        switch(joypad()){   // commandes
            case J_UP:
                if(dansClavier(souris[0], souris[1]-16)){// 54 <= y <= 118
                    souris[1] -= 16;
                    souris[3] --;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 0, -16); }
                }else{
                    souris[1] = 118;
                    souris[3] = 4;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 0, (4*16)); }
                }
                keydown = 1;
                break;

            case J_DOWN:
                if(dansClavier(souris[0], souris[1]+16)){// 54 <= y <= 118
                    souris[1] += 16;
                    souris[3] ++;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 0, 16); }
                }else{
                    souris[1] = 54;
                    souris[3] = 0;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 0, (4*-16)); }
                }
                keydown = 1;
                break;

            case J_LEFT:
                if(dansClavier(souris[0]-16, souris[1])){// 14 <= x <= 142
                    souris[0] -= 16;
                    souris[2] --;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, -16, 0); }
                }else{
                    souris[0] = 142;
                    souris[2] = 8;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, (8*16), 0); }
                }
                keydown = 1;
                break;

            case J_RIGHT:
                if(dansClavier(souris[0]+16, souris[1])){// 14 <= x <= 142
                    souris[0] += 16;
                    souris[2] ++;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 16, 0); }
                }else{
                    souris[0] = 14;
                    souris[2] = 0;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, (8*-16), 0); }
                }
                keydown = 1;
                break;

            case J_A:
                KeyboardInput();
                keydown = 1;
                break;

            case J_B:
                TextDel();
                keydown = 1;
                break;

            case J_START:
                TxtValidation();
                keydown = 1;
                break;
        }
    }
    fadeOut();
    HIDE_SPRITES;       // chache l'inutile 
    scroll_bkg(-8, 0);  // reset la position du background
    return;
}


// affiche le nom et le score
void ecranScore(){
    set_bkg_data(0, 64, keyboardTiles);
    set_bkg_tiles(0, 0, 20, 18, ScorescreenMap);
    TextVisualOutput(1,4);
    TextVisualOutput(1,4);
    fadeIn();
    
    waitpad(J_B);
    fadeOut();
}

void bouclePrincipale(){
    // titlescreen
    set_bkg_data(0, 240, TitlescreenTiles);    
    set_bkg_tiles(0, 0, 20, 18, TitlescreenMap);
    fadeIn();

    waitpad(J_START|J_A);
    fadeOut();

    // si le premier char de txt[] est plus grande que 43..(43 -> 0x2B)
    if((0 <= txt[0]) && (txt[0] >= 0x2B)){
        // afficher le clavier pour que l'utilisateur tape son nom
        ecranClavier();
    }
    ecranScore();
    
    bouclePrincipale();
}

void main(){
    // active la memory bank 01, sur laquelle on peut sauvegarder
    ENABLE_RAM_MBC1;
    
    SHOW_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;

    // splashscreen
    set_bkg_data(0, 101, SplashscreenTiles);    
    set_bkg_tiles(0, 0, 20, 18, SplashscreenMap);
    //
    perfDelay(120);
    fadeOut();

    bouclePrincipale();

    DISABLE_RAM_MBC1;
}