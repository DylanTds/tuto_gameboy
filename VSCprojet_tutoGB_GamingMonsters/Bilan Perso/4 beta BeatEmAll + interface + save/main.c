// interface
#include <gb/gb.h>
#include <stdio.h>
//
#include "maps/SplashscreenMap.c"
#include "maps/TitlescreenMap.c"
#include "maps/ScorescreenMap.c"
#include "maps/keyboardMap.c"
//
#include "tuiles/SplashscreenTiles.c"
#include "tuiles/TitlescreenTiles.c"
#include "tuiles/keyboardTiles.c"
#include "tuiles/CursorTiles.c"

// jeu
#include <gb/font.h>
//
#include "jeu_tuiles/HeroSprite.c"
#include "jeu_tuiles/BackgroundTiles.c"
//
#include "jeu_maps/WM_commandscreen.c"
#include "jeu_maps/BackgroundMap.c"
#include "jeu_maps/WM_msgbox.c"
#include "jeu_maps/WM_randMessages.c"

// variables interface
extern UINT8 txt[12];  // txt[0-11] -> txt saisi par l'utilisateur | txt[12]-> nbr de char
UINT8 score[6];          // txt[0-5] -> score utilisateur
//
UINT8 i;
UBYTE keydown;         // utilisateur appuie encore sur un bouton ou non
UBYTE txtValid;         // fin de la saisie
//
UINT8 souris[4]; // curseur pour le clavier

// variables jeu
BYTE started;       // jeu demarre ou non
BYTE paused;        // jeu en pause ou non
font_t police_char;
//
BYTE  jumping;
BYTE  moving;
UINT8 playerlocation[2];
UINT8 currSpeedY;
UINT8 gravity = -2;
UINT8 speed = 6;
//
INT8 roll;         // compteur pour les roues du skate
INT8 rollDirect;   // orientation des roues du skate

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
    HIDE_SPRITES;
    // background
    set_bkg_data(0, 64, keyboardTiles);
    set_bkg_tiles(0, 0, 20, 18, ScorescreenMap);
    // affichage nom
    TextVisualOutput(1,4);

    // exemple, score = 001 553
    // 0-> 0x1B   1-> 0x1C   2-> 0x1D   3-> 0x1E   4-> 0x1F
    // 5-> 0x20   6-> 0x21   7-> 0x22   8-> 0x23   9-> 0x24
    score[0] = 0x1B;
    score[1] = 0x1B;
    score[2] = 0x1C;
    score[3] = 0x20;
    score[4] = 0x20;
    score[5] = 0x1E;
    // affichage score
    set_bkg_tiles(13,4, 6,1, score);
    fadeIn();
    
    waitpad(J_B);
    fadeOut();
    SHOW_SPRITES;
}

// evite de sortire de l'ecran
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

// declanche le saut
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

// boucle du jeu
void Game(){
    // cacher "press start"
    for(i=0;i<11;i++){
        if(i!=5){
            move_sprite(i, 0,0);
        }
    }
    fadeIn();
    HIDE_SPRITES;
    SHOW_WIN;
    // Police
    font_init();
    font_set(font_load(font_min));
        
    // chargement des sprite HERO
    set_sprite_data(0, 10, Hero);
    set_sprite_tile(0, 0);      // sprite HERO
    set_sprite_tile(1, 2);      // sprite skate01
    set_sprite_tile(2, 3);      // sprite skate02

    // initialisation des valeurs
    playerlocation[0]   = 84;
    playerlocation[1]   = 100;
    jumping             = 0;
    started = 0;
    paused  = 0;
    roll    = 0;
    //
    started = 1;
    paused = 1;

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
                move_win(7,0); // reset position window
                move_bkg(0,0); // reset position background
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
    SHOW_SPRITES;
    HIDE_WIN;
    fadeOut();
}

// boucle principale
void bouclePrincipale(){
    SHOW_SPRITES;
    SHOW_BKG;
    // titlescreen
    set_bkg_data(0, 193, TitlescreenTiles);    
    set_bkg_tiles(0, 0, 20, 18, TitlescreenMap);
    // press start
    set_sprite_data(0, 21, keyboardTiles);
    //
    set_sprite_tile(0, 16);  // P
    set_sprite_tile(1, 18);  // R
    set_sprite_tile(2, 05);  // E
    set_sprite_tile(3, 19);  // S
    set_sprite_tile(4, 19);  // S
    set_sprite_tile(6, 19);  // S
    set_sprite_tile(7, 20);  // T
    set_sprite_tile(8, 01);  // A
    set_sprite_tile(9, 18);  // R
    set_sprite_tile(10, 20);  // T
    // positionne les tuiles et
    // change le background en dessous
    for(i=0;i<11;i++){
        if(i!=5){
            set_bkg_tiles((4+i),15,  1,1, 0);
            move_sprite(i, (40+(i*8)),136);
        }
    }
    //
    fadeIn();
    //commandes
    waitpad(J_START|J_A);
    for(i=0;i<11;i++){
        if(i!=5){
            move_sprite(i, 200,0);
        }
    }
    waitpadup();

    //jeu
    fadeOut();
    Game();

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