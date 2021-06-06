#include <stdio.h> 
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

void affiche_tab_entier(int tab[], int n)   // on affiche le tableau en entier
{
    int i;
    for(i=0;i<n;i++)
        printf("%d ",tab[i]);  
        
    printf("\n\n");
}

void tri_bulle(float tab[], int n)
{
    int i, j, temp;
    for(i = 0; i < n; i++)    
        for(j = 0; j < n; j++)
            if(tab[j] > tab[j+1])   // si tab[j] > tab[j+1] , alors on effectue l'échange
            {
                temp=tab[j];
                tab[j]=tab[j+1];
                tab[j+1]=temp;
            }
}

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

SDL_Surface *mediane(SDL_Surface *image, float eps)
{
    
    float mediane = 0.0;

    int m = image -> w; // m : largeur 
    int n = image -> h; // n : hauteur 
    int len = n*m;
    Uint32 pixel;
    int i,j;

    float y =  1/(float)(len); // on divise par la taille de l'image
    float r1,v1,b1;

// on fait un tableau en 1D où l'on note tous les niveaux de gris de chaque pixel

    float *t_R = (float *)malloc((len+100)*sizeof(float));
    float *t_V = (float *)malloc((len+100)*sizeof(float));
    float *t_B = (float *)malloc((len+100)*sizeof(float));

    float m_R, m_B, m_V;
    
    int index = 0;

    for (int i = 0 ; i < m ; i++) // on somme la quantité locale de rouge, vert et bleu
    {   
        for (int j = 0 ; j < n ; j++)
        {
            SDL_Color res_amplitude1 = couleur_pixel(image , i,j);
            r1 = res_amplitude1.r;
            v1 = res_amplitude1.g;
            b1 = res_amplitude1.b;

            t_R[index] = r1;
            t_V[index] = v1;
            t_B[index] = b1;


            index++;
        }
    }

// on trie le tableau

    tri_bulle(t_R,len);
    tri_bulle(t_V,len);
    tri_bulle(t_B,len);

    float tg,td;


// si n*m est pair, alors la mediane est la demi somme des deux valeurs du milieu 

    if (len%2 == 0)
    {
        tg = t_R[(len)/2];
        td = t_R[(len/2) + 1];
        m_R = (tg+td)/2;

        tg = t_V[(len)/2];
        td = t_V[(len/2) + 1];
        m_V = (tg+td)/2;

        tg = t_B[(len)/2];
        td = t_B[(len/2) + 1];
        m_B = (tg+td)/2;
    }

// si n*m est impair, alors la mediane est la valeur du milieu  

    else
    {
        tg = t_R[((len-1)/2) + 1];
        td = t_R[((len-1)/2) + 1];
        m_R = (tg+td)/2;

        tg = t_V[((len-1)/2) + 1];
        td = t_V[((len-1)/2) + 1];
        m_V = (tg+td)/2;

        tg = t_B[((len-1)/2) + 1];
        td = t_B[((len-1)/2) + 1];
        m_B = (tg+td)/2;


    }
    
    
    SDL_Rect rect2;
    SDL_Rect taille = {0,0,1,1};
    
    Uint8 r, g ,b , a;
    Uint32 pixel;

    
   
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

            float point = niv_gris(image,i,j);
            
            if (abs(m_R - point) > eps)
            {
                point = mediane;
            }
            
            SDL_Surface *p = SDL_CreateRGBSurface(0,1,1,32,0,0,0,255);
        
            SDL_FillRect(p,&taille, SDL_MapRGBA(p->format, point, point, point , a));
            
            SDL_BlitSurface(p,&taille,image,&rect2);
            
            SDL_FreeSurface(p);
        }
    }
    
    return image;
}

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

    int compt = 0;
    
    for(i = 0;i < x;i++){ // on a plusieurs possibilités de dépassement et un cas où rien ne dépasse
        for(j = 0;j < y;j++){ 
            compt++;

            if (compt%1000 == 0)
            {
                printf("COMPT %d\n",compt);
            }
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

                temp = mediane(temp,eps);
                printf("1\n");
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

                temp = mediane(temp,eps);

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
                
                temp = mediane(temp,eps); 
                
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
                temp = mediane(temp,eps);
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

    copy1 = mediane(image1,100);
    IMG_SavePNG(copy1,"i1.png");
   
    copy2 = mediane(image2,200);
    IMG_SavePNG(copy2,"i2.png");


    SDL_FreeSurface(image1);
    SDL_FreeSurface(copy1);

    SDL_FreeSurface(image2);
    SDL_FreeSurface(copy2);

    SDL_Quit();

    return 0;
}