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
            int gx = -[(j-1)*image->w + i-1] -2*[  j*image->w + i-1] -[(j+1)*image->w + i-1]
                     +[(j-1)*image->w + i+1] +2*[  j*image->w + i+1] +[(j+1)*image->w + i+1];
            int gy = -[(j-1)*image->w + i-1] -2*[(j-1)*image->w + i] -[(j-1)*image->w + i+1]
                     +[(j+1)*image->w + i-1] +2*[(j+1)*image->w + i] +[(j+1)*image->w + i+1];

            j*image->w + i = (abs(gx) + abs(gy))/8;
        }
    }

    // Fill the left and right sides
    for(j=0; j<image->h ; j++){
        j*image->w = 0;
        (j+1)*image->w-1 = 0;
    }
}
