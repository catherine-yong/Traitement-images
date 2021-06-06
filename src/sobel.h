//fichier .h

#define MAX_L 1024
#define MAX_H 1024
#define MAX_LUM 255
#define NVGRIS 256
#define MAX_NOM 256
#define MAX_BUFFER 256

float image1[MAX_L][MAX_H] __attribute__((aligned(64))),image2[MAX_L][MAX_H] __attribute__((aligned(64)));

int x_size1,y_size1,x_size2,y_size2;

void charger_image_data();
void save_image();
void charger_image_fic(char*);
void save_image_fic(char*);


void charger_image_data(){
    char nom_fic[MAX_NOM];
    char buffer[MAX_BUFFER];
    FILE *fic;
    int max_gris,x,y;
    printf("Nom du fichier d'entrée : ");
    scanf("%s", nom_fic);
    fic = fopen(nom_fic,"rb");
    if (fic == NULL){
        printf("Fichier inexistant\n");
        exit(1);
    }
    fgets(buffer,MAX_BUFFER,fic);
    if(buffer[0] != 'P' || buffer[1]!== '5'){
        printf("Fichier pas au format P5!\n");
        exit(1);
    }
    x_size1 = 0;
    y_size1 = 0;
    
    while(x_size1 == 0 || y_size1 == 0){
        fgets(buffer, MAX_BUFFER,fic);
        if(buffer[0] != '#'){
        sscanf(buffer,"%d %d", &x_size1,&y_size1);
        }
    }
    
    max_gris = 0;
    while(max_gris == 0){
        fgets(buffer,MAX_BUFFER,fic);
        if(buffer[0]!='#'){
            sscanf(buffer,"%d",&max_gris);
        }
    }
    printf("Largeur de l'image = %d, hauteur = %d \n", x_size1,y_size1);
    printf("Niveau maximal de gris = %d \n", max_gris);
    
    if (x_size1 >MAX_L || y_size1>MAX_H){
        printf("La largeur de l'image est supérieur à %d x %d \n Veuillez utiliser une image plus petite\n", MAX_L,MAX_H);
        exit(1);
    }
    if (max_gris!=MAX_LUM){
        printf("Valeur de gris incorrecte \n");
        exit(1);
    
    }
#pragma simd //???
    for (x = 0; x_size1;x++){
#pragma simd
        for( y = 0,y<y_size1;y++){
            image1[x][y] = (unsigned char)fgetc(fic);
        }
    }
    fclose(fic);
}

void save_image_donnee(){
    char nom_fichier[MAX_NOM];
    FILE* fic;
    
    printf("Nom du fichier ?\n");
    scanf("%s",nom_fichier);
    fic = fopen(nom_fichier,"wb");
    fputs("P5\n", fic);
    fprintf(fic,"%d %d\n", x_size2, y_size2);
    fprintf(fic,"%d\n",MAX_LUM);
#pragma simd
    for(int x = 0;x<x_size2;x++){
#pragma simd
        for(int y = 0; y<y_size2;y++){
            fputc(image2[x][y],fic);
        }
    }
    fclose(fic);
}

void charger_image_fic(char* nom_fic){
    char buffer[MAX_BUFFER];
    FILE *fic;
    int max_gris;
    
    fic = fopen(nom_fic,"rb");
    if(fic == NULL){
        printf("Le fichier n'existe pas \n");
        exit(1);
    }
    fgets(buffer, MAX_BUFFER, fic);
       if (buffer[0] != 'P' || buffer[1] != '5') {
         printf("Mauvais format de fichier\n");
         exit(1);
       }
       x_size1 = 0;
       y_size1 = 0;
       while (x_size1 == 0 || y_size1 == 0) {
         fgets(buffer, MAX_BUFFER, fic);
         if (buffer[0] != '#') {
           sscanf(buffer, "%d %d", &x_size1, &y_size1);
         }
       }

       max_gris = 0;
       while (max_gris == 0) {
         fgets(buffer, MAX_BUFFER, fic);
         if (buffer[0] != '#') {
            sscanf(buffer, "%d", &max_gris);
         }
       }
       if (x_size1 > MAX_L || y_size1 > MAX_H) {
         printf("La taille est supérieur à %d x %d\n\n",
            MAX_L, MAX_H);
         exit(1);
       }
       if (max_gris != MAX_LUM) {
         printf("Valeur du niveau de gris invalide \n");
         exit(1);
       }
       /* Input of image data*/
      #pragma simd
      for (x = 0; x < x_size1; x++) {
        #pragma simd
        for (y = 0; y < y_size1; y++) {
           image1[x][y] = (float)fgetc(fic);
         }
      }
      fclose(fic);
    }

    void save_image_fic(char *nom_fichier)
    /* Output of image2[ ][ ], x_size2, y_size2 */
    /* into pgm file with header & body information */
    {
      FILE *fic; /* File pointer */
      int x, y; /* Loop variable */

      fic = fopen(nom_fichier, "wb");
      /* output of pgm file header information */
      fputs("P5\n", fic);
      fputs("# Created by Image Processing\n", fic);
      fprintf(fic, "%d %d\n", x_size2, y_size2);
      fprintf(fic, "%d\n", MAX_LUM);
      /* Output of image data */
     #pragma simd
      for (x = 0; x < x_size2; x++) {
        #pragma simd
        for (y = 0; y < y_size2; y++) {
          fputc(image2[x][y], fic);
        }
      }
    fclose(fic);
}
