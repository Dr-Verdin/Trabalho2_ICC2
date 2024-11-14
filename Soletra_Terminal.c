#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
    int *tam;
    int *quant;
    int l_word_max;
} Jogo;

bool verifica(char letra, char *palavra, char *letras);
int busca_binaria(Jogo *jogo, char* answer, int total);
bool carregar_palavras(Jogo *jogo);
void validador(Jogo *jogo, char* answer);
void progresso(Jogo *jogo);
void solucao(Jogo *jogo);
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
    FILE *fp=fopen("valid_words.txt", "r");
    char buffer[100];

    if(fp==NULL){
        printf("Erro ao abrir o arquivo");

        return false;
    }

    jogo->palavras=(char**)malloc(sizeof(char*)*300);
    jogo->discovers=(bool*)calloc(300, sizeof(bool));
    jogo->tam=(int*)calloc(19, sizeof(int));
    jogo->quant=(int*)calloc(19, sizeof(int));

    if(jogo->palavras==NULL || jogo->discovers==NULL){
        printf("Erro na alocação de memória");
        fclose(fp);
        return false;
    }

    jogo->total=0;
    jogo->l_word_max=0;

    while(fscanf(fp, "%s", buffer)!= EOF){
        if(verifica(jogo->obrigada, buffer, jogo->letras)){
            int size=strlen(buffer);
            
            if(size > jogo->l_word_max){
                jogo->l_word_max=size;
            }

            jogo->tam[size-4]++;
            jogo->palavras[jogo->total]=(char*)malloc(sizeof(char)*(size+1));
            strcpy(jogo->palavras[jogo->total], buffer);
            jogo->total++;
        }
    }
    fclose(fp);
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
                sup=meio-1;
            }else{
                inf=meio+1;
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
        printf("sucesso + 1\n");
         
        int aux2= strlen(answer)-4;
        jogo->descobertas++;
        jogo->discovers[aux]=true;
        jogo->quant[aux2]++;
    }
}

void solucao(Jogo *jogo) {
    if (jogo != NULL) {
        printf("para encerrar o jogo, estavam faltando as palavras:\n");

        for (int i=4; i<=jogo->l_word_max; i++) {
            if (jogo->tam[i - 4] > 0) {
                printf("(%d letras) ", i);
                
                for (int j=0; j<jogo->total; j++) {
                    
                    if (strlen(jogo->palavras[j])==i && jogo->discovers[j]==false) {
                        printf("%s ", jogo->palavras[j]);
                    }
                }
                printf("\n");
            }
        }
        printf("fim!\n");
    }
}

void progresso(Jogo *jogo){
    if(jogo!=NULL){
        printf("progresso atual:\n");

        for(int i=0; i<=(jogo->l_word_max-4); i++){
            if(jogo->tam[i]!=0){
                printf("(%d letras) ", i+4);
                printf("%d palavra(s) encontrada(s) / %d palavra(s) faltando\n", jogo->quant[i], jogo->tam[i]-jogo->quant[i]);
            }
        }
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

        free(jogo->tam);
        jogo->tam=NULL;

        free(jogo->quant);
        jogo->quant=NULL;
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
                scanf(" %c", &jogo.letras[i]);
            }
            jogo.obrigada=jogo.letras[0];
            jogo.descobertas=0;

            if(!carregar_palavras(&jogo)){
                printf("Problema no momento de consulta ao arquivo de palavras\n");
            }
        }else 
        if(strcmp(comando, "palavra")==0){
            scanf("%s", answer);

            validador(&jogo, answer);

            if(jogo.descobertas==jogo.total){
                printf("parabens! voce encontrou todas as palavras\n");
                break;
            }
        }else
        if(strcmp(comando, "progresso")==0){
            progresso(&jogo);
        }else
        if(strcmp(comando, "solucao")==0){
            solucao(&jogo);
            break;
        }

    }

    apagar_tudo(&jogo);

return 0;
}