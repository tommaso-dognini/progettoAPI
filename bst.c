#include <stdio.h>
#include <stdlib.h>
#define COUNT 10
#define N 256

//-------- BST  --------//
// Struct per arrichire albero...
typedef struct Dati
{
    char nome[N];
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



// Definisco il nodo sentilenna NILL
//  Per semplificazione di condizioni al contorno invece che usare tipo NULL
// impongo che tutte le foglie puntano a un nodo NILL sentinella
// chiave e colore sono indifferenti... Mi interessa che punti a se stesso (NILL)
Nodo NILL = {'g', -999, NULL, &NILL, &NILL, &NILL};

// FUNZIONI DI GESTIONE ALBERI ROSSO-NERI

// Funzione per la creazione di un nodo -> Malloc della struct con chiave data e puntatori a NILL
Nodo *crea_nodo(int chiave);

// inserimento come se fosse bst di nodo nell'albero -> restituisce l'abero = puntatore alla radice
void inserisci(Nodo **albero, Nodo* nuovo_nodo);

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

// Funzione che restituisce il puntatore del primo nodo, se esiste, con la chiave inserita.
Nodo *cerca(Nodo *radice, int chiave);

// Funzione che restituisce il successore (puntatore al nodo) del nodo passato
Nodo *successore(Nodo *nodo);

// Funzione che restituisce il successore (puntatore al nodo) del nodo passato
Nodo *predecessore(Nodo *nodo);

// FUNZIONI ACCESSORIE PER SOLA VISUALIZZAZIONE

// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(Nodo *root, int space)
{
    // Base case
    if (root == &NILL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process r child first
    print2DUtil(root->dx, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", root->chiave);

    // Process l child
    print2DUtil(root->sx, space);
}

// Wrapper over print2DUtil()
void print2D(Nodo *root)
{
    // Pass initial space count as 0
    print2DUtil(root, 'n');
}

// MAIN PER TEST
int main()
{
    // Inizializzo alebero = puntatore alla radice
    Nodo *albero, *nodo;
    albero = &NILL;

    // test inserimento
    int chiavi[] = {17, 20, 5, 4, 9, 18, 2, 1, 7, 13, 21, 8, 6};
    // printf("Chiave: %d \n", &chiave);
    int i;
    for (i = 0; i < 13; i++)
    {
        nodo = crea_nodo(chiavi[i]);
        inserisci(&albero, nodo);
    }
    printf("RADICE: %d \n", albero->chiave);
    stampa_in_ordine(albero);
    // print2D(albero);

    // test minimo e massimo
    printf("\n\n");
    printf("Il minimo e: %d\n", minimo(albero)->chiave);
    printf("Il massimo e: %d\n", massimo(albero)->chiave);

    // test cerca
    printf("\n\n");
    Nodo *x, *xa, *xb;
    int n = 20, na = 17, nb = 9;
    x = cerca(albero, n);
    printf("Nodo cercato: %d\n", x->chiave);

    xa = cerca(albero, na);
    printf("Nodo cercato: %d\n", xa->chiave);

    xb = cerca(albero, nb);
    printf("Nodo cercato: %d\n", xb->chiave);

    // successore e predecessore
    printf("\n\n");
    printf("Il successore di %d e: %d\n", x->chiave, successore(x)->chiave);
    printf("Il predecessore di %d e: %d\n", x->chiave, predecessore(x)->chiave);

    // test cancella
    printf("\n\n");
    printf("ELimina nodo %d\n", n);
    elimina_nodo(&albero, x);
    printf("RADICE: %d \n", albero->chiave);
    stampa_in_ordine(albero);

    printf("\n\n");
    printf("ELimina nodo %d\n", na);
    elimina_nodo(&albero, xa);
    printf("RADICE: %d \n", albero->chiave);
    stampa_in_ordine(albero);

    printf("\n\n");
    printf("ELimina nodo %d\n", nb);
    elimina_nodo(&albero, xb);
    printf("RADICE: %d \n", albero->chiave);
    stampa_in_ordine(albero);
    //print2D(albero);
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

void inserisci(Nodo **albero, Nodo* nuovo_nodo)
{
    // // creo il nodo
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
        y=y->padre;
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
        y=y->padre;
    }
    return y;
}
