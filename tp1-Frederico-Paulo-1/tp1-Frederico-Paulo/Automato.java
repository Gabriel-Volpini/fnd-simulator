import java.io.File;
import java.util.*;
import java.io.*;

class Estado_adjacente{
    private int idDestino;
    private char entradaNecessaria;
    public Estado_adjacente(int idDestino, char entradaNecessaria){
        this.idDestino = idDestino;
        this.entradaNecessaria = entradaNecessaria;
    }
    public int getIdDestino(){
        return idDestino;
    }
    public char getEntradaNecessaria(){
        return entradaNecessaria;
    }
}

class Estado{
    private String nome;
    private int id;
    private ArrayList<Estado_adjacente> estadosAdjacentes = new ArrayList<Estado_adjacente>();

    public Estado(String nome, int id){
        this.nome = nome;
        this.id = id;
    }

    public int getId(){
        return id;
    }

    public String getNome(){
        return nome;
    }

    public ArrayList<Estado_adjacente> getEstadosAdjacentes(){
        ArrayList<Estado_adjacente> copiaEstadosAdjacentes = new  ArrayList<Estado_adjacente>();
        for(int i = 0; i < estadosAdjacentes.size(); i++){
            copiaEstadosAdjacentes.add(estadosAdjacentes.get(i));
        }
        return copiaEstadosAdjacentes;
    }

    public ArrayList<Integer> proximosEstados(char entrada){
        ArrayList<Integer> output = new ArrayList<Integer>();
        Estado_adjacente ea;
        for(int i = 0; i < estadosAdjacentes.size(); i++){
            ea = estadosAdjacentes.get(i);
            if(ea.getEntradaNecessaria() == entrada){
                output.add(ea.getIdDestino());
            }
        }
        return output;
    }

    public void addAjacente(int id, char entradaNecessaria){
        Estado_adjacente ea = new Estado_adjacente(id, entradaNecessaria);
        estadosAdjacentes.add(ea);
    }

    public void print(){
        System.out.println("Estado: " + nome + " ("+ id +")");
        System.out.println("Estados adjacentes:");
        for(int i=0; i<estadosAdjacentes.size(); i++){
            System.out.println("\tID: " + estadosAdjacentes.get(i).getIdDestino() + " | Entrada necessaria: " + estadosAdjacentes.get(i).getEntradaNecessaria()); 
        }
    }
}

public class Automato {
    private int inicio;
    private ArrayList<Integer> aceite = new ArrayList<Integer>();
    private ArrayList<Estado> estados = new ArrayList<Estado>();

    public Automato(String path){
        readXML(path);
        if(true){
            Nd_to_D();
        }
    }

    private ArrayList<Estado_adjacente> adjacenteA(ArrayList<Integer> origem){
        ArrayList<Estado_adjacente> aux = new ArrayList<Estado_adjacente>();
        int i;
        boolean flag = true;
        for(int j = 0; j< origem.size(); j++){
            i = 0;
            flag = true;
            while(flag){
                if(estados.get(i).getId() == origem.get(j))
                    flag = false;
                else
                    i++;
            }
            aux.addAll(estados.get(i).getEstadosAdjacentes());
        }
        return aux;
    }

    private Estado findEstadoById(ArrayList<Estado> listaEstados, int id){
        Estado e = null;
        for(int i =0; i < listaEstados.size(); i++){
            if(listaEstados.get(i).getId() == id){
                e = listaEstados.get(i);
                i += listaEstados.size();
            }
        }
        return e;
    }

    private Estado findEstadoByName(ArrayList<Estado> listaEstados, String nome){
        Estado e = null;
        for(int i =0; i < listaEstados.size(); i++){
            if(listaEstados.get(i).getNome().compareTo(nome) == 0){
                e = listaEstados.get(i);
                i += listaEstados.size();
            }
        }
        return e;
    }

    private boolean eAceite(int id){
        boolean result = false;
        for(int k = 0; k < this.aceite.size() && !result; k++){
            result = this.aceite.get(k) == id;
        }
        return result;
    }
    
    private void Nd_to_D(){
        ArrayList<Estado> novosEstados = new ArrayList<Estado>();
        ArrayList<Integer> novosAceites = new ArrayList<Integer>();

        ArrayList<ArrayList<Integer>> aSimular = new ArrayList<ArrayList<Integer>>();
        ArrayList<ArrayList<Estado_adjacente>> possiveis_estados;
        ArrayList<Estado_adjacente> destinos = new ArrayList<>();


        ArrayList<Integer> novoASimular = new ArrayList<>();
        novoASimular.add(inicio);
        aSimular.add(novoASimular);

        Estado aux = new Estado(String.valueOf(findEstadoById(this.estados, inicio).getId()), findEstadoById(this.estados, inicio).getId());
        novosEstados.add(aux);

        ArrayList<Estado_adjacente> estadosAdjacentes;
        
        while(aSimular.size() != 0){
            //coleta todos os estados que podem ser alcançados a partir dos estados atuais e os de divide de acordo com a entrada necessaria para alcançalos
            estadosAdjacentes = adjacenteA(aSimular.get(0));
            possiveis_estados = new ArrayList<ArrayList<Estado_adjacente>>();
            for(int i = 0; i < estadosAdjacentes.size(); i++){

                if(i == 0){
                    destinos = new ArrayList<>();
                    destinos.add(estadosAdjacentes.get(i));
                    possiveis_estados.add(destinos);
                }else{
                    for(int j = 0; j < possiveis_estados.size(); j++){
                        if(possiveis_estados.get(j).get(0).getEntradaNecessaria() == estadosAdjacentes.get(i).getEntradaNecessaria()){
                            possiveis_estados.get(j).add(estadosAdjacentes.get(i));
                            j += possiveis_estados.size();
                        }else if(j == possiveis_estados.size()-1){
                            destinos = new ArrayList<>();
                            destinos.add(estadosAdjacentes.get(i));
                            possiveis_estados.add(destinos);
                            j += possiveis_estados.size();
                        }
                    }

                }

            }
            // remove elementos duplicados (mesmo destino e mesma entrada necessaria)
            for(int i = 0; i < possiveis_estados.size(); i++){
                for(int j = 0; j <  possiveis_estados.get(i).size(); j++){
                    for(int k = j+1; k<  possiveis_estados.get(i).size(); k++){
                        if(possiveis_estados.get(i).get(j).getIdDestino() == possiveis_estados.get(i).get(k).getIdDestino()){
                            possiveis_estados.get(i).remove(k);
                        }
                    }
                }
            }
            //recurepa o estado que gerou os possiveis estados atuais para adicionalos como seus decendentes
            String nomeEstadoPai = new String();
            ArrayList<Integer> estadoPaiList = aSimular.get(0);
            for(int j = 0; j < estadoPaiList.size(); j++){
                nomeEstadoPai +=  String.valueOf(estadoPaiList.get(j)) + (j == estadoPaiList.size()-1 ? "" : "-");
            }
            Estado estadoPai = findEstadoByName(novosEstados, nomeEstadoPai);

            String nomeNovoEstado;
            Estado novoEstado;
            ArrayList<Integer> novoEstadoLista;
            int idEstadoAdjacente = -1;
            boolean flagNovoEstado, aceite;

            //gera um nome para para cada possivel estado a partir dos seus antigos id, observa se ele ja esta entre os novos estados, 
            //caso não estaja o aadiciona e o bota como adjacente do estado que o gerou
            for(int i = 0; i < possiveis_estados.size(); i++){
                nomeNovoEstado = new String();
                novoEstadoLista = new ArrayList<Integer>();
                flagNovoEstado = true;
                aceite = false;
                
                //inicio - gera nome
                for(int j = 0; j < possiveis_estados.get(i).size(); j++){
                    novoEstadoLista.add(possiveis_estados.get(i).get(j).getIdDestino());
                }

                Collections.sort(novoEstadoLista);

                for(int j = 0; j < novoEstadoLista.size(); j++){
                    nomeNovoEstado +=  String.valueOf(novoEstadoLista.get(j)) + (j == novoEstadoLista.size()-1 ? "" : "-");
                }
                //fim - gera nome

                //checa se ja esta na lista de novos estados
                for(int j = 0; j < novosEstados.size() && flagNovoEstado; j++){
                    if(novosEstados.get(j).getNome().compareTo(nomeNovoEstado) == 0){
                        flagNovoEstado = false;
                        idEstadoAdjacente = novosEstados.get(j).getId();
                    }
                }

                //caso o estado não esteja presente o adiciona na lista de novos estados
                if(flagNovoEstado){
                    for(int j = 0; j < possiveis_estados.get(i).size() && !aceite; j++){
                        aceite = eAceite(possiveis_estados.get(i).get(j).getIdDestino());
                    }
                    if(aceite)
                        novosAceites.add(novosEstados.size());
                    idEstadoAdjacente = novosEstados.size();
                    novoEstado = new Estado(nomeNovoEstado, novosEstados.size());
                    novosEstados.add(novoEstado);
                    aSimular.add(novoEstadoLista);
                }
                //coloca o estado novo como adjacente a aquele que o gerou
                if(idEstadoAdjacente != -1){
                    estadoPai.addAjacente(idEstadoAdjacente, possiveis_estados.get(i).get(0).getEntradaNecessaria());
                }else{
                    System.out.println("erro");
                }
                
            }
            aSimular.remove(0);
        }
        this.aceite = novosAceites;
        this.estados = novosEstados;
    }

    public boolean simular(String entrada){
        Estado estadoAtual = findEstadoById(this.estados, this.inicio);
        ArrayList<Integer> aux;
        boolean result = true;
        for(int i = 0; i < entrada.length() && result; i ++){
            aux = estadoAtual.proximosEstados(entrada.charAt(i));
            if(aux == null){
                result = false;
            }else{
                estadoAtual = findEstadoById(this.estados, aux.get(0));
            }
        }

        if(result){
            result = false;
            for(int i = 0; i < this.aceite.size() && !result; i++){
                result = this.aceite.get(i) == estadoAtual.getId();
            }
        }
        
        return result;
    }

    public void saveAsXML(String outPutName){
        try {
            FileWriter fileWriter = new FileWriter(outPutName);
            PrintWriter printWriter = new PrintWriter(fileWriter);

            ArrayList<String> estadosOutPut = new ArrayList<String>();
            ArrayList<String> transicoesOutPut = new ArrayList<String>();
            String aux = new String();
            boolean aceite, inicio;
            for(int i = 0; i < this.estados.size(); i++){
                aceite = eAceite(this.estados.get(i).getId());
                inicio = this.estados.get(i).getId() == this.inicio;
                aux = "\t\t<state id=\""+this.estados.get(i).getId()+"\" name=\""+this.estados.get(i).getNome()+"\">\n\t\t\t<x>100.0</x>\n\t\t\t<y>48.0</y>\n"+ (inicio ? "\t\t\t<initial/>\n" : "") + (aceite ? "\t\t\t<final/>\n" : "") +"\t\t</state>";
                estadosOutPut.add(aux);
                for(int j =0; j < this.estados.get(i).getEstadosAdjacentes().size(); j++){
                    aux = "\t\t<transition>\n\t\t\t<from>"+this.estados.get(i).getId()+"</from>\n\t\t\t<to>" + this.estados.get(i).getEstadosAdjacentes().get(j).getIdDestino() + "</to>\n\t\t\t<read>" + this.estados.get(i).getEstadosAdjacentes().get(j).getEntradaNecessaria() + "</read>\n\t\t</transition>";
                    transicoesOutPut.add(aux);
                }
            }

            printWriter.println("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?><!--Created with JFLAP 6.4.--><structure>");
            printWriter.println("\t<type>fa</type>");
            printWriter.println("\t<automaton>");
            for(int i = 0; i < estadosOutPut.size(); i++){
                printWriter.println(estadosOutPut.get(i));
            }
            for(int i = 0; i < transicoesOutPut.size(); i++){
                printWriter.println(transicoesOutPut.get(i));
            }
            printWriter.println("\t</automaton>");
            printWriter.println("</structure>");
            printWriter.close();
        } catch (Exception e) {
            //TODO: handle exception
        }
    }
    
    private void readXML(String path){
        try {
            File file = new File(path);
            FileReader fr = new FileReader(file);
            BufferedReader br = new BufferedReader(fr);  //creates a buffering character input stream  
            StringBuffer sb = new StringBuffer();
            String line = new String();
            String id, nome;
            String from, to, read;
            String[] idNome;
            Estado estadoAux;

            boolean flagInitial, flagFinal;

            while((line = br.readLine()) != null){
                if(line.indexOf("state id") != -1 && line.indexOf("/") == -1){
                    flagInitial = false;
                    flagFinal = false;
                    idNome = line.split("\" name=\"");
                    id = idNome[0];
                    nome = idNome[1];

                    id = id.replaceAll(".*\"", "");
                    nome = nome.replace("\">", "");
            
                    estadoAux = new Estado(nome, Integer.parseInt(id));
                    this.estados.add(estadoAux);

                    do{
                        line = br.readLine();
                        if(line != null){
                            flagInitial = flagInitial || line.indexOf("<initial/>") != -1;
                            flagFinal = flagFinal || line.indexOf("<final/>") != -1;
                        }
                    }while(line != null && line.indexOf("</state>") == -1);

                    if(flagFinal){
                        this.aceite.add(Integer.parseInt(id));
                    }

                    if(flagInitial){
                        this.inicio = Integer.parseInt(id);
                    }
                } else if(line.indexOf("from") != -1){
                    //from
                    from = line.replaceAll("\t*<.{1,5}>", "");
                    //to
                    to = br.readLine();
                    to = to.replaceAll("\t*<.{1,5}>", "");
                    //read
                    read = br.readLine();
                    read = read.replaceAll("\t*<.{1,5}>", "");

                    for(int i = 0; i < this.estados.size(); i++){
                        if(this.estados.get(i).getId() == Integer.parseInt(from)){
                            this.estados.get(i).addAjacente(Integer.parseInt(to), read.charAt(0));
                        }
                    }
                }

            }
            br.close();
        } catch (IOException  e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }

    public void print(){
        for(int i = 0; i < estados.size(); i++){
            estados.get(i).print();
            for(int j = 0; j < this.aceite.size(); j++){
                if(estados.get(i).getId() == this.aceite.get(j)){
                    System.out.println("\tEstado de aceite");
                }
            }
            if(estados.get(i).getId() == this.inicio){
                System.out.println("\tEstado de inicio");
            }
        }
    }
    }