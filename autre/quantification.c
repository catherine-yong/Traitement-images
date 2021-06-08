#include <stdio.h> 
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

int **diviser_gris(SDL_Surface *image, int niveaux)
{
    // on divise les 255 possibilités de gris en niveaux.

    int taille = (int) 255/niveaux;

    int **tab_gris = malloc(niveaux*sizeof(int*)); // tableau qui contient le début et la fin de chaque niveau de gris

    int i;
    int j;
    int k;
    int compteur = 1;
    int index = 0;

    for (i = 0 ; i < niveaux ; i++)
    {
        tab_gris[i] = malloc((taille+100)*sizeof(int));
    }

    for(i=0;i<niveaux;i++)    // on initialise la matrice à 0
    {
        for(j=0;j<taille;j++)
        {
            tab_gris[i][j] = 0;
            printf("%d\t",tab_gris[i][j]);
        }
        printf("\n");
    }

    j = 0;

    for (i = 0 ; i <= 255 ; i++)
    {

        if((compteur%taille == 0) && (j < niveaux)) // cas normal , on ajoute soit le début soit la fin de l'intervalle
        {
            tab_gris[j][index] = compteur;
            j++;
            index ++;
        }

        if((compteur%taille != 0)&& (compteur ))

        if((compteur%taille == 0) && (j == niveaux)) // cas final : le dernier niveau est plus grand que le reste
        {
            for (k = i ; k <= 255 ; k++)
            {
                tab_gris[j][k] = k;
            }

            i = k;
        }


    }

    for(i=0;i<niveaux;i++)    // on initialise la matrice à 0
    {
        for(j=0;j<taille;j++)
        {
            printf("%d\t",tab_gris[i][j]);
        }
        printf("\n");
    }

   return tab_gris;
}


int main()
{
    SDL_version nb;
    SDL_VERSION(&nb);

    char *filename1 = "images/tournesol.jpeg";
    
    SDL_Surface *image1 = IMG_Load(filename1);
    SDL_Surface *copy1 = IMG_Load(filename1);
    
    if (image1 == NULL) 
    {
        fprintf(stderr, "Could not load image: %s\n", IMG_GetError());
        return -1;
    }
    
    if (copy1 == NULL) 
    {
        fprintf(stderr, "Could not load image: %s\n", IMG_GetError());
        return -1;
    }

    diviser_gris(image1,2);

    SDL_FreeSurface(image1);
    SDL_FreeSurface(copy1);

    SDL_Quit();

    return 0;
}
