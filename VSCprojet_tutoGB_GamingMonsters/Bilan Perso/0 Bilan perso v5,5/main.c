//7h
#include <gb/gb.h>
#include <stdio.h>

#include "HeroTuiles.c"
#include "ObjetTuiles.c"

#include "Level01Tuiles.c"
#include "Level01Map.c"
#include "Level01MapComplet.c"

UINT8 i;
UBYTE Goal;             // savoir si la partie est finie

// deplacement du personnage
UINT8 playerXY[2];      // coords joueur
UINT8 playerDim[2];     // dimentions joueur

// variables pour charger une tres gande map avec scrolling + chargement
UINT16 col_scrolled;    // col_scrolled
UINT16 stage_width;     // pour savoir si il reste des choses a afficher
UINT16 next_vram;       // position pour la col a charger
BYTE fin_scroll;        // on arrive a la fin du niveau
UINT16 taille_Map;      // taille du niveau en tuiles

// stop tout pendant un certain nombre de rafraichissement de l'ecran
void perfDelay(UINT8 numloops){ UINT8 i; for( i = 0; i < numloops; i++){ wait_vbl_done(); } }

// verification pour savoir si on sort de l'ecran GB
UBYTE dansEcran(UINT8 x, UINT8 y){ return (0+playerDim[0] <= x) && (x <= 152+playerDim[0]) && (8+playerDim[1] <= y) && (y <= 144+playerDim[1]); }

void main(){
    // charge les tuiles et la map du labyrinth et l'injecte dans bakground
    set_bkg_data(0, 37, Level01Tuiles);
    set_bkg_tiles(0,0, 32,18, Level01Map);
    // charge les 7 tuiles des objets
    set_sprite_data(1, 7, ObjetTuiles);
    //set_sprite_tile(1, 0);

    // charge les 4 tuiles du personnage
    set_sprite_data(0, 4, HeroTuiles);
    set_sprite_tile(0, 0);
    // definit coords joueur (milieu ecran)
    playerXY[0] = 10*8;
    playerXY[1] = 9*8;
    move_sprite(0, playerXY[0], playerXY[1]);
    // dimentions joueur
    playerDim[0] = 8;
    playerDim[1] = 8;

    // pour les commandes

    // termine la boucle de jeu
    Goal = 0;

    // initialise les variables pour le scroll
    taille_Map      = (sizeof(Level01MapComplet) / sizeof(Level01MapComplet[0]));
    fin_scroll      = 0;
    col_scrolled    = 32;
    next_vram       = 0;

    SHOW_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;
    
    while (!Goal){
        switch(joypad()){
            case J_SELECT:
                Goal =1;
            break;

            case J_UP:
                if(dansEcran(playerXY[0], playerXY[1]-8)){
                    playerXY[1] -= 8;
                    move_sprite(0, playerXY[0], playerXY[1]);
                }
            break;

            case J_DOWN:
                if(dansEcran(playerXY[0], playerXY[1]+8)){
                    playerXY[1] += 8;
                    move_sprite(0, playerXY[0], playerXY[1]);
                }
            break;

            case J_LEFT:
                if(dansEcran(playerXY[0]-8, playerXY[1])){
                    playerXY[0] -= 8;
                    move_sprite(0, playerXY[0], playerXY[1]);
                }
            break;

            case J_RIGHT:
                if(dansEcran(playerXY[0]+8, playerXY[1])){
                    // si le joueur touche ou penetre le coté droit de l'ecran
                    if((playerXY[0]+8 >= 126) && (!fin_scroll)){
                        // on defille le background plutot que de deplacer le perso
                        scroll_bkg(8,0);

                        // on scroll de 8px (donc une colonne entiere)
                        if (col_scrolled < taille_Map) {// on verifie qu'il reste des colonnes a charger   
                            //on charge la prochaine colonne de la map en fonction du nombre de colonne chargees
                            set_bkg_tiles(next_vram,0, 1,18, Level01MapComplet[col_scrolled]);
                            // on place les 18 tuiles de la colonne chargee dans le background
                            for(i=0; i<18; i++){
                                Level01Map[(i * 32) + next_vram] = Level01MapComplet[col_scrolled][i];
                            }
                        }
                        //on ajoute 1 au total des colonnes chargees
                        col_scrolled += 1; 
                        // verifie qu'on a bien affiché toute les colonnes du map.
                        if (col_scrolled - 12 == taille_Map) { 
                            // arret du scoll vers la droite et du chargement de colonnes
							fin_scroll = 1;
						}
                        //on actualise le numero pour ne pas copier toujours au meme endrois
						next_vram += 1;
                        //on remet a zero si jamais il y a d'autres colonnes a afficher
						if (next_vram == 32) {
							next_vram = 0;
						}

                    }
                    else{
                        // sinon on deplace le perso
                        playerXY[0] += 8;
                        move_sprite(0, playerXY[0], playerXY[1]);
                    }
                }
            break;
        }
        perfDelay(5);
    }
    // message de fin
    printf("\n \n \n \n \n \n \n \n \n== FIN DU NIVEAU  ==");
}