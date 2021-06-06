#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "sobel.h"

void filtre_sobel(){
    //def filtre de sobel à l'horizontal
    int poids[3][3] = {{-1,0,1},
        {-2,0,2},
        {-1,0,1}
    };
    double val_pix, min, max;
    min = DBL_MAX; //??
    max = -DBL_MAX;
    for (int y = 1; y<y_size1 - 1;y++){
        for(int x = 1;x<x_size1 - 1; x++){
            val_pix = 0.0;
            for (int j = -1;j <= 1;j++){
                for(int i = -1; i <= 1;i++){
                    val_pix +=poids[j+1][i+1]*image1[y+j][x+i];
                }
            }
            if (val_pix<min)
                min = val_pix;
            if (val_pix>max)
                max = val_pix;
        }
    }
    if((int)(max-min)==0){
        printf("Il n'y a rien\n");
        exit(1);
    }
    // initialise la deuxieme image
    x_size2 = x_size1;
    y_size2 = y_size1;
    for (int y = 0; y<y_size2;y++){
        for (int x = 0; x<x_size2;x++){
            image2[y][x] = 0;
        }
    }
    //genere la deuxieme image apres transformation lineaire
    for (int y = 1; y<y_size1-1 ;y++){
        for(int x = 1; x < x_size1-1;x++){
            val_pix = 0.0;
            for (int j = -1; j<= 1;j++){
                for(int i = -1;i<=1;i++){
                    val_pix += poids[j+1][i+1]*image1[y+j][x+i];
                }
            }
            val_pix = MAX_LUM *(val_pix - min) / (max - min);
            image2[y][x] = (unsigned)val_pix;
        }
    }
}


int main(){
    char* nom_fic = "images/tournesol.jpeg";
    charger_image_data(nom_fic);//entrée de l'image 1
    filtre_sobel(); //filtre sobel appliqué à l'image 1
    save_image(nom_fic);// sortie de limage 2
    
    return 0;
}
