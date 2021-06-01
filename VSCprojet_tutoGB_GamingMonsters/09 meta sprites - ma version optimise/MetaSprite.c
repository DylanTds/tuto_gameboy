#include <gb/gb.h>

// definition de la stucture "MetaSprite"
struct MetaSprite {
    // tabl des ID des 4 sprites composant le metasprite (16x16)
    UBYTE spriteIds[4];
    // coordonnee du metasprite
    UINT8 x;
    UINT8 y;
    // dimensions du metasprite
    UINT8 width;
    UINT8 height;
};

// fonction generale pour deplacer les 4 sprites du metasprite d'un coup
void moveMetaSprite(struct MetaSprite* mMS, UINT8 x, UINT8 y){
    // le " * " permet d'indiquer un emplacement memoire via un pointer, c'est comme un raccourcis
    move_sprite(mMS->spriteIds[0], x, y);
    move_sprite(mMS->spriteIds[1], x, y+8);
    move_sprite(mMS->spriteIds[2], x+8, y);
    move_sprite(mMS->spriteIds[3], x+8, y+8);
}

// methode pour initier les MetaSprites
void setupMetaSprite(struct MetaSprite* sMS, UINT8 msX, UINT8 msY, UINT8 msW, UINT8 msH, UBYTE msT01, UBYTE msT02, UBYTE msT03, UBYTE msT04, UINT8 nbr){
    // coords et dimensions
    sMS->x      = msX;
    sMS->y      = msY;
    sMS->width  = msW;
    sMS->height = msH;
    // ajout des id des tiles dans le tab
    sMS->spriteIds[0] = msT01;
    sMS->spriteIds[1] = msT02;
    sMS->spriteIds[2] = msT03;
    sMS->spriteIds[3] = msT04;
    // chargement des 4 sprites
    nbr = nbr * 4;
    set_sprite_tile(nbr+0, msT01);      // haut gauche
    set_sprite_tile(nbr+1, msT02);      // bas  gauche
    set_sprite_tile(nbr+2, msT03);      // haut droit
    set_sprite_tile(nbr+3, msT04);      // bas  droit
    // positionnement
    moveMetaSprite(sMS, msX, msY);
}