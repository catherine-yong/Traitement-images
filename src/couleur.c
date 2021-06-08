#include "../header/fonctions.h"

// commande pour compiler : gcc differencier_pixels.c -lSDL -lSDL_image

// ====================================== RECUPERER LA COULEUR D'UN PIXEL ====================================

SDL_Color couleur_pixel(const SDL_Surface *image , int X, int Y)
{
    // retourne la quantité de rouge , vert , bleu et opacité pour un pixel de coordonnées (X,Y)

    Uint8 bpp = image->format->BitsPerPixel;

    Uint8 *ppixel = (Uint8 *) image->pixels + Y*(image->pitch) + X*bpp;   // on récupère l'information du pixel choisi
    Uint32 data_pixel = *(Uint32*) ppixel;

    SDL_Color color = {0 , 0 , 0 , 0};  

    SDL_GetRGBA(data_pixel , image->format , &color.r , &color.g , &color.b , &color.a );   // on remplit le contenu de color

    return color;
}

// =============================== NIVEAUX DE GRIS =================

float niv_gris(const SDL_Surface *image , int X, int Y)
{
    SDL_Color pix = couleur_pixel(image,X,Y);

    float gris = (float) 0.2125*pix.r + 0.7154*pix.g + 0.0721*pix.b;

    return gris;

}

// ===================================== GENERALISER SUR UNE IMAGE ENTIERE ========================================

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
    
    int X;
    int Y;
    Uint32 data_pixel;
    Uint32 *ppixel = (Uint32 *) malloc(1000*sizeof(Uint32));

    for (i = 0 ; i < largeur ; i++)
    {
        Y = i;
        for (j = 0 ; j < longueur ; j++)
        {
            X = j;
            
            ppixel = image->pixels + Y*image->pitch + X*bpp;   // on récupère l'information du pixel choisi
            data_pixel = *(Uint32*) ppixel;

            SDL_GetRGBA(data_pixel , image->format , &color_pixel.r , &color_pixel.g , &color_pixel.b , &color_pixel.a );

            // on ajoute les valeurs obtenues du pixel dans celle de l'image entière
            color_image[0] += color_pixel.r;
            color_image[1] += color_pixel.g;
            color_image[2] += color_pixel.b;
            color_image[3] += color_pixel.a;

        }
        free(ppixel);
    }

    for (i = 0 ; i <= 4 ; i++)
    {
        color_image[i] /= (largeur*longueur);
    }


    return color_image;
}


// ===================================== COMPARER LES DEUX IMAGES ===============================================




/*int main() 
{
    SDL_version nb;
    SDL_VERSION(&nb);

    char *filename = "images/pepers.jpg";
    
    SDL_Surface *image, *converted;
    
    image = IMG_Load(filename);
    
    if (image == NULL) {
        fprintf(stderr, "Could not load image: %s\n", IMG_GetError());
        return -1;
    }

    SDL_Color couleur_p = couleur_pixel(image , 100 , 100);
    
    
    //printf("Le pixel est composé de %d rouge, %d vert et %d bleu.\n",couleur_p.r , couleur_p.g , couleur_p.b);

    int color_image[4];
    int i;

    for (i = 0 ; i < 4 ; i++)
    {
        color_image[i] = 0;
    }

    int *couleur_i = couleur_image(image , color_image);
    printf("L'image est composée de %d rouge , %d vert , %d bleu et est %d opaque.\n", couleur_i[0] , couleur_i[1] , couleur_i[2] , couleur_i[3]);
    printf("Niveau de gris : %f\n\n",niv_gris(image, 100, 100));
    free(image);
    return 0;
}*/




