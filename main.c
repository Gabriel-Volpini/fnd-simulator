#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//Autor  - Gabriel Nagem Volpini

//!---------------------------------------------

    typedef struct Transicao
    {
        int idDestino;
        int simbolo;
    } Transicao;

    typedef struct Estado {
        int id, numTransicoes;
        char name;
        bool ehAceite;
        Transicao transcoes[50]; //contem somente transições que saem do estado
    }Estado;

    typedef struct Automato
    {
        int idInicio;
        int numEstados;
        Estado estados[50];
    }Automato;
//!---------------------------------------------

void getAutomato() {
    FILE *file;
    file = fopen("AFN.xml","r");
    char buff[100];

    Automato AFN;
    Estado  arrEstados[50];
    Transicao arrTransicao[50];
    int numEstados = 0;
    int numTransicoes = 0;


    for(int i = 0; i != 5; i++){
        fgets(buff, 100, (FILE*)file); //Ignorando cabeçalho xml
    }

    for(int i = 0; i <= 50; i++){
        AFN.numEstados = i;
        fscanf(file, "\t\t<state id=\"%d\" name=\"%c\">\n", &arrEstados[i].id, &arrEstados[i].name );

        printf("%d - %d,%c \n",i,arrEstados[i].id, arrEstados[i].name);
        fgets(buff, 100, (FILE*)file); //Ignorando X
        fgets(buff, 100, (FILE*)file); //Ignorando Y

        fgets(buff, 100, (FILE*)file); 

        if(strcmp(buff, "\t\t\t<initial/>\n") == 0){ //Se possuit tag inicial
            AFN.idInicio = arrEstados[i].id;

            fgets(buff, 100, (FILE*)file); //skip </state>

        } else if(strcmp(buff, "\t\t\t<final/>\n") == 0){ //se possuir tag final
            arrEstados[i].ehAceite = true;
            fgets(buff, 100, (FILE*)file); //skip </state>

        } else {
            arrEstados[i].ehAceite = false;
        }

        fgets(buff, 100, (FILE*)file); //next lien

        if(strcmp(buff,"\t\t<!--The list of transitions.-->\n") == 0){
            break;
        }else{
            fseek(file, -26, SEEK_CUR); //volta cursor
        }
    }

    printf("NUM EST %d", AFN.numEstados);


    fclose(file);
}

int main () {

    getAutomato();

    printf("\n");
    return 0;
}