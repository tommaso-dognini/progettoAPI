#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define CMD_LEN 256
//-----------------------------------------------------------------------------------------
//--------  FUNZIONI STRUTTURE DATI ----------------------------------------------------------
//-----------------------------------------------------------------------------------------

// Definisco la struttura di un Nodo
typedef struct Nodo
{
    char nome_ingrediente[CMD_LEN];
    int qta;
    int scadenza;
    struct Nodo *successore;
} Nodo;

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

// Definisco la struttura di un Nodo Ordine
typedef struct Ordine
{
    char nome_ricetta[CMD_LEN];
    int qta;
    int tempo;
    int peso;
    struct Ordine *successore;
} Ordine;

//----------------- Liste  semplici (single linked) X RICETTARIO E MAGAZZINO ---------//

void stampa_lista(Nodo *testa);

Nodo *min_scadenza(Nodo *testa);

void *elimina_lista(Nodo *testa);

Nodo *elimina_nodo_ptr(Nodo *testa, Nodo *nodo);

Nodo *inserisci_nodo_in_testa(Nodo *testa, Nodo *nodo);

Nodo *crea_nodo(char *nome_ingrediente, int qta, int scadenza);

int calcola_peso(Nodo *testa);

// ---------------------------  ORDINI ---------------------------------------------//

void stampa_lista_ordini(Ordine *testa);

Ordine *elimina_lista_ordini(Ordine *testa);

Ordine *elimina_nodo_ptr_ordini(Ordine *testa, Ordine *nodo);

Ordine *inserisci_nodo_in_testa_ordini(Ordine *testa, Ordine *nodo);

Ordine *crea_ordine(char *nome_ricetta, int qta, int tempo, int peso);

// verifica se un ingrediente e presente in quantita sufficiente (non scaduto) per la ricetta desiderata
int verifica_ingrediente(HashTable *magazzino, Bucket **bucket, char *nome_ingrediente, int qta_necessaria, int clock);

// modifica le quantita nel ricettario sottraendo quelle utilizzate per produrre la ricetta
void produci_ordine(HashTable *magazzino, Bucket *Bucket_ricetta, int qta);

// ritorna 1 se la ricetta e presente nella lista oppure 0 se non e presente
int cerca_in_lista(Ordine *testa, char *nome_ricetta);

// --------------------------- FUNZIONI DI ORDINAMENTO LISTE ----------------------------//

// liste di ordini
void sottoliste_ordini(Ordine *testa, Ordine **inizio, Ordine **fine);

Ordine *merge_crescente_ordini(Ordine *a, Ordine *b);

Ordine *merge_decrescente_corriere(Ordine *a, Ordine *b);

void merge_sort_ordini(Ordine **testa_indirizzo);

void merge_sort_corriere(Ordine **testa_indirizzo);

// liste in hashtables
void sottoliste(Nodo *testa, Nodo **inizio, Nodo **fine);

Nodo *merge_crescente(Nodo *a, Nodo *b);

void merge_sort(Nodo **testa_indirizzo);

// ---------------------------  HASH TABLE ---------------------------------------------//

// crea HashTable
void inizializza_ht(HashTable *ht);

// crea Bucket
Bucket *crea_bucket(char *string, Nodo *lista);

// funzione di hash: uso rolling polinomial con p = 53 e dimensione = 1e9 + 9
int hash(char *string);

// Restituisce NULL se il Bucket non c'e, altrimenti resituisce il puntatore al Bucket cercato.
Bucket *ht_cerca(HashTable *ht, char *string);

// ---------------------------  RICETTARIO  ---------------------------------------------//

void ht_inserisci_ricetta(HashTable *ht, Bucket *nuovo_bucket, char *string);

void ht_elimina_ricetta(HashTable *ht, char *string);

void elimina_lotto(Bucket **bucket, Nodo *lotto);

// ---------------------------  MAGAZZINO ---------------------------------------------//
void ht_inserisci_lotto(HashTable *ht, Nodo *lotto, char *string);

//-----------------------------------------------------------------------------------------
//--------  MAIN --------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

int main()
{

    char comando[CMD_LEN];
    int clock = 0; // istanti di tempo della simulazione

    HashTable *ricettario = (HashTable *)malloc(sizeof(HashTable));
    HashTable *magazzino = (HashTable *)malloc(sizeof(HashTable));

    Nodo *ingrediente;
    // Nodo *nodo_temp;

    inizializza_ht(ricettario);
    inizializza_ht(magazzino);

    Ordine *ordini_pronti = NULL;
    Ordine *ordini_attesa = NULL;
    Ordine *ordini_corriere = NULL;

    Ordine *ordine = NULL;
    Ordine *ordine_prec;
    Ordine *ordine_corriere = NULL;

    Bucket *bucket = NULL;
    Bucket *bucket_temp = NULL;

    char nome_ricetta[CMD_LEN];
    char nome_ingrediente[CMD_LEN];

    int qta = 0;
    int scadenza = 0;
    int controllo = 0;
    int attesa = 0;
    int peso = 0;
    // int qta_necessaria = 0;
    char separatore = 'a';

    // acquisisco parametri corriere
    int periodo, capienza;
    int capienza_rimasta;
    int stop;

    controllo = scanf("%d %d", &periodo, &capienza);
    // printf("Il periodo e: %d \nLa capienza del corriere e: %d \n", periodo, capienza);
    controllo = scanf("%c", &separatore);

    while (separatore == '\n' && controllo != -1)
    {
        // printf("CLOCK: %d\n", clock);

        // VERIFICA CORRIERE
        if (clock % periodo == 0 && clock != 0)
        {
            // gestisco il corriere
            // scorro la lista di ordini pronti e metto in lista corriere
            capienza_rimasta = capienza;
            ordine = ordini_pronti;
            stop = 0;
            while (ordine != NULL && stop == 0)
            {
                if (ordine->peso <= capienza_rimasta)
                {
                    // creo il nuovo nodo ordine per la lista del corriere
                    ordine_corriere = crea_ordine(ordine->nome_ricetta, ordine->qta, ordine->tempo, ordine->peso);
                    // inserisco nella lista delgi ordini del corriere
                    ordini_corriere = inserisci_nodo_in_testa_ordini(ordini_corriere, ordine_corriere);

                    // aggiorno campienza rimasta
                    capienza_rimasta -= ordine->peso;

                    // rimuovo ordine dalla lista degli ordini
                    ordine_prec = ordine;
                    ordine = ordine->successore;
                    ordini_pronti = elimina_nodo_ptr_ordini(ordini_pronti, ordine_prec);
                }
                else
                {
                    stop = 1;
                    ordine = ordine->successore;
                }
            }

            // stampo come da specifica tempo di acquisizione, nome ricetta, qta
            if (ordini_corriere == NULL)
            {
                printf("camioncino vuoto\n");
            }
            else
            {
                // ordino la lista di ordini del corriere per peso in senso decrescente
                merge_sort_corriere(&ordini_corriere);

                ordine = ordini_corriere;
                while (ordine != NULL)
                {
                    printf("%d %s %d\n", ordine->tempo, ordine->nome_ricetta, ordine->qta);
                    ordine = ordine->successore;
                }
            }
            // elimino la lista del corriere
            ordini_corriere = elimina_lista_ordini(ordini_corriere);
        }

        //----------------------------- ACQUISISCO COMANDO -------------------//

        controllo = scanf("%s", comando);

        //----------------------------- GESTIONE COMANDI -------------------//

        // AGGIUNGI_RICETTA
        if (strcmp(comando, "aggiungi_ricetta") == 0)
        {
            controllo = scanf("%s", nome_ricetta);
            controllo = scanf("%c", &separatore);

            // printf("Nome ricetta:%s\n", nome_ricetta);
            bucket_temp = ht_cerca(ricettario, nome_ricetta);

            if (bucket_temp != NULL)
            {
                // LA RICETTA E GIA PRESENTE -> LA IGNORO
                printf("ignorato\n");
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
                bucket = crea_bucket(nome_ricetta, NULL);

                // ACQUISISCO INGREDIENTI E CREO LISTA INGREDIENTI
                while (separatore != '\n')
                {
                    controllo = scanf("%s", nome_ingrediente);
                    controllo = scanf("%d", &qta);
                    controllo = scanf("%c", &separatore);
                    // printf("Ingrediente:%s,qta:%d\n", nome_ingrediente, qta);

                    // AGGIUNGO INGREDIENTE ALLA NODO RICETTA
                    ingrediente = crea_nodo(nome_ingrediente, qta, 0);
                    bucket->lista = inserisci_nodo_in_testa(bucket->lista, ingrediente);
                }

                // AGGIUNGO IL NODO RICETTA AL RICETTARIO
                ht_inserisci_ricetta(ricettario, bucket, bucket->string);
                // stampa_lista(bucket->lista);
            }

            nome_ricetta[0] = 0;
        }

        // RIMUOVI_RICETTA
        else if (strcmp(comando, "rimuovi_ricetta") == 0)
        {
            // ACQUISISCO NOME RICETTA
            controllo = scanf("%s", nome_ricetta);
            controllo = scanf("%c", &separatore);

            // printf("Nome ricetta:%s\n", nome_ricetta);
            //    VERIFICO CHE NON SIA IN USO = ordini_attesa e CHE NON E' LA RICETTA DI UN ORDINE CHE NON HO ANCORA SPEDITO = oridini_pronti
            if (cerca_in_lista(ordini_attesa, nome_ricetta) == 1 || cerca_in_lista(ordini_pronti, nome_ricetta) == 1)
            {
                printf("ordini in sospeso\n");
            }
            else
            {
                // ALTRIMENTI RIMUOVO RICETTA DA RICETTARIO
                ht_elimina_ricetta(ricettario, nome_ricetta);
            }
        }

        // ORDINE
        else if (strcmp(comando, "ordine") == 0)
        {
            Bucket *bucket_ricetta;
            Nodo *nodo_ingrediente;

            controllo = scanf("%c", &separatore);

            // PROCESSO GLI ORDINI UNO AD UNO
            while (separatore != '\n')
            {
                attesa = 0; // inizzializzo valore di attesa per nuovo ordine a 0 cioe pronto per essere eseguito fino a prova contraria
                peso = 0;   // inizializzo peso dell'ordine

                controllo = scanf("%s", nome_ricetta);
                controllo = scanf("%d", &qta);
                controllo = scanf("%c", &separatore);
                // printf("Ordine:%s,qta:%d\n", nome_ricetta, qta);

                // PRELEVO LA RICETTA DA RICETTARIO
                bucket_ricetta = ht_cerca(ricettario, nome_ricetta);
                if (bucket_ricetta == NULL || bucket_ricetta->lista == NULL)
                {
                    // non ho la ricetta oppure ho una ricetta vuota -> rifiuto ordine
                    printf("rifiutato\n");
                }
                else
                { // HO LA RICETTA -> VERIFICO DI POTERLA PRODURRE: HO INGREDIENTI NON SCADUTI A SUFFICIENZA
                    printf("accettato\n");
                    // VERIFICO PER OGNI INGREDIENTE:
                    nodo_ingrediente = bucket_ricetta->lista;
                    while (nodo_ingrediente != NULL) // scorro tutta la ricetta indipendentemente da stato di attesa in modo da calcolare perlomeno il peso dell'ordine.
                    {
                        // calcolo peso dell'ordine
                        peso += (nodo_ingrediente->qta) * qta;

                        // CONTROLLO MAGAZZINO
                        bucket = ht_cerca(magazzino, nodo_ingrediente->nome_ingrediente);

                        if (bucket == NULL || bucket->lista == NULL)
                        {
                            // non ci sono ingredinti in magazzino
                            attesa = 1;
                        }
                        else
                        {   //ho una lista di lotti da controllare: voglio verificare di avere ingredienti non scaduti a sufficienza
                            if (verifica_ingrediente(magazzino, &bucket, nodo_ingrediente->nome_ingrediente, nodo_ingrediente->qta * qta, clock) == 0)
                            {
                                // ce un ingrediente che manca
                                attesa = 1;
                            }
                        }
                        // printf("%s, attesa=%d\n", nodo_ingrediente->nome_ingrediente, attesa);
                        //     avanzo all'ingrediente successivo
                        nodo_ingrediente = nodo_ingrediente->successore;
                    }
                    // SE SI PRODUCO L'ORDINE E METTO IN LISTA DI ORDINI PRONTI
                    if (attesa == 0)
                    {
                        ordine = crea_ordine(nome_ricetta, qta, clock, peso);
                        ordini_pronti = inserisci_nodo_in_testa_ordini(ordini_pronti, ordine);

                        // produco ordine
                        produci_ordine(magazzino, bucket_ricetta, qta);

                        // ordino lotti in senso crescente per tempo di acquisizione (tempo)
                        merge_sort_ordini(&ordini_pronti);
                    }
                    else
                    { // SE NO MARCO ORDINE COME IN ATTESA E CONTINUO
                        ordine = crea_ordine(nome_ricetta, qta, clock, peso);
                        ordini_attesa = inserisci_nodo_in_testa_ordini(ordini_attesa, ordine);

                        // ordino lotti in senso crescente per tempo di acquisizione (tempo)
                        merge_sort_ordini(&ordini_attesa);
                    }
                }
            }
            // printf("ordini_pronti:\n");
            // stampa_lista_ordini(ordini_pronti);

            // printf("ordini_attesa:\n");
            // stampa_lista_ordini(ordini_attesa);
        }

        // RIFORNIMENTO
        else if (strcmp(comando, "rifornimento") == 0)
        {
            controllo = scanf("%c", &separatore);

            // PROCESSO GLI INGREDIENTI RIFORNITI UNO AD UNO
            while (separatore != '\n')
            {
                controllo = scanf("%s", nome_ingrediente);
                controllo = scanf("%d", &qta);
                controllo = scanf("%d", &scadenza);
                controllo = scanf("%c", &separatore);
                // printf("Rifornimento:%s,qta:%d,scadenza:%d\n", nome_ingrediente, qta, scadenza);

                // AGGIUNGO NEL MAGAZZINO
                ingrediente = crea_nodo(nome_ingrediente, qta, scadenza);
                ht_inserisci_lotto(magazzino, ingrediente, nome_ingrediente);

                // ripristino ordine crescente per data di scadenza in lista di lotti
                merge_sort(&(magazzino->buckets[hash(nome_ingrediente)]->lista));
            }

            // HO AGGIORNATO IL MAGAZZINO
            printf("rifornito\n");

            // VERIFICO SE HO ORDINI IN ATTESA CHE POSSO PROCESSARE
            Ordine *temp = ordini_attesa;
            Ordine *prec_temp;
            Bucket *bucket_ricetta;
            Nodo *nodo_ingrediente;

            while (temp != NULL) // scorro tutta la lista di ordini in attesa
            {
                attesa = 0;

                // PRELEVO LA RICETTA DA RICETTARIO
                bucket_ricetta = ht_cerca(ricettario, temp->nome_ricetta);
                if (bucket_ricetta == NULL || bucket_ricetta->lista == NULL) // in realta so gia che ce lho di sicuro....
                {
                    //attesa = 1; // non ho la ricetta -> metto in attesa
                    // passo al prossimo ordine in attesa
                    temp = temp->successore;
                }
                else
                { // HO LA RICETTA -> VERIFICO DI POTERLA PRODURRE: HO INGREDIENTI NON SCADUTI A SUFFICIENZA
                    // VERIFICO PER OGNI INGREDIENTE:
                    nodo_ingrediente = bucket_ricetta->lista;
                    while (nodo_ingrediente != NULL && attesa != 1) //scorro la lista di ingredienti della ricetta
                    {
                        // CONTROLLO MAGAZZINO
                        bucket = ht_cerca(magazzino, nodo_ingrediente->nome_ingrediente);

                        if (bucket == NULL || bucket->lista == NULL) 
                        {
                            //non ho nessun ingrediente del tipo desiderato
                            attesa = 1;
                        }
                        else
                        {
                            //ci sono dei lotti dell'ingrediente desiderato verifico di averne abbastanza non scaduto!
                            if (verifica_ingrediente(magazzino, &bucket, nodo_ingrediente->nome_ingrediente, (nodo_ingrediente->qta)*(temp->qta), clock) == 0)
                            {
                                // ce un ingrediente che manca
                                attesa = 1;
                            }
                        }
                        // printf("%s, attesa=%d\n", nodo_ingrediente->nome_ingrediente, attesa);
                        //     avanzo all'ingrediente successivo
                        nodo_ingrediente = nodo_ingrediente->successore;
                    }

                    // SE SI PRODUCO L'ORDINE E METTO IN LISTA DI ORDINI PRONTI
                    if (attesa == 0)
                    {
                        ordine = crea_ordine(temp->nome_ricetta, temp->qta, temp->tempo, temp->peso);
                        ordini_pronti = inserisci_nodo_in_testa_ordini(ordini_pronti, ordine);

                        // produco ordine
                        produci_ordine(magazzino, bucket_ricetta, ordine->qta);

                        // ordino lotti in senso crescente per tempo di acquisizione (tempo)
                        merge_sort_ordini(&ordini_pronti);

                        prec_temp = temp;
                        // passo al prossimo ordine in attesa
                        temp = temp->successore;

                        // elimino l'ordine dalla lista di attesa e non devo riordinare nulla perche la proprieta si preserva
                        ordini_attesa = elimina_nodo_ptr_ordini(ordini_attesa, prec_temp);
                    }
                    else
                    {
                        // passo al prossimo ordine in attesa
                        temp = temp->successore;
                    }
                }
            }
            // printf("ordini_pronti:\n");
            // stampa_lista_ordini(ordini_pronti);

            // printf("ordini_attesa:\n");
            // stampa_lista_ordini(ordini_attesa);
        }

        // AGGIUSTAMENTI
        comando[0] = 0;
        clock++;
    }
    return 0;
}

// ------------------------------------- HASH TABLES ------------------------------------//
// inizializza HashTable
void inizializza_ht(HashTable *ht)
{
    ht->dimensione = 1e9 + 9;
    ht->num_buckets_inseriti = 0;
    ht->buckets = (Bucket **)calloc(ht->dimensione, sizeof(struct Bucket *));
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
    long long m = 1e9 + 9;
    unsigned long long hash = 0;
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

// ------------------------------- RICETTARIO -----------------------------------------//

void ht_inserisci_ricetta(HashTable *ht, Bucket *nuovo_bucket, char *string)
{
    // calcolo hash
    int indice = hash(string);

    Bucket *temp = ht_cerca(ht, string);

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
        printf("non presente\n");
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
            printf("rimossa\n");
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
                printf("rimossa\n");
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

//------------------------LISTE -----------------------------------------------------//

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

Nodo *elimina_nodo_ptr(Nodo *testa, Nodo *nodo)
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
        printf("%s, %d ,%d -> ", temp->nome_ingrediente, temp->qta, temp->scadenza);
        temp = temp->successore;
    }
    printf("\n");
    return;
}

void sottoliste(Nodo *testa, Nodo **inizio, Nodo **fine)
{
    Nodo *lepre;
    Nodo *tartaruga;

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

Nodo *merge_crescente(Nodo *a, Nodo *b)
{
    Nodo *testa = NULL;

    // casi base
    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    if (a->scadenza <= b->scadenza)
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

void merge_sort(Nodo **testa_indirizzo)
{
    Nodo *testa = *testa_indirizzo;
    Nodo *a;
    Nodo *b;

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

//--------------------------------- ORDINI ------------------------------------------//

// se chiamata gia verificato che ogni ingrediente e presente per poter produrre ordine
void produci_ordine(HashTable *magazzino, Bucket *bucket_ricetta, int qta)
{
    Nodo *nodo_ingrediente = bucket_ricetta->lista;
    Nodo *lotto;
    Nodo *lotto_prec;
    Bucket *bucket_magazzino;
    int qta_necessaria;

    // NOTA:
    // so gia che la ricetta non e vuota e che ci sono tutti gli ingredienti necessari e che ho gia tolto gli ingredienti scadutin o gia usati

    // scorro la lista di ingredienti e per ciascuno modifico le quantita/ rimuovo i lotti nel magazino
    while (nodo_ingrediente != NULL)
    {
        bucket_magazzino = ht_cerca(magazzino, nodo_ingrediente->nome_ingrediente);
        lotto = bucket_magazzino->lista;

        qta_necessaria = qta * nodo_ingrediente->qta;

        // per ogni ingrediente scorro la lista dei lotti
        while (lotto != NULL && qta_necessaria > 0)
        {
            if (lotto->qta > qta_necessaria)
            {
                lotto->qta -= qta_necessaria;
                qta_necessaria = 0; // per questo ingrediente ho tolto la qta utilizzata e sono aposto
                // continuo a scorrere la lista di lotti
                lotto = lotto->successore;
            }
            else
            { // il primo lotto non basta ma so che ne ho a sufficienza nei succesivi lotti.
                // qta necessaria diminuisce
                qta_necessaria -= lotto->qta;

                // uso tutta la qta del lotto quindi lo elimino
                // per farlo prima avanzo e poi faccio la free del vecchio lotto
                lotto_prec = lotto;
                lotto = lotto->successore;
                // ora posso eliminare lotto_prec senza fare casini
                elimina_lotto(&bucket_magazzino, lotto_prec);
            }

            // ho gia fatto avanzamento al lotto successivo
        }

        // continuo a scorre la lista di ingredienti e ripeto lo stesso procedimento per ogni ingrediente
        nodo_ingrediente = nodo_ingrediente->successore;
    }
    return;
}

// ritorna 0 se ingrediente mancante, 1 se ingrediente presente
int verifica_ingrediente(HashTable *magazzino, Bucket **bucket, char *nome_ingrediente, int qta_necessaria, int clock)
{
    Bucket *bucket_magazzino = *bucket;
    Nodo *lotto = bucket_magazzino->lista;
    Nodo *lotto_prec;

    if (bucket_magazzino == NULL || bucket_magazzino->lista == NULL)
    {
        return 0; // ingrediente mancante
    }
    else
    { // verifico la lista di lotti

        while (lotto != NULL)
        {
            if (lotto->scadenza <= clock || lotto->qta <= 0)
            {
                // devo fare la free di lotto mi preparo una copia, avanzo e poi faccio la free
                lotto_prec = lotto;
                lotto = lotto->successore;
                // ingrediente scaduto o gia utilizzato --> lo elimino dalla lista dei lotti
                elimina_lotto(bucket, lotto_prec);
            }
            else
            { // non e scaduto ->verifico che ce ne sia abbastanza
                if (lotto->qta >= qta_necessaria)
                {
                    return 1;
                }
                else
                {
                    qta_necessaria -= lotto->qta;
                }
                lotto = lotto->successore;
            }
        }
    }
    return 0;
}

void elimina_lotto(Bucket **bucket, Nodo *lotto)
{
    // faccio la free della lista di lotti
    Nodo *temp = (*bucket)->lista;
    (*bucket)->lista = elimina_nodo_ptr(temp, lotto);
    return;

    // // ora devo eliminare il bucket dalla eventuale lista di bucket
    // int indice = hash((*bucket)->string);

    // Bucket *bucket_temp = ht->buckets[indice];
    // Bucket *bucket_prec = ht->buckets[indice];

    // // verifico se e' il primo
    // if (strcmp(bucket_temp->string, (*bucket)->string) == 0)
    // {
    //     // modifico testa della lista
    //     ht->buckets[indice] = bucket_temp->successore;
    //     // elimino temp
    //     free(bucket_temp);
    // }
    // else
    // {
    //     //  altrimenti cerco il bucket e lo elimino
    //     while (strcmp(bucket_temp->string, (*bucket)->string) != 0 && bucket_temp->successore != NULL)
    //     {
    //         bucket_prec = bucket_temp;
    //         bucket_temp = bucket_temp->successore;
    //     }
    //     if (strcmp(bucket_temp->string, (*bucket)->string) == 0)
    //     {
    //         bucket_prec->successore = bucket_temp->successore;
    //         // elimino temp
    //         free(bucket_temp);
    //     }
    // }
}

void stampa_lista_ordini(Ordine *testa)
{
    Ordine *temp = testa;
    while (temp != NULL)
    {
        printf("%s, %d ,%d -> ", temp->nome_ricetta, temp->qta, temp->tempo);
        temp = temp->successore;
    }
    printf("\n");
    return;
}

Ordine *elimina_lista_ordini(Ordine *testa)
{
    Ordine *temp = testa;
    Ordine *prec = testa;
    while (temp != NULL)
    {
        prec = temp;
        temp = temp->successore;
        free(prec);
    }
    return NULL;
}

Ordine *elimina_nodo_ptr_ordini(Ordine *testa, Ordine *nodo)
{
    Ordine *temp = testa;
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

int cerca_in_lista(Ordine *testa, char *nome_ricetta)
{
    Ordine *temp = testa;
    while (temp != NULL)
    {
        if (strcmp(temp->nome_ricetta, nome_ricetta) == 0)
            return 1; // trovata

        temp = temp->successore;
    }
    return 0; // non trovata
}

// ordinamento liste di ordini
void sottoliste_ordini(Ordine *testa, Ordine **inizio, Ordine **fine)
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

Ordine *merge_crescente_ordini(Ordine *a, Ordine *b)
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
        testa->successore = merge_crescente_ordini(a->successore, b);
    }
    else
    {
        testa = b;
        testa->successore = merge_crescente_ordini(a, b->successore);
    }
    return testa;
}

Ordine *merge_decrescente_corriere(Ordine *a, Ordine *b)
{
    Ordine *testa = NULL;

    // casi base
    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    if (a->peso > b->peso)
    {
        testa = a;
        testa->successore = merge_decrescente_corriere(a->successore, b);
    }
    else if (a->peso == b->peso)
    {
        if (a->tempo < b->tempo)
        {
            testa = a;
            testa->successore = merge_decrescente_corriere(a->successore, b);
        }
        else
        {
            testa = b;
            testa->successore = merge_decrescente_corriere(a, b->successore);
        }
    }
    else
    {
        testa = b;
        testa->successore = merge_decrescente_corriere(a, b->successore);
    }
    return testa;
}

void merge_sort_ordini(Ordine **testa_indirizzo)
{
    Ordine *testa = *testa_indirizzo;
    Ordine *a;
    Ordine *b;

    // caso base
    if (testa == NULL || testa->successore == NULL)
        return;

    // divido la lista testa in due sottoliste a e b
    sottoliste_ordini(testa, &a, &b);
    // ordino le due sottoliste
    merge_sort_ordini(&a);
    merge_sort_ordini(&b);

    // unisco le due sottoliste
    *testa_indirizzo = merge_crescente_ordini(a, b);
    return;
}

void merge_sort_corriere(Ordine **testa_indirizzo)
{
    Ordine *testa = *testa_indirizzo;
    Ordine *a;
    Ordine *b;

    // caso base
    if (testa == NULL || testa->successore == NULL)
        return;

    // divido la lista testa in due sottoliste a e b
    sottoliste_ordini(testa, &a, &b);
    // ordino le due sottoliste
    merge_sort_corriere(&a);
    merge_sort_corriere(&b);

    // unisco le due sottoliste
    *testa_indirizzo = merge_decrescente_corriere(a, b);
    return;
}
