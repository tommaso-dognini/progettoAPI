#include <stdlib.h>
#include <stdio.h>

// dichiaro la struttura nodo
typedef struct Nodo
{
    char colore; // 'r' = rosso , 'n' = nero
    int chiave;
    struct Nodo *dx;
    struct Nodo *sx;
    struct Nodo *padre;
} Nodo;
// inizializzo la setinella nill (il valore della chiave e irrilevante)
Nodo NILL = {'n', 0, &NILL, &NILL, &NILL};

Nodo *crea_nodo(int chiave);
void inserisci(Nodo **albero, int chiave);
void inserisci_fixup(Nodo **albero, Nodo *nuovo_nodo);
void ruota_dx(Nodo **albero, Nodo *nodo_perno);
void ruota_sx(Nodo **albero, Nodo *nodo_perno);
void stampa_in_ordine(Nodo *albero);

int main()
{
    // Inizializzo alebero = puntatore alla radice
    Nodo *albero;
    albero = &NILL;

    int chiavi[] = {1, 3, 4, 5 , 10 , 20, 33 , 18 , 7 , 9};
    // printf("Chiave: %d \n", &chiave);
    int i;
    for (i = 0; i < 10; i++)
    {
        inserisci(&albero, chiavi[i]);
    }
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

// void inserisci(Nodo **albero, int chiave)
// {
//     // creo il nodo
//     Nodo *nuovo_nodo = crea_nodo(chiave);

//     Nodo *x = *albero;
//     Nodo *y = &NILL;

//     // se albero vuoto imposto nuovo nodo come radice e lo coloro di nero
//     if (x == &NILL)
//     {
//         (*albero) = nuovo_nodo;
//         (*albero)->colore = 'n';
//     }
//     else
//     {
//         while (x != &NILL)
//         {
//             if (nuovo_nodo->chiave < x->chiave)
//                 x = x->sx;
//             else
//                 x = x->dx;
//         }

//         nuovo_nodo->padre = y->padre;
//         if (nuovo_nodo->chiave < y->chiave)
//             y->dx = nuovo_nodo;
//         else
//             y->sx = nuovo_nodo;

//         nuovo_nodo->sx = &NILL;
//         nuovo_nodo->dx = &NILL;
//         nuovo_nodo->colore = 'r';

//         inserisci_fixup(albero, nuovo_nodo);
//     }
//     return;
// }

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

// void ruota_sx(Nodo **albero, Nodo *nodo_perno)
// {
//     Nodo *y = nodo_perno->dx;
//     nodo_perno->dx = y->sx;

//     if (y->sx != &NILL)
//         y->sx->padre = nodo_perno;

//     y->padre = nodo_perno->padre;
//     if (nodo_perno->padre == &NILL)
//         *(albero) = y;
//     else if (nodo_perno == nodo_perno->padre->sx)
//         nodo_perno->padre->sx = y;
//     else
//         nodo_perno->padre->dx = y;

//     y->sx = nodo_perno;
//     nodo_perno->padre = y;

//     return;
// }

// void ruota_dx(Nodo **albero, Nodo *nodo_perno)
// {
//     Nodo *y = nodo_perno->sx;
//     nodo_perno->sx = y->dx;

//     if (y->dx != &NILL)
//         y->dx->padre = nodo_perno;

//     y->padre = nodo_perno->padre;
//     if (nodo_perno->padre == &NILL)
//         *(albero) = y;
//     else if (nodo_perno == nodo_perno->padre->dx)
//         nodo_perno->padre->dx = y;
//     else
//         nodo_perno->padre->sx = y;
        
//     y->dx = nodo_perno;
//     nodo_perno->padre = y;
// }


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
