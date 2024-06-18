#include <stdlib.h>
#include <stdio.h>

// --- ALBERI ROSSO NERI ---

// Definisco la struttura di un Nodo
typedef struct Nodo
{
    char colore; // 'r' = rosso , 'n' = nero
    int chiave;
    struct Nodo *dx;
    struct Nodo *sx;
    struct Nodo *padre;
} Nodo;

// Struct per arrichire albero...
typedef struct Dati
{
    int valore;
} Dati;

// Definisco il nodo sentilenna NILL
//  Per semplificazione di condizioni al contorno invece che usare tipo NULL
// impongo che tutte le foglie puntano a un nodo NILL sentinella
// chiave e colore sono indifferenti... Mi interessa che punti a se stesso (NILL)
Nodo NILL = {'n', -999, &NILL, &NILL, &NILL};

// FUNZIONI DI GESTIONE ALBERI ROSSO-NERI

// Funzione per la creazione di un nodo -> Malloc della struct con chiave data e puntatori a NILL
Nodo *crea_nodo(int chiave);

// inserimento come se fosse bst di nodo nell'albero -> restituisce l'abero = puntatore alla radice
void inserisci(Nodo **albero, int chiave);

// Funzione per aggiustare albero e mantenere bilanciamento e proprieta R-B dopo inserimento di un nodo
void inserisci_fixup(Nodo **albero, Nodo *nuovo_nodo);

// Funzioni per la rotazione sx e dx attorno al nodo perno
void ruota_dx(Nodo **albero, Nodo *nodo_perno);
void ruota_sx(Nodo **albero, Nodo *nodo_perno);

// Funzione ausiliaria per stampare l'albero in senso crescente
void stampa_in_ordine(Nodo *albero);

// Funzione per impiantare un sotto albero ad un nodo
void trapianta(Nodo **albero, Nodo *u, Nodo *v);

// Funzione che restituisce il minimo (puntatore al nodo) del sotto albero del nodo passato.
Nodo *minimo(Nodo *nodo);

// Funzione che restituisce il massimo (puntatore al nodo) del sotto albero del nodo passato
Nodo *massimo(Nodo *nodo);

// Funzione che restituisce il puntatore del primo nodo, se esiste, con la chiave inserita.
Nodo *cerca(Nodo *radice, int chiave);

// Funzione per eliminare un nodo (come fosse bst)
void elimina_nodo(Nodo **albero, Nodo *nodo);

// Funzione per aggiustare albero e mantere bilanciamento e proprieta r-d dopo eliminizione di un nodo
void elimina_fixUp(Nodo **albero, Nodo *nodo);

// MAIN PER TEST
int main()
{
    // Inizializzo alebero = puntatore alla radice
    Nodo *albero;
    albero = &NILL;

    // test inserimento
    int chiavi[] = {1, 3, 4, 5, 10, 20, 33, 18, 7, 9};
    // printf("Chiave: %d \n", &chiave);
    int i;
    for (i = 0; i < 10; i++)
    {
        inserisci(&albero, chiavi[i]);
    }
    printf("RADICE: %d \n", albero->chiave);
    stampa_in_ordine(albero);

    // test minimo e massimo
    printf("\n\n");
    printf("Il minimo e: %d\n", minimo(albero)->chiave);
    printf("Il massimo e: %d\n", massimo(albero)->chiave);

    // test cerca
    printf("\n\n");
    Nodo *x;
    int n = 7;
    x = cerca(albero, n);
    printf("Nodo cercato: %d\n", x->chiave);

    // test cancella
    printf("\n\n");
    printf("ELimina nodo %d\n", n);
    elimina_nodo(&albero, x);
    printf("RADICE: %d \n", albero->chiave);
    stampa_in_ordine(albero);

    return 0;
}

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

void inserisci(Nodo **albero, int chiave)
{
    // creo il nodo
    Nodo *nuovo_nodo = crea_nodo(chiave);

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

    inserisci_fixup(albero, nuovo_nodo);
    return;
}

void ruota_sx(Nodo **albero, Nodo *nodo_perno)
{
    Nodo *y = nodo_perno->dx;
    nodo_perno->dx = y->sx;

    if (y->sx != &NILL)
        nodo_perno->dx->padre = nodo_perno;

    y->padre = nodo_perno->padre;
    if (nodo_perno->padre == &NILL)
        *(albero) = y;
    else if (nodo_perno == nodo_perno->padre->sx)
        nodo_perno->padre->sx = y;
    else
        nodo_perno->padre->dx = y;

    y->sx = nodo_perno;
    nodo_perno->padre = y;

    return;
}

void ruota_dx(Nodo **albero, Nodo *nodo_perno)
{
    Nodo *y = nodo_perno->sx;
    nodo_perno->sx = y->dx;

    if (y->sx != &NILL)
        nodo_perno->sx->padre = nodo_perno;

    y->padre = nodo_perno->padre;
    if (nodo_perno->padre == &NILL)
        *(albero) = y;
    else if (nodo_perno == nodo_perno->padre->sx)
        nodo_perno->padre->sx = y;
    else
        nodo_perno->padre->dx = y;

    y->dx = nodo_perno;
    nodo_perno->padre = y;
}

void inserisci_fixup(Nodo **albero, Nodo *nuovo_nodo)
{
    Nodo *y;
    while (nuovo_nodo->padre->colore == 'r')
    {
        if (nuovo_nodo->padre == nuovo_nodo->padre->padre->sx)
        {
            y = nuovo_nodo->padre->padre->dx;

            // caso 1
            if (y->colore == 'r')
            {
                nuovo_nodo->padre->padre->sx->colore = 'n';
                y->colore = 'n';
                nuovo_nodo->padre->padre->colore = 'r';
                nuovo_nodo = nuovo_nodo->padre->padre;
            }
            else
            {
                // caso 2
                if (nuovo_nodo == nuovo_nodo->padre->dx)
                {
                    nuovo_nodo = nuovo_nodo->padre;
                    ruota_sx(albero, nuovo_nodo);
                }
                // caso 3
                nuovo_nodo->padre->colore = 'n';
                nuovo_nodo->padre->padre->colore = 'r';
                ruota_dx(albero, nuovo_nodo->padre->padre);
            }
        }
        else
        { // come sopra ma con sx e dx scambiati
            y = nuovo_nodo->padre->padre->sx;
            if (y->colore == 'r')
            { // caso 1
                nuovo_nodo->padre->padre->dx->colore = 'n';
                y->colore = 'n';
                nuovo_nodo->padre->padre->colore = 'r';
                nuovo_nodo = nuovo_nodo->padre->padre;
            }
            else
            {
                if (nuovo_nodo == nuovo_nodo->padre->sx)
                { // caso 2
                    nuovo_nodo = nuovo_nodo->padre;
                    ruota_dx(albero, nuovo_nodo);
                }
                // caso 3
                nuovo_nodo->padre->colore = 'n';
                nuovo_nodo->padre->padre->colore = 'r';
                ruota_sx(albero, nuovo_nodo->padre->padre);
            }
        }
    }
    (*albero)->colore = 'n';
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
    if (u->padre == &NILL)
        (*albero) = v;
    else if (u == u->padre->sx)
        u->padre->sx = v;
    else
        u->padre->dx = v;

    if (v != &NILL)
        v->padre = u->padre;
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
    char colore_originale_y;

    //verifico che il nodo esiste
    if (cerca(nodo,nodo->chiave)== &NILL)
    {
        printf("Nodo inesistente!\n");
        return;
    }

    //se si procedo con eliminazione
    colore_originale_y = y->colore;
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
        colore_originale_y = y->colore;
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
            trapianta(albero, nodo, y);
            y->sx = nodo->sx;
            y->sx->padre = y;
            y->colore = nodo->colore;
        }
        if (colore_originale_y == 'n')
            elimina_fixUp(albero, x);
    }
    return;
}

void elimina_fixUp(Nodo **albero, Nodo *nodo)
{
    Nodo *w;

    while (nodo != &NILL && nodo->colore == 'n')
    {
        if (nodo == nodo->padre->sx)
        {
            w = nodo->padre->dx;
            if (w->colore == 'r')
            { // caso 1
                w->colore = 'n';
                nodo->padre->colore = 'r';
                ruota_sx(albero, nodo->padre);
                w = nodo->padre->dx;
            }
            if (w->sx->colore == 'n' && w->dx->colore == 'n')
            { // caso 2
                w->colore = 'r';
                nodo = nodo->padre;
            }
            else
            {
                if (w->dx->colore == 'n')
                { // caso 3
                    w->sx->colore = 'n';
                    w->colore = 'r';
                    ruota_dx(albero, w);
                    w = nodo->padre->dx;
                }
                // caso 4
                w->colore = nodo->padre->colore;
                nodo->padre->colore = 'n';
                w->dx->colore = 'n';
                ruota_sx(albero, nodo->padre);
                nodo = (*albero);
            }
        }
        else
        { // come sopra ma con dx e sx scambiati
            w = nodo->padre->sx;
            if (w->colore == 'r')
            { // caso 1
                w->colore = 'n';
                nodo->padre->colore = 'r';
                ruota_dx(albero, nodo->padre);
                w = nodo->padre->sx;
            }
            if (w->dx->colore == 'n' && w->sx->colore == 'n')
            { // caso 2
                w->colore = 'r';
                nodo = nodo->padre;
            }
            else
            {
                if (w->sx->colore == 'n')
                { // caso 3
                    w->dx->colore = 'n';
                    w->colore = 'r';
                    ruota_sx(albero, w);
                    w = nodo->padre->sx;
                }
                // caso 4
                w->colore = nodo->padre->colore;
                nodo->padre->colore = 'n';
                w->sx->colore = 'n';
                ruota_dx(albero, nodo->padre);
                nodo = (*albero);
            }
        }
        nodo->colore = 'n';
    }
    return;
}
