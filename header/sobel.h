//fichier .h
#include <float.h>
#define MAX_L 1024
#define MAX_H 1024
#define MAX_LUM 255
#define NVGRIS 256

#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>


void sobel(const SDL_Surface *image){
    int i,j;

    // Apply the filter
    for(j=1; j<image->h-1; j++){
        for(i=1; i<image->w-1; i++){
            int gx = -input[(j-1)*image->w + i-1] -2*input[  j*image->w + i-1] -input[(j+1)*image->w + i-1]
                     +input[(j-1)*image->w + i+1] +2*input[  j*image->w + i+1] +input[(j+1)*image->w + i+1];
            int gy = -input[(j-1)*image->w + i-1] -2*input[(j-1)*image->w + i] -input[(j-1)*image->w + i+1]
                     +input[(j+1)*image->w + i-1] +2*input[(j+1)*image->w + i] +input[(j+1)*image->w + i+1];

            output[j*image->w + i] = (abs(gx) + abs(gy))/8;
        }
    }

    // Fill the left and right sides
    for(j=0; j<image->h ; j++){
        output[j*image->w] = 0;
        output[(j+1)*image->w-1] = 0;
    }
}
