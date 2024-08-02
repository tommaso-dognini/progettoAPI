#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define CMD_LEN 20
#include <stdio.h>
#include <stdlib.h>

//-------------------- Liste  semplici (single linked)  -----------------------------------//
// Definisco la struttura di un Nodo
typedef struct Nodo
{
    char nome_ingrediente[CMD_LEN];
    int qta;
    int scadenza;
    struct Nodo *successore;
} Nodo;

//----------------------------------- LISTE ------------------------------//
Nodo *crea_nodo(char *nome_ingrediente, int qta, int scadenza)
{
    Nodo *nuovo_nodo = (Nodo *)malloc(sizeof(Nodo));
    strcpy(nuovo_nodo->nome_ingrediente, nome_ingrediente);
    nuovo_nodo->qta = qta;
    nuovo_nodo->scadenza = scadenza;
    nuovo_nodo->successore = NULL;
    return nuovo_nodo;
}

Nodo *inserisci_nodo_in_testa(Nodo *testa, Nodo *nodo)
{
    if (testa == NULL)
    {
        return nodo;
    }
    else
    {
        Nodo *temp = nodo;
        while (temp->successore != NULL)
        {
            temp = temp->successore;
        }
        temp->successore = testa;
        return nodo;
    }
}

Nodo *elimina_nodo(Nodo *testa, Nodo *nodo)
{
    Nodo *temp = testa;
    // se e il primo
    if (testa == nodo)
    {
        testa = testa->successore;
        free(temp);
        return testa;
    }
    // altrimenti lo cerco
    while (temp->successore != nodo && temp->successore != NULL)
    {
        temp = temp->successore;
    }
    if (temp->successore != NULL)
    {
        temp->successore = temp->successore->successore;
        free(nodo);
    }
    return testa;
}

void *elimina_lista(Nodo *testa)
{
    Nodo *temp = testa;
    Nodo *prec = testa;
    while (temp != NULL)
    {
        prec = temp;
        temp = temp->successore;
        free(prec);
    }
    return NULL;
}

Nodo *min_scadenza(Nodo *testa)
{
    Nodo *min = testa;
    Nodo *temp = testa;

    if (testa == NULL)
        return testa;

    while (temp != NULL)
    {
        if (temp->scadenza < min->scadenza)
        {
            min = temp;
        }
        temp = temp->successore;
    }
    return min;
}

void stampa_lista(Nodo *testa)
{
    Nodo *temp = testa;
    while (temp != NULL)
    {
        printf("%s, %d ,%d\n", temp->nome_ingrediente, temp->qta, temp->scadenza);
        temp = temp->successore;
    }
    return;
}

int main()
{
    // char string[50];
    int v[] = {1, 2, 3};
    Nodo *lista = NULL;
    Nodo *nuovo_nodo;
    Nodo *nuovo_nodo2;

    // printf("String: ");
    // scanf("%s", string);
    // printf("Lista:\n");
    nuovo_nodo2 = crea_nodo("tommaso", 1, v[0]);
    lista = inserisci_nodo_in_testa(lista, nuovo_nodo2);
    stampa_lista(lista);

    nuovo_nodo = crea_nodo("riccardo", 1, v[1]);
    lista = inserisci_nodo_in_testa(lista, nuovo_nodo);
    stampa_lista(lista);

    nuovo_nodo = crea_nodo("francesca", 1, v[2]);
    lista = inserisci_nodo_in_testa(lista, nuovo_nodo);
    stampa_lista(lista);

    printf("Minimo:\n");
    printf("%s, scadenza: %d", min_scadenza(lista)->nome_ingrediente, min_scadenza(lista)->scadenza);

    lista = elimina_nodo(lista, nuovo_nodo2);
    stampa_lista(lista);

    elimina_lista(lista);
    stampa_lista(lista);

    return 0;
}