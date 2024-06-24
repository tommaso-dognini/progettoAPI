#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define CMD_LEN 256
// ------- FUNZIONI STRUTTURE DATI -------------

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

// BST
//  Definisco il nodo sentilenna NILL
//  Per semplificazione di condizioni al contorno invece che usare tipo NULL
//  impongo che tutte le foglie puntano a un nodo NILL sentinella
//  chiave e colore sono indifferenti... Mi interessa che punti a se stesso (NILL)
Nodo NILL = {'g', -999, NULL, &NILL, &NILL, &NILL};

// FUNZIONI DI GESTIONE ALBERI ROSSO-NERI

// Funzione per la creazione di un nodo -> Malloc della struct con chiave data e puntatori a NILL
Nodo *crea_nodo(int chiave);

// inserimento come se fosse bst di nodo nell'albero -> restituisce l'abero = puntatore alla radice
void inserisci(Nodo **albero, Nodo *nuovo_nodo);

// Funzione per eliminare un nodo (come fosse bst)
void elimina_nodo(Nodo **albero, Nodo *nodo);

// Funzione ausiliaria per stampare l'albero in senso crescente
void stampa_in_ordine(Nodo *albero);

// Funzione per impiantare un sotto albero ad un nodo
void trapianta(Nodo **albero, Nodo *u, Nodo *v);

// Funzione che restituisce il minimo (puntatore al nodo) del sotto albero del nodo passato.
Nodo *minimo(Nodo *nodo);

// Funzione che restituisce il massimo (puntatore al nodo) del sotto albero del nodo passato
Nodo *massimo(Nodo *nodo);

// Funzione che restituisce il puntatore al primo nodo avente chiave = chiave, se esiste.
Nodo *cerca(Nodo *radice, int chiave);

// Funzione che restituisce il successore (puntatore al nodo) del nodo passato
Nodo *successore(Nodo *nodo);

// Funzione che restituisce il successore (puntatore al nodo) del nodo passato
Nodo *predecessore(Nodo *nodo);

//-----------------------------------------------------------------------------------------
//--------  FUNZIONI PASTICCERIA ----------------------------------------------------------
//-----------------------------------------------------------------------------------------

// Funzione aggiungi ricetta:  ricettario = puntatore a radice albero delle ricette, codice_ricetta = intero identificativo della ricetta , comando = stringa conentente (nome_ingrediente qta)
void aggiungi_ricetta(Nodo **ricettario, int codice_ricetta, char *comando);

// Funzione rimuovi ricetta:  ricettario = puntatore a radice albero delle ricette, codice_ricetta = intero identificativo della ricetta
void rimuovi_ricetta(Nodo **ricettario, int codice_ricetta);

// rifornimento (nome_ingrediente, qta, scadenza)

// ordine (nome_ricetta, numero_elementi_ordinati)

//--------  FUNZIONI ACCESSORIE ---------------
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

    int clock = 0; // istanti di tempo della simulazione

    Nodo *ricettario;
    // Nodo *magazzino, *ordini;
    char nome_ricetta[CMD_LEN];
    int codice_ricetta;
    char dati[CMD_LEN];

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
        scanf("%s", comando);
        n = strlen(comando);

        // aggiungi ricetta
        if (strcmp(comando, cmd1) == 0)
        {
            scanf("%s", nome_ricetta);
            codice_ricetta = calcola_codice(nome_ricetta, 0);
            printf("codice ricetta:  %d\n",codice_ricetta);
            // acquisisco il comando per intero -- espressione regolare che fa malloc e gestiesce in automatico lungheza fino a ENTER
            scanf("%c", &temp); // pulisco il buffer di input perche da ora in poi considero solo  ENTER come separatore
            scanf("%[^\n]s", dati); // acquisisco comando
            aggiungi_ricetta(&ricettario, codice_ricetta, dati);

            // pulisco e faccio le free
            codice_ricetta = 0;
            *nome_ricetta = 0;
            //free(dati);
        }

        // rimuovi ricetta
        if (strcmp(comando, cmd2) == 0)
        {
            scanf("%s", nome_ricetta);
            codice_ricetta = calcola_codice(nome_ricetta, 0);
            rimuovi_ricetta(&ricettario, codice_ricetta);
        }

        // ordine
        if (strcmp(comando, cmd3) == 0)
        {
            printf("%s\n", comando);
        }

        // rifornimento
        if (strcmp(comando, cmd4) == 0)
        {
            printf("%s\n", comando);
        }

        // inizzializzo lunghezza comando
        comando[0] = 0;

        // aumento contatore istanti di tempo
        clock++;
    }

    // STAMPO SITUAZIONE CORRIERE
    // stampo alberi
    printf("RADICE: %d \n", ricettario->chiave);
    stampa_in_ordine(ricettario);
    return 0;
}

// ------- FUNZIONI STRUTTURE DATI -------------

Nodo *crea_nodo(int chiave)
{
    // creo il nodo
    Nodo *nuovo_nodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    nuovo_nodo->chiave = chiave;
    nuovo_nodo->colore = 'r';
    nuovo_nodo->dx = &NILL;
    nuovo_nodo->sx = &NILL;
    nuovo_nodo->padre = &NILL;

    return nuovo_nodo;
}

void inserisci(Nodo **albero, Nodo *nuovo_nodo)
{
    // creo il nodo
    // Nodo *nuovo_nodo = crea_nodo(chiave);

    Nodo *x = *albero;
    Nodo *y = &NILL;

    while (x != &NILL)
    {
        y = x;
        if (nuovo_nodo->chiave < x->chiave)
            x = x->sx;
        else
            x = x->dx;
    }

    nuovo_nodo->padre = y;
    if (y == &NILL)
        (*albero) = nuovo_nodo;
    else if (nuovo_nodo->chiave < y->chiave)
        y->sx = nuovo_nodo;
    else
        y->dx = nuovo_nodo;

    nuovo_nodo->sx = &NILL;
    nuovo_nodo->dx = &NILL;
    nuovo_nodo->colore = 'r';
    return;
}

void stampa_in_ordine(Nodo *albero)
{
    if (albero != &NILL)
    {
        stampa_in_ordine(albero->sx);
        printf("%4c-%d", albero->colore, albero->chiave);
        stampa_in_ordine(albero->dx);
    }
    return;
}

void trapianta(Nodo **albero, Nodo *u, Nodo *v)
{
    //!!!!!!!!!!!! VERIFICARE SE CE DA FARE UNA FREE DI U !!!!!!!!!!!!!!!
    if (u->padre == &NILL)
        (*albero) = v;
    else if (u == u->padre->sx)
        u->padre->sx = v;
    else
        u->padre->dx = v;

    // if (v != &NILL) -->libro lo mette per bst ma affinche funzioni anche per rb lo tolgo
    v->padre = u->padre;
    return;
}

Nodo *minimo(Nodo *nodo)
{
    while (nodo->sx != &NILL)
        nodo = nodo->sx;

    return nodo;
}

Nodo *massimo(Nodo *nodo)
{
    while (nodo->dx != &NILL)
        nodo = nodo->dx;

    return nodo;
}

Nodo *cerca(Nodo *radice, int chiave)
{
    // NOTA: se il nodo cercato non esiste ritorna NILL, la cui chiave e -999
    if (radice == &NILL || chiave == radice->chiave)
        return radice;
    if (chiave < radice->chiave)
        return cerca(radice->sx, chiave);
    else
        return cerca(radice->dx, chiave);
}

void elimina_nodo(Nodo **albero, Nodo *nodo)
{
    Nodo *y = nodo;
    Nodo *x;

    // verifico che il nodo esiste
    if (cerca(nodo, nodo->chiave) == &NILL)
    {
        printf("Nodo inesistente!\n");
        return;
    }

    // se si procedo con eliminazione
    if (nodo->sx == &NILL)
    {
        x = nodo->dx;
        trapianta(albero, nodo, nodo->dx);
    }
    else if (nodo->dx == &NILL)
    {
        x = nodo->sx;
        trapianta(albero, nodo, nodo->sx);
    }
    else
    {
        y = minimo(nodo->dx);
        x = y->dx;
        if (y != nodo->dx)
        {
            trapianta(albero, y, y->dx);
            y->dx = nodo->dx;
            y->dx->padre = y;
        }
        else
        {
            x->padre = y;
        }
        trapianta(albero, nodo, y);
        y->sx = nodo->sx;
        y->sx->padre = y;
        y->colore = nodo->colore;
    }

    // ho eliminato il nodo che volevo eliminare: per non lasciare garbage faccio la free
    free(nodo);
    return;
}

Nodo *successore(Nodo *nodo)
{
    if (nodo->dx != &NILL)
        return minimo(nodo->dx);

    Nodo *y = nodo->padre;
    while (y != &NILL && nodo == y->dx)
    {
        nodo = y;
        y = y->padre;
    }
    return y;
}

Nodo *predecessore(Nodo *nodo)
{
    if (nodo->dx != &NILL)
        return massimo(nodo->sx);

    Nodo *y = nodo->padre;
    while (y != &NILL && nodo == y->sx)
    {
        nodo = y;
        y = y->padre;
    }
    return y;
}

//--------  FUNZIONI ACCESSORIE ---------------

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

//--------  FUNZIONI PASTICCERIA ---------------
void aggiungi_ricetta(Nodo **ricettario, int codice_ricetta, char *comando)
{
    // char nome_ingrediente[CMD_LEN];
    // int qta;

    printf("codice ricetta: %d", codice_ricetta);

    // verifico se la ricetta e gia esistente
    if (cerca(*ricettario, codice_ricetta) == &NILL)
    { // se esiste
        printf("ignorato\n");
    }
    else
    { // se non esiste
        // creo il nodo della ricetta
        Nodo *nodo = crea_nodo(codice_ricetta);

        // inserisco la lista di ingredienti come lista concatenata con puntatore alla ridice = nodo->dati

        // inserisco il nodo nell'albero.
        inserisci(ricettario, nodo);
        printf("aggiunta\n");
    }
    return;
}

void rimuovi_ricetta(Nodo **ricettario, int codice_ricetta)
{
    Nodo * nodo = cerca(*ricettario, codice_ricetta);
    if ( nodo == &NILL)
    {// se non ce
        printf("non presente\n");
    }
    else
    {
        elimina_nodo(ricettario, nodo);
    }
    return;
}