#include "fonctions.h"

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

SDL_Surface *moy_pixel(SDL_Surface *image, float eps)
{
    // change la couleur de l'image étudiée pixel par pixel si le pixel est trop éloigné de la moyenne de l'image étudiée 

    int m = image -> w; // m : largeur 
    int n = image -> h; // n : hauteur 

    float y =  1/(float)(m*n); // on divise par la taille de l'image
    
    float m_R = 0.0; // moyenne de rouge
    float m_V = 0.0; // moyenne de vert 
    float m_B = 0.0; // moyenne de bleu 
    
    float r1,v1,b1;
    
    
    SDL_Rect rect2;
    SDL_Rect taille = {0,0,1,1};
    
    Uint8 r, g ,b , a;
    Uint32 pixel;

    for (int i = 0 ; i < m ; i++) // on somme la quantité locale de rouge, vert et bleu
    {   
        for (int j = 0 ; j < n ; j++)
        {
            SDL_Color res_amplitude1 = couleur_pixel(image , i,j);
            r1 = res_amplitude1.r;
            v1 = res_amplitude1.g;
            b1 = res_amplitude1.b;
            
            m_R += r1 ;
            m_V += v1 ;
            m_B += b1 ;
            
            
        }
    }

    // on calcule la moyenne locale 
    m_R = m_R*y ;
    m_V = m_V*y ;
    m_B = m_B*y ;
   
    for (int i = 0 ; i < m ; i++) // on remplace la valeur du pixel courant par celle de la moyenne locale si besoin 
    {
        for (int j = 0 ; j < n ; j++)
        {
            
            rect2.x = i;
            rect2.y = j;
            rect2.h = 1;
            rect2.w = 1;
            
            pixel = i*m + j;
            SDL_GetRGBA(pixel,image->format ,&r,&g,&b,&a);
            
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
            
            SDL_Surface *p = SDL_CreateRGBSurface(0,1,1,32,0,0,0,255);
        
            SDL_FillRect(p,&taille, SDL_MapRGBA(p->format, r, g, b , a));
            
            SDL_BlitSurface(p,&taille,image,&rect2);
            
            SDL_FreeSurface(p);
        }
    }
    
    return image;
}



// ======================================= CORRECTION DE L'IMAGE ENTIERE ======================================

SDL_Surface *corr_image(SDL_Surface *image, Uint32 nb_sub,Uint32 eps)
{
    // applique l'algorithme de moy_pixel à chaque pixel de l'image en prenant en compte les différents cas de configuration

    image = SDL_ConvertSurfaceFormat(image,SDL_PIXELFORMAT_RGBA8888,0);
    const Uint32 x = image->w;
    const Uint32 y = image->h;
    Uint32 a=floor(x/nb_sub); 
    Uint32 b=floor(y/nb_sub); 
    SDL_Surface *temp = NULL; // variable temporaire où on effectue les modifications
    SDL_Rect rect1; 
    SDL_Rect rect2;
    int i,j;
    temp = SDL_CreateRGBSurface(0,a,b,32,0,0,0,0);

    if (!temp)
    {
        printf("Erreur allocation mémoire\n");
        exit(1);
    }
    
    for(i = 0;i < x;i++){ // on a plusieurs possibilités de dépassement et un cas où rien ne dépasse
        for(j = 0;j < y;j++){ 
            
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

            else if ((a*(i+1) > x) && (b*(j+1) <= y)){ // si on dépasse par la droite
                
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
                
                temp = moy_pixel(temp,eps); 
                
                SDL_BlitSurface(temp,&rect2,image,&rect1);

            }
            else if ((a*(i+1)<=x) && (b*(j+1)<=y)){ // si tout va bien
                
                // on définit le rectangle à extraire      
                rect1.x=a*i;
                rect1.y=b*j;
                rect1.w=a;
                rect1.h=b;

                // on redéfinit la taille de l'image temporaire
                temp->w=a;
                temp->h=b;        
                rect2.x=0;
                rect2.y=0;
                rect2.w=a;
                rect2.h=b;

                SDL_BlitSurface(image,&rect1,temp,&rect2);
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

    char *filename1 = "images/walkbridge.png";
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

   

    copy1 = corr_image(copy1,50,10);
    IMG_SavePNG(copy1,"i1.png");
    
    copy2 = corr_image(copy2,50,1);
    IMG_SavePNG(copy2,"i2.png");


    SDL_FreeSurface(image1);
    SDL_FreeSurface(copy1);

    SDL_FreeSurface(image2);
    SDL_FreeSurface(copy2);

    SDL_Quit();

    return 0;
}