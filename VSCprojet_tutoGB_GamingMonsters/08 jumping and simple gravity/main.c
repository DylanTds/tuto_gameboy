#include <gb/gb.h>
#include <stdio.h>


INT8 playerlocation[2];         // stoquer les coordonnees du sprite dans un tableau a deux entree


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


void main(){
    
    set_sprite_data(0, 2, Hero);
    set_sprite_tile(0, 0);

    playerlocation[0] = 84;     // position X
    playerlocation[1] = 80;     // position Y

    move_sprite(0, playerlocation[0], playerlocation[1]);

    DISPLAY_ON;
    SHOW_SPRITES;

    while(1){
        if(joypad()&J_LEFT){
            playerlocation[0] -= 4;
            move_sprite(0, playerlocation[0], playerlocation[1]);
            perfDelay(5);
        }

        if(joypad()&J_RIGHT){
            playerlocation[0] += 4;
            move_sprite(0, playerlocation[0], playerlocation[1]);
            perfDelay(5);
        }
        

        


    }
}