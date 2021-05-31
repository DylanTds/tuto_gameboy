#include <gb/gb.h>
#include <stdio.h>
#include <gb/font.h>

#include "HeroSprite.c"

#include "WM_startscreen.c"
#include "WM_commandscreen.c"
#include "WM_msgbox.c"

#include "BackgroundTiles.c"
#include "BackgroundMap.c"

BYTE started;       // jeu demarre ou non
BYTE paused;        // jeu en pause ou non
font_t police_char;

BYTE  jumping;
UINT8 playerlocation[2];
UINT8 currSpeedY;
UINT8 gravity = -2;

INT8 roll;         // compteur pour les roues du skate
INT8 rollDirect;   // orientation des roues du skate

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
    if( (56  < playerlocation[0]) && (playerlocation[0] < 104) &&
        (16 < playerlocation[1]) && (playerlocation[1] < 152) ){ 
        // ne rien faire
    }else{                              // si le sprite sort de l'ecran..
        if(playerlocation[0] < 56){      // rep. du coté gauche de l'ecran (8px)
            playerlocation[0] = 56;
            scroll_bkg(-4,0);
        }
        if(playerlocation[0] > 104){    // rep. du coté droit de l'ecran (160px)
            playerlocation[0] = 104;
            scroll_bkg(4,0);
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
        playerlocation[1]   = 100;
        jumping             = 0;
            
        // chargement des sprite HERO
        set_sprite_data(0, 10, Hero);
        set_sprite_tile(0, 0);      // sprite HERO
        set_sprite_tile(1, 2);      // sprite skate01
        set_sprite_tile(2, 3);      // sprite skate02

        if(joypad() & J_START){    
            started = 1;            // jeu demarre      : oui
            paused = 1;             // jeu mis en pause : oui
            HIDE_SPRITES;
            perfDelay(10);
        }

        roll = 0;                   // compteur pour les roues du skate

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
            
            // charge 40 tiles mais decale de 38eme places a cause de la police
            set_bkg_data(38, 40, BackgroundTiles);
            // position : (0,0) | taille : 32*18 tiles | BackgroundMap
            set_bkg_tiles(0, 0, 32, 18, BackgroundMap);

            // Commandes
            if(joypad() & J_UP){
                //playerlocation[1] -= 4;
            }
            if(joypad() & J_DOWN){
                //playerlocation[1] += 4;
            }
            if(joypad() & J_LEFT){
                playerlocation[0] -= 4;
                rollDirect = -1;
            }
            if(joypad() & J_RIGHT){
               playerlocation[0] += 4;
                rollDirect = 1;
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
            
            // animation des roues du skate
            if(rollDirect==1){
                if( (roll < 6) && (roll >= 0) ){
                    roll += 2;
                    set_sprite_tile(1, (roll+2));      // sprite skate01
                    set_sprite_tile(2, (roll+3));      // sprite skate02
                }else{
                    roll = 0;
                    set_sprite_tile(1, 2);      // sprite skate01
                    set_sprite_tile(2, 3);      // sprite skate02
                }
            }
            if(rollDirect==-1){
                if( (roll > -6) && (roll <= 0) ){
                    roll -= 2;
                    set_sprite_tile(1, (roll+8));      // sprite skate01
                    set_sprite_tile(2, (roll+9));      // sprite skate02
                }else{
                    roll = 0;
                    set_sprite_tile(1, 8);      // sprite skate01
                    set_sprite_tile(2, 9);      // sprite skate02
                }
            }

            

            

            // Verification et repositionnement du sprite Hero
            checkScreenLimit();
            move_sprite(0, playerlocation[0], playerlocation[1]);

            // affichage sprite skate01 et skate02 par rapport au Hero
            move_sprite(1, (playerlocation[0]-4), (playerlocation[1]+8));
            move_sprite(2, (playerlocation[0]+4), (playerlocation[1]+8));

            perfDelay(2);
        }

    }
}