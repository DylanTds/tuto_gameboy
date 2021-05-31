#include <gb/gb.h>
#include <stdio.h>


INT8 playerlocation[2];         // stoque les coordonnees du personnage dans un tableau a deux colonnes
BYTE jumping;                   // 0 ou 1 / vrai ou faux pour connaitre l'etat du personnage
UINT8 gravity = -2;             // valeur qui permet de definire la vitesse de montee ou chute
UINT8 currSpeedY;               // vitesse de montee ou chute


// sprite random a utiliser comme hero
unsigned char Hero[] =
{
  0xFF,0xFF,0xFF,0x81,0xFF,0xA5,0xFF,0xC3,
  0x66,0x5A,0xFF,0x81,0x7E,0x42,0x7E,0x7E,
  0x7E,0x7E,0xFF,0x81,0xFF,0xA5,0xFF,0xA5,
  0xFF,0xC3,0x7E,0x5A,0xFF,0x99,0x7E,0x42
};


/* 
    la fonction delay utilise beaucoup de ressource CPU
    donc on va faire une fontion alternative qui va simplement
    attendre un certain nombre ("numloops") de rafraichissement d'ecran
*/
void perfDelay(UINT8 numloops){ //attends un certain nombre de rafraichissement d'ecran
    UINT8 i;
    for( i = 0; i < numloops; i++){
        wait_vbl_done();
    }
}


void jump(){
    
    if( (jumping == 1) && (currSpeedY == -12)){  // si on attein la vitesse max de chute..
        jumping     = 0;                        // on arrete le deplacement horizontal (dans ce cas, on chute..)
    }

    else {
        if(jumping == 0){
            jumping     = 1;    // alors hero est en train de sauter
            currSpeedY  = 14;   // reinitialisation de la vitesse de montee a son max
        }

        // on deduit la gravite (2) a la vitesse Y (10) a chaque boucle pour reduire la taille du saut
        // a un moment la vitesse Y va etre neutre (0) donc le saut et a son apogee..
        // puis negative (-2), le personnage va redescendre en accelerant a chaque boucle jusqu'a (-12)
        currSpeedY += gravity;

        playerlocation[1] -= currSpeedY; //8
    }

    move_sprite(0, playerlocation[0], playerlocation[1]);
}


void main(){
    playerlocation[0]   = 84;       // position X
    playerlocation[1]   = 80;       // position Y
    jumping             = 0;        // hero n'est pas en train de sauter
    
    set_sprite_data(0, 2, Hero);
    set_sprite_tile(0, 0);
    move_sprite(0, playerlocation[0], playerlocation[1]);

    DISPLAY_ON;
    SHOW_SPRITES;

    while(1){
        if( (joypad() & J_B) || (jumping == 1) ){
            jump();
        }
        if(joypad() & J_LEFT){
            playerlocation[0] -= 4;
            move_sprite(0, playerlocation[0], playerlocation[1]);
        }
        if(joypad() & J_RIGHT){
            playerlocation[0] += 4;
            move_sprite(0, playerlocation[0], playerlocation[1]);
        }

        perfDelay(5);
    }
}