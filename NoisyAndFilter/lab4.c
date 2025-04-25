#include <stdlib.h> // Para srand() e rand()
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void troca(unsigned char *a, unsigned char *b) {
    unsigned char temp = *a;
    *a = *b;
    *b = temp;
}

void insertionsort(unsigned char *v, int n) {
    int i, j;
    for (i = 1; i < n; i++) {
        for (j = i; j > 0 ; j--) {
            if (v[j] < v[j-1]){
                troca(&v[j], &v[j-1]);
            }
        }
    }
}



void ruido_sal_pimenta(unsigned char *img_noisy,unsigned char *img, int width, int height , float probabilidade){

    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            int index = (row * width + col);
            float k = (float)rand() / RAND_MAX;  
            unsigned char c = img[index];    
            if(k < probabilidade){
                //sal
                img_noisy[index] = 255;
            }else if(k > 1 - probabilidade){
                //pimenta
                img_noisy[index] = 0;
            }else{
                //mantem
                img_noisy[index] = c;             
            }
        }

    }
}


void filtro_mediana(unsigned char *img_filtred, unsigned char *img, int width, int height){
    //filtro mediana 3x3
    unsigned char vetor[9];

    //limita as bordas
    for(int row = 1; row < height-1; row++){
        for(int col = 1; col < width-1; col++){

            int index = (row * width + col);

            vetor[0] = img[index - width - 1];
            vetor[1] = img[index - width];
            vetor[2] = img[index - width + 1];
            vetor[3] = img[index - 1];
            vetor[4] = img[index];
            vetor[5] = img[index + 1];
            vetor[6] = img[index + width - 1];
            vetor[7] = img[index + width];
            vetor[8] = img[index + width + 1];

            insertionsort(vetor, 9);
            img_filtred[index] = vetor[4]; // mediana
        }
    }
}

void save_image(const char* filename, unsigned char* data, int width, int height, int channels) {
    //função do lab2
    if (!stbi_write_png(filename, width, height, channels, data, width * channels)) {
         fprintf(stderr, "Error writing image %s\n", filename);
         exit(1);
    }
    printf("Image saved: %s\n", filename);
}

int main(){
    srand(42);
    int width_p, height_p, channels_p;
    int width_f, height_f, channels_f;
    int width_b, height_b, channels_b;

    unsigned char *img_pepper = stbi_load("pepper.png", &width_p, &height_p, &channels_p, 1);
    unsigned char *img_fruit = stbi_load("fruit.png", &width_f, &height_f, &channels_f, 1);
    unsigned char *img_baboon = stbi_load("baboon.png", &width_b, &height_b, &channels_b, 1);

    unsigned char *img_baboon_sal_pimenta = malloc((width_b*height_b)*sizeof(unsigned char));
    unsigned char *img_pepper_sal_pimenta = malloc((width_p*height_p)*sizeof(unsigned char));
    unsigned char *img_fruit_sal_pimenta = malloc((width_f*height_f)*sizeof(unsigned char));

    unsigned char *img_filtred_pepper = malloc((width_p*height_p)*sizeof(unsigned char));
    unsigned char *img_filtred_fruit = malloc((width_f*height_f)*sizeof(unsigned char));
    unsigned char *img_filtred_baboon = malloc((width_b*height_b)*sizeof(unsigned char));

    //add noise
    ruido_sal_pimenta(img_pepper_sal_pimenta, img_pepper, width_p, height_p, 0.15);
    ruido_sal_pimenta(img_fruit_sal_pimenta, img_fruit, width_f, height_f, 0.15);
    ruido_sal_pimenta(img_baboon_sal_pimenta, img_baboon, width_b, height_b, 0.15);
    //apply filter
    filtro_mediana(img_filtred_pepper, img_pepper_sal_pimenta, width_p, height_p);
    filtro_mediana(img_filtred_fruit, img_fruit_sal_pimenta, width_f, height_f);
    filtro_mediana(img_filtred_baboon, img_baboon_sal_pimenta, width_b, height_b);
    
    //save images
    save_image("pepper_sal_pimenta.png", img_pepper_sal_pimenta, width_p, height_p, 1);
    save_image("fruit_sal_pimenta.png", img_fruit_sal_pimenta, width_f, height_f, 1);
    save_image("baboon_sal_pimenta.png", img_baboon_sal_pimenta, width_b, height_b, 1);
    save_image("filtred_pepper.png", img_filtred_pepper, width_p, height_p, 1);
    save_image("filtred_fruit.png", img_filtred_fruit, width_f, height_f, 1);
    save_image("filtred_baboon.png", img_filtred_baboon, width_b, height_b, 1);
}