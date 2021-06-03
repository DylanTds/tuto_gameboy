#include <gb/gb.h>
#include <stdio.h>
#include "keyboardMap.c"
#include "keyboardTiles.c"
#include "CursorTiles.c"
#include "Cursor.c"

UINT8 i;
// curseur pour le clavier
struct Cursor souris;
// utilisateur appuie encore sur un bouton ou non
UBYTE keydown;
// txt saisi par l'utilisateur
UINT8 txt[11];
UINT8 txtNbr;
// fin de la saisie
UBYTE txtConf;

void perfDelay(UINT8 numloops){ UINT8 i; for( i = 0; i < numloops; i++){ wait_vbl_done(); } }

// background -> ecran noir
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
//  ecran noir -> background
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

// affiche a l'ecran le text saisi dans "txt[]"
void TextVisualOutput(){
    // coords (tuiles) : (1;4) | dimension (tuiles) : 11*1 | les valeur de "txt[]"
    set_bkg_tiles(7,2, 11, 1, txt);
}

// saisi une lettre via l'id de la tuile sous la souris
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

// supprimer la derniere lettre saisie 
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
// met fin a la saisie de text
void TextConf(){
    txtConf = 1;
    // valider la saisie
}

// demande de verifier sur quelle tuile se trouve la souris
// et change de comportement en convenance
void KeyboardInput(struct Cursor* souris){
    if((souris->col == 8) && (souris->row == 4)){       // valider la saisie
        TextConf();
    }else if((souris->col == 7) && (souris->row == 4)){ // supprimer une lettre
        TextDel();
    }else if((souris->col == 6) && (souris->row == 4)){
        // il n'y a pas de touche de clavier ici..
    }else{                                              // ajouter une lettre
        TextAdd(souris);
    }
}

// verification pour savoir si on sort du clavier ou non
// repositionne "souris" si besoin
UBYTE dansClavier( UINT8 x, UINT8 y){
    return (14 <= x) && (x <= 142 ) && (54 <= y) && (y <= 118 );
}

void main(){
    
    SHOW_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;

    fadeOut();

    // clavier
    set_bkg_data(0, 64, keyboardTiles);
    set_bkg_tiles(0, 0, 20, 18, keyboardMap);
    scroll_bkg(0, 0, 4);
    fadeIn();

    // tableau pour le text saisi
    for(i=0; i<11; i++){ txt[i] = 47; }

    // curseur
    set_sprite_data(0, 4, cursorTiles);
    set_sprite_tile(0, 0);
    souris.x = 14;   // +2
    souris.y = 54;   // -16 (par rapport video 80)
    souris.col = 0;
    souris.row = 0;
    move_sprite(0, souris.x, souris.y);
    //
    set_sprite_tile(1, 1);
    move_sprite(1, souris.x, souris.y+4);
    //
    set_sprite_tile(2, 2);
    move_sprite(2, souris.x+4, souris.y);
    //
    set_sprite_tile(3, 3);
    move_sprite(3, souris.x+4, souris.y+4);

    // continuer tant que la saisie n'est pas finie
    while (!txtConf){
        // si on appuie sur un bouton
        if(keydown){
            // attend jusqu'a ce qu'on relache le bouton
            waitpadup();
            // reset keydown on appuie plus sur un bouton
            keydown = 0;
        }

        switch(joypad()){
            case J_UP:
                if(dansClavier(souris.x, souris.y-16)){// 54 <= y <= 118
                    souris.y -= 16;
                    souris.row --;
                    scroll_sprite(0, 0, -16);
                    scroll_sprite(1, 0, -16);
                    scroll_sprite(2, 0, -16);
                    scroll_sprite(3, 0, -16);
                }
                keydown = 1;
                break;

            case J_DOWN:
                if(dansClavier(souris.x, souris.y+16)){// 54 <= y <= 118
                    souris.y += 16;
                    souris.row ++;
                    scroll_sprite(0, 0, 16);
                    scroll_sprite(1, 0, 16);
                    scroll_sprite(2, 0, 16);
                    scroll_sprite(3, 0, 16);
                }
                keydown = 1;
                break;

            case J_LEFT:
                if(dansClavier(souris.x-16, souris.y)){// 14 <= x <= 142
                    souris.x -= 16;
                    souris.col --;
                    scroll_sprite(0, -16, 0);
                    scroll_sprite(1, -16, 0);
                    scroll_sprite(2, -16, 0);
                    scroll_sprite(3, -16, 0);
                }
                keydown = 1;
                break;

            case J_RIGHT:
                if(dansClavier(souris.x+16, souris.y)){// 14 <= x <= 142
                    souris.x += 16;
                    souris.col ++;
                    scroll_sprite(0, 16, 0);
                    scroll_sprite(1, 16, 0);
                    scroll_sprite(2, 16, 0);
                    scroll_sprite(3, 16, 0);
                }
                keydown = 1;
                break;

            case J_A:
                KeyboardInput(&souris);
                keydown = 1;
                break;
        }
    }
    HIDE_SPRITES;
    fadeOut();
}