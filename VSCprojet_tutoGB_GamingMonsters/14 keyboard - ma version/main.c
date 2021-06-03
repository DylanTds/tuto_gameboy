#include <gb/gb.h>
#include <stdio.h>
#include "SplashscreenMap.c"
#include "SplashscreenTiles.c"
#include "keyboardMap.c"
#include "keyboardTiles.c"
#include "CursorTiles.c"
#include "Cursor.c"

UINT8 i;
struct Cursor souris;   // curseur pour le clavier
UBYTE keydown;          // utilisateur appuie encore sur un bouton ou non
UINT8 txt[11];          // txt saisi par l'utilisateur
UINT8 txtNbr;           // nombre de caracteres saisi
UBYTE txtConf;          // fin de la saisie

void perfDelay(UINT8 numloops){ UINT8 i; for( i = 0; i < numloops; i++){ wait_vbl_done(); } }

void fadeOut(){ for(i=0; i<3; i++){ switch (i){ case 0: BGP_REG = 0xF9; break;
                                                case 1: BGP_REG = 0xFE; break;
                                                case 2: BGP_REG = 0xFF; break; }
                                    perfDelay(5); } }

void fadeIn(){ for(i=0; i<3; i++){ switch (i){  case 0: BGP_REG = 0xFE; break; 
                                                case 1: BGP_REG = 0xF9; break;
                                                case 2: BGP_REG = 0xE4; break; }
                                    perfDelay(5); } }

// met fin a la saisie de text
void TextConf(){ txtConf = 1; }

// affiche, via les tuiles de background, le text saisi dans "txt[]"
void TextVisualOutput(){
    // coords (tuiles) : (1;4) | dimension (tuiles) : 11*1 | les valeur de "txt[]"
    set_bkg_tiles(7,2, 11, 1, txt);
}

// supprime la derniere lettre saisie 
void TextDel(){
    // mais seulement si il y a deja quelque chose dans txt[]
    if(txtNbr == 0) return;
    // efface la derniere lettre saisie dans "txt[]"
    txt[txtNbr-1] = 47;
    //actualise l'affichage des lettres saisies
    TextVisualOutput();
    //actualise le nombre de lettres saisies
    txtNbr --;
}

// saisi une lettre via l'id de la tuile actuellement sous la souris
void TextAdd(struct Cursor* souris){
    UINT8 keyboardIndex;
    // si le tableau "txt[]" n'est pas plein (0-11)
    if(txtNbr == 11) return;
    // position de la souris en tuiles
    keyboardIndex = (souris->row * 9) + ((souris->col)+1);
    // sauvegarde la valeur de la tuile dans "txt[]" par ordre de saisie
    txt[txtNbr] = keyboardIndex;
    //actualise l'affichage des lettres saisies
    TextVisualOutput();
    //actualise le nombre de lettres saisies
    txtNbr ++;
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

// verification pour savoir si on sort du clavier ou non
UBYTE dansClavier( UINT8 x, UINT8 y){ return (14 <= x) && (x <= 142 ) && (54 <= y) && (y <= 118 ); }

void main(){
    set_bkg_data(0, 101, SplashscreenTiles);    // splash
    set_bkg_tiles(0, 0, 20, 18, SplashscreenMap);
    
    SHOW_SPRITES;                               // preparations
    SHOW_BKG;
    DISPLAY_ON;

    waitpad(J_START);                           // affiche le clavier
    fadeOut();
    set_bkg_data(0, 64, keyboardTiles);         // clavier
    set_bkg_tiles(0, 0, 20, 18, keyboardMap);
    scroll_bkg(0, 0, 4);
    fadeIn();
    
    for(i=0; i<11; i++){ txt[i] = 47; } // tableau pour le text saisi

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

    // montre que l'on a bien fini l'edition de text
    HIDE_SPRITES;
    fadeOut();
}