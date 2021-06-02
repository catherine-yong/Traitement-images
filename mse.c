#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Color couleur_pixel(const SDL_Surface *image , int X, int Y)
{
    // retourne la quantité de rouge , vert , bleu et opacité pour un pixel de coordonnées (X,Y)

    Uint8 bpp = image->format->BitsPerPixel;

    Uint8 *ppixel = (Uint8 *) image->pixels + Y*(image->pitch) + X*bpp;   // on récupère l'information du pixel choisi
    Uint32 data_pixel = *(Uint32*) ppixel;

    SDL_Color color = {0 , 0 , 0 , 0};  

    SDL_GetRGBA(data_pixel , image->format , &color.r , &color.g , &color.b , &color.a );   // on remplit le contenu de color

    //printf("Le pixel est composé de %d rouge, %d vert et %d bleu et est %d opaque.\n",color.r , color.g , color.b , color.a);

    return color;
}

int *couleur_image(SDL_Surface *image, int *color_image) 
{
    // retourne la quantité de rouge , vert , bleu et opacité pour une image 

    int i; // parcourt en largeur 
    int j; // parcourt en longueur 

    // on récupère la taille de l'image 
    int largeur = image -> w;
    int longueur = image -> h;

    Uint8 bpp = image->format->BitsPerPixel;
    SDL_Color color_pixel = {0x00 , 0x00 , 0x00 , SDL_ALPHA_OPAQUE};    // couleur d'un pixel
    
    int X, Y;


    for (i = 1 ; i < largeur ; i++)
    {
        Y = i;
        for (j = 1 ; j < longueur ; j++)
        {
            X = j;
            
            Uint8 *ppixel = (Uint8 *) image->pixels + Y*image->pitch + X*bpp;   // on récupère l'information du pixel choisi
            Uint32 data_pixel = *(Uint32*) ppixel;

            SDL_GetRGBA(data_pixel , image->format , &color_pixel.r , &color_pixel.g , &color_pixel.b , &color_pixel.a );

            // on ajoute les valeurs obtenues du pixel dans celle de l'image entière
            color_image[0] += color_pixel.r;
            color_image[1] += color_pixel.g;
            color_image[2] += color_pixel.b;
            color_image[3] += color_pixel.a;
        }
        
    }

    for (i = 0 ; i <= 4 ; i++)
    {
        color_image[i] /= (largeur*longueur);
    }

    return color_image;
}


float mse_images(SDL_Surface *image1 , SDL_Surface *image2)
{
    // on a deux images et on souhaite calculer leur mse.
    // si les images sont similaires, alors il se rapproche de 0. sinon, il est positif et peut être très grand.

    // les deux images ont la même taille 
    int n = image1 -> w;
    int m = image1 -> h;

    float y = (float) 1/(m*n);
    int i,j;
    float difference_carre = 0.0;

    for (i = 1 ; i < m-4 ; i++)
    {
        for (j = 1 ; j < n-4 ; j++)
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

int main()
{
    SDL_version nb;
    SDL_VERSION(&nb);

    char *filename1 = "images/red.jpg";
    char *filename2 = "images/pepers.jpg";
    
    SDL_Surface *image1, *converted1;
    SDL_Surface *image2, *converted2;

    image1 = IMG_Load(filename1);
    image2 = IMG_Load(filename2);
    
    if ((image1 == NULL) || (image2 == NULL)) 
    {
	fprintf(stderr, "Could not load image: %s\n", IMG_GetError());
	return -1;
    }

    float mse = mse_images(image1 , image2);

    printf("mse = %f\n",mse);

    SDL_Quit();

    return 0;
}