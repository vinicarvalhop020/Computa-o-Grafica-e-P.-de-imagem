#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned char clamp(float valor){
    if(valor < 0.0f) return 0;
    if(valor > 255.0f) return 255;
    return (unsigned char)valor;
}

void extract_rgb(unsigned char *img, char channel, int width, int height, unsigned char *img_extracted){
    //extract the chanel RGB 

    int offset;
    if (channel == 'r'){
     offset = 0;
    }
    else if (channel == 'g'){
     offset = 1;
    }
    else{
     offset = 2;
    }
    
    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){ //R-g-b
            int index = row * width + col;

            unsigned char valor = img[(row * width + col)*3 + offset]; // valor RED
            img_extracted[index] = clamp(valor);
        }
    }
}

void extract_cmy(unsigned char *img, char channel, int width, int height, unsigned char *img_extracted){
    //extract the channel CMY
    int offset;
    if (channel == 'c'){
     offset = 0;//red
    }
    else if (channel == 'm'){
     offset = 1;//green
    }
    else{//yellow
     offset = 2;//blue
    }
    
    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            int index = row * width + col;

            unsigned char ch = img[(row * width + col)*3 + offset];  
            unsigned char valor = 255-ch; //W - (R G B)
            img_extracted[index] = clamp(valor);
        }
    }

}

float find_max(unsigned char r, unsigned char g, unsigned char b) {
    float max = 0;

    if (r > max){
        max = r;
    }
    if (g > max){
        max = g;
    }
    if (b > max){
        max = b;
    }

    return max;
}

float find_min(unsigned char r, unsigned char g, unsigned char b) {
    
    float min = 255.0;

    if (r < min){
        min = r;
    }
    if (g < min){
        min = g;
    }
    if (b < min){
        min = b;
    }

    return min;
}



void extract_hsv(unsigned char *img, char channel, int width, int height, unsigned char *img_extracted){
    //extract the channel HSV

    float h, s, v, max, min; // float para nao ocorrer overflow e problemas de divisão interia (podem ocorrer caso unsigned char fosse usado)

    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            int index = row * width + col;
            //find max and min
                unsigned char r = img[(row * width + col)*3];  
                unsigned char g = img[(row * width + col)*3 + 1];  
                unsigned char b = img[(row * width + col)*3 + 2];

                max = find_max(r,g,b);
                min = find_min(r,g,b);

                if (channel == 'h'){

                    if (max-min == 0){
                        h = 0;
                    }
                    else if(max == r){
                        h = 60.0*(g-b)/(max-min);
                    }
                    else if(max == g){
                        h = 60.0*(b-r)/(max-min) + 120;
                    }
                    else{
                        h = 60.0*(r-g)/(max-min) + 240;
                    }

                    if(h < 0){
                        h = h + 360;
                    }
                    img_extracted[index] = clamp((h/360 * 255)); //hue é uma grandeza medida em graus (360) então é necessario o mapeamento de 0-255            
                }
                else if(channel == 's'){
                    if(max != 0){
                        s = (max-min)/max; 
                        s = s*255; //s varia de 0-1, então multiplica-se por 255 para escalonar entre o valor de um unsigned char
                    }
                    else{
                        s = 0;
                    }
                    img_extracted[index] = clamp(s);//clamp por redundancia de manter o valor em maximo 255
                }
                else{
                    v = max;
                    img_extracted[index] = clamp(v);
                }              
        }
    }    
}



int main(){

    //read the img
    int width, height, channels;
    unsigned char *img = stbi_load("img.png", &width, &height,&channels,0);
    
    //alocate images
    unsigned char *image_red = malloc(width*height);
    unsigned char *image_green = malloc(width*height);
    unsigned char *image_blue = malloc(width*height);
    unsigned char *image_cyan = malloc(width*height);
    unsigned char *image_magenta = malloc(width*height);
    unsigned char *image_yellow = malloc(width*height);
    unsigned char *image_hue = malloc(width*height);
    unsigned char *image_saturation = malloc(width*height);
    unsigned char *image_value = malloc(width*height);

    //extract images
    extract_rgb(img,'r', width, height, image_red);
    extract_rgb(img,'g', width, height, image_green);
    extract_rgb(img,'b', width, height, image_blue);
    extract_cmy(img,'c',width, height, image_cyan);
    extract_cmy(img,'m',width, height, image_magenta);
    extract_cmy(img,'y',width, height, image_yellow);
    extract_hsv(img,'h',width, height, image_hue);
    extract_hsv(img,'s',width, height, image_saturation);
    extract_hsv(img,'v',width, height, image_value);

    //save imges
    stbi_write_png("image1_red.png", width, height, 1, image_red , width); 
    stbi_write_png("image1_green.png", width, height, 1 , image_green , width); 
    stbi_write_png("image1_blue.png", width, height, 1 , image_blue , width); 
    stbi_write_png("image1_cyan.png", width, height, 1, image_cyan, width);
    stbi_write_png("image1_magenta.png", width, height, 1, image_magenta, width);
    stbi_write_png("image1_yellow.png", width, height, 1, image_yellow, width);
    stbi_write_png("image1_hue.png", width, height, 1, image_hue, width);
    stbi_write_png("image1_saturation.png", width, height, 1, image_saturation, width);
    stbi_write_png("image1_value.png", width, height, 1, image_value, width);

    //free
    stbi_image_free(img);
    free(image_red);
    free(image_green);
    free(image_blue);
    free(image_cyan);
    free(image_magenta);
    free(image_yellow);
    free(image_hue);
    free(image_saturation);
    free(image_value);

    return 0;
}
