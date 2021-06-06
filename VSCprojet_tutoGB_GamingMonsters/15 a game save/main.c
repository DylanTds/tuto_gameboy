#include <gb/gb.h>
#include <stdio.h>
#include "SplashscreenMap.c"
#include "SplashscreenTiles.c"
#include "keyboardMap.c"
#include "keyboardTiles.c"
#include "CursorTiles.c"
#include "Cursor.c"
#include "BienvenueMap.c"

// pour en savoir plus sur les banques de sauvegardes : http://www.devrs.com/gb/files/sram.txt

extern UINT8 txt[12];          // txt saisi par l'utilisateur [13]-> nbr de char
UINT8 i;
struct Cursor souris;   // curseur pour le clavier
UBYTE keydown;          // utilisateur appuie encore sur un bouton ou non
UBYTE txtConf;          // fin de la saisie

void perfDelay(UINT8 numloops){ UINT8 ii; for(ii=0; ii<numloops; ii++){ wait_vbl_done(); } }

void fadeOut(){ for(i=0; i<3; i++){ switch (i){ case 0: BGP_REG = 0xF9; break;
                                                case 1: BGP_REG = 0xFE; break;
                                                case 2: BGP_REG = 0xFF; break; }
                                    perfDelay(5); } }

void fadeIn(){ for(i=0; i<3; i++){ switch (i){  case 0: BGP_REG = 0xFE; break; 
                                                case 1: BGP_REG = 0xF9; break;
                                                case 2: BGP_REG = 0xE4; break; }
                                    perfDelay(5); } }

// met fin a la saisie de text, si il y a au moins une lettre
void TextConf(){ if(txt[12]>=1){ txtConf = 1; } }

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
void TextAdd(struct Cursor* souris){
    UINT8 keyboardIndex;
    // si le tableau "txt[]" n'est pas plein (0-11)
    if(txt[12] >= 11) return;
    // position de la souris en tuiles
    keyboardIndex = (souris->row * 9) + ((souris->col)+1);
    // sauvegarde la valeur de la tuile dans "txt[]" par ordre de saisie
    txt[txt[12]] = keyboardIndex;
    //actualise le nombre de lettres saisies
    txt[12] ++;
    //actualise l'affichage des lettres saisies
    TextVisualOutput(8,2);
}

// verifie sur quelle tuile se trouve la souris et change l'action en accord
void KeyboardInput(struct Cursor* souris){
    if((souris->col == 8) && (souris->row == 4)){       // valider la saisie
        TextConf();
    }else if((souris->col == 7) && (souris->row == 4)){ // supprimer une lettre
        TextDel();
    }else if((souris->col == 6) && (souris->row == 4)){ // rien
    }else{                                              // ajouter une lettre
        TextAdd(souris);
    }
}

// parcours le tableau et le remplis d'espace
void resetTxt(){
    for(i=0; i<11; i++){ txt[i] = 0x00; }
    txt[12] = 0;
}

// verification pour savoir si on sort du clavier ou non
UBYTE dansClavier( UINT8 x, UINT8 y){ return (14 <= x) && (x <= 142 ) && (54 <= y) && (y <= 118 ); }

// maintenant on peu l'appeller quand on veut pour taper un nom ou autre..
void ecranClavier(){
    set_bkg_data(0, 64, keyboardTiles);         // affiche le clavier
    set_bkg_tiles(0, 0, 21, 18, keyboardMap);
    scroll_bkg(8, 0);
    fadeIn();

    resetTxt();

    // defini la position de base de la souris
    souris.x = 14;
    souris.y = 54;
    souris.col = 0;
    souris.row = 0;
    // charge les tuiles de la souris
    set_sprite_data(0, 4, cursorTiles);
    // defini les 4 tuiles de la souris
    for(i=0; i<4 ;i++){ set_sprite_tile(i, i); }
    // positionne les 4 tuiles de la souris
    move_sprite(0, souris.x, souris.y);
    move_sprite(1, souris.x, souris.y+4);
    move_sprite(2, souris.x+4, souris.y);
    move_sprite(3, souris.x+4, souris.y+4);

    // continue tant que la saisie n'est pas finie
    while (!txtConf){

        if(keydown){        // si on appuie sur un bouton
            waitpadup();    // attend jusqu'a ce qu'on relache le bouton
            keydown = 0;    // appuie plus sur le bouton
        }

        switch(joypad()){   // commandes
            case J_UP:
                if(dansClavier(souris.x, souris.y-16)){// 54 <= y <= 118
                    souris.y -= 16;
                    souris.row --;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 0, -16); }
                }else{
                    souris.y = 118;
                    souris.row = 4;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 0, (4*16)); }
                }
                keydown = 1;
                break;

            case J_DOWN:
                if(dansClavier(souris.x, souris.y+16)){// 54 <= y <= 118
                    souris.y += 16;
                    souris.row ++;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 0, 16); }
                }else{
                    souris.y = 54;
                    souris.row = 0;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 0, (4*-16)); }
                }
                keydown = 1;
                break;

            case J_LEFT:
                if(dansClavier(souris.x-16, souris.y)){// 14 <= x <= 142
                    souris.x -= 16;
                    souris.col --;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, -16, 0); }
                }else{
                    souris.x = 142;
                    souris.col = 8;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, (8*16), 0); }
                }
                keydown = 1;
                break;

            case J_RIGHT:
                if(dansClavier(souris.x+16, souris.y)){// 14 <= x <= 142
                    souris.x += 16;
                    souris.col ++;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, 16, 0); }
                }else{
                    souris.x = 14;
                    souris.col = 0;
                    for(i=0; i<4 ;i++){ scroll_sprite(i, (8*-16), 0); }
                }
                keydown = 1;
                break;

            case J_A:
                KeyboardInput(&souris);
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
    HIDE_SPRITES;       // chache l'inutile 
    scroll_bkg(-8, 0);  // reset la position du background
    return;
}

// affiche "Bienvenu" + le nom choisi par l'utilisateur
void ecranBienvenue(){
    set_bkg_data(0, 64, keyboardTiles);
    set_bkg_tiles(0, 0, 20, 18, BienvenueMap);
    TextVisualOutput(11,1);
    fadeIn();
}

void main(){
    // active la memory bank 01, sur laquelle on peut sauvegarder
    ENABLE_RAM_MBC1;

    set_bkg_data(0, 101, SplashscreenTiles);    // splash
    set_bkg_tiles(0, 0, 20, 18, SplashscreenMap);
    
    SHOW_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;

    waitpad(J_START);
    fadeOut();

    // par defaut les données vides sont très grandes
    // si la premire donnee du tableau est plus grande que 43 alors ce n'est pas une lettre mais une donnee vide
    // donc on peut afficher le clavier pour que l'utilisateur tape son nom ou autre..
    if((0 <= txt[0]) && (txt[0] >= 0x2B)){ //43 -> 0x2B (le text est formé avec les 43 tiles premiere tiles)
        ecranClavier();
    }
    ecranBienvenue();

    DISABLE_RAM_MBC1;
    
}