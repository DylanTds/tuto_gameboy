#include <gb/gb.h>
#include <stdio.h>


INT8 playerlocation[2];         // stoque les coordonnees du personnage dans un tableau a deux colonnes
BYTE jumping;                   // 0 ou 1 / vrai ou faux pour connaitre l'etat du personnage

UINT8 currSpeedY;               // vitesse de montee ou chute
UINT8 gravity   = -2;           // valeur qui permet de definire la vitesse de montee ou chute

UINT8 floorYPos = 80;           // definit la position Y d'entree en collision avec le sol


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

/*
    detecte si la position Y donnee est en contact avec le sol ou non
*/
INT8 detecSurface( UINT8 futureYPos ){
    if(futureYPos >= floorYPos){
        return floorYPos;
    }
    return -1;
}


/*
    accelere ou decelere le personage sur l'axe Y pour sauter puis chuter
*/
void jump(){
    INT8 colisionY;

    if(jumping == 0){       // si hero ne saute deja pas actuellement..
        jumping     = 1;    // alors hero est maintenant en train de sauter
        currSpeedY  = 14;   // on initie alors le saut en definissant la vitesse Y a son max
    }

    // on deduit la gravite (2) a la vitesse Y (14) a chaque boucle pour reduire la taille du saut
    // petit a petit, a un moment la vitesse Y va etre neutre (0) donc le saut et a son apogee..
    // puis negative (-2), le personnage va redescendre en accelerant a chaque boucle jusqu'a (-12)
    currSpeedY          += gravity;
    playerlocation[1]   -= currSpeedY;              // actualisation de la position Y avec la vitesse Y (+ou-)

    colisionY = detecSurface(playerlocation[1]);    // verification de collision de la new positon Y par rapport au sol

    if(colisionY > -1){                                         // si il y a collision..
        jumping = 0;                                            // on arrete le saut du personnage
        move_sprite(0, playerlocation[0], colisionY);           // et on aligne le personnage avec le sol
    }else{
        move_sprite(0, playerlocation[0], playerlocation[1]);   // sinon on actualisa la position du sprite Hero
    }
}


void main(){
    playerlocation[0]   = 84;           // position X
    playerlocation[1]   = floorYPos;    // position Y
    jumping             = 0;            // hero n'est pas en train de sauter
    
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