#include <gb/gb.h>

// definition de la stucture "GameCharacter"
struct GameCharacter {
    // tabl des ID des 4 sprites composant le metasprite (16x16)
    UBYTE spriteIds[4];
    // coordonnee du metasprite
    UINT8 x;
    UINT8 y;
    // dimensions du metasprite
    UINT8 width;
    UINT8 height;
};
