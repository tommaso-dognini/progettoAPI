# progettoAPI
Prova finale dell'esame "Algoritmi e principi dell'informatica", Politecninco di Milano, Ingegneria Informatica.

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

// Risultato ottenuto: Voto=27 , Tempo = 6.260 (s), Memoria_utilizzata = 16.8 (MiB)

// ULTERIORI MIGLIORAMENTI
// - Funzione per sfilare un elemento da una coda. Questa permetterebbe migliore velocita nel passaggio di un ordine da lista di attesa alla
// lista di ordini pronti in quanto eliminerebbe la necessita di creare un nuovo ordine (avrebbe la stessa complessita temporale della sola eliminazione)

// - una ulteriore hash-table per la gestione del nome delle ricette potrebbe migliorare utilizzo della memoria dinamica in quanto ordini della stessa ricetta
// verrebbero creati con il solo puntatore ad un unica cella della hash-table senza duplicazione di stirnghe identiche.

// - Implementazione di una politica di rimozione degli ingredienti dalla hash-table ingredienti se non presenti in nessuna ricetta (migliore utilizzo della memoria)
// e miglioramenti in termini di tempo (hash-table meno affollata)
