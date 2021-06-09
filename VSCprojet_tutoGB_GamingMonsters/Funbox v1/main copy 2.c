
/*----------------------------------------   SCORE   --------------------------------------------------*/
void ecranScore(){
    set_bkg_data(0, 64, keyboardTiles);
    set_bkg_tiles(0, 0, 20, 18, ScorescreenMap);
    TextVisualOutput(3,3);
    fadeIn();
    while(1){ if(joypad()&J_B){ fadeOut();return; } }
}
/*-----------------------------------------------------------------------------------------------------*/




    // affiche les donnees sauvegardees
    TextVisualOutput(6,0);
    fadeIn();

    while (1){
        if(keydown==1){ waitpadup(); keydown = 0; }
        switch(joypad()){   // commandes
            case J_UP:
                keydown = 1;
                if((souris[0] == 144 ) && (16 <= souris[1]-16) && (souris[1]-16 <= 108 )){// 16 <= y <= 108
                    souris[1] -= 16;
                    souris[2] --;
                    scroll_sprite(0, 0, -16);
                }else{
                    souris[1] = 108;
                    souris[2] = 2;
                    scroll_sprite(0, 0, souris[2]*16);
                }
                break;

            case J_DOWN:
                keydown = 1;
                if((souris[0] == 144 ) && (16 <= souris[1]+16) && (souris[1]+16 <= 108 )){// 16 <= y <= 108
                    souris[1] += 16;
                    souris[2] ++;
                    scroll_sprite(0, 0, +16);
                }else{
                    souris[1] = 16;
                    souris[2] = 0;
                    scroll_sprite(0, 0, souris[2]*-16);
                }
                break;

            case J_A:
                keydown = 1;
                if(souris[2] == 0){
                    fadeOut();
                    HIDE_SPRITES;
                    ecranSave();
                }
                else if(souris[2] == 1){
                    fadeOut();
                    HIDE_SPRITES;
                    return;
                    break;
                }
                else if(souris[2] == 2){
                    fadeOut();
                    HIDE_SPRITES;
                    ecranScore();
                    return;
                }
                break;

            case J_B:
                keydown = 1;
                fadeOut();
                HIDE_SPRITES;
                return;
                break;
        }
    }

