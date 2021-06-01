#include <gb/gb.h>
#include <stdio.h>

#include "SnailTiles.c"
#include "MazeTiles.c"
#include "MazeMap.c"

// coords joueur
UINT8 playerlocation[2];
// constante qui a la valeur d'une tuile de map vide, pour conparaison
const char blankmap[1] = {0x00};

// pour activer le debug et savoir si la partie est finie
UBYTE debug, Goal;
// savoir si on a recuperer une clef
UINT8 Clef;

void perfDelay(UINT8 numloops){ UINT8 i; for( i = 0; i < numloops; i++){ wait_vbl_done(); } }

// verification via index de la future tile sur laquelle le personnage sera
// c'est un peu comme une partie de bataille navale
UBYTE canPlayerMove(UINT8 newPX, UINT8 newPY){
    // variables
    UINT16 indexTLx, indexTLy, tileIndexTL;
    UBYTE result;

    // deduction de la position sur la map
    indexTLx = (newPX - 8)  / 8;                //  -8 car il est a x=16 au debut
    indexTLy = (newPY - 16) / 8;                // -16 car il est a y=24 au debut
    tileIndexTL = (20 * indexTLy) + indexTLx;

    // permet d'afficher les coords et valeur des tuile que l'on veut rentre interactive
    if(debug){
        //on affiche les coords (x;y)du joueur
        printf("(%u;%u) \n", (UINT16)(newPX), (UINT16)(newPY));
        //on affiche les coords en tuiles de la future tuile et sa valeur
        printf("(%u;%u) -> %u \n", (UINT16)(indexTLx), (UINT16)(indexTLy), (UINT16)(tileIndexTL));
    }

    // on cherche a savoir si la position est bien une tuile "vide" de la map
    result = MazeMap[tileIndexTL] == blankmap[0];

    if((tileIndexTL==321) || (tileIndexTL==285)){       // joueur passe sur une clef
        Clef += 1;       // recuperer la clef
        // remplace la tuile de la clef par une tuile vide de taille 1*1 (8*8px).
        set_bkg_tiles(indexTLx,indexTLy, 1,1, blankmap);
        result = 1;     // joueur peut passer sur la tuile
    }
    else if(((tileIndexTL==263) || (tileIndexTL==339)) && (Clef)){  // joueur passe sur une porte avec une clef
        Clef -= 1;       // perd la clef
        // remplace la tuile de la clef par une tuile vide de taille 1*1 (8*8px).
        set_bkg_tiles(indexTLx,indexTLy, 1,1, blankmap);
        result = 1;     // joueur peut passer sur la tuile

    }
    else if(tileIndexTL==340){  // joueur passe la ligne d'arrivee
        Goal = 1;
    }

    return result;
}

// alternative a movesprite pour animer le deplacement du personage
// pixel par pixel et pas avoir un changement brutal de positions
void moveAnimation(UINT8 spriteIndex, INT8 moveX, INT8 moveY){
    while(moveX!=0){                                     // si il y a un changement dans l'axe X
        scroll_sprite(spriteIndex, (moveX<0 ? -1:1), 0); // si moveX<0 c'est -1 sinon c'est 1 (deplacement du sprite)
        moveX += (moveX<0 ? 1:-1);                       // si moveX<0 c'est +1 sinon c'est -1 (diminue la valeur de moveX)
        wait_vbl_done();                                 // pour eviter les glitch graphiques.. on patiente sagement.. que sa s'affiche
    }
    while(moveY!=0){                                     // pareil mais pour l'axe Y
        scroll_sprite(spriteIndex, 0, (moveY<0 ? -1:1));
        moveY += (moveY<0 ? 1:-1);
        wait_vbl_done();
    }
}



void main(){
    // charge les tuiles et la map du labyrinth et l'injecte dans bakground
    set_bkg_data(0, 5, MazeTiles);
    set_bkg_tiles(0,0, 20,18, MazeMap);

    // charge la tuile du personnage et l'injecte dans sprite 0
    set_sprite_data(0, 1, SnailTiles);
    set_sprite_tile(0, 0);

    // definit coords joueur
    playerlocation[0] = 16;
    playerlocation[1] = 24;

    move_sprite(0, playerlocation[0], playerlocation[1]);

    Goal = 0;
    Clef = 0;

    SHOW_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;
    
    while (!Goal){
        if(joypad()){
            if(joypad() & J_A){// on active l'affichage des mess de debug
                debug = 1;
            }
            // on verifie si le joueur va se deplacer sur une tuile vide
            // si c'est le cas, il se deplace, sinon rien ne se passe
            if(joypad() & J_UP){
                if(canPlayerMove(playerlocation[0], playerlocation[1]-8)){
                    playerlocation[1] -= 8;
                    moveAnimation(0, 0, -8);
                }
            }
            if(joypad() & J_DOWN){
                if(canPlayerMove(playerlocation[0], playerlocation[1]+8)){
                playerlocation[1] += 8;
                    moveAnimation(0, 0, 8);
                }
            }
            if(joypad() & J_LEFT){
                if(canPlayerMove(playerlocation[0]-8, playerlocation[1])){
                playerlocation[0] -= 8;
                moveAnimation(0, -8, 0);
                }
            }
            if(joypad() & J_RIGHT){
                if(canPlayerMove(playerlocation[0]+8, playerlocation[1])){
                playerlocation[0] += 8;
                moveAnimation(0, 8, 0);
                }
            }
            perfDelay(1);
        }
    }
    
    HIDE_SPRITES;
    // message de fin de partie
    printf("\n \n \n \n \n \n \n \n \n====== Bravo  ======");
}