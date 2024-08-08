#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define CMD_LEN 20
#include <stdio.h>
#include <stdlib.h>

//-------------------- Liste  semplici (single linked)  -----------------------------------//
typedef struct Ordine
{
    char nome_ricetta[CMD_LEN];
    int qta;
    int tempo;
    int peso;
    struct Ordine *successore;
} Ordine;

void stampa_lista(Ordine *testa)
{
    Ordine *temp = testa;
    while (temp != NULL)
    {
        printf("%s, %d\n", temp->nome_ricetta, temp->tempo);
        temp = temp->successore;
    }
    return;
}

Ordine *crea_ordine(char *nome_ricetta, int qta, int tempo, int peso)
{
    Ordine *nuovo_nodo = (Ordine *)malloc(sizeof(Ordine));
    strcpy(nuovo_nodo->nome_ricetta, nome_ricetta);
    nuovo_nodo->qta = qta;
    nuovo_nodo->tempo = tempo;
    nuovo_nodo->peso = peso;
    nuovo_nodo->successore = NULL;
    return nuovo_nodo;
}

Ordine *inserisci_nodo_in_testa_ordini(Ordine *testa, Ordine *nodo)
{
    if (testa == NULL)
    {
        return nodo;
    }
    else
    {
        Ordine *temp = nodo;
        while (temp->successore != NULL)
        {
            temp = temp->successore;
        }
        temp->successore = testa;
        return nodo;
    }
}

// faccio funzione void per comodita di utilizzo

void sottoliste(Ordine *testa, Ordine **inizio, Ordine **fine)
{
    Ordine *lepre;
    Ordine *tartaruga;

    tartaruga = testa;
    lepre = testa->successore;

    // la lepre avanza due nodi e la tartaruga uno solo
    while (lepre != NULL)
    {
        lepre = lepre->successore;
        if (lepre != NULL)
        {
            tartaruga = tartaruga->successore;
            lepre = lepre->successore;
        }
    }

    *inizio = testa;
    *fine = tartaruga->successore;
    tartaruga->successore = NULL;
    return;
}

Ordine *merge_crescente(Ordine *a, Ordine *b)
{
    Ordine *testa = NULL;

    // casi base
    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    if (a->tempo <= b->tempo)
    {
        testa = a;
        testa->successore = merge_crescente(a->successore, b);
    }
    else
    {
        testa = b;
        testa->successore = merge_crescente(a, b->successore);
    }
    return testa;
}

Ordine *merge_decrescente(Ordine *a, Ordine *b)
{
    Ordine *testa = NULL;

    // casi base
    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    if (a->tempo <= b->tempo)
    {
        testa = a;
        testa->successore = merge_decrescente(a->successore, b);
    }
    else
    {
        testa = b;
        testa->successore = merge_decrescente(a, b->successore);
    }
    return testa;
}

void merge_sort(Ordine **testa_indirizzo)
{
    Ordine *testa = *testa_indirizzo;
    Ordine *a;
    Ordine *b;

    // caso base
    if (testa == NULL || testa->successore == NULL)
        return;

    // divido la lista testa in due sottoliste a e b
    sottoliste(testa, &a, &b);
    // ordino le due sottoliste
    merge_sort(&a);
    merge_sort(&b);

    // unisco le due sottoliste
    *testa_indirizzo = merge_crescente(a, b);
    return;
}

int main()
{
    Ordine *ordini = NULL;
    Ordine *ordine;

    ordine = crea_ordine("tommaso", 1, 10, 20);
    ordini = inserisci_nodo_in_testa_ordini(ordini, ordine);

    ordine = crea_ordine("tommaso", 1, 3, 20);
    ordini = inserisci_nodo_in_testa_ordini(ordini, ordine);

    ordine = crea_ordine("tommaso", 1, 5, 20);
    ordini = inserisci_nodo_in_testa_ordini(ordini, ordine);

    ordine = crea_ordine("tommaso", 1, 10, 20);
    ordini = inserisci_nodo_in_testa_ordini(ordini, ordine);

    ordine = crea_ordine("tommaso", 1, 11, 20);
    ordini = inserisci_nodo_in_testa_ordini(ordini, ordine);

    merge_sort(&ordini);

    stampa_lista(ordini);
    return 0;
}