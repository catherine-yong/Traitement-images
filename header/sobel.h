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
void filtre_sobel();


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
    if(buffer[0] != 'P' || buffer[1]!= '5'){
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

    for (x = 0; x_size1;x++){

        for( y = 0;y<y_size1;y++){
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

    for(int x = 0;x<x_size2;x++){

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

      for (int x = 0; x < x_size1; x++) {
  
        for (int y = 0; y < y_size1; y++) {
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

      for (x = 0; x < x_size2; x++) {

        for (y = 0; y < y_size2; y++) {
          fputc(image2[x][y], fic);
        }
      }
    fclose(fic);
}

void filtre_sobel(){
    //def filtre de sobel à l'horizontal
    int poids[3][3] = {{-1,0,1},
        {-2,0,2},
        {-1,0,1}
    };
    double val_pix, min, max;
    min = DBL_MAX; //??
    max = -DBL_MAX;
    for (int y = 1; y<y_size1 - 1;y++){
        for(int x = 1;x<x_size1 - 1; x++){
            val_pix = 0.0;
            for (int j = -1;j <= 1;j++){
                for(int i = -1; i <= 1;i++){
                    val_pix +=poids[j+1][i+1]*image1[y+j][x+i];
                }
            }
            if (val_pix<min)
                min = val_pix;
            if (val_pix>max)
                max = val_pix;
        }
    }
    if((int)(max-min)==0){
        printf("Il n'y a rien\n");
        exit(1);
    }
    // initialise la deuxieme image
    x_size2 = x_size1;
    y_size2 = y_size1;
    for (int y = 0; y<y_size2;y++){
        for (int x = 0; x<x_size2;x++){
            image2[y][x] = 0;
        }
    }
    //genere la deuxieme image apres transformation lineaire
    for (int y = 1; y<y_size1-1 ;y++){
        for(int x = 1; x < x_size1-1;x++){
            val_pix = 0.0;
            for (int j = -1; j<= 1;j++){
                for(int i = -1;i<=1;i++){
                    val_pix += poids[j+1][i+1]*image1[y+j][x+i];
                }
            }
            val_pix = MAX_LUM *(val_pix - min) / (max - min);
            image2[y][x] = (unsigned)val_pix;
        }
    }
}
