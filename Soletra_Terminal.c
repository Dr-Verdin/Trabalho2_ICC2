#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 7
#define MIN 4
#define ERRO -32000

/*Estrutura de dados utilizada para organizar as informações do jogo. A estrutura utilizada foi uma lista
sequencial com um vetor auxiliar para dizer quais palavras da lista foram descobertas.*/
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

/*Cabeçalho das funções criadas para o projeto.*/
void merge(char **vet, int inicio, int meio, int fim);
void Merge_Sort(char **vet, int inicio, int fim);
bool verifica(char letra, char *palavra, char *letras);
int busca_binaria(Jogo *jogo, char* answer, int total);
bool carregar_palavras(Jogo *jogo);
void validador(Jogo *jogo, char* answer);
void progresso(Jogo *jogo);
void solucao(Jogo *jogo);
void apagar_tudo(Jogo *jogo);

/*Função auxiliar que divide junta dois subvetores que foram passados pela função Merge_Sort ordenadamente.*/
void merge(char **vet, int inicio, int meio, int fim){
    int tamanho, p1, p2;
    char **temp;
    bool fim1=false, fim2=false;

    tamanho=fim-inicio+1;
    p1=inicio;
    p2=meio+1;

    temp=(char**)malloc(sizeof(char*)*tamanho);

    if(temp!=NULL){
        for(int i=0; i<tamanho; i++){
            if(!fim1 && !fim2){
                if(strlen(vet[p1])<strlen(vet[p2])){
                    temp[i]=vet[p1++];
                }else
                    temp[i]=vet[p2++];
                
                if(p1>meio){
                    fim1=true;
                }
                if(p2>fim){
                    fim2=true;
                }
            }else
            if(!fim1){
                temp[i]=vet[p1++];
            }else
                temp[i]=vet[p2++];
        }
        for(int j=0, k=inicio; j<tamanho; j++, k++){
            vet[k]=temp[j];
        }
    }
    free(temp);
}

/*Função recursiva que divide o vetor de palavras em subvetores de 1 palavra e depois os vai juntando 
ordenamente em relação ao tamanho da palavra. Esta sendo utilizado para diminuir a complexidade na hora da
impressão da solução.*/
void Merge_Sort(char **vet, int inicio, int fim){
    if(inicio<fim){
        int meio=(inicio+fim)/2;

        Merge_Sort(vet, inicio, meio);
        Merge_Sort(vet, meio+1, fim);

        merge(vet, inicio, meio, fim);
    }
}

/*Função auxiliar que é utilizada na função carregar_palavras. Ela serve para verificar se a palavra passada pelo seus 
parâmetros é valida checando se ela tem a letra obrigatória e olhando se ela também é uma composição das outras letras 
passadas ao jogo ser iniciado.*/
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

/*Essa função é chamada ao se utilizar o comando inicio. Seu objetivo é procurar no documento todas as palavras válidas e 
guardar dentro da lista.*/
bool carregar_palavras(Jogo *jogo){
    FILE *fp=fopen("valid_words.txt", "r");
    char buffer[100];

    if(fp==NULL){
        printf("Erro ao abrir o arquivo");

        return false;
    }

    jogo->palavras=(char**)malloc(sizeof(char*)*2000);
    jogo->discovers=(bool*)calloc(2000, sizeof(bool));
    jogo->tam=(int*)calloc(19, sizeof(int));
    jogo->quant=(int*)calloc(19, sizeof(int));

    if(jogo->palavras==NULL || jogo->discovers==NULL || jogo->tam==NULL || jogo->quant==NULL){
        printf("Erro na alocação de memória\n");
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

/*A busca binária é utilizada pela função validador para ver se a palavra tentada pelo jogador está na lista
de palavras válidas.*/
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

/* Função que verica se a resposta que o jogador deu é válida. Ela verifica com ajuda da busca binária se a resposta
do jogador é correta. Caso seja é impresso "sucesso +1", se não é impresso "palavra invalida".*/
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

/* Essa função quando é chamada imprime todos as palavras válidas guardadas na lista em relação ao tamanho das palavras e depois encerra o programa.
 Ele imprime indo das menores (que são as de 4 letras) até as de maior tamanho. Essa função faz a chamada do Merge_Sort para deixar o vetor ordenado
em relação ao tamanho, fazendo com que ao invés da complexidade ser O(n²), caso não estivesse ordenado, ela seja O(n).*/
void solucao(Jogo *jogo) {
    int acumulador=0;
    if (jogo != NULL) {
        Merge_Sort(jogo->palavras, 0, jogo->total-1);
        printf("para encerrar o jogo estavam faltando as palavras:\n");

        for (int i=4; i<=jogo->l_word_max; i++) {
            if (jogo->tam[i - 4] > 0) {
                printf("(%d letras) ", i);
                acumulador+=jogo->tam[i-4];
                int aux=acumulador;
                while(jogo->tam[i-4]>0){
                    printf("%s", jogo->palavras[aux-1]);
                    jogo->tam[i-4]--;
                    aux--;

                    if(jogo->tam[i-4]>0){
                        printf(", ");
                    }
                }
                printf("\n");
            }
        }
        printf("fim!\n");
    }
}

/* Função que tem o intuito de imprimir em relação ao número de letras quantas palavras palavras o jogador acertou e quantas faltam. */
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

/*Função que da free em todas as alocações que são feitas na heap ao o programa ser encerrado.*/
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

/*Função principal onde se é feito lido os comandos, as letras, as respostas e se é também onde se faz o controle das chamadas das funções
principais através dos comandos que são lidos.*/
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
