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
UINT16 playerXY[2];      // coords joueur
UINT8 playerDim[2];     // dimentions joueur
UINT8 gravity;          // valeur pour definir la gravitee
UBYTE jumping;           // savoir si le joueur est dans les airs
UBYTE falling;           // savoir si le joueur est dans les airs
UINT8 playerSpeed;      // valeur pour definir la gravitee
UINT8 PlayerTravelled;  // distance en px traversee par le joueur reset a 8
UINT16 collOffset;      // garde en memoire le nombre de col chargee
INT8 speedY;        // vitesse de deplacement vertical (saut / chute)

// variables pour charger une tres gande map avec scrolling + chargement
UINT16 col_scrolled;    // col_scrolled
UINT16 stage_width;     // pour savoir si il reste des choses a afficher
UINT16 next_vram;       // position pour la col a charger
BYTE fin_scroll;        // on arrive a la fin du niveau
UINT16 taille_Map;      // taille du niveau en tuiles

// stop tout pendant un certain nombre de rafraichissement de l'ecran
void perfDelay(UINT8 numloops){ UINT8 i; for( i = 0; i < numloops; i++){ wait_vbl_done(); } }

// on cherche a savoir si on peut bien marcher sur la future tuile
UBYTE canPlayerMove(UINT8 newX, UINT8 newY){
    // not solid -> tuiles qui n'engendrerons pas de collision avec le perso
    const unsigned char NotSolid[4] = {0x00, 0x04, 0x18, 0x19}; // Id des tuiles en Hex
    // on converti les coords du perso en id de tuiles ((x-8)/8) et ((x-16)/8)
    // de cette facon on peut savoir sur quelle colonne et ligne de tuiles se trouve le personnage
    // par contre dans mon cas il faut ajouter "collOffset" a X car je charge des colonnes et je quitte l'ecran vers la droite
    // il faut donc pouvoir decaler ausser vers la droite le tableau de colonnes visee du nombre de colonnes que j'ai charge
    // ensuite il ne faut pas oublier d'ajouter "gravity" a Y sinon la graviter va décaler la zone d'aterrisage du personnage
    // on cherche deja dans la bonne colonne du map et ensuite dans la bonne ligne
    unsigned char TileFromMap[1] = {Level01MapComplet[ ((newX-8)/8)+collOffset ][ ((newY-16+gravity)/8) ]};
    // compare la valeur de la tuile sous le personnage a la valeur des tuiles non solides pour savoir si il y a collision
    return (TileFromMap[0]==NotSolid[0]) || (TileFromMap[0]==NotSolid[1]) || (TileFromMap[0]==NotSolid[2]) || (TileFromMap[0]==NotSolid[3]);
}

/*
    Les sprites sont affiche sur une grille decalee pour pouvoir les faire deborder ou les cacher si besoin
    L'origine n'est donc pas (0;0) mais (8;16) donc pour trouver la tuile en dessous il faut :
    enlever 8 à X ET elever 16 à Y pour connaitre la position (du coin haut droit) du sprite (en pixel).
    Par contre pour trouver leurs position sur la grille de tuile il faut encore diviser par 8 X et Y (en plus du -8 et -16).
    
    donc  hautGauche -> (X-8) (Y-16)                   hautDroit -> ((X-8)+playerDim[0]) (Y-16)
    donc  basGauche  -> (X-8) ((Y-16)+playerDim[1])    basDroit  -> ((X-8)+playerDim[0]) ((Y-16)+playerDim[1])

    Si on detecte un mouvement 
    verifier la collision avec la prochaine tuile directe
    (verifier si on se deplace sur 1 tuile entiere ou on est sur plusieurs tuiles)
    verifier si on va ver le haut OU le bas OU la droite OU la gauche
      si haut utiliser les coords hautGauche et hautDroit
      si bas utiliser les coords basGauche et basdroit
      si droite utiliser les coords hautDroit et basDroit
      si gauche utiliser les coords hautGauche et basGauche
    compter le nombre de tuiles que les coords vont traverser
    pour chaque tuile verifier qu'il n'y a pas collision
     si il y a collision on retourne la position a laquelle le sprite doit s'arreter en milieu de course
     si il n'y a pas collision on va jusqu'au bout du mouvement on retourne les coords de la fin de la course
    



*/






void nextTile(){

}

/*
void Fall(){
    if(falling == 0){
        falling = 1;
        speedY = gravity*2;
    }

    // on reduit a chaque boucle la "vitesse de chute" pour faire une jolie parabole
    speedY = -gravity; // 0 -4 -8 .. -12 -16 -20 -24 ...
    
    if( (canPlayerMove(playerXY[0], playerXY[1]-speedY)) ){
        playerXY[1] -= speedY;
        scroll_sprite(0, 0, -speedY);
    }
    else if( (speedY%8) ){
        // sinon arret net du saut
        jumping = 0;
        falling = 0;
        speedY = 0;
    }
    else{
        playerXY[1] -= (speedY-4);
        scroll_sprite(0, 0, -(speedY-4));
        jumping = 0;
        falling = 0;
        speedY = 0;
    }
}

void Jump(){
    // si on est pas en plein saut, on initialise le saut
    if(jumping == 0){
        jumping = 1;
        speedY = playerDim[1]+gravity;// 8 + 4 = 12
    }

    // on reduit a chaque boucle la "vitesse de chute" pour faire une jolie parabole lors du saut
    speedY -= gravity; // 8 4 0 -4 -8 .. -12 -16 -20 -24 ...
    
    //if( canPlayerMove(playerXY[0], playerXY[1]-speedY) ){} elle est a refaire ! ça ne va pas avec les deplacement de plus ou moin de 8px
    if( speedY > 0 ){
        playerXY[1] -= speedY;
        scroll_sprite(0, 0, -speedY);
    }
    if( (speedY == 0) && J_B ){
        speedY = 4;
        playerXY[1] -= speedY;
        scroll_sprite(0, 0, -speedY);
    }
    else{
        Fall();
    }
}
*/


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
    // dimentions joueur
    playerDim[0] = 8;
    playerDim[1] = 8;
    // definit coords joueur (en tuiles)
    playerXY[0] = (1*8) +8;    // 8
    playerXY[1] = (10*8) +16;   // 16
    // placement du joueur
    move_sprite(0, playerXY[0], playerXY[1]);

    // pour les commandes
    gravity = 4;
    jumping = 0;
    falling = 0;
    playerSpeed = 8;// course -> 8
    PlayerTravelled = 0;
    collOffset = 0;
    speedY = 0;

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
    
    while (Goal==0){
        /*
        if (falling == 1){
            Fall();
        }
        if( (joypad() & J_B) || (jumping == 1) ){
            Jump();
        }
        if( joypad() & J_LEFT ){
            if(canPlayerMove(playerXY[0]-playerSpeed, playerXY[1])){
                playerXY[0] -= playerSpeed;
                scroll_sprite(0, -playerSpeed, 0);
                falling = 1;
            }
        }
        */
        if( joypad() & J_RIGHT ){
            if(canPlayerMove(playerXY[0]+playerSpeed, playerXY[1])){
                // si le joueur touche ou penetre le coté droit de l'ecran
                if((playerXY[0]+playerSpeed >= 126) && (!fin_scroll)){
                    // on defille le background plutot que de deplacer le perso
                    scroll_bkg(playerSpeed,0);
                    falling = 1;
                    PlayerTravelled += playerSpeed;
                    // evite que le nouveau map s'affiche dans la partie visible de l'ecran,
                    // car le joueur ne bouge pas forcement de 8px d'un coup (8px -> 1 colonne)
                    if(PlayerTravelled>=8){
                        if (col_scrolled < taille_Map) {// on verifie qu'il reste des colonnes a charger   
                            //on place la prochaine colonne de la map en fonction du nombre de colonne chargees
                            set_bkg_tiles(next_vram,0, 1,18, Level01MapComplet[col_scrolled]);
                            // on place les 18 tuiles les une sous les autres dans le background
                            for(i=0; i<18; i++){
                                Level01Map[(i * 32) + next_vram] = Level01MapComplet[col_scrolled][i];
                            }
                        }
                        //on ajoute 1 au total des colonnes chargees
                        collOffset ++;
                        col_scrolled += 1;
                        // verifie si on a affiché toute les colonnes du map.
                        if (col_scrolled - 12 == taille_Map) { 
                            // arret du scoll vers la droite ET du chargement de colonnes
                            fin_scroll = 1;
                        }
                        //on actualise le numero pour ne pas copier toujours au meme endrois
                        next_vram += 1;
                        //on remet a zero si jamais on sort des 32 colonnes de "l'ecran"
                        if (next_vram == 32) {
                            next_vram = 0;
                        }
                        // reset pour continuer la verification
                        PlayerTravelled = 0;
                    }
                }
                else{
                    // sinon on deplace le perso
                    playerXY[0] += playerSpeed;
                    scroll_sprite(0, playerSpeed, 0);
                    falling = 1;
                }
            }
        }
        perfDelay(5);
    }
    HIDE_SPRITES;
    // fin
}