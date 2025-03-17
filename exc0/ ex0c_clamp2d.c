#include <stdio.h>
#include <stdlib.h>

unsigned char clamp(float valor){
    if(valor < 0.0f) return 0;
    if(valor > 255.0f) return 255;
    return (unsigned char)valor;
}

int main(){
    int width = 4;
    int height = 4;
    unsigned char *img = malloc(width * height); //array 1-D


    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            int index = row * width + col;
            float valor = (row + col) * 50.0f;
            img[index] = clamp(valor);
        }
    }

    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            int index = row * width + col;
            printf("%3d ", img[index]);
        }
        printf("\n");
    }

    free(img);
    return 0;
}