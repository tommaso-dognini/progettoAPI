#include <stdio.h>
#include <stdlib.h>

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
Nodo *inserisci(Nodo *albero, Nodo *nodo);
Nodo* bst(Nodo* trav, Nodo* temp);

// Funzioni per la rotazione sx e dx attorno al nodo perno
void ruota_sx(Nodo *albero, Nodo *perno);
void ruota_dx(Nodo *albero, Nodo *perno);

// Funzione per aggiustare albero e mantenere bilanciamento e proprieta R-B dopo inserimento di un nodo
void inserisci_fixUp(Nodo *albero, Nodo *nodo);

void fixup(Nodo* root, Nodo* pt);

// Funzione per eliminare un nodo (come fosse bst)
void elimina_nodo(Nodo *albero, Nodo *nodo);

// Funzione per aggiustare albero e mantere bilanciamento e proprieta r-d dopo eliminizione di un nodo
void elimina_fixUp(Nodo *albero);

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
        albero = inserisci(albero, nodo);
    }
    printf("RADICE: %d \n", albero->chiave);
    stampa_inOrdine(albero);

    return 0;
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
Nodo *inserisci(Nodo *albero, Nodo *nodo)
{
    Nodo *x = albero; // radice

    // se l'albero e vuoto restituisco il nuovo nodo come radice
    if (albero == &nill)
    {
        // coloro di nero (perche diventa la radice)
        nodo->colore = 0; // nero
        // resituisco nodo come radice dell'albero
        return nodo;
    }
    // Altrimenti: l'albero non e vuoto

    // Scorro l'albero fino a che arrivo alla foglia dove voglio aggiungere (seguendo proprieta bst)
    while (x != &nill)
    {
        if (nodo->chiave < x->chiave)
            x = x->sx; // scorro nel sotto albero sinistro
        else
            x = x->dx; // scorro nell sotto albero destro
    }
    // quando esco so che x = &nill --> sono alla foglia dove fare inserimento.

    // Eseguo l'inserimento
    // il padre di nuovo nodo diventa il padre della foglia x
    nodo->padre = x->padre;

    // modifico puntatore del padre di x in modo che punti al nuovo nodo
    if (nodo->chiave < x->padre->chiave)
        x->padre->sx = nodo; // nuovo nodo e un figlio sinistro
    else
        x->padre->dx = nodo; // nuovo nodo diventa un figlio destro

    nodo->colore = 'r'; // lo metto rosso di defualt e sistemo problemi con fixUp
    inserisci_fixUp(albero, nodo);
    return albero;
}

// Funzioni per la rotazione sx attorno al nodo perno
void ruota_sx(Nodo *albero, Nodo *perno)
{
    Nodo *x = perno->dx;
    // sposto sottoalbero sinistro di x in quello destro di perno
    perno->dx = x->sx;

    // Se sotto albero destro di perno non e' nill
    if (perno->dx != &nill)
        perno->dx->padre = perno; // rendo perno padre del sotto albero destro di perno

    x->padre = perno->padre; // assegno a x il padre di perno

    // se il padre del perno e nill allora il perno e la radice e quindi per rotazione la radice diventa x
    if (perno->padre == &nill)
        albero = x;                     // x diventa la radice
    else if (perno == perno->padre->sx) // se perno era figlio sinistro
        perno->padre->sx = x;
    else // perno era figlio destro
        perno->padre->dx = x;

    x->sx = perno;
    perno->padre = x;
    return;
}
// Funzioni per la rotazione sx attorno al nodo perno
// uguale alla ruota_sx ma con sx e dx scambiati
void ruota_dx(Nodo *albero, Nodo *perno)
{
    Nodo *x = perno->sx;
    // sposto sottoalbero destro di x in quello sinistro di perno
    perno->sx = x->dx;

    // Se sotto albero sinistro di perno non e' nill
    if (perno->sx != &nill)
        perno->sx->padre = perno; // rendo perno padre del sotto albero sinistro di perno

    x->padre = perno->padre; // assegno a x il padre di perno

    // se il padre del perno e nill allora il perno e la radice e quindi per rotazione la radice diventa x
    if (perno->padre == &nill)
        albero = x;                     // x diventa la radice
    else if (perno == perno->padre->sx) // se perno era figlio sinistro
        perno->padre->sx = x;
    else // perno era figlio destro
        perno->padre->dx = x;

    x->dx = perno;
    perno->padre = x;
    return;
}

// Funzione per aggiustare albero e mantenere bilanciamento e proprieta R-B dopo inserimento di un nodo
void inserisci_fixUp(Nodo *albero, Nodo *nodo)
{
    Nodo *y;
    while (nodo->padre->colore == 1) // se padre e rosso
    {
        if (nodo->padre == nodo->padre->padre->sx)
        {
            y = nodo->padre->padre->dx;

            // caso 1
            if (y->colore == 1) // se rosso
            {
                nodo->padre->padre->sx->colore = 0; // nero
                y->colore = 0;                      // nero
                nodo->padre->padre->colore = 1;     // rosso
                nodo = nodo->padre->padre;
            }
            else
            {
                // caso 2
                if (nodo == nodo->padre->dx)
                {
                    nodo = nodo->padre;
                    ruota_sx(albero, nodo);
                }
                // caso 3
                nodo->padre->colore = 0;
                nodo->padre->padre->colore = 1;
                ruota_dx(albero, nodo);
            }
        }
        else
        { // come sopra ma con sx e dx scambiati
            y = nodo->padre->padre->sx;
            if (y->colore == 1)
            { // caso 1
                nodo->padre->padre->dx->colore = 0;
                y->colore = 0;
                nodo->padre->padre->colore = 1;
                nodo = nodo->padre->padre;
            }
            else
            {
                if (nodo == nodo->padre->sx)
                { // caso 2
                    nodo = nodo->padre;
                    ruota_dx(albero, nodo);
                }
                // caso 3
                nodo->padre->colore = 0;
                nodo->padre->padre->colore = 1;
                ruota_sx(albero, nodo);
            }
        }
    }
    albero->colore = 0;
    return;
}


void fixup(Nodo* root, Nodo* pt)
{
	Nodo* parent_pt = &nill;
	Nodo* grand_parent_pt = &nill;

	while ((pt != root) && (pt->colore != 0)
		&& (pt->padre->colore == 1)) 
	{
		parent_pt = pt->padre;
		grand_parent_pt = pt->padre->padre;

		/* Case : A
			Parent of pt is left child 
			of Grand-parent of
		pt */
		if (parent_pt == grand_parent_pt->sx) 
		{

			Nodo* uncle_pt = grand_parent_pt->dx;

			/* Case : 1
				The uncle of pt is also red
				Only Recoloring required */
			if (uncle_pt != &nill && uncle_pt->colore == 1) 
			{
				grand_parent_pt->colore = 1;
				parent_pt->colore = 0;
				uncle_pt->colore = 0;
				pt = grand_parent_pt;
			}

			else {

				/* Case : 2
					pt is right child of its parent
					Left-rotation required */
				if (pt == parent_pt->dx) {
					ruota_sx(root,parent_pt);
					pt = parent_pt;
					parent_pt = pt->padre;
				}

				/* Case : 3
					pt is left child of its parent
					Right-rotation required */
				ruota_dx(root,grand_parent_pt);
				int t = parent_pt->colore;
				parent_pt->colore = grand_parent_pt->colore;
				grand_parent_pt->colore = t;
				pt = parent_pt;
			}
		}

		/* Case : B
			Parent of pt is right 
			child of Grand-parent of
		pt */
		else {
			Nodo* uncle_pt = grand_parent_pt->sx;

			/* Case : 1
				The uncle of pt is also red
				Only Recoloring required */
			if ((uncle_pt != &nill) && (uncle_pt->colore == 1)) 
			{
				grand_parent_pt->colore = 1;
				parent_pt->colore = 0;
				uncle_pt->colore = 0;
				pt = grand_parent_pt;
			}
			else {
				/* Case : 2
				pt is left child of its parent
				Right-rotation required */
				if (pt == parent_pt->sx) {
					ruota_dx(root, parent_pt);
					pt = parent_pt;
					parent_pt = pt->padre;
				}

				/* Case : 3
					pt is right child of its parent
					Left-rotation required */
				ruota_sx(root, grand_parent_pt);
				int t = parent_pt->colore;
				parent_pt->colore = grand_parent_pt->colore;
				grand_parent_pt->colore = t;
				pt = parent_pt;
			}
		}
	}
}

Nodo* bst(Nodo* trav, Nodo* temp)
{
	// If the tree is empty, 
	// return a new node
	if (trav == &nill)
		return temp;

	// Otherwise recur down the tree
	if (temp->chiave < trav->chiave) 
	{
		trav->sx = bst(trav->sx, temp);
		trav->sx->padre = trav;
	}
	else if (temp->chiave > trav->chiave) 
	{
		trav->dx = bst(trav->dx, temp);
		trav->dx->padre = trav;
	}

	// Return the (unchanged) node pointer
    fixup(trav,temp);
	return trav;
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