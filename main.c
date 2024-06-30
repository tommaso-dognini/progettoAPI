#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define CMD_LEN 256
//-----------------------------------------------------------------------------------------
//--------  FUNZIONI STRUTTURE DATI ----------------------------------------------------------
//-----------------------------------------------------------------------------------------

// Definisco la struttura di un Nodo
// lascio attibuto colore per poi adattare a rb
typedef struct Nodo
{
    char colore; // 'r' = rosso , 'n' = nero
    int chiave;
    struct Nodo *ingredienti;
    int qta; // utilizzo solo per ordini e per albero degli ingredienti di una ricetta
    struct Nodo *dx;
    struct Nodo *sx;
    struct Nodo *padre;
} Nodo;

// BST
//  Definisco il nodo sentilenna NILL
//  Per semplificazione di condizioni al contorno invece che usare tipo NULL
//  impongo che tutte le foglie puntano a un nodo NILL sentinella
//  chiave e colore sono indifferenti... Mi interessa che punti a se stesso (NILL)
Nodo NILL = {'g', -999, &NILL, 0, &NILL, &NILL, &NILL};

// FUNZIONI DI GESTIONE ALBERI

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

// Funzione aggiungi ricetta:  ricettario = puntatore a radice albero delle ricette, codice_ricetta = intero identificativo della ricetta
void aggiungi_ricetta(Nodo **ricettario, int codice_ricetta);

// Funzione aggiungi ricetta:  ricettario = puntatore a radice albero delle ricette, codice_ricetta = intero identificativo della ricetta
void aggiorna_ricetta(Nodo **ricettario, int codice_ricetta, char *ingrediente, int qta);

// Funzione rimuovi ricetta:  ricettario = puntatore a radice albero delle ricette, codice_ricetta = intero identificativo della ricetta
void rimuovi_ricetta(Nodo **ricettario, int codice_ricetta);

// rifornimento (nome_ingrediente, qta, scadenza)

// ordine (nome_ricetta, numero_elementi_ordinati)

//-----------------------------------------------------------------------------------------
//--------  FUNZIONI ACCESSORIE ----------------------------------------------------------
//-----------------------------------------------------------------------------------------

// Funzione che calcola, data una stringa in input ed un ofset la codifica in decimale. (la codifica e univoca)
int calcola_codice(char *nome, int offset);

//-----------------------------------------------------------------------------------------
//--------  MAIN --------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

int main()
{

    char comando[CMD_LEN];
    int clock = 0; // istanti di tempo della simulazione

    Nodo *ricettario = &NILL;
    Nodo *nodo = &NILL; // nodo accessorio
    Nodo *ingrediente = &NILL;
    //   Nodo *magazzino, *ordini;
    char nome_ricetta[CMD_LEN];
    // char ordine[CMD_LEN];
    char nome_ingrediente[CMD_LEN];
    int qta = 0;
    int codice_ricetta;
    int controllo = 0;
    char separatore = 'a';

    // acquisisco parametri corriere
    int periodo, capienza;

    scanf("%d %d", &periodo, &capienza);
    printf("Il periodo e: %d \nLa capienza del corriere e: %d \n", periodo, capienza);
    controllo = scanf("%c", &separatore);

    while (separatore == '\n' && controllo != -1)
    {
        // VERIFICA CORRIERE
        if (clock % periodo == 0 && clock != 0)
        {
            // gestisco il corriere
            printf("corriere\n");
        }
        // ACQUISISCO COMANDO
        controllo = scanf("%s", comando);
        // AGGIUNGI_RICETTA
        if (strcmp(comando, "aggiungi_ricetta") == 0)
        {
            controllo = scanf("%s", nome_ricetta);
            controllo = scanf("%c", &separatore);
            codice_ricetta = calcola_codice(nome_ricetta, 0);
            printf("Nome ricetta:%s Codice ricetta:%d\n", nome_ricetta, codice_ricetta);

            if (cerca(ricettario, codice_ricetta) != &NILL)
            {
                // LA RICETTA E GIA PRESENTE -> LA IGNORO
                printf("ignorata\n");
                // DEVO LO STESSO CONSUMARE INPUT DA STDIN
                while (separatore != '\n')
                {
                    controllo = scanf("%s", nome_ingrediente);
                    controllo = scanf("%d", &qta);
                    controllo = scanf("%c", &separatore);
                }
            }
            else
            {
                // CREO IL NODO RICETTA
                nodo = crea_nodo(codice_ricetta);
                while (separatore != '\n')
                {
                    controllo = scanf("%s", nome_ingrediente);
                    controllo = scanf("%d", &qta);
                    controllo = scanf("%c", &separatore);
                    printf("Ingrediente:%s,qta:%d\n", nome_ingrediente, qta);
                    // AGGIUNGO INGREDIENTE ALLA NODO RICETTA
                    ingrediente = crea_nodo(calcola_codice(nome_ingrediente, 0));
                    ingrediente->qta = qta;
                    inserisci(&(nodo->ingredienti), ingrediente);
                }
                // AGGIUNGO IL NODO RICETTA AL RICETTARIO
                inserisci(&ricettario, nodo);
                printf("Stampa in ordine\n");
                stampa_in_ordine(ricettario);
                stampa_in_ordine(nodo->ingredienti);
            }
        }

        // RIMUOVI_RICETTA
        else if (strcmp(comando, "rimuovi_ricetta") == 0)
        {
            controllo = scanf("%s", nome_ricetta);
            controllo = scanf("%c", &separatore);
            codice_ricetta = calcola_codice(nome_ricetta, 0);
            printf("Nome ricetta:%s, Codice ricetta:%d\n", nome_ricetta, codice_ricetta);
            // RIMUOVO RICETTA DA RICETTARIO
            rimuovi_ricetta(&ricettario, codice_ricetta);
            stampa_in_ordine(ricettario);
        }

        // ORDINE
        else if (strcmp(comando, "ordine") == 0)
        {
            printf("Comando:%s\n", comando);
            controllo = scanf("%c", &separatore);
        }

        // RIFORNIMENTO
        else if (strcmp(comando, "rifornimento") == 0)
        {
            printf("Comando:%s\n", comando);
            controllo = scanf("%c", &separatore);
        }

        // AGGIUSTAMENTI
        comando[0] = 0;
        clock++;
    }

    // STAMPO SITUAZIONE CORRIERE
    printf("Fine, clock:%d", clock);
    // stampo alberi
    // printf("RADICE: %d \n", ricettario->chiave);
    // stampa_in_ordine(ricettario);
    return 0;
}

// ------- FUNZIONI STRUTTURE DATI -------------

Nodo *crea_nodo(int chiave)
{
    // creo il nodo
    Nodo *nuovo_nodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    nuovo_nodo->chiave = chiave;
    nuovo_nodo->ingredienti = &NILL;
    nuovo_nodo->qta = 0;
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
// void aggiungi_ricetta(Nodo **ricettario, int codice_ricetta)
// {
//     return;
// }

void rimuovi_ricetta(Nodo **ricettario, int codice_ricetta)
{
    Nodo *nodo = cerca(*ricettario, codice_ricetta);
    if (nodo == &NILL)
    { // se non ce
        printf("non presente\n");
    }
    else
    {
        // elimina albero degli ingredienti
        elimina_nodo(ricettario, nodo);
        printf("eliminata\n");
    }
    return;
}