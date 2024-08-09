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

// ---------------------------  HASH TABLE ---------------------------------------------//

// Definisco la struttura della hash table
typedef struct Bucket
{
    char string[CMD_LEN];
    struct Nodo *lista;
    struct Bucket *successore;
} Bucket;

typedef struct HashTable
{
    int dimensione;
    int num_buckets_inseriti;
    Bucket **buckets;
} HashTable;

// crea HashTable
void crea_ht(HashTable *ht)
{
    ht->dimensione = 1e9 + 9;
    ht->num_buckets_inseriti = 0;
    ht->buckets = (Bucket **)malloc(sizeof(struct Bucket *) * ht->dimensione);
}

// crea Bucket
Bucket *crea_bucket(char *string, Nodo *lista)
{
    Bucket *nuovo_bucket = (Bucket *)malloc(sizeof(Bucket));
    nuovo_bucket->successore = NULL;
    strcpy(nuovo_bucket->string, string);
    nuovo_bucket->lista = lista;
    return nuovo_bucket;
}

// funzione di hash: uso rolling polinomial con p = 53 e dimensione = 1e9 + 9
int hash(char *string)
{
    // vettore con valori di potenze di p precalcolati per aumentare efficienza. Calcolati fino a p ^CMD_LEN = 256
    int p_pow[] = {53, 2809, 148877, 7890481, 418195493, 164360931, 711129271, 689851030, 562104266, 791525837, 950868992, 396056126, 990974498, 521647926, 647339835, 309010949, 377580153, 11747929, 622640237, 999932273, 996410001, 809729585, 915667627, 530383799, 110341095, 848077990, 948133074, 251052472, 305780899, 206387503, 938537569, 742490716, 352007597, 656402479, 789331081, 834546924, 230986576, 242288420, 841286152, 588165660, 172779701, 157324072, 338175744, 923314279, 935656355, 589786374, 258677543, 709909662, 625211753, 136222612, 219798373, 649313670, 413624204, 922082623, 870378587, 130064697, 893428887, 351730588, 641721002, 11212800, 594278400, 496754921, 328010579, 384560534, 381708122, 230530286, 218105050, 559567551, 657079942, 825236620, 737540473, 89644718, 751170018, 812010603, 36561572, 937763307, 701454830, 177105657, 386599740, 489786040, 958659895, 808973985, 875620827, 407903417, 618880912, 800688048, 436466166, 132706591, 33449260, 772810771, 958970503, 825436209, 748118690, 650290219, 465381301, 665208737, 256062746, 571325421, 280247043, 853093153, 213936704, 338645213, 948196136, 254394758, 482922057, 594868796, 528045909, 986432934, 280945034, 890086676, 174593405, 253450384, 432870235, 942122257, 932479180, 421396099, 333993049, 701631444, 186466199, 882708466, 783548284, 528058683, 987109956, 316827200, 791841456, 967596799, 282629888, 979383938, 907348255, 89457083, 741225363, 284943888, 102025929, 407374192, 590831987, 314095032, 647036552, 292936950, 525658215, 859885152, 573912651, 417370233, 120622151, 392973949, 827619117, 863812814, 782078737, 450172692, 859152469, 535080452, 359263704, 40976141, 171735455, 101979034, 404888757, 459103932, 332508180, 622933387, 15469214, 819868342, 453021739, 10151951, 538053403, 516830107, 391995428, 775757504, 115147343, 102809125, 448883580, 790829533, 913964880, 440138208, 327324817, 348215148, 455402682, 136341930, 226122227, 984477932, 177329928, 398486103, 119763270, 347453256, 415022406, 996187329, 797927969, 290181979, 379644752, 121171676, 422098774, 371234824, 675445501, 798611238, 326395236, 298947355, 844209680, 743112644, 384969781, 403398213, 380105100, 145570120, 715216297, 906463408, 42560192, 255690158, 551578257, 233647360, 383309972, 315428336, 717701664, 38187850, 23956032, 269669687, 292493285, 502143970, 613630176, 522399040, 687148877, 418890157, 201178123, 662440429, 109342422, 795148321, 142860635, 571613592, 295520106, 662565483, 115970284, 146424998, 760524831, 307815683, 314231055, 654245771, 675025557, 776354206, 146772549, 778945034, 284086433, 56580814, 998783124, 935505104, 581770071, 833813493, 192114733, 182080759, 650280146, 464847432, 636913680, 756424743, 90511019, 797083971, 245450085, 8854388, 469282564, 871975676, 214710414, 379651843};
    // long long p = 53;
    long long m = 1e9 + 7;
    long long hash = 0;
    for (int i = 0; i < strlen(string); i++)
    {
        hash = (hash + (string[i] - 'a' + 1) * p_pow[i]) % m;
    }
    return hash;
}

// Restituisce NULL se il Bucket non c'e, altrimenti resituisce il puntatore al Bucket cercato.
Bucket *ht_cerca(HashTable *ht, char *string)
{
    // calcolo hash
    int indice = hash(string);

    if (ht->buckets[indice] == NULL)
        return NULL;
    else
    {
        Bucket *temp = ht->buckets[indice];
        // verifico se e il primo
        if (strcmp(temp->string, string) == 0)
            return temp;

        while (temp != NULL)
        {
            if (strcmp(temp->string, string) == 0)
                return temp;
            else
                temp = temp->successore;
        }
        return NULL;
    }
}

void ht_inserisci_ricettario(HashTable *ht, Bucket *nuovo_bucket, Bucket *temp, char *string)
{
    // calcolo hash
    int indice = hash(string);

    // non ce quindi inserisco in testa
    if (temp == NULL)
    {
        // inserimento in testa
        // Non ci sono altri bucket con stesso hash
        if (ht->buckets[indice] == NULL)
        {
            // Non ci sono altri bucket con stesso hash
            ht->buckets[indice] = nuovo_bucket;
        }
        else
        {
            // ci sono gia altri bucket, inserisco in testa
            nuovo_bucket->successore = ht->buckets[indice];
            ht->buckets[indice] = nuovo_bucket;
        }
        printf("aggiunta\n");
    }
    else
    {
        // significa che ce gia quindi non faccio nulla.
        printf("ignorata\n");
    }
    return;
}

void ht_elimina_ricetta(HashTable *ht, char *string)
{
    int indice = hash(string);

    if (ht->buckets[indice] == NULL)
    {
        // non ce niente da eliminare
        printf("Non presente\n");
        return;
    }
    else
    {
        Bucket *temp = ht->buckets[indice];
        Bucket *prec = ht->buckets[indice];

        // verifico se e' il primo
        if (strcmp(temp->string, string) == 0)
        {
            // elimino la lista di ingredienti di temp
            Nodo *nodo = temp->lista;
            Nodo *nodo_prec;
            while (nodo != NULL)
            {
                nodo_prec = nodo;
                nodo = nodo->successore;
                free(nodo_prec);
            }
            // modifico testa della lista
            ht->buckets[indice] = temp->successore;
            // elimino temp
            free(temp);
        }
        else
        {
            // altrimenti cerco il bucket e lo elimino
            while (strcmp(temp->string, string) != 0 && temp->successore != NULL)
            {
                prec = temp;
                temp = temp->successore;
            }
            if (strcmp(temp->string, string) == 0)
            {
                prec->successore = temp->successore;

                // elimino la lista di ingredienti di temp
                Nodo *nodo = temp->lista;
                Nodo *nodo_prec;
                while (nodo != NULL)
                {
                    nodo_prec = nodo;
                    nodo = nodo->successore;
                    free(nodo_prec);
                }

                // elimino temp
                free(temp);
            }
            else
            {
                // non ce niente da eliminare
                printf("Non presente\n");
            }
        }
    }
    return;
}

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

// MAIN PER TEST
int main()
{
    char string1[] = "cioccolato";
    // char string2[] = "zucchero";
    //  char string3[] = "fecola";

    HashTable *ricettario = (HashTable *)malloc(sizeof(HashTable));

    crea_ht(ricettario);

    Bucket *bucket = crea_bucket(string1, NULL);

    Nodo *nodo1 = crea_nodo("zucchero", 10, 1);
    Nodo *nodo2 = crea_nodo("pane", 10, 1);
    Nodo *nodo3 = crea_nodo("acqua", 10, 1);

    inserisci_nodo_in_testa(nodo1, nodo2);
    inserisci_nodo_in_testa(nodo1, nodo3);
    stampa_lista(nodo1);

    Bucket *temp = ht_cerca(ricettario, bucket->string);
    ht_inserisci_ricettario(ricettario, bucket, temp, bucket->string);

    printf("%s\n", ht_cerca(ricettario, string1) != NULL ? ht_cerca(ricettario, string1)->string : "not found");

    ht_elimina_ricetta(ricettario, string1);

    printf("%s\n", ht_cerca(ricettario, string1) != NULL ? ht_cerca(ricettario, string1)->string : "not found");

    stampa_lista(bucket->lista);
    return 0;
}

// ------------------------  MAGAZZINO ---------------------------------//

void ht_elimina_lotto(HashTable *ht, char *string)
{
    int indice = hash(string);

    if (ht->buckets[indice] == NULL)
    {
        // non ce niente da eliminare
        printf("Non presente\n");
        return;
    }
    else
    {
        Bucket *temp = ht->buckets[indice];
        Bucket *prec = ht->buckets[indice];

        while (strcmp(temp->string, string) != 0 && temp->successore != NULL)
        {
            prec = temp;
            temp = temp->successore;
        }
        if (strcmp(temp->string, string) == 0)
        {
            prec->successore = temp->successore;

            // elimino la lista di ingredienti di temp
            Nodo *nodo = temp->lista;
            Nodo *nodo_prec;
            while (nodo != NULL)
            {
                nodo_prec = nodo;
                nodo = nodo->successore;
                free(nodo_prec);
            }

            // elimino temp
            free(temp);
        }
        else
        {
            // non ce niente da eliminare
            printf("Non presente\n");
        }
    }
    return;
}

//-------------------------------------- MAGAZZINO -----------------------------------//

void ht_inserisci_lotto(HashTable *ht, Nodo *lotto, char *string)
{
    // calcolo hash
    int indice = hash(string);

    // se il bucket non esiste
    if (ht->buckets[indice] == NULL)
    {
        // inizializzo il bucket e lo inserisco
        Bucket *nuovo_bucket = crea_bucket(string, lotto);
        ht->buckets[indice] = nuovo_bucket;
        return;
    }
    else
    {
        // altrimenti lo cerco e modifico soltanto la sua lista di lotti aggiungendo lotto in testa.
        Bucket *bucket = ht_cerca(ht, string);
        // faccio inserimento in testa alla lista bucket->lista
        bucket->lista = inserisci_nodo_in_testa(bucket->lista, lotto);
    }
    return;
}
