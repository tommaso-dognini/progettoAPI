#include <stdlib.h>
#include <stdio.h>

// --- ALBERI ROSSO NERI ---

// Definisco la struttura di un Nodo
typedef struct Nodo
{
    int chiave;
    int colore; // 1 se rosso , 0 se nero
    struct Nodo *padre, *sx, *dx;
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
Nodo nill = {0, 0, &nill, &nill, &nill};

// FUNZIONI DI GESTIONE ALBERI ROSSO-NERI

// Funzione per la creazione di un nodo -> Malloc della struct con chiave data e puntatori a NILL
Nodo *crea_nodo(int chiave);

// inserimento come se fosse bst di nodo nell'albero -> restituisce l'abero = puntatore alla radice
void inserisci(Nodo **albero, Nodo *nodo);

// Funzioni per la rotazione sx e dx attorno al nodo perno
void ruota_sx(Nodo **albero, Nodo *perno);
void ruota_dx(Nodo **albero, Nodo *perno);

// Funzione per aggiustare albero e mantenere bilanciamento e proprieta R-B dopo inserimento di un nodo
void inserisci_fixUp(Nodo **albero, Nodo *nodo);

// Funzione per eliminare un nodo (come fosse bst)
void elimina_nodo(Nodo **albero, Nodo *nodo);

// Funzione per aggiustare albero e mantere bilanciamento e proprieta r-d dopo eliminizione di un nodo
void elimina_fixUp(Nodo **albero);

// Funzione ausiliaria per stampare l'albero in senso crescente
void stampa_inOrdine(Nodo *albero);

int main()
{
    // Inizializzo alebero = puntatore alla radice
    Nodo *albero;
    albero = &nill;
    Nodo *nodo;
    int chiavi[] = {1, 3, 4, 5};

    int i;
    for (i = 0; i < 4; i++)
    {
        nodo = crea_nodo(chiavi[i]);
        inserisci(&albero, nodo);
    }
    printf("RADICE: %d \n", albero->chiave);
    stampa_inOrdine(albero);

    return 0;
}

// Funzioni per la rotazione sx e dx atto
void ruota_sx(Nodo **albero, Nodo *x)
{
    // assumendo che x=nodo perno ha un figlio destro (altrimenti non posso fare)
    Nodo *y = x->dx;
    x->dx = y->sx;

    if (y->sx != &nill)
        y->sx->padre = x;

    y->padre = x->padre;

    if (x->padre == &nill)
        (*albero) = y;
    else if (x == x->padre->sx)
        x->padre->sx = y;
    else
        x->padre->dx = y;

    y->sx = x;
    x->padre = y;
}

void ruota_dx(Nodo **albero, Nodo *x)
{
    Nodo *y = x->sx;
    x->sx = y->dx;
    if (y->dx != &nill)
        y->dx->padre = x;
    y->padre = x->padre;
    if (x->padre == &nill)
        (*albero) = y;
    else if (x == x->padre->dx)
        x->padre->dx = y;
    else
        x->padre->sx = y;
    y->dx = x;
    x->padre = y;
}

// Funzione per la creazione di un nodo -> Malloc della struct con chiave data e puntatori a NILL
Nodo *crea_nodo(int chiave)
{
    // creo il nodo
    Nodo *nuovo_nodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    nuovo_nodo->chiave = chiave;
    nuovo_nodo->colore = 1; // rosso
    nuovo_nodo->dx = &nill;
    nuovo_nodo->sx = &nill;
    nuovo_nodo->padre = &nill;

    return nuovo_nodo;
}

// inserimento come se fosse bst di nodo nell'albero -> restituisce l'abero = puntatore alla radice
void inserisci(Nodo **albero, Nodo *nodo)
{
    Nodo *x = *albero;
    Nodo *y = &nill;

    while (x != &nill)
    {
        y = x;
        if (nodo->chiave < x->chiave)
            x = x->sx;
        else
            x = x->dx;
    }
    nodo->padre = y;
    if (y == &nill)
        (*albero) = nodo;
    else if (nodo->chiave < y->chiave)
        y->sx = nodo;
    else
        y->dx = nodo;

    inserisci_fixUp(albero, nodo);
    return;
}

void inserisci_fixUp(Nodo **albero, Nodo *nodo)
{
    Nodo *y;

    while (nodo->padre->colore == 1)
    { // rosso
        if (nodo->padre == nodo->padre->padre->sx)
        {
            y = nodo->padre->padre->dx;
            if (y->colore == 1)
            {
                nodo->padre->colore = 0;
                y->colore = 0;
                nodo->padre->padre->colore = 1;
                nodo = nodo->padre->padre;
            }
            else
            {
                if (nodo == nodo->padre->dx)
                {
                    nodo = nodo->padre;
                    ruota_sx(albero, nodo);
                }
                nodo->padre = 0;
                nodo->padre->padre->colore = 1;
                ruota_dx(albero, nodo->padre->padre);
            }
        }
        else
        {
            y = nodo->padre->padre->sx;
            if (y->colore == 1)
            {
                nodo->padre->colore = 0;
                y->colore = 0;
                nodo->padre->padre->colore = 1;
                nodo = nodo->padre->padre;
            }
            else
            {
                if (nodo == nodo->padre->sx)
                {
                    nodo = nodo->padre;
                    ruota_dx(albero, nodo);
                }
                nodo->padre = 0;
                nodo->padre->padre->colore = 1;
                ruota_sx(albero, nodo->padre->padre);
            }
        }
    }
    (*albero)->colore = 0;
}


// Funzione ausiliaria per stampare l'albero in senso crescente
void stampa_inOrdine(Nodo *albero)
{
    if (albero != &nill)
    {
        stampa_inOrdine(albero->sx);
        printf("%4c-%d", albero->colore, albero->chiave);
        stampa_inOrdine(albero->dx);
    }
    return;
}