#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define CMD_LEN 256

//FUNZIONI PASTICCERIA
// aggiungi ricetta (nome_ricetta, nome_ingrediente1 , qta1, nome_ingrediente2 , qta2, ....)

// rimuovi ricetta (nome_ricetta)

// rifornimento (nome_ingrediente, qta, scadenza)

// ordine (nome_ricetta, numero_elementi_ordinati)

//FUNZIONI ACCESSORIE


int main()
{
    // prendo parametri corriere
    int periodo, capienza;

    scanf("%d %d", &periodo, &capienza);
    printf("Il periodo e: %d \nLa capienza del corriere e: %d \n", periodo, capienza);

    // inizio a processare comandi in ingresso, termino quando non ci sono piu comandi <--> scanf vuoto <--> lunghezza input =0
    char comando[CMD_LEN];
    int n = 1; // numero di caretteri letti da input
    char temp; //variabile accessoria per pulire buffer da ENTER

    //COMANDI POSSIBILI
    char cmd1[] = "aggiungi_ricetta";
    char cmd2[] = "rimuovi_ricetta";
    char cmd3[] = "ordine";
    char cmd4[] = "rifornimento";
    int len_cmd1 = strlen(cmd1);
    int len_cmd2 = strlen(cmd2);
    int len_cmd3 = strlen(cmd3);
    int len_cmd4 = strlen(cmd4);
    
    int clock = 0; //istanti di tempo della simulazione

    while (n > 0)
    {
        // VERIFICA CORRIERE
        if (clock % periodo == 0 && clock != 0)
        {
            // gestisco il corriere
            printf("corriere\n");
        }

        //GESTISCO COMANDO
        scanf("%c", &temp); // pulisco il buffer di input perche da ora in poi considero solo  ENTER come separatore
        scanf("%[^\n]s", comando);
        n = strlen(comando);

        // aggiungi ricetta
        if (strncmp(comando, cmd1, len_cmd1) == 0)
        {
            printf("%s\n", comando);
        }

        // rimuovi ricetta
        if (strncmp(comando, cmd2, len_cmd2) == 0)
        {
            printf("%s\n", comando);
        }

        // ordine
        if (strncmp(comando, cmd3, len_cmd3) == 0)
        {
            printf("%s\n", comando);
        }

        // rifornimento
        if (strncmp(comando, cmd4, len_cmd4) == 0)
        {
            printf("%s\n", comando);
        }

        // inizzializzo lunghezza comando
        comando[0] = 0;
        // aumento contatore istanti di tempo
        clock++;
    }

    // STAMPO SITUAZIONE CORRIERE
    return 0;
}
