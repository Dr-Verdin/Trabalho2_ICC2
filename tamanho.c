#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
int maio=0;
FILE *fp=fopen("valid_words.txt", "r");
char buffer[100];

if(fp==NULL){
    printf("Erro ao abrir o arquivo");
    return 1;
}

while(fscanf(fp, "%s", buffer)!= EOF){
    int size=strlen(buffer);

    if(size>maio){
        maio=size;
    }
}
fclose(fp);

printf("O maior tamanho é: %d", maio);

return 0;
}