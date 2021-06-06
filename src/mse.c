#include "fonctions.h"

#define SEUIL 15000

float mse_images(SDL_Surface *image1 , SDL_Surface *image2)
{
    // on souhaite calculer le mse de deux images : si les images sont similaires, alors il se rapproche de 0. sinon, il est positif et peut être très grand.

    // les deux images ont la même taille 
    int n = image1 -> w;
    int m = image1 -> h;

    float y = (float) 1/(m*n);
    int i,j;
    float difference_carre = 0.0;

    for (i = 1 ; i < m-1 ; i++) // on s'arrête avant pour éviter toute confusion avec les bords
    {
        for (j = 1 ; j < n-1 ; j++)
        {
            SDL_Color res_amplitude1 = couleur_pixel(image1 , i,j);
            int r1 = res_amplitude1.r;
            int v1 = res_amplitude1.g;
            int b1 = res_amplitude1.b;

            SDL_Color res_amplitude2 = couleur_pixel(image2 , i,j);
            int r2 = res_amplitude2.r;
            int v2 = res_amplitude2.g;
            int b2 = res_amplitude2.b;

            difference_carre += (float) pow((r1+v1+b1) - (r2+v2+b2),2); // (I - I')^2
        }
    }

    return (float) difference_carre*y;

}

int similaires(SDL_Surface *image1, SDL_Surface *image2)
{
    float mse = mse_images(image1, image2);

    if (mse <= SEUIL)
    {
        printf("Les deux images sont similaires.\nLa MSE est de %f, ce qui est cohérent.\n",mse);
        return 1;
    }

    printf("Les deux images ne sont pas similaires.\nLa MSE est de %f, qui est trop élevée.\n",mse);

    return 0;
}

