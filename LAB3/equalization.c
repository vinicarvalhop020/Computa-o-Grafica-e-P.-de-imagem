#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct {
    unsigned char c;
    float count; 
} ColorEntry;

void troca(ColorEntry *a, ColorEntry *b) {
    ColorEntry temp = *a;
    *a = *b;
    *b = temp;
}

void insertionsort(ColorEntry *v, int n) {
    int i, j;
    for (i = 1; i < n; i++) {
        for (j = i; j > 0 ; j--) {
            if (v[j].c < v[j-1].c){
                troca(&v[j], &v[j-1]);
            }
        }
    }
}

int add_or_update_gray(ColorEntry **hist, int *n, unsigned char c) {
    //funçao modificada do lab 2 
    for (int i = 0; i < *n; i++) {
         if ((*hist)[i].c == c) {
             (*hist)[i].count++;
             return i;
         }
    }
    *hist = realloc(*hist, (*n + 1) * sizeof(ColorEntry));
    if (*hist == NULL) {
         fprintf(stderr, "Memory allocation error in histogram creation.\n");
         exit(1);
    }
    (*hist)[*n].c = c;
    (*hist)[*n].count = 1;
    (*n)++;
    return *n - 1;
}


ColorEntry* build_histogram(unsigned char* data, int width, int height, int channels, int* uniqueCount) {
    //histograma modificado para só niveis de cinza
    ColorEntry* hist = NULL;
    *uniqueCount = 0;
    int totalPixels = width * height;
    for (int i = 0; i < totalPixels; i++) {
         int idx = i * channels;
         unsigned char c = data[idx];
         add_or_update_gray(&hist, uniqueCount, c);
    }
    return hist;
}

ColorEntry * Normalize_histogram(ColorEntry* histograma, int unique_grays, int qnd_pixels){
    ColorEntry* Normalized_histogram = malloc(unique_grays*sizeof(ColorEntry));
    for (int i = 0; i < unique_grays; i ++){
        Normalized_histogram[i].c = histograma[i].c;//recebe o nivel de cinza
        Normalized_histogram[i].count = histograma[i].count/qnd_pixels;//recebe a normalização
    }
    return Normalized_histogram;
}

ColorEntry * Transform_histogram(ColorEntry* histogram, ColorEntry* Normalized_histogram, int unique_grays, int L_minus_one){
    int tr_atual = 0;
    ColorEntry* Transformed_histogram = malloc(unique_grays*sizeof(ColorEntry));
    for (int i = 0; i < unique_grays; i++){
        tr_atual += L_minus_one*Normalized_histogram[i].count;//calcula tr
        Transformed_histogram[i].c = histogram[i].c;//salva o tom de cinza
        Transformed_histogram[i].count = tr_atual;//salva a transformada
    }
    return Transformed_histogram;
}

ColorEntry * Round_histogram(ColorEntry* histogram, ColorEntry* Transformed_histogram, int unique_grays){
    ColorEntry* Rounded_histogram = malloc(unique_grays*sizeof(ColorEntry));
    int aux;
    for (int i = 0; i < unique_grays; i++){
        Rounded_histogram[i].c = histogram[i].c;//salva o tom de cinza
        aux = round(Transformed_histogram[i].count); //arredondamento
        Rounded_histogram[i].count = aux;//salva o arredondamento
    }
    return Rounded_histogram;
}

unsigned char target_gray(unsigned char pixel, ColorEntry* histogram, int unique_grays){
    // recebe um nivel de cinza e devolve o target associado a esse nivel
    int target;
    for(int i = 0; i < unique_grays; i ++){
        if(pixel == histogram[i].c){
            target = histogram[i].count;
        }
    }
    return target;
}



void Equalize_img(unsigned char* eq_data,unsigned char* data, int width, int height, int unique_grays, ColorEntry* Rounded_histogram){

    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            int index = (row * width + col);
                unsigned char c = data[index];  
                eq_data[index] = target_gray(c, Rounded_histogram, unique_grays);          
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
    int width, height, channels;

    unsigned char *img = stbi_load("beach.png", &width, &height,&channels,0);
    unsigned char *image_equalized = malloc(width*height);

    int unique_gray;
    ColorEntry* histograma = build_histogram(img, width, height, channels, &unique_gray);
    insertionsort(histograma, unique_gray);//oredena o histograma
    ColorEntry* Normalized_histogram = Normalize_histogram(histograma,unique_gray,width*height);
    
    int L_minus_one = 255;//niveis de cinza em uma imagem de 8 bits
    ColorEntry* Transformed_histogram = Transform_histogram(histograma,Normalized_histogram, unique_gray, L_minus_one);
    ColorEntry* Rounded_histogram = Round_histogram(histograma, Transformed_histogram, unique_gray);

    Equalize_img(image_equalized, img, width, height, unique_gray, Rounded_histogram);
    save_image("equalized_img.png",image_equalized, width, height, channels);

    //libera memoria
    free(img);
    free(image_equalized);
    free(histograma);
    free(Normalized_histogram);
    free(Transformed_histogram);
    free(Rounded_histogram);
}