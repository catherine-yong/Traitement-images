#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Color couleur_pixel(const SDL_Surface *image , int X, int Y)
{
    // retourne la quantité de rouge , vert , bleu et opacité pour un pixel de coordonnées (X,Y)

    Uint8 bpp = image->format->BytesPerPixel;

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

    Uint8 bpp = image->format->BytesPerPixel;
    SDL_Color color_pixel = {0x00 , 0x00 , 0x00 , SDL_ALPHA_OPAQUE};    // couleur d'un pixel
    

    for (i = 0 ; i <= largeur ; i++)
    {
        int Y = i;
        for (j = 0 ; j <= longueur ; j++)
        {
            int X = j;
            
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


// Fonction pour la valeur absolue :
double val_abs(double a)
{
    if(a < 0) a=-a;
    return a;
}

SDL_Surface *moy_pixel(SDL_Surface *image, float eps)
{
    int n = image -> w;
    int m = image -> h;

    float y = (float) 1/(m*n);
    
    int i,j;
    //float moyenne = 0.0;
    float m_R = 0.0;
    float m_V = 0.0;
    float m_B = 0.0;
    
    //int *color_image;

    for (i = 0 ; i < m ; i++)
    {   
        for (j = 0 ; j < n ; j++)
        {
            SDL_Color res_amplitude1 = couleur_pixel(image , i,j);
            float r1 = res_amplitude1.r;
            float v1 = res_amplitude1.g;
            float b1 = res_amplitude1.b;
            
            m_R += r1 ;
            m_V += v1 ;
            m_B += b1 ;
            
            //moyenne += (r1 + v1 + b1);
            
        }
    }
   
    m_R = m_R*y ;
    m_V = m_V*y ;
    m_B = m_B*y ;
    
    SDL_Rect rect1;
    SDL_Rect rect2;
    Uint32 *pixels;

    rect1.x =0;
    rect1.y =0;
    rect1.h=1;
    rect1.w=1;
 
    for (i = 0 ; i < m ; i++)
    {
        for (j = 0 ; j < n ; j++)
        {
                        
            rect2.x =i;
            rect2.y =j;
            rect2.h=1;
            rect2.w=1;
            
            Uint8 r, g ,b;
            Uint32 pixel = i*m + j;
            SDL_GetRGB(pixel,image->format ,&r,&g,&b);

            if (abs(m_R - r) > eps)
            {
                r = m_R;
            }

            if (abs(m_V - g) > eps)
            {
                g = m_V;
            }
            
            if (abs(m_B - b) > eps)
            {
                b = m_B;
            }
            
            SDL_LockSurface(image);
            pixels = image->pixels;
            SDL_Surface *surface = NULL;
            SDL_PixelFormat *format = NULL;
            format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
            pixels[i * 255 + j] = SDL_MapRGB(image->format, r, g, b);
            
            surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels, 1, 1, 32, sizeof(Uint32), SDL_PIXELFORMAT_RGBA8888);

            SDL_FreeFormat(format);

        }
    }

    return image;
}

SDL_Surface *correction_image(SDL_Surface *image, Uint32 nb_sub,Uint32 eps){

    image = SDL_ConvertSurfaceFormat(image,SDL_PIXELFORMAT_RGBA8888,0);
    Uint8 bpp = image->format->BytesPerPixel;
    const Uint32 x = image->w;
    const Uint32 y = image->h;
    Uint32 a=floor(x/nb_sub); // floor : partie entière     a c'est le nombre de sous divisions verticales
    Uint32 b=floor(y/nb_sub); // b : nb de sous divisions horizontales
    SDL_Surface *temp = NULL; // sous division
    temp = SDL_CreateRGBSurface(0,a,b,32,0,0,0,0);

    if (!temp)
    {
        printf("Erreur allocation mémoire\n");
        exit(1);
    }

    SDL_Rect rect1; 
    SDL_Rect rect2;

    int i,j;
    
    for(i=1;i<a;i++){
        for(j=1;j<b;j++){  // on ne fait ça qu'une fois
            
            if ((a*(i+1)>x) && (b*(j+1)>y)){ // si on dépasse l'image de base en bas et à droite
                
                rect1.x=a*i;
                rect1.y=b*j;
                rect1.w=x - a*i;
                rect1.h=y - b*j;
                temp->w=x - a*i; 
                
                temp->h=y - b*j;     
                  
                rect2.x=0;
                rect2.y=0;
                
                rect2.w=x - a*i;
                
                rect2.h=y - b*j;
                
                SDL_BlitSurface(image,&rect1,temp,&rect2);

                
                temp = moy_pixel(temp,eps);
                SDL_BlitSurface(temp,&rect2,image,&rect1);

            }
            else if ((a*(i+1)<=x) && (b*(j+1)>y)){   // si on dépasse l'image par le bas
                
                rect1.x=a*i;
                rect1.y=b*j;
                rect1.w=a;
                rect1.h=y - b*j;

                temp->w=a;
                temp->h=y - b*j;        
                rect2.x=0;
                rect2.y=0;
                rect2.w=a;
                rect2.h=y - b*j;

                SDL_BlitSurface(image,&rect1,temp,&rect2);

                temp = moy_pixel(temp,eps);

                SDL_BlitSurface(temp,&rect2,image,&rect1);

                
            }
            else if ((a*(i+1)>x) && (b*(j+1)<=y)){ // si on dépasse par la droite
                
                rect1.x=a*i;
                rect1.y=b*j;
                rect1.w=x - a*i;
                rect1.h=b;
                   
                temp->w=x - a*i;
                temp->h=b;        
                rect2.x=0;
                rect2.y=0;
                rect2.w=x - a*i;
                rect2.h=b;

                SDL_BlitSurface(image,&rect1,temp,&rect2);

                temp = moy_pixel(temp,eps); // La ligne qui pose problème

                SDL_BlitSurface(temp,&rect2,image,&rect1);

            }
            else if ((a*(i+1)<=x) && (b*(j+1)<=y)){ // si tout va bien
                
//          On définit le rectangle à extraire      
                rect1.x=a*i;
                rect1.y=b*j;
                rect1.w=a;
                rect1.h=b;
//          On redéfinit la taille de l'image temporaire
                temp->w=a;
                temp->h=b;        
                rect2.x=0;
                rect2.y=0;
                rect2.w=a;
                rect2.h=b;

                SDL_BlitSurface(image,&rect1,temp,&rect2);
//          moyenne à remplacer par le nom de la fonction
                temp = moy_pixel(temp,eps);

                SDL_BlitSurface(temp,&rect2,image,&rect1);
                
            }
        }
    }
    SDL_FreeSurface(temp);
    
    return image;
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
    SDL_Surface *copy = image ;

    copy=correction_image(copy,1,1);
    SDL_SaveBMP(copy,"copy.bmp");

    printf("MSE ENTRE LES DEUX IMAGES : %f \n",mse_images(image,copy));

    
    SDL_FreeSurface(image);
    SDL_FreeSurface(copy);

    SDL_Quit();

    return 0;
}
