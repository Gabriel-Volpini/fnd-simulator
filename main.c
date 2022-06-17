#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//Autor  - Gabriel Nagem Volpini

//!---------------------------------------------

    typedef struct TransicaoCompleta {
        int idDestino;
        int idOrigem;
        char simbolo;
    }TransicaoCompleta;

    typedef struct Transicao
    {
        int idDestino;
        char simbolo;
    } Transicao;

    typedef struct Estado {
        int id, numTransicoes;
        char name;
        bool ehAceite;
        Transicao transcoes[10]; //contem somente transições que saem do estado
    }Estado;

    typedef struct Automato
    {
        int idInicio;
        int numEstados;
        Estado estados[20];
    }Automato;
//!---------------------------------------------

void skipXmlHeader(FILE *file){
    char buff[100];

    for(int i = 0; i <= 4; i++){
        fgets(buff, 100, (FILE*)file); //Ignorando cabeçalho xml
    }
    return;
}

void getEstadosFromFile( FILE *file, Automato *AFN){
    char buff[100];

    for(int i = 0; i <= 20; i++){
        AFN->numEstados = i;
        fscanf(file, "\t\t<state id=\"%d\" name=\"%c\">\n", &AFN->estados[i].id, &AFN->estados[i].name );

        fgets(buff, 100, (FILE*)file); //Ignorando X
        fgets(buff, 100, (FILE*)file); //Ignorando Y

        fgets(buff, 100, (FILE*)file); 

        if(strcmp(buff, "\t\t\t<initial/>\n") == 0){ //Se possuit tag inicial
            AFN->idInicio = AFN->estados[i].id;

            fgets(buff, 100, (FILE*)file); //skip </state>

        } else if(strcmp(buff, "\t\t\t<final/>\n") == 0){ //se possuir tag final
            AFN->estados[i].ehAceite = true;
            fgets(buff, 100, (FILE*)file); //skip </state>

        } else {
            AFN->estados[i].ehAceite = false;
        }

        fgets(buff, 100, (FILE*)file); //next line

        if(strcmp(buff,"\t\t<!--The list of transitions.-->\n") == 0){
            break;
        }else{
            fseek(file, -26, SEEK_CUR); //volta cursor
        }
    }
    
}

void getTransicoesFromFile ( FILE *file, TransicaoCompleta arrTransicao[20], int *arrSize){
    char buff[100];

    for(int i = 0; i <= 20; i++){
        *arrSize = i;
        fgets(buff, 100, (FILE*)file); //skip <transitions>

        fscanf(file, "\t\t\t<from>%d</from>\n", &arrTransicao[i].idOrigem );
        fscanf(file, "\t\t\t<to>%d</to>\n", &arrTransicao[i].idDestino );
        fscanf(file, "\t\t\t<read>%c</read>\n", &arrTransicao[i].simbolo );

        fgets(buff, 100, (FILE*)file); //skip </transitions>
        fgets(buff, 100, (FILE*)file); 

        if(strcmp(buff,"\t</automaton>\n") == 0){
            break;
        } else {
            fseek(file, -14, SEEK_CUR); //volta cursor
        }
    }
}

int findEstadoIndexById(int id, Automato *AFN){
    for(int i = 0; i <= AFN->numEstados; i++){
        if(AFN->estados[i].id == id){
            return(i);
        }
    }
    return 100000;
}

void insertTransicoesEstadosFromFile( FILE *file, Automato *AFN){
    TransicaoCompleta arrTransicao[20];
    int arrSize;
    getTransicoesFromFile(file, arrTransicao, &arrSize);
    
    for(int i = 0; i <= arrSize; i++){
        int estadoIndex = findEstadoIndexById(arrTransicao[i].idOrigem, AFN);
        int *idxTransicao = &AFN->estados[estadoIndex].numTransicoes;

        AFN->estados[estadoIndex].transcoes[*idxTransicao].simbolo = arrTransicao[i].simbolo;
        AFN->estados[estadoIndex].transcoes[*idxTransicao].idDestino = arrTransicao[i].idDestino;        
        AFN->estados[estadoIndex].numTransicoes = *idxTransicao + 1;
    }

}

void getAutomato(Automato *AFN) {
    FILE *file;
    file = fopen("AFN.xml","r");

    int numEstados = 0;
    int numTransicoes = 0;

    skipXmlHeader(file);
    getEstadosFromFile(file, AFN);
    insertTransicoesEstadosFromFile(file, AFN);

    // printf("%d", AFN->estados[0].id);

    

    fclose(file);
}

void inputDefaultValues(Automato *AFN) {
    for(int i = 0; i <= 20; i++){
        AFN->estados[i].ehAceite = false;
        AFN->estados[i].id = 0;
        AFN->estados[i].name = 'w';
        AFN->estados[i].numTransicoes = 0;
    }
}

int main () {
    Automato AFN;

    inputDefaultValues(&AFN);
    getAutomato(&AFN);
    //converter afd
    //gerar xml convertido

    printf("\n");
    return 0;
}