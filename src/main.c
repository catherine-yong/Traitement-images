#include "fonctions.h"
#include "sobel.h"
int main()
{
    SDL_version nb;
    SDL_VERSION(&nb);

    char *filename1 = "images/tournesol.jpeg";
    char *filename2 = "images/house.png";
    
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

    copy1 = correction_image(copy1,50,10);
    IMG_SavePNG(copy1,"t1.png");

    copy2 = correction_image(copy2,50,1);
    IMG_SavePNG(copy2,"t2.png");
    charger_image_data("t2.png");// charge l'image
    filtre_sobel();// on lui applique le filtre
    save_image("t2.png");//sortie de l'image avec le filtre de sobel
    similaires(copy2,image1);

    SDL_FreeSurface(image1);
    SDL_FreeSurface(copy1);

    SDL_FreeSurface(image2);
    SDL_FreeSurface(copy2);

    SDL_Quit();

    return 0;
}
