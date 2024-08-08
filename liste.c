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
        printf("%s, %d\n", temp->nome_ricetta, temp->qta);
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




int main()
{
    return 0;
}