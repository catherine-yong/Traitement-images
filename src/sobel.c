#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "sobel.h"


int main(){
    char* nom_fic = "images/tournesol.jpeg";
    charger_image_data(nom_fic);//entrée de l'image 1
    filtre_sobel(); //filtre sobel appliqué à l'image 1
    save_image(nom_fic);// sortie de limage 2
    
    return 0;
}
