#ifndef FONCT 
#define FONCT 10

#include <stdio.h> 
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

SDL_Color couleur_pixel(const SDL_Surface *image , int X, int Y);
float niv_gris(const SDL_Surface *image , int X, int Y);
int *couleur_image(SDL_Surface *image, int *color_image) ;
float mse_images(SDL_Surface *image1 , SDL_Surface *image2) ;
SDL_Surface *moy_pixel(SDL_Surface *image, float eps);
SDL_Surface *correction_image(SDL_Surface *image, Uint32 nb_sub,Uint32 eps);
int similaires(SDL_Surface *image1, SDL_Surface *image2);
int main();

#endif 