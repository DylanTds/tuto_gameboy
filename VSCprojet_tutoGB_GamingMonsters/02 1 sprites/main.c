#include <gb/gb.h>
#include <stdio.h>
#include "HeroSprite.c" // importation des sprites de "Hero"

void main(){
    set_sprite_data(0, 2, Hero);    // chargement des donnes du sprites "Hero". On le charge du debut (tile "0") et on charge "2" tiles.
    set_sprite_tile(0, 0);          // creation du premier sprite "0" et lien a la premiere tile de sprite_data ("0").
    move_sprite(0, 88, 78);         // position du premier sprite "0" aux coordonees (x88;y-78). [(x0;y0) -> le coin haut gauche de l'ecran]
    SHOW_SPRITES;                   // demande a la console d'afficher les sprites
}