//7h
#include <gb/gb.h>
#include <stdio.h>

#include "HeroTiles.c"

#include "TerrainTiles.c"

#include "TerrainMap.c"
#include "TerrainMapComplet.c"

UINT8 playerXY[2];  // coords joueur
UBYTE keydown;      // utilisateur appuie encore sur un bouton ou non
UBYTE Goal;         // savoir si la partie est finie
UBYTE AuSol;

UINT16 col_scrolled; // col_scrolled
UINT16 stage_width; // pour savoir si il reste des choses a afficher
UINT16 next_vram; // ooooooooooooooooooooo
BYTE fin_scroll;
UINT16 taille_Map;
UINT8 i;


void perfDelay(UINT8 numloops){ UINT8 i; for( i = 0; i < numloops; i++){ wait_vbl_done(); } }

// verification pour savoir si on sort de l'ecran ou non
UBYTE dansEcran( UINT8 x, UINT8 y){ return (8 <= x) && (x <= 160 ) && (16 <= y) && (y <= 136 ); }


void main(){
    // charge les tuiles et la map du labyrinth et l'injecte dans bakground
    set_bkg_data(0, 37, TerrainTiles);
    set_bkg_tiles(0,0, 32,18, TerrainMap);

    // charge la tuile du personnage et l'injecte dans sprite 0
    set_sprite_data(0, 1, HeroTiles);
    set_sprite_tile(0, 0);

    // definit coords joueur
    playerXY[0] = 76;
    playerXY[1] = 136;
    move_sprite(0, playerXY[0], playerXY[1]);

    // termine la boucle de jeu
    Goal = 0;
    AuSol = 1;

    // variables pour le scroll
    taille_Map = (UINT16)(sizeof(TerrainMapComplet) / sizeof(TerrainMapComplet[0]));
    fin_scroll = 0;
    col_scrolled = 32;
    next_vram = 0;

    SHOW_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;
    
    while (!Goal){

        switch(joypad()){
            case J_SELECT:
                Goal =1;
                break;

            case J_LEFT:
                if(dansEcran(playerXY[0]-8, playerXY[1])){
                    // si le joueur touche ou penetre le coté droit
                    playerXY[0] -= 8;
                    move_sprite(0, playerXY[0], playerXY[1]);
                }
                break;

            case J_RIGHT:
                if(dansEcran(playerXY[0]+8, playerXY[1])){
                    // si le joueur touche ou penetre le coté droit
                    if((playerXY[0]+8 >= 126) && (!fin_scroll)){
                        // on defille le background plutot que de deplacer le perso
                        scroll_bkg(8,0);

                        // on scroll de 8px (donc une colonne entiere)
                        if (col_scrolled < taille_Map) {// on verifie qu'il reste des colonnes a charger   
                            //on charge la prochaine colonne de la map en fonction du nombre de colonne chargees
                            set_bkg_tiles(next_vram,0, 1,18, TerrainMapComplet[col_scrolled]);
                            // on place les 18 tuiles de la colonne chargee dans le background
                            for(i=0; i<18; i++){
                                TerrainMap[(i * 32) + next_vram] = TerrainMapComplet[col_scrolled][i];
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
    // message de fin de partie
    printf("\n \n \n \n \n \n \n \n \n====== FIN  ======");
}