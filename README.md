# Progetto Algoritmi e Principi dell'Informatica

<p align="center">
    <img src="https://skillicons.dev/icons?i=git,c,cmake,vscode,linux" />
</p>

Progetto della prova finale dell'esame "Algoritmi e principi dell'informatica", Politecninco di Milano, Ingegneria Informatica.\
[Polimi Course Website](https://www4.ceda.polimi.it/manifesti/manifesti/controller/ManifestoPublic.do?EVN_DETTAGLIO_RIGA_MANIFESTO=evento&aa=2024&k_cf=225&k_corso_la=358&k_indir=IT1&codDescr=086067&lang=IT&semestre=2&anno_corso=2&idItemOfferta=168849&idRiga=308552)

## Assignment
Implementare in C un programma secondo la specifica di progetto di seguito riportata. L'implementazione deve rispettare la specifica in ogni suo ambito di funzionamento e produrre un output corretto per ogni possibile combinazione di comandi.L'obiettivo del progetto e' di produrre un programma che sia efficiente in termini di memoria utilizzata e tempo impiegato per l'esecuzione. Di seguito la tabella dei requisiti di prestazioni da ottenere suddivisa per fasce di valutazione:

<div style="text-align: center;">
    <table border="1" style="margin: 0 auto; border-collapse: collapse; text-align: center;">
        <thead>
            <tr>
                <th>TEST (voto)</th>
                <th>TEMPO (s)</th>
                <th>MEMORIA (MiB)</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td>18</td>
                <td>14.00</td>
                <td>35</td>
            </tr>
            <tr>
                <td>21</td>
                <td>11.50</td>
                <td>30</td>
            </tr>
            <tr>
                <td>24</td>
                <td>9.00</td>
                <td>25</td>
            </tr>
            <tr>
                <td>27</td>
                <td>6.50</td>
                <td>20</td>
            </tr>
            <tr>
                <td>30</td>
                <td>4.00</td>
                <td>15</td>
            </tr>
            <tr>
                <td>30L</td>
                <td>1.500</td>
                <td>14</td>
            </tr>
        </tbody>
    </table>
</div>
<br></br>

[Link server verificatore](https://dum-e.deib.polimi.it/)

E' possibile utilizzare solamente le librerie standard di C e nessun altro pezzo di codice reperito online.

---

### Specifica di progetto
Una pasticceria industriale vuole migliorare il proprio sistema di gestione degli ordini, e vi incarica di sviluppare un software che simuli il funzionamento della pasticceria. L’intera simulazione avviene a tempo discreto. Si assuma che, a valle dell’esecuzione di ogni comando ricevuto in ingresso, trascorra un istante di tempo. La simulazione ha inizio al tempo 0. Nella simulazione si dovranno considerare i seguenti elementi:
- Gli **ingredienti** dei dolci, ognuno identificato dal suo nome, costituito da una sequenza di caratteri.

- L’insieme delle **ricette** offerte dalla pasticceria, identificate anch’esse da un nome. Ogni ricetta utilizza diverse quantità di ciascun ingrediente necessario (indicate da un numero intero, in grammi).

- Il **magazzino** degli ingredienti della pasticceria, che stocca ogni ingrediente utilizzato. Il magazzino viene **rifornito** da nuovi **lotti** di ingredienti, in base a una pianificazione stabilita dal fornitore. Ogni lotto è caratterizzato da una quantità (sempre in grammi) e da una data di scadenza, indicata dal numero dell’istante di tempo a partire dal quale il lotto è scaduto.

- I clienti della pasticceria effettuano **ordini** di uno o più dolci tramite una piattaforma online o per telefono. In ogni caso, la pasticceria procede subito a preparare i dolciumi ordinati. Gli avanzati (e costosi) macchinari che preparano i dolci sono così veloci che si può assumere che la preparazione di un numero arbitrario di dolci avvenga in un singolo istante della simulazione. Gli ingredienti necessari per ogni preparazione vengono prelevati dal magazzino privilegiando sempre i lotti con la scadenza più prossima. Se non sono disponibili ingredienti a sufficienza da consentire la preparazione per intero di un ordine, esso viene messo in attesa. È possibile avere un numero arbitrario di ordini in attesa. La pasticceria procede a preparare eventuali ordini successivi. Ad ogni rifornimento, la pasticceria valuta se è possibile, con gli ingredienti ricevuti, preparare ordini attualmente in attesa. Se questo è il caso, li prepara nello stesso istante di tempo. Gli ordini in attesa vengono smaltiti in ordine cronologico di arrivo dell’ordine.

- Periodicamente, il **corriere** si reca dalla pasticceria a ritirare gli ordini pronti. All’arrivo del corriere, gli ordini da caricare vengono scelti in ordine cronologico di arrivo. Il processo si ferma appena viene incontrato un ordine che supera la capienza rimasta (in grammi) sul camioncino. Si assuma che il peso di ogni dolce preparato sia uguale alla somma delle quantità in grammi di ciascun ingrediente. Ogni ordine viene sempre caricato nella sua interezza. Scelti gli ordini, la pasticceria procede a caricarli in ordine di peso decrescente. A parità di peso, gli ordini vanno caricati in ordine cronologico di arrivo.

Si assuma che tutte le quantità siano intere e maggiori di zero, indipendentemente dall’unità di misura. I nomi, siano essi di ingredienti o ricette, sono definiti sull’alfabeto {a, ..., z, A, ..., Z, _} e sono lunghi fino a 255 caratteri. La simulazione termina in seguito alla lettura dell’ultimo comando. Il file testuale in ingresso inizia con una riga contenente due interi: la periodicità del corriere e la sua capienza. Seguono una sequenza di comandi, uno per riga, con il seguente formato. Tutti i valori interi positivi o nulli sono codificabili in 32 bit.
- `aggiungi_ricetta ⟨nome_ricetta⟩ ⟨nome_ingrediente⟩ ⟨quantità⟩...`\
    **Esempio:** `aggiungi_ricetta meringhe_della_prozia zucchero 100 albumi 100`\
    Aggiunge una ricetta al catalogo. Il numero di coppie (⟨nome_ingrediente⟩ ⟨quantità⟩) è arbitrario. Se una ricetta con lo stesso nome è già presente, viene ignorato. Stampa attesa come risposta: `aggiunta` oppure `ignorato`.

- `rimuovi_ricetta ⟨nome_ricetta⟩`\
    **Esempio:** `rimuovi_ricetta cannoncini`\
    Rimuove una ricetta dal catalogo. Non ha effetto se la ricetta non è presente, oppure ci sono ordini relativi ad essa non ancora spediti. Stampa attesa come risposta: `rimossa`, `ordini in sospeso` oppure `non presente`.

- `rifornimento ⟨nome_ingrediente⟩ ⟨quantità⟩ ⟨scadenza⟩...`\
    **Esempio:** `rifornimento zucchero 200 150 farina 1000 220`
    La pasticceria viene rifornita di un insieme di lotti, uno per ingrediente. Il numero di lotti è arbitrario. Stampa attesa come risposta: `rifornito`.
    
- `ordine ⟨nome_ricetta⟩ ⟨numero_elementi_ordinati⟩`\
    **Esempio:** `ordine torta_paradiso 36`\
    Effettua un ordine di ⟨numero_elementi_ordinati⟩ dolci con ricetta ⟨nome_ricetta⟩. Stampa attesa come risposta: `accettato` oppure `rifiutato` se non esiste nessuna ricetta col nome specificato.

In aggiunta alle stampe sopraelencate, il programma stampa gli ordini contenuti nel camioncino del corriere come una sequenza di triple ⟨istante_di_arrivo_ordine⟩ ⟨nome_ricetta⟩ ⟨numero_elementi_ordinati⟩, una per rigo, in ordine di caricamento. Dato un corriere di periodicità n, la stampa è effettuata prima di gestire i comandi al tempo kn con k ∈ 1, 2, . . .. Se il camioncino è vuoto, viene stampato il messaggio camioncino vuoto.

---

## Scelte implementative

### SCELTA STRUTTURA DATI
Dopo vari test e implementazioni con diverse strutture dati quali alberi BST, alberi R-B e liste sono giunto alla conclusione che per il problema dato la struttura dati piu' consona fossero le Hash-Tables. Infatti il problema piu grosso che si e' presentato durante lo svolgimento del progetto e' stato quello di trovare una tecnica di ricerca che fosse veloce ed efficiente, questo perche' ad ogni ricetta da produrre e necessario sapere gli ingredienti di cui e composta (ricerca nel ricettario), della disponibilita' di questi (ricerca nel magazzino e confronto con qta disponibili). Analoghi problemi si sono poi presentati nella produzione degli ordini infatti anche in questo caso dato un ordine sono necessarie tutto un insieme di informazioni solamente per stabilire se puo' essere prodotto e sono poi necessarie ulteriori azioni per produrlo.
Per questi motivi le hash-tables si sono rivelate perfette: ottima velocita' di ricerca e di conseguenza di inserimento e rimozione. Inloltre mi hanno permesso di gestire le stringhe (altro punto importante del problema) in modo efficiente in quanto strutture dati come alberi R-B avrebbero avuto bisogno di una regola di ordinamento tra di esse cosa che avrebbe sicuramente aumentato il numero di strcmp necessarie all'inserimento, ricerca e rimozione.

Ho inoltre scelto di utilizzare hash-tables con politica di gestione delle collisioni con liste single-linked. La scelta e' stata fatta sulla base di molteplici criteri: 
- volevo evitare di dover, in caso di numerosi inserimenti, gestire realloc e re-hashing delle strutture. 
- Mi ha permesso di riutilizzare varie funzioni in altre parti del codice, sapevo infatti che da qualche parte avrei avuto bisogno delle classiche, semplici liste. 
- la scelta particolare di usare liste single 
- linked e' dovuta ad una pura scelta di ottimizzazione (duoble linked sarebbero state piu semplici da gestire).

Ho infine scelto di utilizzare delle semplici code di liste single-linked per la gestione di Ordini e Corriere. Anche in questo caso si tratta di una scelta fatta per semplicita di gestione e implementazione ed in modo particolare perche' implementando le code (liste con puntatore alla coda) ho potuto rendere l'inserimento ordinato molto rapido in quanto la necessita' era di inserire gli ordini nelle liste di attesa e ordini pronti in modo ordinato con criterio di ordine crescente per data (tempo della simulazione) di acquisizione. Grazie alle code questa operazione avviene in O(1).

La gestione del corriere fa una piccola eccezione a questo ragionamento inquanto ho deciso per ottimizare utilizzo della memoria di implementare delle semplici liste poiche' in questo caso la metrica d'oridne non permetteva un inserimento ordinato efficiente (ordine decrescente di peso). Per questo ho semplicemente implementato inserimento in testa e un ordinamento tramite merge-sort crescente che viene fatto una sola volta al momento della stampa.

### GESTIONE STRUTTURE DATI
Inizialmente avevo scelto di implementare 2 hash tables: il ricettario e il magazzino. Questo mi permetteva di raggiungere tutte le funzionalita' richieste con un livello di efficienza buono ma non sufficiente. Infatti il problema di memorizzare (e quindi di confrontare ad ogni ricerca) il nome di ingredienti e il nome delle ricette in ogni nodo del ricettario e del magazzino creava due grossi problemi: 
- molta memoria utilizzata (ogni stringa puo essere fino a 255 caratt.)
- molto tempo sprecato nel copiare le stringhe ad ogni creazione di nuovi nodi. 
- molti duplicati memorizzati in entrambi le strutture: ingredienti con lo stesso nome ma di ricette diverse venivano memorizzati 2 volte.

Ho per questo deciso di introdurre un'altra struttura, una hash-tables per memorizzare (senza duplicati) i nomi degli ingredienti.
questa scelta ha 2 grossi vantaggi:
- Niente duplicati di stringhe memorizzati piu' volte (vengono memorizzati una volta sola e in ricettario e magazzino sono presenti soltanto puntatori alla h.t. degli ingredienti)
- Avendo una struttura specifica per i soli ingredienti ho potuto con un solo puntatore in piu' legare ad ogni ingrediente l'indirizzo nel magazzino dei suoi lotti questo ha ridotto di molto il numero di ricerce in magazzino avvelocizzando la produzione degli ordini.

Per quanto riguarda la gestione del corriere e degli ordini come detto sopra ho preferito la semplicita delle liste in particolare ho implementato delle code in modo da poter gestire l'inserimento in ordine (in fondo = in ordine di tempo) in modo semplice e veloce senza dover ri-ordinare le varie liste ad ogni inserimento.

### EFFICIENZA E VALUTAZIONE

<div style="text-align: center;">
    <table border="1" style="margin: 0 auto; border-collapse: collapse; text-align: center;">
        <thead>
            <tr>
                <th>TEST (voto)</th>
                <th>TEMPO (s)</th>
                <th>MEMORIA (MiB)</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td>18</td>
                <td>14.00</td>
                <td>35</td>
            </tr>
            <tr>
                <td>21</td>
                <td>11.50</td>
                <td>30</td>
            </tr>
            <tr>
                <td>24</td>
                <td>9.00</td>
                <td>25</td>
            </tr>
            <tr>
                <td>27</td>
                <td>6.50</td>
                <td>20</td>
            </tr>
            <tr>
                <td>30</td>
                <td>4.00</td>
                <td>15</td>
            </tr>
            <tr>
                <td>30L</td>
                <td>1.500</td>
                <td>14</td>
            </tr>
        </tbody>
    </table>
</div>
<br></br>

**Risultato ottenuto: VOTO =27 , TEMPO = 6.26(s), MEMORIA_UTILIZZATA = 16.8 (MiB)**

### ULTERIORI MIGLIORAMENTI
- Funzione per sfilare un elemento da una coda. Questa permetterebbe migliore velocita nel passaggio di un ordine da lista di attesa alla lista di ordini pronti in quanto eliminerebbe la necessita di creare un nuovo ordine (avrebbe la stessa complessita temporale della sola eliminazione)

- una ulteriore hash-table per la gestione del nome delle ricette potrebbe migliorare utilizzo della memoria dinamica in quanto ordini della stessa ricetta verrebbero creati con il solo puntatore ad un unica cella della hash-table senza duplicazione di stirnghe identiche.

- Implementazione di una politica di rimozione degli ingredienti dalla hash-table ingredienti se non presenti in nessuna ricetta (migliore utilizzo della memoria) e miglioramenti in termini di tempo (hash-table meno affollata)