#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//Autor  - Gabriel Nagem Volpini

#define VALOR_INVALIDO = 1000;

//!---------------------------------------------

    typedef struct Transicao
    {
        int idOrigem;
        int idDestino;
        char simbolo;
    } Transicao;

    typedef struct Estado {
        int id, numIdParaConvercao;
        char name;
        int idParaConvercao[10]; 
        bool ehAceite;
    }Estado;

    typedef struct Automato
    {
        int idInicio, numEstados, numTransicoes;

        Estado estados[20];
        Transicao Transicoes[20];
    }Automato;
//!---------------------------------------------
void imprimeAutomato(Automato *A) {
    printf("idInicio => %d\n", A->idInicio);

    printf("numEstados => %d\n", A->numEstados);
    for(int i = 0; i <= A->numEstados; i++){
        printf("\t%d - { id = %d, numIdParaConvercao = %d, name = %c  ehAceite = %d } ",
            i,
            A->estados[i].id , 
            A->estados[i].numIdParaConvercao , 
            A->estados[i].name ,  
            A->estados[i].ehAceite 
        );
        printf("[");
        for(int j =0; j<= A->estados[i].numIdParaConvercao; j++){
            printf(" %d,", A->estados[i].idParaConvercao[j]);
        }
        printf("]\n");
    }

    printf("numTransicoes => %d\n", A->numTransicoes);
    for(int i = 0; i <= A->numTransicoes; i++) {
        printf("\t%d - {  idOrigem = %d, idDestino = %d, simbolo = %c }\n",
            i,
            A->Transicoes[i].idOrigem , 
            A->Transicoes[i].idDestino , 
            A->Transicoes[i].simbolo 
        );
    }
    return;
}

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

        }
        if(strcmp(buff, "\t\t\t<final/>\n") == 0){ //se possuir tag final
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
    return;
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
    return -1;
}

void getAutomatoFromXmlFile(Automato *AFN) {
    FILE *file;
    file = fopen("AFN.xml","r");

    int numEstados = 0;
    int numTransicoes = 0;

    skipXmlHeader(file);
    getEstadosFromFile(file, AFN);
    getTransicoesFromFile(file, AFN);

    fclose(file);
    return;
}

void inputDefaultValues(Automato *A) {
    A->numEstados = -1;
    A->numTransicoes = -1;
    A->idInicio = -1;

    for(int i = 0; i <= 20; i++){
        A->estados[i].ehAceite = false;
        A->estados[i].id = -1;
        A->estados[i].name = 'w';
        A->estados[i].numIdParaConvercao = -1;

        for(int j = 0; j <= 10; j++){ 
            A->estados[i].idParaConvercao[j] = -1;
        }

    }
    for(int i = 0; i <= 20; i++){
        A->Transicoes[i].idDestino = -1;
        A->Transicoes[i].idOrigem = -1;
        A->Transicoes[i].simbolo = -'w';
    }
    return;
}

void criarEstadoInicial(Automato *atualAFN, Automato *convertidoAFD) {
    convertidoAFD->idInicio = atualAFN->idInicio;
    convertidoAFD->numEstados = convertidoAFD->numEstados + 1;

    Estado *estadoAtual = &convertidoAFD->estados[convertidoAFD->numEstados];
    *estadoAtual = atualAFN->estados[atualAFN->idInicio]; //insercao do estado
    estadoAtual->numIdParaConvercao = estadoAtual->numIdParaConvercao + 1; //começando a utilizar a stack de transformação
    estadoAtual->idParaConvercao[estadoAtual->numIdParaConvercao] = estadoAtual->id; //criando o estado [idEstadoInicial]

    return;
}

Estado gerarEstadoDestinoDoSimbolo(Automato *atualAFN, Automato *convertidoAFD, int idEstadoOrigem, char simbolo){
    // numIdParaConvercao = -1 nao possui transicao
    // numIdParaConvercao = 0 possui transicao para um estado
    // numIdParaConvercao > 0 possui transicao para mais de um estaddo com nome descrito na vairavel idParaconvesao

    Estado retorno = {.ehAceite = false, .id = -1, .numIdParaConvercao = -1, .name='w', };
    int estadoOrigemIndex = findEstadoIndexById(idEstadoOrigem, convertidoAFD);
    Estado currentEstado = convertidoAFD->estados[estadoOrigemIndex];

        for(int j =0; j<= currentEstado.numIdParaConvercao; j++){ //para cada estado antigo no meu estado novo
            
            for(int i = 0; i <= atualAFN->numTransicoes; i++){
                Transicao *currentTransition = &atualAFN->Transicoes[i];
                
                if(currentTransition->idOrigem == currentEstado.idParaConvercao[j] && currentTransition->simbolo == simbolo){
                    retorno.ehAceite = retorno.ehAceite || atualAFN->estados[currentTransition->idDestino].ehAceite;
                    
                    retorno.numIdParaConvercao = retorno.numIdParaConvercao + 1;
                    retorno.idParaConvercao[retorno.numIdParaConvercao] = currentTransition->idDestino;

                }
            }
        }
   
    return retorno;
}

void inserirEstadoAutomato(Estado *e, Automato *a){
    a->numEstados = a->numEstados + 1;
    e->id = a->numEstados;
    a->estados[a->numEstados] = *e;
    return;
}

int validarExistenciaEstado(Estado e, Automato *a){
    for(int i = 0; i <= a->numEstados; i++){ //iteração estados
        Estado currentEstado = a->estados[i];
        bool achouEstado = true;


        for(int j = 0; j <= currentEstado.numIdParaConvercao; j++){ //iteração numeros de converçao do estado de fora
            bool achouId = false;

            for(int k = 0; k <= e.numIdParaConvercao; k++){ //iteração numeros de converçao do estado E
                if(currentEstado.idParaConvercao[j] == e.idParaConvercao[k]){
                    achouId = true;
                } else {
                    achouId = false;
                }
            }

            if(achouId == false){
                achouEstado = false;
                break;
            }
        }

        if(achouEstado == true) {
            return currentEstado.id;
        }
    }
    return -1; 
}

void gerarEstadosPorSimbolo(Automato *atualAFN, Automato *convertidoAFD, int idOrigem, char simbolo){

    Estado estadoGerado = gerarEstadoDestinoDoSimbolo(atualAFN, convertidoAFD, idOrigem, simbolo);

    int IdEstadoExiste = validarExistenciaEstado(estadoGerado, convertidoAFD);

    if(IdEstadoExiste == -1){
        inserirEstadoAutomato(&estadoGerado, convertidoAFD);
    }
}

void inserirTransicaoAutomato(Automato *atualAFN, Automato *convertidoAFD, int index, char simbolo){
     Estado fakeEstado = {.numIdParaConvercao = -1};    

        for(int j=0; j<= convertidoAFD->estados[index].numIdParaConvercao; j++ ){ //iteraçao sobre ids

            for(int k =0; k<= atualAFN->numTransicoes; k++){

                if( convertidoAFD->estados[index].idParaConvercao[j] == atualAFN->Transicoes[k].idOrigem && atualAFN->Transicoes[k].simbolo == simbolo){
                    fakeEstado.numIdParaConvercao = fakeEstado.numIdParaConvercao + 1;
                    fakeEstado.idParaConvercao[fakeEstado.numIdParaConvercao] = atualAFN->Transicoes[k].idDestino;
                }
            }
        }

        
        for(int z =0; z<= convertidoAFD->numEstados; z++){ //iteraçao siobre estados de destino
            if(convertidoAFD->estados[z].numIdParaConvercao == fakeEstado.numIdParaConvercao){

                bool ehOmesmoEstado = true;

                for(int y = 0; y<= fakeEstado.numIdParaConvercao; y++){ //iteracao dos ids do fakeEstado
                    bool ehIgual = false;

                    for(int w = 0; w <= convertidoAFD->estados[z].numIdParaConvercao; w++){ //iteracao sobre os ids do estado
                        if(convertidoAFD->estados[z].idParaConvercao[w] == fakeEstado.idParaConvercao[y]){
                            ehIgual = true;
                        }
                    }
                    if(ehIgual == false){
                        ehOmesmoEstado = false;
                    }
                }
                if(ehOmesmoEstado == true){
                    convertidoAFD->numTransicoes = convertidoAFD->numTransicoes + 1;
                    Transicao a = {.idDestino = convertidoAFD->estados[z].id, .idOrigem = convertidoAFD->estados[index].id, .simbolo = simbolo};
                    convertidoAFD->Transicoes[convertidoAFD->numTransicoes] = a;
                }
            }
        }
}

void converterAutomato(Automato *atualAFN, Automato *convertidoAFD){
    criarEstadoInicial(atualAFN, convertidoAFD);
    //obter estado que a transação deve ir com o simbolo 

    for (int i = atualAFN->idInicio; i <= atualAFN->numEstados; i++ ){

        gerarEstadosPorSimbolo(atualAFN, convertidoAFD, i, '0');
        gerarEstadosPorSimbolo(atualAFN, convertidoAFD, i, '1');
    }


    for(int i=0; i<= convertidoAFD->numEstados; i++){ //iteraçaõ sobre estado origem
        inserirTransicaoAutomato(atualAFN, convertidoAFD, i, '0');
        inserirTransicaoAutomato(atualAFN, convertidoAFD, i, '1');

    }
               
}

void gerarXml(Automato *a){
    FILE * file;
    file = fopen("AFD.xml", "w"); 
    fprintf(file,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?><!--Created with JFLAP 6.4.-->\n");
    fprintf(file,"<structure>\n\t<type>fa</type>\n\t<automaton>\n\t\t<!--The list of states.-->\n");
    
    for(int i =0; i<=a->numEstados; i++){
        fprintf(file,"\t\t\t<state id=\"%d\" name=\"[", a->estados[i].id);

        for(int j =0; j<= a->estados[i].numIdParaConvercao; j++){
            fprintf(file,"%d-", a->estados[i].idParaConvercao[j]);
        }
        fseek(file, -1, SEEK_CUR); //volta cursor
        fprintf(file,"]\">\n");
        fprintf(file,"\t\t\t\t<x>55.0</x>\n");
        fprintf(file,"\t\t\t\t<y>85.0</y>\n");
        
        if(a->estados[i].id == a->idInicio){
            fprintf(file,"\t\t\t\t<initial/>\n");
        }
        if(a->estados[i].ehAceite == true){
            fprintf(file,"\t\t\t\t<final/>\n");
        }
        fprintf(file,"\t\t\t</state>\n");
        
    }
    
    fprintf(file,"\t\t<!--The list of transitions.-->\n");
    
    for(int i =0; i<=a->numTransicoes; i++){
        fprintf(file,"\t\t\t<transition>\n");
        fprintf(file,"\t\t\t\t<from>%d</from>\n", a->Transicoes[i].idOrigem);
        fprintf(file,"\t\t\t\t<to>%d</to>\n", a->Transicoes[i].idDestino);
        fprintf(file,"\t\t\t\t<read>%c</read>\n", a->Transicoes[i].simbolo);
        fprintf(file,"\t\t\t</transition>\n");
    }

    fprintf(file,"\t</automaton>\n</structure>\n");


}

int findTransacaoIndexPorIdOrigem(Automato *a, int idOrigem, char simbolo){

    for(int i= 0; i <= a->numTransicoes; i++){
        Transicao currentTransicao = a->Transicoes[i];
        if(currentTransicao.idOrigem == idOrigem && currentTransicao.simbolo == simbolo){
            return i;
        }
    }

    return -1;
}

bool frasePertenceAutomato(Automato *a, char frase[20]){
    int idCurrentState = a->idInicio;

    // imprimeAutomato(a);

    for(int i = 0; i <= strlen(frase)-1; i++){
    int indexTransacao = findTransacaoIndexPorIdOrigem(a, idCurrentState, frase[i]);
        if(indexTransacao == -1){
            return false;
        }
        idCurrentState = a->Transicoes[indexTransacao].idDestino;
    }

    int indexLastEstado = findEstadoIndexById(idCurrentState, a);

    return a->estados[indexLastEstado].ehAceite;
}

int main () {
    Automato AFN;
    Automato AFD;

    inputDefaultValues(&AFN);
    inputDefaultValues(&AFD);

    getAutomatoFromXmlFile(&AFN); //Mesmo automato apresentado no slide de converção

    converterAutomato(&AFN, &AFD);
    gerarXml(&AFD);

    char imprimir;
    printf("digite s para imprimir os automatos: ");
    scanf("%c",&imprimir);
    if(imprimir == 's'){
        printf("AFN:\n");
        imprimeAutomato(&AFN);
        printf("AFD:\n");
        imprimeAutomato(&AFD);
    }


    printf("\n\nAutomato convertido e salvo no arquivo AFD.xml\n");
    char continuar = 's';

    while (continuar == 's')
    {
        printf("Insira uma frase usando 0 ou 1 para simular: ");

        char frase[20];
        scanf("%s", frase);
        if( frasePertenceAutomato(&AFD, frase) ==true ){
            printf("A frase %s PERTENCE a linguagem\n", frase);
        } else {
            printf("A frase %s NAO PERTENCE linguagem\n", frase);
        }

        printf("digite 's' para testar uma nova frase: ");
        scanf("%1s", &continuar);
    }
    
    
    printf("\n");
    exit(EXIT_SUCCESS);    
    return 0;
}