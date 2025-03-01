#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define CMD_LEN 256
//----------------------------------------------------------------------------------------------------------------------
//------------------------------------------   DESCRIZIONE    ----------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
// DESCRIZIONE PROBLEMA
// Vedi specifica.pdf

// SCELTE IMPLEMENTATIVE:

// 1) SCELTA STRUTTURA DATI
// Dopo vari test e implementazioni con diverse strutture dati quali alberi BST, alberi R-B e liste sono giunto alla conclusione che per il problema dato
// la struttura dati piu' consona fossero le Hash-Tables. Infatti il problema piu grosso che si e' presentato durante lo svolgimento del progetto e' stato
// quello di trovare una tecnica di ricerca che fosse veloce ed efficiente, questo perche' ad ogni ricetta da produrre e necessario sapere gli ingredienti
// di cui e composta (ricerca nel ricettario), della disponibilita' di questi (ricerca nel magazzino e confronto con qta disponibili). Analoghi problemi si
// sono poi presentati nella produzione degli ordini infatti anche in questo caso dato un ordine sono necessarie tutto un insieme di informazioni solamente
// per stabilire se puo' essere prodotto e sono poi necessarie ulteriori azioni per produrlo.
// Per questi motivi le hash-tables si sono rivelate perfette: ottima velocita' di ricerca e di conseguenza di inserimento e rimozione. Inloltre mi hanno
// permesso di gestire le stringhe (altro punto importante del problema) in modo efficiente in quanto strutture dati come alberi R-B avrebbero avuto bisogno
// di una regola di ordinamento tra di esse cosa che avrebbe sicuramente aumentato il numero di strcmp necessarie all'inserimento, ricerca e rimozione.
// Ho inoltre scelto di utilizzare hash-tables con politica di gestione delle collisioni con liste single-linked. La scelta e' stata fatta sulla base di
// molteplici criteri: - volevo evitare di dover, in caso di numerosi inserimenti, gestire realloc e re-hashing delle strutture. - Mi ha permesso di riutilizzare
// varie funzioni in altre parti del codice, sapevo infatti che da qualche parte avrei avuto bisogno delle classiche, semplici liste. - la scelta particolare di
// usare liste single - linked e' dovuta ad una pura scelta di ottimizzazione (duoble linked sarebbero state piu semplici da gestire).

// Ho infine scelto di utilizzare delle semplici code di liste single-linked per la gestione di Ordini e Corriere. Anche in questo caso si tratta di una scelta
// fatta per semplicita di gestione e implementazione ed in modo particolare perche' implementando le code (liste con puntatore alla coda) ho potuto rendere l'inserimento
// ordinato molto rapido in quanto la necessita' era di inserire gli ordini nelle liste di attesa e ordini pronti in modo ordinato con criterio di ordine crescente
// per data (tempo della simulazione) di acquisizione. Grazie alle code questa operazione avviene in O(1).
// La gestione del corriere fa una piccola eccezione a questo ragionamento inquanto ho deciso per ottimizare utilizzo della memoria di implementare delle semplici liste
// poiche' in questo caso la metrica d'oridne non permetteva un inserimento ordinato efficiente (ordine decrescente di peso). Per questo ho semplicemente implementato
// inserimento in testa e un ordinamento tramite merge-sort crescente che viene fatto una sola volta al momento della stampa

// 2)GESTIONE STRUTTURE DATI
//  Inizialmente avevo scelto di implementare 2 hash tables: il ricettario e il magazzino. Questo mi permetteva di raggiungere tutte le funzionalita' richieste
//  con un livello di efficienza buono ma non sufficiente. Infatti il problema di memorizzare (e quindi di confrontare ad ogni ricerca) il nome di ingredienti e
//  il nome delle ricette in ogni nodo del ricettario e del magazzino creava due grossi problemi: -molta memoria utilizzata (ogni stringa puo essere fino a 255 caratt.)
//  -molto tempo sprecato nel copiare le stringhe ad ogni creazione di nuovi nodi. -molti duplicati memorizzati in entrambi le strutture: ingredienti con lo stesso
//  nome ma di ricette diverse venivano memorizzati 2 volte.
//  Ho per questo deciso di introdurre un'altra struttura, una hash-tables per memorizzare (senza duplicati) i nomi degli ingredienti.
//  questa scelta ha 2 grossi vantaggi:
//  - Niente duplicati di stringhe memorizzati piu' volte (vengono memorizzati una volta sola e in ricettario e magazzino sono presenti soltanto puntatori alla h.t. degli ingredienti)
//  - Avendo una struttura specifica per i soli ingredienti ho potuto con un solo puntatore in piu' legare ad ogni ingrediente l'indirizzo nel magazzino dei suoi lotti
//  questo ha ridotto di molto il numero di ricerce in magazzino avvelocizzando la produzione degli ordini.

// Per quanto riguarda la gestione del corriere e degli ordini come detto sopra ho preferito la semplicita delle liste in particolare ho implementato delle code
// in modo da poter gestire l'inserimento in ordine (in fondo = in ordine di tempo) in modo semplice e veloce senza dover ri-ordinare le varie liste ad ogni inserimento.

// EFFICIENZA E VALUTAZIONE
// Il problema imponeva le seguenti caratteristiche di efficienza:
//  TEST (voto)  --  TEMPO (s) -- MEMORIA (MiB)
//  18           --   14.00    --    35
//  21           --   11.50    --    30
//  24           --    9.00    --    25
//  27           --    6.50    --    20
//  30           --    4.00    --    15
//  30L          --    1.500   --    14

// Risultato ottenuto: Voto=27 , Tempo = 6.26(s), Memoria_utilizzata = 16.8 (MiB)

// ULTERIORI MIGLIORAMENTI
// - Funzione per sfilare un elemento da una coda. Questa permetterebbe migliore velocita nel passaggio di un ordine da lista di attesa alla
// lista di ordini pronti in quanto eliminerebbe la necessita di creare un nuovo ordine (avrebbe la stessa complessita temporale della sola eliminazione)

// - una ulteriore hash-table per la gestione del nome delle ricette potrebbe migliorare utilizzo della memoria dinamica in quanto ordini della stessa ricetta
// verrebbero creati con il solo puntatore ad un unica cella della hash-table senza duplicazione di stirnghe identiche.

// - Implementazione di una politica di rimozione degli ingredienti dalla hash-table ingredienti se non presenti in nessuna ricetta (migliore utilizzo della memoria)
// e miglioramenti in termini di tempo (hash-table meno affollata)

//----------------------------------------------------------------------------------------------------------------------
//------------------------------------------   STRUTTURE DATI ----------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

typedef struct Lotto
{
    int qta;
    int scadenza;
    struct Lotto *successore;
} Lotto;

typedef struct BucketMagazzino
{
    char nome_ingrediente[CMD_LEN];
    struct Lotto *lista;
    struct BucketMagazzino *successore;
} BucketMagazzino;

typedef struct BucketIngredienti
{
    char nome_ingrediente[CMD_LEN];
    struct BucketMagazzino *bucket_magazzino;
    struct BucketIngredienti *successore;
} BucketIngredienti;

typedef struct Ingrediente
{
    struct BucketIngredienti *bucket_ingrediente;
    int qta;
    struct Ingrediente *successore;
} Ingrediente;

typedef struct BucketRicettario
{
    char nome_ricetta[CMD_LEN];
    struct Ingrediente *lista;
    struct BucketRicettario *successore;
} BucketRicettario;

typedef struct Magazzino
{
    int dimensione;
    BucketMagazzino **buckets;
} Magazzino;

typedef struct Ricettario
{
    int dimensione;
    BucketRicettario **buckets;
} Ricettario;

typedef struct Ht_Ingredienti
{
    int dimensione;
    BucketIngredienti **buckets;
} Ht_Ingredienti;

typedef struct Ordine
{
    int qta;
    int tempo;
    int peso;
    struct BucketRicettario *bucket_ricetta;
    struct Ordine *successore;
} Ordine;

typedef struct Coda
{
    Ordine *testa;
    Ordine *coda;
} Coda;

//----------------------------------------------------------------------------------------------------------------------
//---------------------------------   FUNZIONI STRUTTURE DATI ----------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

// Liste  semplici (single linked) X RICETTARIO E MAGAZZINO

void stampa_lista_ingredienti(Ingrediente *testa);
void stampa_lista_lotti(Lotto *testa);
void *elimina_lista_ingredienti(Ingrediente *testa);
Lotto *elimina_lotto_ptr(Lotto *testa, Lotto *nodo);
Ingrediente *inserisci_ingrediente(Ingrediente *testa, Ingrediente *nodo);
Lotto *inserisci_lotto(Lotto *testa, Lotto *nodo);
Ingrediente *crea_nodo_ricettario(int qta, BucketIngredienti *bucket_ingredienti);
Lotto *crea_nodo_magazzino(int qta, int scadenza);

// ---------------------------  ORDINI ---------------------------------------------//

void stampa_lista_ordini(Ordine *testa);
Ordine *elimina_lista_ordini(Ordine *testa);
Ordine *elimina_nodo_ptr_ordini(Ordine *testa, Ordine *nodo);
Ordine *inserisci_nodo_in_testa_ordini(Ordine *testa, Ordine *nodo);
Ordine *crea_ordine(char *nome_ricetta, BucketRicettario *bucket_ricetta, int qta, int tempo, int peso);

// verifica se un ingrediente e presente in quantita sufficiente (non scaduto) per la ricetta desiderata
int verifica_ingrediente(BucketMagazzino **bucket, int qta_necessaria, int clock);

// modifica le quantita nel ricettario sottraendo quelle utilizzate per produrre la ricetta
void produci_ordine(Magazzino *magazzino, BucketRicettario *bucket_ricetta, int qta);

// ritorna 1 se la ricetta e presente nella lista oppure 0 se non e presente
int cerca_in_lista(Ordine *testa, char *nome_ricetta);

// CODE x ordini
void inizializza_coda(Coda **coda);
Coda *elimina_ordine_ptr_coda(Coda *coda, Ordine *nodo);
Coda *sfila_ordine_ptr_coda(Coda *coda, Ordine *nodo);
Coda *inserisci_in_coda(Coda *coda, Ordine *ordine);
Coda *inserisci_inordine_ordini(Coda *coda, Ordine *ordine);

// --------------------------- FUNZIONI DI ORDINAMENTO LISTE ----------------------------//

void sottoliste_ordini(Ordine *testa, Ordine **inizio, Ordine **fine);
Ordine *merge_decrescente_corriere(Ordine *a, Ordine *b);
void merge_sort_corriere(Ordine **testa_indirizzo);

// ---------------------------  HASH TABLE ---------------------------------------------//

void inizializza_ricettario(Ricettario *ricettario);
void inizializza_magazzino(Magazzino *magazzino);
void inizializza_ht_ingredienti(Ht_Ingredienti *ht_ingredienti);

BucketMagazzino *crea_bucket_magazzino(char *nome_ingrediente, Lotto *lista);
BucketRicettario *crea_bucket_ricettario(char *nome_ricetta, Ingrediente *lista);
BucketIngredienti *crea_bucket_ht_ingredienti(char *nome_ingrediente, BucketMagazzino *bucket_magazzino);

int hash(char *string);
BucketMagazzino *cerca_magazzino(Magazzino *ht, char *string);
BucketRicettario *cerca_ricettario(Ricettario *ht, char *string);
BucketIngredienti *cerca_ht_ingredienti(Ht_Ingredienti *ht_ingredienti, char *nome_ingrediente);

void inserisci_bucket_ht_ingredienti(Ht_Ingredienti *ht_ingredienti, BucketIngredienti *nuovo_bucket);
void inserisci_ricetta(Ricettario *ht, BucketRicettario *nuovo_bucket, char *string);
void elimina_ricetta(Ricettario *ht, char *string);
void elimina_lotto(BucketMagazzino **bucket, Lotto *lotto);
void inserisci_bucket_magazzino(Magazzino *ht, Lotto *lotto, char *nome_ricetta);

//----------------------------------------------------------------------------------------------------------------------
//------------------------------------------   MAIN     ----------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

int main()
{

    char comando[CMD_LEN];
    int clock = 0; // istanti di tempo della simulazione

    Ricettario *ricettario = (Ricettario *)malloc(sizeof(Ricettario));
    Magazzino *magazzino = (Magazzino *)malloc(sizeof(Magazzino));
    Ht_Ingredienti *ht_ingredienti = (Ht_Ingredienti *)malloc(sizeof(Ht_Ingredienti));

    Ingrediente *ingrediente;

    inizializza_ricettario(ricettario);
    inizializza_magazzino(magazzino);
    inizializza_ht_ingredienti(ht_ingredienti);

    Coda *ordini_pronti = (Coda *)malloc(sizeof(Coda));
    Coda *ordini_attesa = (Coda *)malloc(sizeof(Coda));
    Ordine *ordini_corriere = NULL;

    inizializza_coda(&ordini_pronti);
    inizializza_coda(&ordini_attesa);

    Ordine *ordine = NULL;
    Ordine *ordine_prec;
    Ordine *ordine_corriere = NULL;

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

    int cont = 0;

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
            ordine = ordini_pronti->testa;
            stop = 0;
            while (ordine != NULL && stop == 0)
            {
                if (ordine->peso <= capienza_rimasta)
                {
                    // creo il nuovo nodo ordine per la lista del corriere
                    ordine_corriere = crea_ordine(ordine->bucket_ricetta->nome_ricetta, ordine->bucket_ricetta, ordine->qta, ordine->tempo, ordine->peso);
                    // inserisco nella lista delgi ordini del corriere
                    ordini_corriere = inserisci_nodo_in_testa_ordini(ordini_corriere, ordine_corriere);

                    // aggiorno campienza rimasta
                    capienza_rimasta -= ordine->peso;

                    ordine_prec = ordine;
                    ordine = ordine->successore;
                    ordini_pronti = elimina_ordine_ptr_coda(ordini_pronti, ordine_prec);
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
                merge_sort_corriere(&(ordini_corriere));

                // STAMPO LA LISTA DEL CORRIERE
                ordine = ordini_corriere;
                while (ordine != NULL)
                {
                    printf("%d %s %d\n", ordine->tempo, ordine->bucket_ricetta->nome_ricetta, ordine->qta);
                    ordine = ordine->successore;
                }
            }
            // elimino la lista del corriere
            ordini_corriere = elimina_lista_ordini(ordini_corriere);
        }

        //----------------------------- ACQUISISCO COMANDO ----------------------------------------------------------------------//

        controllo = scanf("%s", comando);

        //----------------------------- GESTIONE COMANDI ------------------------------------------------------------------------//

        // AGGIUNGI_RICETTA
        if (strcmp(comando, "aggiungi_ricetta") == 0)
        {
            controllo = scanf("%s", nome_ricetta);
            controllo = scanf("%c", &separatore);

            // printf("Nome ricetta:%s\n", nome_ricetta);
            BucketRicettario *ricetta = cerca_ricettario(ricettario, nome_ricetta);

            if (ricetta != NULL)
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
                ricetta = crea_bucket_ricettario(nome_ricetta, NULL);
                BucketIngredienti *bucket_ingrediente;

                // ACQUISISCO INGREDIENTI E CREO LISTA INGREDIENTI
                while (separatore != '\n')
                {
                    controllo = scanf("%s", nome_ingrediente);
                    controllo = scanf("%d", &qta);
                    controllo = scanf("%c", &separatore);
                    // printf("Ingrediente:%s,qta:%d\n", nome_ingrediente, qta);

                    // AGGIUNGO INGREDIENTE IN HASH TABLE INGREDIENTI
                    bucket_ingrediente = cerca_ht_ingredienti(ht_ingredienti, nome_ingrediente);
                    if (bucket_ingrediente == NULL)
                    {
                        // creo bucket e inserisco in ht ingredienti
                        bucket_ingrediente = crea_bucket_ht_ingredienti(nome_ingrediente, cerca_magazzino(magazzino, nome_ingrediente));
                        inserisci_bucket_ht_ingredienti(ht_ingredienti, bucket_ingrediente);
                    }

                    // AGGIUNGO INGREDIENTE ALLA NODO RICETTA
                    ingrediente = crea_nodo_ricettario(qta, bucket_ingrediente);
                    ricetta->lista = inserisci_ingrediente(ricetta->lista, ingrediente);
                }

                // AGGIUNGO IL NODO RICETTA AL RICETTARIO
                inserisci_ricetta(ricettario, ricetta, nome_ricetta);
                // stampa_lista_ingredienti(ricetta->lista);
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
            //   VERIFICO CHE NON SIA IN USO = ordini_attesa e CHE NON E' LA RICETTA DI UN ORDINE CHE NON HO ANCORA SPEDITO = oridini_pronti
            if (cerca_in_lista(ordini_attesa->testa, nome_ricetta) == 1 || cerca_in_lista(ordini_pronti->testa, nome_ricetta) == 1)
            {
                printf("ordini in sospeso\n");
            }
            else
            {
                // ALTRIMENTI RIMUOVO RICETTA DA RICETTARIO
                elimina_ricetta(ricettario, nome_ricetta);
            }
        }

        // ORDINE
        else if (strcmp(comando, "ordine") == 0)
        {
            BucketRicettario *ricetta;
            BucketMagazzino *bucket;

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
                ricetta = cerca_ricettario(ricettario, nome_ricetta);
                if (ricetta == NULL || ricetta->lista == NULL)
                {
                    // non ho la ricetta oppure ho una ricetta vuota -> rifiuto ordine
                    printf("rifiutato\n");
                }
                else
                { // HO LA RICETTA -> VERIFICO DI POTERLA PRODURRE: HO INGREDIENTI NON SCADUTI A SUFFICIENZA
                    printf("accettato\n");
                    // VERIFICO PER OGNI INGREDIENTE:
                    ingrediente = ricetta->lista;

                    cont = 0;
                    while (ingrediente != NULL) // scorro tutta la ricetta indipendentemente da stato di attesa in modo da calcolare perlomeno il peso dell'ordine.
                    {
                        // calcolo peso dell'ordine
                        peso += (ingrediente->qta) * qta;

                        // CONTROLLO MAGAZZINO
                        // cerco ingrediente in magazzino e salvo puntatore in array statico per avveloccizare produci ordine
                        bucket = ingrediente->bucket_ingrediente->bucket_magazzino;
                        if (bucket == NULL)
                        {
                            // potrebbe essere NULL perche ho inserito ricetta prima che venisse registrato ingrediente nel magazzino
                            // lo aggiorno e poi se rimane NUll significa che l'ingrediente non ce per davvero
                            ingrediente->bucket_ingrediente->bucket_magazzino = cerca_magazzino(magazzino, ingrediente->bucket_ingrediente->nome_ingrediente);
                        }

                        if (bucket == NULL || bucket->lista == NULL)
                        {
                            // non ci sono ingredinti in magazzino
                            attesa = 1;
                        }
                        else
                        { // ho una lista di lotti da controllare: voglio verificare di avere ingredienti non scaduti a sufficienza
                            if (attesa == 0)
                            { // se so gia che va in attesa e inutile verificare altri ingredienti
                                if (verifica_ingrediente(&bucket, ingrediente->qta * qta, clock) == 0)
                                {
                                    // ce un ingrediente che manca
                                    attesa = 1;
                                }
                            }
                        }
                        // printf("%s, attesa=%d\n", ingrediente->bucket_ingrediente->nome_ingrediente, attesa);
                        // avanzo all'ingrediente successivo
                        ingrediente = ingrediente->successore;
                        cont++;
                    }
                    // SE SI PRODUCO L'ORDINE E METTO IN LISTA DI ORDINI PRONTI
                    if (attesa == 0)
                    {
                        ordine = crea_ordine(nome_ricetta, ricetta, qta, clock, peso);
                        ordini_pronti = inserisci_inordine_ordini(ordini_pronti, ordine);

                        // produco ordine
                        produci_ordine(magazzino, ricetta, qta);
                    }
                    else
                    { // SE NO MARCO ORDINE COME IN ATTESA E CONTINUO
                        ordine = crea_ordine(nome_ricetta, ricetta, qta, clock, peso);
                        ordini_attesa = inserisci_inordine_ordini(ordini_attesa, ordine);
                    }
                }
            }
            // printf("ordini_pronti:\n");
            // stampa_lista_ordini(ordini_pronti->testa);

            // printf("ordini_attesa:\n");
            // stampa_lista_ordini(ordini_attesa->testa);
        }

        // RIFORNIMENTO
        else if (strcmp(comando, "rifornimento") == 0)
        {
            Lotto *lotto;
            BucketIngredienti *bucket_ingrediente;
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
                // creo il lotto
                lotto = crea_nodo_magazzino(qta, scadenza);
                // inserisco lotto nel magazzino (inserimento e in ordine di data di scadenza crescente)
                inserisci_bucket_magazzino(magazzino, lotto, nome_ingrediente);

                // AGGIORNO RIFERIMENTI A MAGAZZINO IN HT_INGREDIENTI
                bucket_ingrediente = cerca_ht_ingredienti(ht_ingredienti, nome_ingrediente);
                if (bucket_ingrediente == NULL)
                {
                    // creo bucket e inserisco in ht ingredienti
                    bucket_ingrediente = crea_bucket_ht_ingredienti(nome_ingrediente, cerca_magazzino(magazzino, nome_ingrediente));
                    inserisci_bucket_ht_ingredienti(ht_ingredienti, bucket_ingrediente);
                }
                else if (bucket_ingrediente->bucket_magazzino == NULL)
                {
                    // se ce gia ma puntatore a magazzino e NULL provo a cercare nel magazzino se ora ce ingrediente (potrei averlo inserito)
                    bucket_ingrediente->bucket_magazzino = cerca_magazzino(magazzino, nome_ingrediente);
                }
            }

            // HO AGGIORNATO IL MAGAZZINO
            printf("rifornito\n");

            // VERIFICO SE HO ORDINI IN ATTESA CHE POSSO PROCESSARE
            Ordine *ordine = ordini_attesa->testa;
            Ordine *prec_ordine = NULL;
            Ordine *nuovo_ordine;
            BucketRicettario *ricetta;
            BucketMagazzino *bucket;
            Ingrediente *ingrediente;

            while (ordine != NULL) // scorro tutta la lista di ordini in attesa
            {
                attesa = 0;

                // PRELEVO LA RICETTA DA RICETTARIO
                ricetta = ordine->bucket_ricetta;
                if (ricetta == NULL || ricetta->lista == NULL) // in realta so gia che ce lho di sicuro....
                {
                    // non dovrebbe mai succedere
                    printf("errore\n");
                    //  passo al prossimo ordine in attesa
                    prec_ordine = ordine;
                    ordine = ordine->successore;
                }
                else
                { // HO LA RICETTA -> VERIFICO DI POTERLA PRODURRE: HO INGREDIENTI NON SCADUTI A SUFFICIENZA
                    // VERIFICO PER OGNI INGREDIENTE:
                    ingrediente = ricetta->lista;
                    cont = 0;
                    while (ingrediente != NULL && attesa != 1) // scorro la lista di ingredienti della ricetta
                    {
                        // CONTROLLO MAGAZZINO
                        bucket = ingrediente->bucket_ingrediente->bucket_magazzino;
                        if (bucket == NULL)
                        {
                            // potrebbe essere NULL perche ho inserito ricetta prima che venisse registrato ingrediente nel magazzino
                            // lo aggiorno e poi se rimane NUll significa che l'ingrediente non ce per davvero
                            ingrediente->bucket_ingrediente->bucket_magazzino = cerca_magazzino(magazzino, ingrediente->bucket_ingrediente->nome_ingrediente);
                        }

                        if (bucket == NULL || bucket->lista == NULL)
                        {
                            // non ho nessun ingrediente del tipo desiderato
                            attesa = 1;
                        }
                        else
                        {
                            // ci sono dei lotti dell'ingrediente desiderato verifico di averne abbastanza non scaduto!
                            if (verifica_ingrediente(&bucket, (ingrediente->qta) * (ordine->qta), clock) == 0)
                            {
                                // ce un ingrediente che manca
                                attesa = 1;
                            }
                        }
                        // printf("%s, attesa=%d\n", ingrediente->bucket_ingrediente->nome_ingrediente, attesa);
                        //   avanzo all'ingrediente successivo
                        ingrediente = ingrediente->successore;
                        cont++;
                    }

                    // SE SI PRODUCO L'ORDINE E METTO IN LISTA DI ORDINI PRONTI
                    if (attesa == 0)
                    {
                        // creo ordine
                        nuovo_ordine = crea_ordine(ordine->bucket_ricetta->nome_ricetta, ricetta, ordine->qta, ordine->tempo, ordine->peso);

                        // PRODUCO ORDINE
                        produci_ordine(magazzino, ricetta, nuovo_ordine->qta);

                        // INSERISCO ORDINE IN ORDINI_PRONTI
                        ordini_pronti = inserisci_inordine_ordini(ordini_pronti, nuovo_ordine);

                        // ELIMINO ORDINE DA LISTA DI ATTESA
                        prec_ordine = ordine;
                        // passo al prossimo ordine in attesa
                        ordine = ordine->successore;

                        // elimino l'ordine dalla lista di attesa e non devo riordinare nulla perche la proprieta si preserva
                        ordini_attesa = elimina_ordine_ptr_coda(ordini_attesa, prec_ordine);
                    }
                    else
                    {
                        // passo al prossimo ordine in attesa
                        prec_ordine = ordine;
                        ordine = ordine->successore;
                    }
                }
            }
            // printf("ordini_pronti:\n");
            // stampa_lista_ordini(ordini_pronti->testa);

            // printf("ordini_attesa:\n");
            // stampa_lista_ordini(ordini_attesa->testa);
        }

        // AGGIUSTAMENTI
        comando[0] = 0;
        clock++;
        // printf("\n");
    }

    // faccio le free
    return 0;
}

// ------------------------------------- HASH TABLES ------------------------------------//
// inizializza Ricettario
void inizializza_ricettario(Ricettario *ricettario)
{
    ricettario->dimensione = 10007;
    ricettario->buckets = (BucketRicettario **)calloc(ricettario->dimensione, sizeof(struct BucketRicettario *));
}

// inizializza Magazzino
void inizializza_magazzino(Magazzino *magazzino)
{
    magazzino->dimensione = 10007;
    magazzino->buckets = (BucketMagazzino **)calloc(magazzino->dimensione, sizeof(struct BucketMagazzino *));
}

// inizializza Magazzino
void inizializza_ht_ingredienti(Ht_Ingredienti *ht_ingredienti)
{
    ht_ingredienti->dimensione = 10007;
    ht_ingredienti->buckets = (BucketIngredienti **)calloc(ht_ingredienti->dimensione, sizeof(struct BucketIngredienti *));
}

// crea Bucket
BucketMagazzino *crea_bucket_magazzino(char *nome_ingrediente, Lotto *lista)
{
    BucketMagazzino *nuovo_bucket = (BucketMagazzino *)malloc(sizeof(BucketMagazzino));
    nuovo_bucket->successore = NULL;
    strcpy(nuovo_bucket->nome_ingrediente, nome_ingrediente);
    nuovo_bucket->lista = lista;
    return nuovo_bucket;
}

// crea Bucket
BucketRicettario *crea_bucket_ricettario(char *nome_ricetta, Ingrediente *lista)
{
    BucketRicettario *nuovo_bucket = (BucketRicettario *)malloc(sizeof(BucketRicettario));
    nuovo_bucket->successore = NULL;
    strcpy(nuovo_bucket->nome_ricetta, nome_ricetta);
    nuovo_bucket->lista = lista;
    return nuovo_bucket;
}

// crea Bucket
BucketIngredienti *crea_bucket_ht_ingredienti(char *nome_ingrediente, BucketMagazzino *bucket_magazzino)
{
    BucketIngredienti *nuovo_bucket = (BucketIngredienti *)malloc(sizeof(BucketIngredienti));
    nuovo_bucket->successore = NULL;
    nuovo_bucket->bucket_magazzino = bucket_magazzino;
    strcpy(nuovo_bucket->nome_ingrediente, nome_ingrediente);
    return nuovo_bucket;
}

int hash(char *string)
{
    unsigned long hash = 5381;
    long long m = 10007;
    int c;
    while ((c = *string++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % m;
}

// Restituisce NULL se il Bucket non c'e, altrimenti resituisce il puntatore al Bucket cercato.
BucketMagazzino *cerca_magazzino(Magazzino *ht, char *string)
{
    // calcolo hash
    int indice = hash(string);

    if (ht->buckets[indice] == NULL)
        return NULL;
    else
    {
        BucketMagazzino *temp = ht->buckets[indice];

        while (temp != NULL)
        {
            if (strcmp(temp->nome_ingrediente, string) == 0)
                return temp;
            else
                temp = temp->successore;
        }
        return NULL;
    }
}

// Restituisce NULL se il Bucket non c'e, altrimenti resituisce il puntatore al Bucket cercato.
BucketRicettario *cerca_ricettario(Ricettario *ricettario, char *nome_ricetta)
{
    // calcolo hash
    int indice = hash(nome_ricetta);

    if (ricettario->buckets[indice] == NULL)
        return NULL;
    else
    {
        BucketRicettario *temp = ricettario->buckets[indice];

        while (temp != NULL)
        {
            if (strcmp(temp->nome_ricetta, nome_ricetta) == 0)
                return temp;
            else
                temp = temp->successore;
        }
        return NULL;
    }
}

// Restituisce NULL se il Bucket non c'e, altrimenti resituisce il puntatore al Bucket cercato.
BucketIngredienti *cerca_ht_ingredienti(Ht_Ingredienti *ht_ingredienti, char *nome_ingrediente)
{
    // calcolo hash
    int indice = hash(nome_ingrediente);

    if (ht_ingredienti->buckets[indice] == NULL)
        return NULL;
    else
    {
        BucketIngredienti *temp = ht_ingredienti->buckets[indice];

        while (temp != NULL)
        {
            if (strcmp(temp->nome_ingrediente, nome_ingrediente) == 0)
                return temp;
            else
                temp = temp->successore;
        }
        return NULL;
    }
}

void inserisci_bucket_ht_ingredienti(Ht_Ingredienti *ht_ingredienti, BucketIngredienti *nuovo_bucket)
{
    // calcolo hash
    int indice = hash(nuovo_bucket->nome_ingrediente);

    BucketIngredienti *ingrediente = cerca_ht_ingredienti(ht_ingredienti, nuovo_bucket->nome_ingrediente);

    if (ingrediente == NULL)
    {
        // non ce quindi inserisco in testa
        nuovo_bucket->successore = ht_ingredienti->buckets[indice];
        ht_ingredienti->buckets[indice] = nuovo_bucket;
    }
    else
    {
        // significa che ce gia quindi non faccio nulla.
    }
    return;
}

// ------------------------------- RICETTARIO -----------------------------------------//

void inserisci_ricetta(Ricettario *ricettario, BucketRicettario *nuovo_bucket, char *nome_ricetta)
{
    // calcolo hash
    int indice = hash(nome_ricetta);

    BucketRicettario *ricetta = cerca_ricettario(ricettario, nome_ricetta);

    if (ricetta == NULL)
    {
        // non ce quindi inserisco in testa
        // ci sono gia altri bucket, inserisco in testa
        nuovo_bucket->successore = ricettario->buckets[indice];
        ricettario->buckets[indice] = nuovo_bucket;

        printf("aggiunta\n");
    }
    else
    {
        // significa che ce gia quindi non faccio nulla.
        printf("ignorata\n");
    }
    return;
}

void elimina_ricetta(Ricettario *ricettario, char *nome_ricetta)
{
    int indice = hash(nome_ricetta);

    if (ricettario->buckets[indice] == NULL)
    {
        // non ce niente da eliminare
        printf("non presente\n");
        return;
    }
    else
    {
        BucketRicettario *temp = ricettario->buckets[indice];
        BucketRicettario *prec = ricettario->buckets[indice];

        // verifico se e' il primo
        if (strcmp(temp->nome_ricetta, nome_ricetta) == 0)
        {
            // elimino la lista di ingredienti di temp
            Ingrediente *nodo = temp->lista;
            Ingrediente *nodo_prec;
            while (nodo != NULL)
            {
                nodo_prec = nodo;
                nodo = nodo->successore;
                free(nodo_prec);
            }
            // modifico testa della lista
            ricettario->buckets[indice] = temp->successore;
            // elimino temp
            free(temp);
            printf("rimossa\n");
        }
        else
        {
            // altrimenti cerco il bucket e lo elimino
            while (strcmp(temp->nome_ricetta, nome_ricetta) != 0 && temp->successore != NULL)
            {
                prec = temp;
                temp = temp->successore;
            }
            if (strcmp(temp->nome_ricetta, nome_ricetta) == 0)
            {
                prec->successore = temp->successore;

                // elimino la lista di ingredienti di temp
                Ingrediente *nodo = temp->lista;
                Ingrediente *nodo_prec;
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
                printf("non presente\n");
            }
        }
    }
    return;
}

//-------------------------------------- MAGAZZINO -----------------------------------//

void inserisci_bucket_magazzino(Magazzino *ht, Lotto *lotto, char *nome_ricetta)
{
    // calcolo hash
    int indice = hash(nome_ricetta);

    // se la lista di bucket e vuota basta che inserisco in testa
    if (ht->buckets[indice] == NULL)
    {
        // inizializzo il bucket e lo inserisco
        BucketMagazzino *nuovo_bucket = crea_bucket_magazzino(nome_ricetta, lotto);
        ht->buckets[indice] = nuovo_bucket;
        return;
    }
    else
    {
        // altrimenti lo cerco e modifico soltanto la sua lista di lotti aggiungendo lotto in testa.
        BucketMagazzino *bucket = cerca_magazzino(ht, nome_ricetta);

        if (bucket == NULL)
        {
            // non ce tra la la lista dei buckets -> lo inserisco in testa
            BucketMagazzino *nuovo_bucket = crea_bucket_magazzino(nome_ricetta, lotto);
            nuovo_bucket->successore = ht->buckets[indice];
            ht->buckets[indice] = nuovo_bucket;
            return;
        }
        else
        {
            // ho gia il bucket devo fare inserimento ORDINATO di lotto nella lista di lotti
            bucket->lista = inserisci_lotto(bucket->lista, lotto);
        }
    }
    return;
}

//------------------------LISTE -----------------------------------------------------//

Ingrediente *crea_nodo_ricettario(int qta, BucketIngredienti *bucket_ingredienti)
{
    Ingrediente *nuovo_nodo = (Ingrediente *)malloc(sizeof(Ingrediente));
    nuovo_nodo->qta = qta;
    nuovo_nodo->bucket_ingrediente = bucket_ingredienti;
    nuovo_nodo->successore = NULL;
    return nuovo_nodo;
}

Lotto *crea_nodo_magazzino(int qta, int scadenza)
{
    Lotto *nuovo_nodo = (Lotto *)malloc(sizeof(Lotto));
    nuovo_nodo->qta = qta;
    nuovo_nodo->scadenza = scadenza;
    nuovo_nodo->successore = NULL;
    return nuovo_nodo;
}

Ingrediente *inserisci_ingrediente(Ingrediente *testa, Ingrediente *nodo)
{
    if (testa == NULL)
    {
        return nodo;
    }
    else
    {
        Ingrediente *temp = nodo;
        while (temp->successore != NULL)
        {
            temp = temp->successore;
        }
        temp->successore = testa;
        return nodo;
    }
}

// implementare inserimento ordinato per data di scadenza
Lotto *inserisci_lotto(Lotto *testa, Lotto *nodo)
{
    Lotto *temp = testa;
    Lotto *temp_prec = NULL;

    while (temp != NULL && nodo->scadenza > temp->scadenza)
    {
        temp_prec = temp;
        temp = temp->successore;
    }
    nodo->successore = temp;
    if (temp_prec != NULL)
    {
        // inserimento interno alla lista
        temp_prec->successore = nodo;
        return testa;
    }
    else
    {
        // inserimento in testa -> aggiorno la coda
        return nodo;
    }
}

Lotto *elimina_lotto_ptr(Lotto *testa, Lotto *nodo)
{
    Lotto *temp = testa;
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

void *elimina_lista_ingredienti(Ingrediente *testa)
{
    Ingrediente *temp = testa;
    Ingrediente *prec = testa;
    while (temp != NULL)
    {
        prec = temp;
        temp = temp->successore;
        free(prec);
    }
    return NULL;
}

void stampa_lista_ingredienti(Ingrediente *testa)
{
    Ingrediente *temp = testa;
    while (temp != NULL)
    {
        printf("%s, %d -> ", temp->bucket_ingrediente->nome_ingrediente, temp->qta);
        temp = temp->successore;
    }
    printf("\n");
    return;
}

void stampa_lista_lotti(Lotto *testa)
{
    Lotto *temp = testa;
    while (temp != NULL)
    {
        printf("QTA:%d SCAD:%d -> ", temp->qta, temp->scadenza);
        temp = temp->successore;
    }
    printf("\n");
    return;
}

//--------------------------------- ORDINI ------------------------------------------//

// se chiamata gia verificato che ogni ingrediente e presente per poter produrre ordine
void produci_ordine(Magazzino *magazzino, BucketRicettario *bucket_ricetta, int qta)
{
    Ingrediente *nodo_ingrediente = bucket_ricetta->lista;
    Lotto *lotto;
    Lotto *lotto_prec;
    BucketMagazzino *bucket_magazzino;
    int qta_necessaria;
    int cont = 0;

    // NOTA:
    // so gia che la ricetta non e vuota e che ci sono tutti gli ingredienti necessari e che ho gia tolto gli ingredienti scadutin o gia usati

    // scorro la lista di ingredienti e per ciascuno modifico le quantita/ rimuovo i lotti nel magazino
    while (nodo_ingrediente != NULL)
    {
        // bucket_magazzino = cerca_magazzino(magazzino, nodo_ingrediente->nome_ingrediente);
        bucket_magazzino = nodo_ingrediente->bucket_ingrediente->bucket_magazzino;
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
        cont++;
    }
    return;
}

// ritorna 0 se ingrediente mancante, 1 se ingrediente presente
int verifica_ingrediente(BucketMagazzino **bucket, int qta_necessaria, int clock)
{
    BucketMagazzino *bucket_magazzino = *bucket;
    Lotto *lotto = bucket_magazzino->lista;
    Lotto *lotto_prec;

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

void elimina_lotto(BucketMagazzino **bucket, Lotto *lotto)
{
    // faccio la free del lotto
    Lotto *temp = (*bucket)->lista;
    (*bucket)->lista = elimina_lotto_ptr(temp, lotto);
    return;
}

void stampa_lista_ordini(Ordine *testa)
{
    Ordine *temp = testa;
    while (temp != NULL)
    {
        printf("%s, %d ,%d -> ", temp->bucket_ricetta->nome_ricetta, temp->qta, temp->tempo);
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

Ordine *crea_ordine(char *nome_ricetta, BucketRicettario *bucket_ricetta, int qta, int tempo, int peso)
{
    Ordine *nuovo_nodo = (Ordine *)malloc(sizeof(Ordine));
    nuovo_nodo->qta = qta;
    nuovo_nodo->bucket_ricetta = bucket_ricetta;
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
        if (strcmp(temp->bucket_ricetta->nome_ricetta, nome_ricetta) == 0)
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

// ------------------------ CODE ----------------------------------//
Coda *inserisci_inordine_ordini(Coda *coda, Ordine *ordine)
{
    Ordine *temp = coda->testa;
    Ordine *temp_prec = NULL;

    // se devo inserire in fondo: inserimento in coda (pu veloce)
    if (coda->coda != NULL)
    {
        if (ordine->tempo >= coda->coda->tempo)
        {
            // inserimento in coda -> risparmio tempo
            ordine->successore = NULL;
            coda->coda->successore = ordine;
            coda->coda = ordine;
            return coda;
        }
    }

    // altrimenti: inserimento in ordine
    while (temp != NULL && ordine->tempo > temp->tempo)
    {
        temp_prec = temp;
        temp = temp->successore;
    }
    ordine->successore = temp;
    if (temp_prec != NULL)
    { // inserimento interno alla lista
        if (temp_prec->successore == NULL)
        {
            // sto inserendo alla fine -> aggiorno la coda
            temp_prec->successore = ordine;
            coda->coda = ordine;
        }
        else
        {
            // sto inserendo in mezzo -> non faccio niente
            temp_prec->successore = ordine;
        }
    }
    else
    {
        // inserimento in testa -> aggiorno la coda
        coda->testa = ordine;
    }
    return coda;
}

Coda *inserisci_in_coda(Coda *coda, Ordine *ordine)
{

    if (coda->testa == NULL)
    {
        coda->testa = ordine;
        coda->coda = ordine;
    }
    else
    {
        ordine->successore = NULL;
        coda->coda->successore = ordine;
        coda->coda = ordine;
    }
    return coda;
}

Coda *elimina_ordine_ptr_coda(Coda *coda, Ordine *nodo)
{
    Ordine *temp = coda->testa;

    // se e' il primo
    if (temp == nodo)
    {
        coda->testa = coda->testa->successore;
        if (coda->coda == nodo)
        { // se e' anche ultimo
            coda->coda = NULL;
        }
        free(temp);
        return coda;
    }
    // altrimenti lo cerco
    while (temp->successore != nodo && temp->successore != NULL)
    {
        temp = temp->successore;
    }
    if (temp->successore != NULL)
    {
        temp->successore = temp->successore->successore;
        if (temp->successore == NULL)
        {
            // se e' l'ultimo
            coda->coda = temp;
        }
        free(nodo);
    }
    return coda;
}

Coda *sfila_ordine_ptr_coda(Coda *coda, Ordine *nodo)
{
    Ordine *temp = coda->testa;

    // se e' il primo
    if (temp == nodo)
    {
        coda->testa = coda->testa->successore;
        if (coda->coda == nodo)
        { // se e' anche ultimo
            coda->coda = NULL;
        }
        return coda;
    }
    // altrimenti lo cerco
    while (temp->successore != nodo && temp->successore != NULL)
    {
        temp = temp->successore;
    }
    if (temp->successore != NULL)
    {
        temp->successore = temp->successore->successore;
        if (temp->successore == NULL)
        {
            // se e' l'ultimo
            coda->coda = temp;
        }
    }
    return coda;
}

void inizializza_coda(Coda **coda)
{
    (*coda)->coda = NULL;
    (*coda)->testa = NULL;
    return;
}