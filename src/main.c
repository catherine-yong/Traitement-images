#include "../header/fonctions.h"
#include "../header/sobel.h"

int main()
{
    SDL_version nb;
    SDL_VERSION(&nb);

    char *filename1 = "images/pingouin.pgm";
    char *filename2 = "images/pingouin.pgm";
    
    SDL_Surface *image1 = IMG_Load(filename1);
    SDL_Surface *copy1 = IMG_Load(filename1);

    SDL_Surface *image2 = IMG_Load(filename2);
    SDL_Surface *copy2 = IMG_Load(filename2);
    
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

    if (image2 == NULL)
    {
        fprintf(stderr, "Could not load image: %s\n", IMG_GetError());
        return -1;
    }
    
    if (copy2 == NULL) 
    {
        fprintf(stderr, "Could not load image: %s\n", IMG_GetError());
        return -1;
    }

    copy1 = corr_image(copy1,50,10);
    IMG_SavePNG(copy1,"t1.pgm");

    copy2 = corr_image(copy2,50,1);
    IMG_SavePNG(copy2,"t2.pgm");
    charger_image_data("t2.pgm");// charge l'image
    filtre_sobel();// on lui applique le filtre
    save_image_donnee("t2.pgm");//sortie de l'image avec le filtre de sobel
    similaires(copy2,image1);

    SDL_FreeSurface(image1);
    SDL_FreeSurface(copy1);

    SDL_FreeSurface(image2);
    SDL_FreeSurface(copy2);

    SDL_Quit();

    return 0;
}
