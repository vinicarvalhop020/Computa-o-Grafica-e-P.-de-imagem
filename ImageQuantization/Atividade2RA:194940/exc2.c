/*
 * Setup Version: Color Quantization Template
 * --------------------------------------------
 *
 * This template is designed to help you structure your program for the lab activity.
 * You must implement two algorithms:
 *
 * 1. Popularity Algorithm:
 *    - Build a histogram of the image’s colors.
 *    - Sort the histogram by frequency.
 *    - Use the top "targetColors" to create a palette.
 *    - Replace each pixel with the nearest palette color (using Euclidean distance).
 *
 * 2. Median Cut Algorithm:
 *    - Build a histogram of unique colors.
 *    - Recursively partition the color space (choose the channel with the largest range)
 *      until you reach the desired number of color boxes.
 *    - Compute a representative color for each box (using a weighted average).
 *    - Replace each pixel with the representative color from the corresponding box.
 *
 * Two output images should be produced for each algorithm:
 *    - One with half the original number of colors.
 *    - One with a quarter of the original number of colors.
 *
 * Compile with:
 *    gcc -o quantize setup_version.c -lm
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <math.h>
 
 // Include the STB image libraries for file I/O.
 #define STB_IMAGE_IMPLEMENTATION
 #include "stb_image.h"
 #define STB_IMAGE_WRITE_IMPLEMENTATION
 #include "stb_image_write.h"
 
 /* ===========================================================
    Data Structures & Function Prototypes
    =========================================================== */
 
 /* ColorEntry:
  *   - Represents an RGB color and its frequency in the image.
  */
 typedef struct {
     unsigned char r, g, b;
     int count;
 } ColorEntry;
 
 typedef struct {
    unsigned char r, g, b;
 } Cor;

 typedef struct {
    int rmin, rmax, gmin, gmax, bmin, bmax;//dimensoes do paralelepipedo (pontinho)
    int r_dist, g_dist, b_dist;//dimensoes em arestas
    ColorEntry *cores;//cores e freq contidas no paralelepipedo
    int qnt_cores;//qntd de cores no paralelepipedo
    int qnt_cores_count; //soma das frequencias de cada cor, 
    Cor cor_media; //ponto medio da cor referente a caixa;
 } paralelepipedo;



 /* Function Prototypes:
  *   - add_or_update_color: Updates the histogram with a given color.
  *   - build_histogram: Constructs a histogram of unique colors from the image.
  *   - apply_popularity_quantization: (Placeholder) Implement the Popularity algorithm.
  *   - apply_mediancut_quantization: (Placeholder) Implement the Median Cut algorithm.
  *   - save_image: Saves an image to disk.
  */
 int add_or_update_color(ColorEntry **hist, int *n, unsigned char r, unsigned char g, unsigned char b);
 ColorEntry* build_histogram(unsigned char* data, int width, int height, int channels, int* uniqueCount);
 unsigned char* apply_popularity_quantization(unsigned char* data, int width, int height, int channels, int targetColors);
 unsigned char* apply_mediancut_quantization(unsigned char* data, int width, int height, int channels, int targetColors);
 void save_image(const char* filename, unsigned char* data, int width, int height, int channels);
 
 /* ===========================================================
    Main Function
    =========================================================== */

    void troca(ColorEntry *a, ColorEntry *b) {
        ColorEntry temp = *a;
        *a = *b;
        *b = temp;
    }

    void insertionsort(ColorEntry *v, int n) {
        int i, j;
        for (i = 1; i < n; i++) {
            for (j = i; j > 0 ; j--) {
                if (v[j].count < v[j-1].count){
                    troca(&v[j], &v[j-1]);
                }
            }
        }
    }

    void insertionsort_cor(ColorEntry *v, int n, int cor) {
        int i, j;

        if (cor == 0){//ordena com base no vermelho
            for (i = 1; i < n; i++) {
                for (j = i; j > 0 ; j--) {
                    if (v[j].r < v[j-1].r){
                        troca(&v[j], &v[j-1]);
                    }
                }
            }
        }

        if (cor == 1){//ordena com base no verde
            for (i = 1; i < n; i++) {
                for (j = i; j > 0 ; j--) {
                    if (v[j].g < v[j-1].g){
                        troca(&v[j], &v[j-1]);
                    }
                }
            }
        }

        if (cor == 2){//ordena com base no blue
            for (i = 1; i < n; i++) {
                for (j = i; j > 0 ; j--) {
                    if (v[j].b < v[j-1].b){
                        troca(&v[j], &v[j-1]);
                    }
                }
            }
        } 
    }   
  
  
    void Min_Euclidian_color(unsigned char r, unsigned char g, unsigned char b, Cor* c, ColorEntry* space, int target) {
        //calcula a distancia euclidiana da cor com um espaço
        unsigned char rd, gd, bd;
        float min_dist = 100000000; 
    
        for(int i = 0; i < target; i++){
            rd = (r - space[i].r);
            gd = (g - space[i].g);
            bd = (b - space[i].b);
            float dist = sqrt(pow(rd, 2) + pow(gd, 2) + pow(bd, 2));
    
            if (dist < min_dist){
                min_dist = dist;
                c->r = space[i].r;
                c->g = space[i].g;
                c->b = space[i].b;
            }
        }
    }

    void point_to_box(unsigned char r, unsigned char g, unsigned char b, Cor* c, paralelepipedo *caixas, int target) {
        //recebe um valor RGB, e encontra qual ponto ele esta mais perto, utilizando todos os paralelepipedos
        unsigned char rd, gd, bd;
        float min_dist = 100000000; 
    
        for(int i = 0; i < target; i++){
            rd = (r - caixas[i].cor_media.r);
            gd = (g - caixas[i].cor_media.g);
            bd = (b - caixas[i].cor_media.b);
            float dist = sqrt(pow(rd, 2) + pow(gd, 2) + pow(bd, 2));
            if (dist < min_dist){
                min_dist = dist;
                c->r = caixas[i].cor_media.r;
                c->g = caixas[i].cor_media.g;
                c->b = caixas[i].cor_media.b;
            }
        }

    }
   
    paralelepipedo constroi_paralelepipedo(ColorEntry* histograma, int i_comeco, int i_fim) {

        // constroi um paralelepipedo e seus parametros
        paralelepipedo p;
            p.rmin = 255;
            p.rmax = 0;
            p.gmin = 255; 
            p.gmax = 0;
            p.bmin = 255; 
            p.bmax = 0;
            p.qnt_cores_count = 0;
        
    
        for(int i = i_comeco; i < i_fim; i++) {
           
            if (histograma[i].r < p.rmin) 
                p.rmin = histograma[i].r;
            if (histograma[i].r > p.rmax) 
                p.rmax = histograma[i].r;
            if (histograma[i].g < p.gmin) 
                p.gmin = histograma[i].g;
            if (histograma[i].g > p.gmax) 
                p.gmax = histograma[i].g;
            if (histograma[i].b < p.bmin) 
                p.bmin = histograma[i].b;
            if (histograma[i].b > p.bmax) 
                p.bmax = histograma[i].b;
            
            //descobre a soma da quantidade de cores considerando a frequencia
            p.qnt_cores_count += histograma[i].count;
        }
    
        p.r_dist = p.rmax - p.rmin;
        p.g_dist = p.gmax - p.gmin;
        p.b_dist = p.bmax - p.bmin;
        p.cores = histograma + i_comeco;
        p.qnt_cores = i_fim - i_comeco; //serve para informar o tamanho do vetor
    
        return p;
    }

    int maior_cordenada(int r, int g, int b){
        /*
            retorna um inteiro correspondente a maior aresta
            red = 0
            green = 1
            blue = 2
        */
       if (r >= g && r >= b) 
            return 0;
       if (g >= r && g >= b) 
            return 1;
       else
            return 2;

    
    }

    unsigned char clamp(float valor){
        if(valor < 0.0f) return 0;
        if(valor > 255.0f) return 255;
        return (unsigned char)valor;
    }

    void calcula_media(paralelepipedo *caixas, int qntd_caixas){

        //calcula a media r,g,b com base nas cores presentes
        float media_r, media_g, media_b, soma_r, soma_g, soma_b;

      
        for(int i = 0; i < qntd_caixas; i++){
            soma_r = 0;
            soma_g = 0;
            soma_b = 0;
            for (int j = 0; j < caixas[i].qnt_cores; j++) {
                //multiplica cada cor pela frequencia dessa cor
                soma_r += caixas[i].cores[j].r*caixas[i].cores[j].count; 
                soma_g += caixas[i].cores[j].g*caixas[i].cores[j].count;
                soma_b += caixas[i].cores[j].b*caixas[i].cores[j].count;
            }
            media_r = soma_r/caixas[i].qnt_cores_count;
            media_g = soma_g/caixas[i].qnt_cores_count;
            media_b = soma_b/caixas[i].qnt_cores_count;

            caixas[i].cor_media.r = clamp(media_r);
            caixas[i].cor_media.g = clamp(media_g);
            caixas[i].cor_media.b = clamp(media_b);
         }
    }


    int mediana(paralelepipedo p) {

        long mediana_freq = p.qnt_cores_count / 2;
        long soma_atual = 0;
        int indice_mediana;
        
        // Encontra o ponto em que passa a metade da frequencia (meio do vetor considerando a frequencia das cores)
        for (int i = 0; i < p.qnt_cores; i++) {
            soma_atual += p.cores[i].count;
            if (soma_atual >= mediana_freq) {
                indice_mediana = i;
                break;
            }
        }
        return indice_mediana + 1;  // envia o indice posterior 
    }


    

    void median_cut(paralelepipedo *p, paralelepipedo *todas_caixas, int* qnt_caixas, int target){
        /*
            p é a caixa atual da recursao
            todas as caixas é o vetor contendo todas as caixas 
            qnt de caixas é quantas caixas possuem até a recursao atual
            target é o numero de caixas necessario
        */
       if (*qnt_caixas >= target || p->qnt_cores < 2 || *qnt_caixas == target - 1) { //para se atingiu a quantidade de divisoes target,maxima possivel da caixa com 2 cores, ou a ultima caixa a ser adiconada antes do target
            if (*qnt_caixas < target && p->qnt_cores > 0) { 
                todas_caixas[*qnt_caixas] = *p;
                (*qnt_caixas)++;
            }
            return;
        }
    

        int indice_mediana, maior_aresta;
        paralelepipedo novo_esq, novo_dir; //cria duas novas caixas (esquerda e direita)
        maior_aresta = maior_cordenada(p->r_dist, p->g_dist, p->b_dist);//encontra qual a maior aresta
        insertionsort_cor(p->cores, p->qnt_cores, maior_aresta); //ordena o vetor pela maior aresta

        indice_mediana = mediana(*p);//alterar para retornar o indice da mediana considerando a frequencia das cores

        if (indice_mediana <= 0 || indice_mediana >= p->qnt_cores) {
            return; //para nao ocorrer de criar caixas vazias
        }

        novo_esq = constroi_paralelepipedo(p->cores, 0, indice_mediana);  //0 até mediana
        novo_dir = constroi_paralelepipedo(p->cores, indice_mediana, p->qnt_cores); //indice da mediana ate o fim
        median_cut(&novo_esq, todas_caixas, qnt_caixas, target);
        median_cut(&novo_dir, todas_caixas, qnt_caixas, target);

    }

    int main(int argc, char* argv[]) {
        if (argc < 2) {
                fprintf(stderr, "Usage: %s <input_image>\n", argv[0]);
                return 1;
        }

    
    // Load the image in RGB mode (3 channels)
    int width, height, channels;

    unsigned char* data = stbi_load(argv[1], &width, &height, &channels, 3);
    if (!data) {
         fprintf(stderr, "Error loading image %s\n", argv[1]);
         return 1;
    }

     // Build a histogram to count unique colors
     int uniqueCount;
     ColorEntry* hist = build_histogram(data, width, height, channels, &uniqueCount);
     printf("Found %d unique colors in the image.\n", uniqueCount);
     free(hist);
     
     // Define target palette sizes (half and quarter reduction)
     int halfColors = uniqueCount / 2;
     int quarterColors = uniqueCount / 4;
     printf("Palette sizes: %d (half reduction), %d (quarter reduction).\n", halfColors, quarterColors);
     
     
     /* ---------- Popularity Algorithm ---------- */
     // TODO: Implement the Popularity algorithm in apply_popularity_quantization.
    unsigned char* popularity_half = apply_popularity_quantization(data, width, height, channels, halfColors);
    save_image("popularity_half.png", popularity_half, width, height, channels);
    free(popularity_half);
     
    unsigned char* popularity_quarter = apply_popularity_quantization(data, width, height, channels, quarterColors);
    save_image("popularity_quarter.png", popularity_quarter, width, height, channels);
    free(popularity_quarter);
     
     /* ---------- Median Cut Algorithm ---------- */
     // TODO: Implement the Median Cut algorithm in apply_mediancut_quantization.
     unsigned char* mediancut_half = apply_mediancut_quantization(data, width, height, channels, halfColors);
     save_image("mediancut_half.png", mediancut_half, width, height, channels);
     free(mediancut_half);
     
     unsigned char* mediancut_quarter = apply_mediancut_quantization(data, width, height, channels, quarterColors);
     save_image("mediancut_quarter.png", mediancut_quarter, width, height, channels);
     free(mediancut_quarter);
     
     stbi_image_free(data);
     return 0;
 }
 
 
 /* ===========================================================
    Histogram Functions
    =========================================================== */
 
 /*
  * add_or_update_color:
  *   Checks if the color (r, g, b) exists in the histogram.
  *   If it does, increments the count; otherwise, adds a new entry.
  */
 int add_or_update_color(ColorEntry **hist, int *n, unsigned char r, unsigned char g, unsigned char b) {
     for (int i = 0; i < *n; i++) {
          if ((*hist)[i].r == r && (*hist)[i].g == g && (*hist)[i].b == b) {
              (*hist)[i].count++;
              return i;
          }
     }
     *hist = realloc(*hist, (*n + 1) * sizeof(ColorEntry));
     if (*hist == NULL) {
          fprintf(stderr, "Memory allocation error in histogram creation.\n");
          exit(1);
     }
     (*hist)[*n].r = r;
     (*hist)[*n].g = g;
     (*hist)[*n].b = b;
     (*hist)[*n].count = 1;
     (*n)++;
     return *n - 1;
 }
 
 /*
  * build_histogram:
  *   Constructs a histogram from the image data.
  *   Returns an array of ColorEntry and sets uniqueCount.
  */
 ColorEntry* build_histogram(unsigned char* data, int width, int height, int channels, int* uniqueCount) {
     ColorEntry* hist = NULL;
     *uniqueCount = 0;
     int totalPixels = width * height;
     for (int i = 0; i < totalPixels; i++) {
          int idx = i * channels;
          unsigned char r = data[idx];
          unsigned char g = data[idx+1];
          unsigned char b = data[idx+2];
          add_or_update_color(&hist, uniqueCount, r, g, b);
     }
     return hist;
 }
 
 
 /* ===========================================================
    Quantization Algorithms (Placeholders)
    =========================================================== */
 
 /*
  * apply_popularity_quantization:
  *   Placeholder for the Popularity algorithm.
  *   Students should replace this implementation with their own.
  */
 unsigned char* apply_popularity_quantization(unsigned char* data, int width, int height, int channels, int targetColors) {
     // For now, simply return a copy of the original image.
     int totalPixels = width * height;
     unsigned char* newImg = malloc(totalPixels * channels);

     

     if (!newImg) {
          fprintf(stderr, "Memory allocation error in Popularity algorithm.\n");
          exit(1);
     }
     
    int unique;
    ColorEntry* histograma = build_histogram(data, width, height, channels, &unique);
    insertionsort(histograma, unique);
    ColorEntry* cores = malloc(targetColors*sizeof(ColorEntry)); // vetor de cores mais frequentes

    for (int i = 0; i < targetColors; i++) {
        cores[i] = histograma[unique - targetColors + i];
    }
        
    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            int index = (row * width + col)*channels;
                unsigned char r = data[index];  
                unsigned char g = data[index + 1];  
                unsigned char b = data[index + 2];

                Cor cor_proxima;
                Min_Euclidian_color(r,g,b,&cor_proxima,cores,targetColors);

                newImg[index] = cor_proxima.r;
                newImg[index + 1] = cor_proxima.g; 
                newImg[index + 2] = cor_proxima.b;           
        }
    }
    return newImg;
 }
 
 /*
  * apply_mediancut_quantization:
  *   Placeholder for the Median Cut algorithm.
  *   Students should replace this implementation with their own.
  */
 unsigned char* apply_mediancut_quantization(unsigned char* data, int width, int height, int channels, int targetColors) {
     // For now, simply return a copy of the original image.
     int totalPixels = width * height;
     unsigned char* newImg = malloc(totalPixels * channels);
     if (!newImg) {
          fprintf(stderr, "Memory allocation error in Median Cut algorithm.\n");
          exit(1);
     }
    

    int unique;
    int qntd_caixas = 0;

    ColorEntry* histograma = build_histogram(data, width, height, channels, &unique);
    paralelepipedo* caixas =  malloc(targetColors*sizeof(paralelepipedo)); //caixa com todas as divisoes
    paralelepipedo caixa_inicial = constroi_paralelepipedo(histograma,0,unique); //paralelepipedo inical

    median_cut(&caixa_inicial, caixas, &qntd_caixas, targetColors);
    calcula_media(caixas, qntd_caixas);
        
    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            int index = (row * width + col)*channels;
                unsigned char r = data[index];  
                unsigned char g = data[index + 1];  
                unsigned char b = data[index + 2];
                Cor cor_proxima;
                point_to_box(r,g,b,&cor_proxima,caixas,qntd_caixas);
                newImg[index] = cor_proxima.r;
                newImg[index + 1] = cor_proxima.g; 
                newImg[index + 2] = cor_proxima.b;           
        }
    }
    free(histograma);
    free(caixas);
    return newImg;
 }
 
 
 /* ===========================================================
    Utility Functions
    =========================================================== */
 
 /*
  * save_image:
  *   Writes the image data to a file using stb_image_write.
  */
 void save_image(const char* filename, unsigned char* data, int width, int height, int channels) {
     if (!stbi_write_png(filename, width, height, channels, data, width * channels)) {
          fprintf(stderr, "Error writing image %s\n", filename);
          exit(1);
     }
     printf("Image saved: %s\n", filename);
 }
 