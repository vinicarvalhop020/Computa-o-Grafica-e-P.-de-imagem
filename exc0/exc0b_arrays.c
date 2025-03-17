#include <stdlib.h>
#include <stdio.h>

int main(){
    unsigned char arrEstatico[10];
    unsigned char *arrDinamico = malloc(10);

    for(int i = 0; i < 10; ++i){
        arrEstatico[i] = i;
        arrDinamico[i] = i;
    }

    printf("Array estático: \n");
    for (int i = 0; i < 10; ++i) {
        printf("%d ", arrEstatico[i]);
    }
    printf("\n");

    printf("Array Dinâmico: \n");
    for (int i = 0; i < 10; ++i) {
        printf("%d ", arrDinamico[i]);
    }
    printf("\n");

    free(arrDinamico);
    return 0;
}

