#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define CMD_LEN 256

// Struct per arrichire albero...
typedef struct Dati
{
    char nome[CMD_LEN];
    int info;
} Dati;

// Definisco la struttura di un Nodo
// lascio attibuto colore per poi adattare a rb
typedef struct Nodo
{
    char colore; // 'r' = rosso , 'n' = nero
    int chiave;
    Dati *dati;
    struct Nodo *dx;
    struct Nodo *sx;
    struct Nodo *padre;
} Nodo;

// FUNZIONI PASTICCERIA

// aggiungi ricetta (nome_ricetta, nome_ingrediente1 , qta1, nome_ingrediente2 , qta2, ....)
void aggiungi_ricetta(Nodo **ricettario, char *comando);

// rimuovi ricetta (nome_ricetta)
void rimuovi_ricetta(char *comando);

// rifornimento (nome_ingrediente, qta, scadenza)

// ordine (nome_ricetta, numero_elementi_ordinati)

// FUNZIONI ACCESSORIE
// Funzione che calcola, data una stringa in input ed un ofset la codifica in decimale. (la codifica e univoca)
int calcola_codice(char *nome, int offset);

int main()
{
    // prendo parametri corriere
    int periodo, capienza;

    scanf("%d %d", &periodo, &capienza);
    printf("Il periodo e: %d \nLa capienza del corriere e: %d \n", periodo, capienza);

    // inizio a processare comandi in ingresso, termino quando non ci sono piu comandi <--> scanf vuoto <--> lunghezza input =0
    char comando[CMD_LEN];
    int n = 1; // numero di caretteri letti da input
    char temp; // variabile accessoria per pulire buffer da ENTER

    // COMANDI POSSIBILI
    char cmd1[] = "aggiungi_ricetta";
    char cmd2[] = "rimuovi_ricetta";
    char cmd3[] = "ordine";
    char cmd4[] = "rifornimento";
    int len_cmd1 = strlen(cmd1);
    int len_cmd2 = strlen(cmd2);
    int len_cmd3 = strlen(cmd3);
    int len_cmd4 = strlen(cmd4);

    int clock = 0; // istanti di tempo della simulazione

    Nodo *ricettario;
    // Nodo *magazzino, *ordini;

    while (n > 0)
    {
        // VERIFICA CORRIERE
        if (clock % periodo == 0 && clock != 0)
        {
            // gestisco il corriere
            printf("corriere\n");
        }

        // GESTISCO COMANDO
        scanf("%c", &temp); // pulisco il buffer di input perche da ora in poi considero solo  ENTER come separatore
        //scanf("%[^\n]s", comando);
        scanf("%s", comando);
        n = strlen(comando);

        // aggiungi ricetta
        if (strncmp(comando, cmd1, len_cmd1) == 0)
        {
            // printf("%s\n", comando);
            aggiungi_ricetta(&ricettario, &comando[len_cmd1]);
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

int calcola_codice(char *nome, int offset)
{
    int len = strlen(nome);
    int i, codice = offset;
    for (i = 0; i < len; i++)
    {
        codice = codice + (int)nome[i];
    }
    return codice;
}

void aggiungi_ricetta(Nodo **ricettario, char *comando)
{
    char nome_ricetta[CMD_LEN];
    int codice_ricetta;
    //char nome_ingrediente[CMD_LEN];
    //int qta;

    // calcolo codice_ricetta
    sscanf(comando, "%s", nome_ricetta);
    printf("nome ricetta: %s", nome_ricetta);
    codice_ricetta = calcola_codice(nome_ricetta, 0);
    printf("codice ricetta: %d", codice_ricetta);

    // verifico se la ricetta e gia esistente
    // if (cerca(ricettario, codice_ricetta) == & NILL)
    // {
    //     // se esiste
    //     printf("ignorato\n");
    // }
    // else
    // {
    //     // se non esiste
    //     // creo il nodo della ricetta
    //     // inserisco la lista di ingredienti come lista concatenata con puntatore alla ridice = nodo->dati
    //     // inserisco il nodo nell'albero.
    // }
    return;
}
