#include <gb/gb.h>
#include <stdio.h>

/*
    il y a 4 chaine audio sur le GB :
    Ch1 -> ondes rectangulaires -> peut augmenter ou diminuer la fréquence pour changer l'audio
    Ch2 -> ondes rectangulaires -> peut pas changer la fréquence
    Ch3 -> table de programation d'ondes -> trop perilleux
    Ch4 -> generateur de bruit -> pour les sfx d'explosion ect
*/

void main(){
    // les trois lignes suivantes doivent etre dans cet ordre :
    NR52_REG = 0x80;    // definit la valeur a 1000 0000 en binaire afin de mettre le son en marche
    NR50_REG = 0x77;    // definit le volume de droite et gauche (on met la valeur au max ici pour du stereo)
    NR51_REG = 0xFF;    // definit les chaines audio a utiliser (ici toutes)

    while(1){
        UBYTE joypad_state = joypad();      // stoquage de la valeur de l'input utilisateur dans la variable

        if(joypad_state){                   // si une action de l'utilisateur est detectee

            NR10_REG = 0x16;                    // j'ai pas compris grand chose, il faudra approfondir !
            NR11_REG = 0x40;
            NR12_REG = 0x73;
            NR13_REG = 0x00;
            NR14_REG = 0xC3;

            delay(1000);
        }
    }
}