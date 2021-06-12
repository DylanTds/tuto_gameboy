#include <gb/gb.h>
#include <stdio.h>
#include <gb/font.h>

#include "HeroSprite.c"

#include "WM_startscreen.c"

#include "WM_commandscreen.c"
#include "WM_msgbox.c"
#include "WM_randMessages.c"

#include "BackgroundTiles.c"
#include "BackgroundMap.c"

BYTE started;       // jeu demarre ou non
BYTE paused;        // jeu en pause ou non
font_t police_char;

BYTE  jumping;
BYTE  moving;
UINT8 playerlocation[2];
UINT8 currSpeedY;
UINT8 gravity = -2;
UINT8 speed = 6;

INT8 roll;         // compteur pour les roues du skate
INT8 rollDirect;   // orientation des roues du skate

// side scrolling beat them all
// faire un puzzle game avec les perso boule
// faire un vertical space shooter avec perso etoile
// plateformer
// metroid vania

void perfDelay(UINT8 numloops){ UINT8 ii; for( ii = 0; ii < numloops; ii++){ wait_vbl_done(); }}

void checkScreenLimit(){
        // si le sprite est dans l'ecran..
    if( (40  < playerlocation[0]) && (playerlocation[0] < 120) &&
        (16 < playerlocation[1]) && (playerlocation[1] < 160) ){ 
        // ne rien faire
    }else{                              // si le sprite sort de l'ecran..
        if(playerlocation[0] < 40){      // rep. du coté gauche de l'ecran (8px)
            playerlocation[0] = 40;
            scroll_bkg(-4,0);
        }
        if(playerlocation[0] > 120){    // rep. du coté droit de l'ecran (160px)
            playerlocation[0] = 120;
            scroll_bkg(4,0);
        }
        if(playerlocation[1] < 16){      // rep. en haut de l'ecran (0 +16px)
            playerlocation[1] = 16;
        }
        if(playerlocation[1] > 160){    // rep. en bas de l'ecran (144 +16px)
            playerlocation[1] = 160;
        }
    }
}

void jump(){
    if( (jumping == 1) && (currSpeedY == -8) ){
        jumping = 0;
    }else{
        if( jumping == 0 ){
            jumping    = 1;
            currSpeedY = 10;
        }
        currSpeedY += gravity;
        playerlocation[1] -= currSpeedY;
    }
}


void main(){


    SHOW_WIN;
    HIDE_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;

    while (1){
        // Police
        font_init();
        font_set(font_load(font_min));

        // initialisation des valeurs
        playerlocation[0]   = 84;
        playerlocation[1]   = 100;
        jumping             = 0;
        started = 0;
        paused  = 0;
        roll    = 0;
        
        // chargement des sprite HERO
        set_sprite_data(0, 10, Hero);
        set_sprite_tile(0, 0);      // sprite HERO
        set_sprite_tile(1, 2);      // sprite skate01
        set_sprite_tile(2, 3);      // sprite skate02

        // position : (0,0) | taille : 20*18 tiles | WM_startscreen
        set_win_tiles(0, 0, 20, 18, WM_startscreen);    // Ecran Titre
        move_win(7,0);
        // attend que l'on appuie sur start et que l'on relache le bouton
        waitpad(J_START);
        started = 1;
        paused = 1;
        waitpadup(J_START);

        while(started == 1){
            while(paused == 1){
                HIDE_SPRITES;
                // position : (0,0) | taille : 20*18 tiles | WM_commandscreen
                set_win_tiles(0, 0, 20, 18, WM_commandscreen);  // Menu Pause
                move_win(7,0);

                // commandes
                if( joypad()&J_SELECT ){    // Select -> quitter vers l'ecran titre
                    waitpadup(J_SELECT);
                    paused  = 0;
                    started = 0;
                    move_win(0,0);
                }
                if( joypad()&J_START ){    // Start  -> quitter la pause
                    waitpadup(J_START);
                    paused = 0;
                    SHOW_SPRITES;
                }
            }

            // position : (0,0) | taille : 20*5 tiles | WM_msgbox
            set_win_tiles(0, 0, 20, 18, WM_msgbox);  // boite de dialogue
            move_win(7,112);                        // (144 - (8px * 4tiles))
            
            // charge 37 tiles mais decale a la 38eme places a cause de la police
            set_bkg_data(38, 37, BackgroundTiles);
            // position : (0,0) | taille : 32*18 tiles | BackgroundMap
            set_bkg_tiles(0, 0, 32, 18, BackgroundMap);

            // Commandes
            if(joypad() & J_START){
                paused = 1;
                waitpadup(J_START);
            }
            //
            if( (joypad() & J_B) || (jumping == 1) ){
                jump();
            }
            //
            if(joypad() & J_LEFT){
                moving = 1;
                playerlocation[0] -= speed;
                rollDirect = -1;
            }
            if(joypad() & J_RIGHT){
                moving = 1;
               playerlocation[0] += speed;
               rollDirect = 1;
            }

            // animation des roues du skate
            if((moving==1) && (!jumping)){
                if(rollDirect==1){
                    if( (roll < 6) && (roll >= 0) ){
                        roll += 2;
                        set_sprite_tile(1, (roll+2));
                        set_sprite_tile(2, (roll+3)); 
                    }else{
                        roll = 0;
                        set_sprite_tile(1, 2);
                        set_sprite_tile(2, 3);
                    }
                }else{
                    if( (roll > -6) && (roll <= 0) ){
                        roll -= 2;
                        set_sprite_tile(1, (roll+8));
                        set_sprite_tile(2, (roll+9));
                    }else{
                        roll = 0;
                        set_sprite_tile(1, 8);
                        set_sprite_tile(2, 9);
                    }
                }
            }
            // Verification et repositionnement du sprite Hero
            checkScreenLimit();
            move_sprite(0, playerlocation[0], playerlocation[1]);
            // affichage sprite skate01 et skate02 par rapport au Hero
            move_sprite(1, (playerlocation[0]-4), (playerlocation[1]+8));
            move_sprite(2, (playerlocation[0]+4), (playerlocation[1]+8));
            moving = 0;
            //
            perfDelay(1);
        }

    }
}