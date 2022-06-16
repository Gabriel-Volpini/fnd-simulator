import java.util.Scanner;
// Frederico Prado Marques & Paulo Henrique Luiz Pereira
public class Main{
    public static void main(String []args){
        Scanner scanner = new Scanner( System.in );
        String sentenca = "0";
        Automato automato = new Automato("AFN.xml");
        System.out.println("\n=============================|AFN de entrada|=============================\n");
        automato.print();
        automato.saveAsXML("AFD.xml");
        do {
            System.out.println("\n==============================|AFN--->>>AFD|==============================\n");
            System.out.println("Digite a sentença que deseja testar ou q! para sair:\n");
            sentenca = scanner.nextLine();
            System.out.println("\nSua sentença será: ");        
            if (automato.simular(sentenca)) {
                System.out.println("\nACEITA\n");
            }
            if (!(automato.simular(sentenca))) {
                System.out.println("\nREJEITADA");
            }
        } while (sentenca != "q!");
    }
}