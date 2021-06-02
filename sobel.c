#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#define N 3

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

float niv_gris(const SDL_Surface *image , int X, int Y)
{
    // renvoie le niveau de gris 

    SDL_Color pix = couleur_pixel(image,X,Y);

    float gris = (float) 0.2125*pix.r + 0.7154*pix.g + 0.0721*pix.b;

    return gris;

}

int in_the_pic(const SDL_Surface *image , int x, int y)
{
    // retourne 1 si le pixel est dans l'image, 0 sinon

    int largeur = image -> w; // nombre de colonnes 
    int hauteur = image -> h; // nombre de lignes 

    int test = 1;

    if ((x > hauteur) || (y > largeur) || (x < 0) || (y < 0))
    {
        test = 0;
    }

    return test;
}

int **A_pixel(SDL_Surface *image, int x, int y)
{
    ;
}

int **A_pix(SDL_Surface *image,int x, int y)    // PAS NECESSAIRE 
{
    // retourne une matrice 3x3 qui présente les niveaux de gris des pixels alentours à (x,y)

    int topleft, topmid , topright , left , right , botleft , botmid, botright;

    // on récupère la taille de l'image 
    int largeur = image -> w; // nombre de colonnes 
    int hauteur = image -> h; // nombre de lignes 

    float mid_hor = (float) hauteur/2;
    float mid_ver = (float) largeur/2;

    int i,j;

    // matrice de retour, on l'initialise de base aux niveaux de gris des pixels alentours. si on est sur les bords, on met les 0 là où c'est nécessaire

    if ((x%hauteur != 0) && (y%largeur != 0))
    {
        topleft = niv_gris(image,x-1,y-1);
        topmid = niv_gris(image, x , y-1);
        topright = niv_gris(image, x+1, y-1);
        left = niv_gris(image, x-1,y);
        right = niv_gris(image, x+1,y);
        botleft = niv_gris(image,x-1, y+1);
        botmid = niv_gris(image,x,y+1);
        botright = niv_gris(image,x+1,y+1);
    } 

    else if (x < mid_hor)    // à gauche 
    {

        if ((x == 0) && (y%largeur != 0)) // bordure de gauche sans l'angle
        {
            topmid = niv_gris(image, x , y-1);
            topright = niv_gris(image, x+1, y-1);
            right = niv_gris(image, x+1,y);
            botmid = niv_gris(image,x,y+1);
            botright = niv_gris(image,x+1,y+1);
            
            topleft = 0;
            left = 0;
            botleft = 0;
        }

        else if (y < mid_ver) // en haut 
        {
            if ((x%hauteur != 0) && (y == 0))  // bordure du haut sans l'angle 
            {
        
                left = niv_gris(image, x-1,y);
                right = niv_gris(image, x+1,y);
                botleft = niv_gris(image,x-1, y+1);
                botmid = niv_gris(image,x,y+1);
                botright = niv_gris(image,x+1,y+1);
                
                topleft = 0;
                topmid = 0;
                topright = 0;
            }

            if ((x == 0) && (y == 0)) // angle supérieur gauche
            {
                right = niv_gris(image, x+1,y);
                botmid = niv_gris(image,x,y+1);
                botright = niv_gris(image,x+1,y+1);
                topleft = 0;
                topmid = 0;
                topright = 0;
                left = 0;
                botleft = 0;
            }
        }

        else    // en bas 
        {
            if ((x%hauteur != 0) && (y == largeur))  // bordure du bas sans l'angle
            {
                topleft = niv_gris(image,x-1,y-1);
                topmid = niv_gris(image, x , y-1);
                topright = niv_gris(image, x+1, y-1);
                left = niv_gris(image, x-1,y);
                right = niv_gris(image, x+1,y);
                
                botleft = 0;
                botmid = 0;
                botright = 0;
            }

            if ((x == 0) && (y == largeur)) // angle inférieur gauche
            {
                
                topmid = niv_gris(image, x , y-1);
                topright = niv_gris(image, x+1, y-1);
                right = niv_gris(image, x+1,y);

                topleft = 0;
                left = 0;
                botleft = 0;
                botmid = 0;
                botright = 0;
            }
        } 
    }

    else    // à droite 
    {
        if ((x == hauteur) && (y%hauteur != 0)) // bordure de droite sans l'angle
        {
            topleft = niv_gris(image,x-1,y-1);
            topmid = niv_gris(image, x , y-1);
            left = niv_gris(image, x-1,y);
            botleft = niv_gris(image,x-1, y+1);
            botmid = niv_gris(image,x,y+1);
            
            topright = 0;
            right = 0;
            botright = 0;
        }
        
        else if (y < mid_ver) // en haut 
        {
            if((x%hauteur != 0) && (y == 0))  // bordure du haut sans l'angle
            {
                
                left = niv_gris(image, x-1,y);
                right = niv_gris(image, x+1,y);
                botleft = niv_gris(image,x-1, y+1);
                botmid = niv_gris(image,x,y+1);
                botright = niv_gris(image,x+1,y+1);

                topleft = 0;
                topmid = 0;
                topright = 0;
            }

            if ((x == hauteur) && (y == 0)) // angle supérieur droite
            {
                        
                left = niv_gris(image, x-1,y);
                botleft = niv_gris(image,x-1, y+1);
                botmid = niv_gris(image,x,y+1);
        
                topleft = 0;
                topmid = 0;
                topright = 0;
                right = 0;
                botright = 0;
            }

        }

        else // en bas 
        {
            if ((x%hauteur != 0) && (y == largeur))  // bordure du bas sans l'angle
            {
                topleft = niv_gris(image,x-1,y-1);
                topmid = niv_gris(image, x , y-1);
                topright = niv_gris(image, x+1, y-1);
                left = niv_gris(image, x-1,y);
                right = niv_gris(image, x+1,y);
                
                botleft = 0;
                botmid = 0;
                botright = 0;
            }

            if ((x == hauteur) && (y == largeur)) // angle inférieur droite
            {
                topmid = niv_gris(image, x , y-1);
                topright = niv_gris(image, x+1, y-1);
                right = niv_gris(image, x+1,y);

                topleft = 0;
                left = 0;
                botleft = 0;
                botmid = 0;
                botright = 0;
            }
        }

    } 

    int **A = (int **) malloc(3*sizeof(int *));

    for (i = 0 ; i < 3 ; i++)
    {
        A[i] = (int *) malloc(3*sizeof(int));

    } 

    A[0][0] = topleft;
    A[0][1] = topmid;
    A[0][2] = topright;
    A[1][0] = left;
    A[1][1] = 0;
    A[1][2] = right;
    A[2][0] = botleft;
    A[2][1] = botmid;
    A[2][2] = botright;

    
    return A;

}

float *filtre_sobel(SDL_Surface *image)
{

    int Dx[N][N] =  {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Dy[N][N] = {{-1, -2, -1}, {0, 0, 0}, {1,2,1}};

    float Gx = 0.0;
    float Gy = 0.0;
    float G = 0.0 ;

    float *G_image = NULL;

    int largeur = image -> w;
    int longueur = image -> h;

    G_image = (float *) malloc(longueur*largeur*sizeof(float));

    if (G_image == NULL)
    {
        printf("Erreur allocation mémoire\n");
        exit(1);
    }

    int index = 0;

    for (int i = 0 ; i < largeur ; i++)
    {
        for (int j = 0 ; j < longueur ; j++)
        {
            int **A = A_pix(image,i,j);
            
            for (int n= 0; n <= N-1; n++)
            {
                
                for (int m = 0; m <= N-1; m++)      // ERREUR DE SEGMENTATION : ON NE SORT PAS DE LA BOUCLE
                {
                    Gx += Dx[N-n][N-m] * A[n][m];
                    Gy += Dy[N-n][N-m] * A[n][m];

                    G = (float) sqrt(pow(Gx,2) + pow(Gy,2)); // norme du gradiant en chaque pixel

                    G_image[index] = G;
            
                    index++;
                }
            
            }
    
       
        }
    }

    return G_image;

}

int main()
{

    SDL_version nb;
    SDL_VERSION(&nb);

    int i,j;

    char *filename = "images/pepers.jpg";
    
    SDL_Surface *image, *converted;
    
    image = IMG_Load(filename);
    
    if (image == NULL) {
        fprintf(stderr, "Could not load image: %s\n", IMG_GetError());
        return -1;
    }

    int largeur = image -> w;
    int longueur = image -> h;

    int **A = A_pix(image,1,0);

    filtre_sobel(image);

    return 0;
}