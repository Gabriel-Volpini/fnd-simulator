#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//Autor  - Gabriel Nagem Volpini

//!---------------------------------------------

    typedef struct Transicao
    {
        int idOrigem;
        int idDestino;
        char simbolo;
    } Transicao;

    typedef struct Estado {
        int id, numTransicoes;
        char name;
        int idParaConvercao[2]; //
        bool ehAceite;
    }Estado;

    typedef struct Automato
    {
        int idInicio, numEstados, numTransicoes;

        Estado estados[20];
        Transicao Transicoes[20];
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

void getTransicoesFromFile ( FILE *file, Automato *AFN){
    char buff[100];

    for(int i = 0; i <= 20; i++){
        AFN->numTransicoes = i;
        fgets(buff, 100, (FILE*)file); //skip <transitions>

        fscanf(file, "\t\t\t<from>%d</from>\n", &AFN->Transicoes[i].idOrigem );
        fscanf(file, "\t\t\t<to>%d</to>\n", &AFN->Transicoes[i].idDestino );
        fscanf(file, "\t\t\t<read>%c</read>\n", &AFN->Transicoes[i].simbolo );

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
    return 1000;
}

void getAutomato(Automato *AFN) {
    FILE *file;
    file = fopen("AFN.xml","r");

    int numEstados = 0;
    int numTransicoes = 0;

    skipXmlHeader(file);
    getEstadosFromFile(file, AFN);
    getTransicoesFromFile(file, AFN);

    fclose(file);
}

void inputDefaultValues(Automato *A) {
    for(int i = 0; i <= 20; i++){
        A->estados[i].ehAceite = false;
        A->estados[i].id = 0;
        A->estados[i].name = 'w';
        A->estados[i].numTransicoes = 0;
    }
    for(int i = 0; i <= 20; i++){
        A->Transicoes[i].idDestino = 0;
        A->Transicoes[i].idOrigem = 0;
        A->Transicoes[i].simbolo = 0;
    }
}



int main () {
    Automato AFN;
    Automato AFD;


    inputDefaultValues(&AFN);
    inputDefaultValues(&AFD);

    getAutomato(&AFN);
    //converter afd
    //gerar xml convertido

    printf("\n");
    return 0;
}