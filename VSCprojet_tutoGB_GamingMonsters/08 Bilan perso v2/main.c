#include <gb/gb.h>
#include <stdio.h>
#include <gb/font.h>
#include "WM_startscreen.c"
#include "WM_commandscreen.c"
#include "WM_msgbox.c"

BYTE started;       // jeu demarre ou non
BYTE paused;        // jeu en pause ou non
font_t police_char;

BYTE  jumping;
UINT8 playerlocation[2];
UINT8 currSpeedY;
UINT8 gravity = -2;

// sprite hero
unsigned char Hero[] =
{
  0xFF,0xFF,0xFF,0x81,0xFF,0xA5,0xFF,0xC3,
  0x66,0x5A,0xFF,0x81,0x7E,0x42,0x7E,0x7E,
  0x7E,0x7E,0xFF,0x81,0xFF,0xA5,0xFF,0xA5,
  0xFF,0xC3,0x7E,0x5A,0xFF,0x99,0x7E,0x42
};


void perfDelay(UINT8 numloops){
    UINT8 i;
    for( i = 0; i < numloops; i++){
        wait_vbl_done();
    }
}


/*
    ecran GB horizontal (x): 0px -> 160px
    ecran GB vertical   (y): 0px -> 144px
    une tile a une dimention de 8px
*/
void checkScreenLimit(){
        // si le sprite est dans l'ecran..
    if( (8  < playerlocation[0]) && (playerlocation[0] < 160) &&
        (16 < playerlocation[1]) && (playerlocation[1] < 152) ){ 
        // ne rien faire
    }else{                              // si le sprite sort de l'ecran..
        if(playerlocation[0] < 8){      // rep. du coté gauche de l'ecran (8px)
            playerlocation[0] = 8;
        }
        if(playerlocation[0] > 160){    // rep. du coté droit de l'ecran (160px)
            playerlocation[0] = 160;
        }
        if(playerlocation[1] < 16){      // rep. en haut de l'ecran (8 +8px)
            playerlocation[1] = 16;
        }
        if(playerlocation[1] > 152){    // rep. en bas de l'ecran (144 +8px)
            playerlocation[1] = 152;
        }
    }
}


void jump(){
    if( (jumping == 1) && (currSpeedY == -8)){
        jumping     = 0;
    }else{
        if(jumping == 0){
            jumping     = 1;
            currSpeedY  = 10;
        }
        currSpeedY += gravity;
        playerlocation[1] -= currSpeedY;
    }
}


void main(){
    // Police
    font_init();
    police_char = font_load(font_min);
    font_set(police_char);

    started = 0;    // jeu demarre      : non
    paused  = 0;    // jeu mis en pause : non

    SHOW_WIN;
    HIDE_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;

    while (1){
        // position : (0,0) | taille : 20*18 tiles | WM_startscreen
        set_win_tiles(0, 0, 20, 18, WM_startscreen);    // Ecran Titre

        // initialisation des valeurs
        playerlocation[0]   = 84;
        playerlocation[1]   = 80;
        jumping             = 0;
            
        // sprite HERO
        set_sprite_data(0, 2, Hero);
        set_sprite_tile(0, 0);
        // sprite Dude01
        set_sprite_tile(1, 1);

        if(joypad() & J_START){    
            started = 1;            // jeu demarre      : oui
            paused = 1;             // jeu mis en pause : oui
            HIDE_SPRITES;
            perfDelay(10);
        }

        while(started == 1){
            while(paused == 1){
                // position : (0,0) | taille : 20*18 tiles | WM_commandscreen
                set_win_tiles(0, 0, 20, 18, WM_commandscreen);  // Menu Pause
                move_win(7,0);
                
                perfDelay(10);

                if(joypad() & J_SELECT){    // Select -> quitter vers l'ecran titre
                    started = 0;
                    paused  = 0;
                }
                if(joypad() & J_START ){    // Start  -> quitter la pause
                    perfDelay(20);
                    paused = 0;
                    SHOW_SPRITES;
                    move_win(07,104);        // (144 - (8px * 5tiles))
                }
                
            }


            // Partie en cours.. 
            // position : (0,0) | taille : 20*5 tiles | WM_msgbox
            set_win_tiles(0, 0, 20, 18, WM_msgbox);  // boite de dialogue
            
            // affichage sprite Dude01
            move_sprite(1, 64, 80);

            // Commandes
            if(joypad() & J_UP){
                //playerlocation[1] -= 4;
            }
            if(joypad() & J_DOWN){
                //playerlocation[1] += 4;
            }
            if(joypad() & J_LEFT){
                playerlocation[0] -= 4;
            }
            if(joypad() & J_RIGHT){
               playerlocation[0] += 4;
            }
            if(joypad() & J_A){
                // rien.
            }
            if( (joypad() & J_B) || (jumping == 1) ){
                jump();
            }
            if(joypad() & J_START){
                paused = 1;
                HIDE_SPRITES;
            }
            if(joypad() & J_SELECT){
                // rien.
            }

            // Verification et repositionnement du sprite Hero
            checkScreenLimit();
            move_sprite(0, playerlocation[0], playerlocation[1]);

            perfDelay(5);
        }

    }
}