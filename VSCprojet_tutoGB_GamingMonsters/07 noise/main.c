#include <gb/gb.h>
#include <stdio.h>



void main(){
    
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;

    while(1){
        UBYTE joypad_state = joypad();

        if(joypad_state){

            // On joue avec la Ch4 maintenant pour generer un noise
            // Utiliser le sound_creator.gb pour se simplifier la vie..
            // une fois le bon sons appuyer sur a+select pour afficher les params a modifier

            NR41_REG = 0x1F;
            // 0001 1111 -> longeure max du son
            /*
                bits 5 a 0 -> la longueur du sons (0 - 0x1F)
            */

            NR42_REG = 0xF1;
            // 1111 0001 -> debut volume max, volume diminue progr, 1 envelope sweep
            /* 
                bit 7 a 4 -> le volume initial de envelope (0 a 0Fh et 0 = pas sons)
                bit 3     -> direction envelope (0 baisse, 1 augmentation)
                bit 2 a 0 -> nbr de sweep de l'envelope (0 a 7 et 0 = stop)
            */

            NR43_REG = 0x30;
            // 0011 0000 -> frequence a 3, step a 0, ratio div. a 0
            /*
                bit 7 a 4 -> frequence de la montre (s)
                bit 3     -> counteur step/width (0 = 15 bits, 1 = 7 bits)
                bit 2 a 0 -> ratio de division des frequences (r)

                l'ampliture alterne aleatoirement entre haut et bas a la frequence donnee.
                une frequence plus elevee donne un sons plus 'doux'
                si le bit 3 est actif certaine frequence ressemblerons plus a des tonalité que du bruit.
            */

            NR44_REG = 0xC0;
            // 1100 0000 -> lance le sons, n'est pas repete en boucle
            /*
                bit 7     -> initier le son (1 = relancer)
                bit 6     -> continuer de jouer le sons en boucle ou non apres la fin de NR41
                             ( 1 = stopper le sons une fois la duree de NR41 est ecoulee)
                bit 5 a 0 -> inutiles
            */

            delay(1000);
        }
    }
}