#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 7
#define MIN 4
#define ERRO -32000

typedef struct{
    char **palavras;
    bool *discovers;
    int total;
    int descobertas;
    char letras[MAX+1];
    char obrigada;
} Jogo;

bool verifica(char letra, char *palavra, char *letras);
int busca_binaria(Jogo *jogo, char* answer, int total);
bool carregar_palavras(Jogo *jogo);
void validador(Jogo *jogo, char* answer);
void progresso(Jogo *jogo);
void solucao(&jogo);
void apagar_tudo(Jogo *jogo);


bool verifica(char letra, char *palavra, char *letras){
    if(strchr(palavra, letra)==NULL){
        return false;
    }

    for(int i=0; i<strlen(palavra); i++){
        if(strchr(letras, palavra[i])==NULL){
            return false;
        }
    }

return true;
}

bool carregar_palavras(Jogo *jogo){
    FILE *fp=fopen(valid_words.txt, "r");
    char buffer[100];

    if(fp==NULL){
        printf("Erro ao abrir o arquivo");

        return false;
    }

    jogo->palavras=(char**)malloc(sizeof(char*)*1000);
    jogo->discovers=(bool*)calloc(1000, sizeof(bool));
    jogo->total=0;

    while(fscanf(fp, "%s", buffer)!= EOF){
        if(verifica(jogo->obrigada, buffer, jogo->letras)){
            jogo->palavras[jogo->total]= strdup(buffer);
            jogo->total++;
        }
    }

return true;
}

int busca_binaria(Jogo *jogo, char* answer, int total){
    if(jogo!=NULL){
        int inf=0;
        int sup=total-1;
        int meio, aux;

        while(inf<=sup){
            meio=(inf+sup)/2;
            aux=strcmp(answer, jogo->palavras[meio]);

            if(aux==0){
                return meio;
            }else
            if(aux<0){
                inf=meio+1;
            }else{
                sup=meio-1;
            }
        }
    }

return ERRO;
}

void validador(Jogo *jogo, char* answer){
    if(strchr(answer, jogo->obrigada)==NULL){
        printf("palavra invalida\n");

        return;
    }

    int aux= busca_binaria(jogo, answer, jogo->total);

    if(aux==ERRO){
        printf("palavra invalida\n");
    }else{
        printf("sucesso + 1");
         
        jogo->discovers[aux]=true;
        jogo->descobertas++;
    }
}

void apagar_tudo(Jogo *jogo){
    if(jogo!=NULL){
        for(int i=0; i<jogo->total; i++){
            free(jogo->palavras[i]);
            jogo->palavras[i]=NULL;
        }
        free(jogo->palavras);
        jogo->palavras=NULL;

        free(jogo->discovers);
        jogo->discovers=NULL;
    }
}

int main(void){
    char comando[10];
    Jogo jogo;
    char answer[8];

    while(1){
        scanf("%s", comando);

        if(strcmp(comando, "inicio")==0){
            for(int i=0; i<7; i++){
                scanf(" %c", &jogo->letras[i]);
            }
            jogo->obrigada=jogo->letras[0];

            if(!carregar_palavras(&jogo)){
                printf("Problema no momento de consulta ao arquivo de palavras\n");
            }
        }else 
        if(strcmp(comando, "palavra")==0){
            scanf("%s", answer);

            validador(&jogo, answer);

            if(jogo.descobertas==jogo.total){
                printf("parabens! voce encontrou todas as palavras");
                break;
            }
        }else
        if(strcmp(comando, "progresso")){
            progresso(&jogo);
        }else
        if(strcmp(comando, "solucao")){
            solucao(&jogo);
        }

    }

    apagar_tudo(&jogo);

return 0;
}