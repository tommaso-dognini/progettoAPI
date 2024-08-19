#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define CMD_LEN 256

int hash(char *string)
{
    // vettore con valori di potenze di p precalcolati per aumentare efficienza. Calcolati fino a p ^CMD_LEN = 256
    int p_pow[] = {53, 2809, 8779, 4965, 2963, 6934, 7250, 3984, 1005, 3230, 1071, 6728, 6339, 5736, 3798, 1154, 1120, 9325, 3882, 5606, 6915, 6243, 648, 4323, 8965, 4816, 5073, 8687, 89, 4717, 9833, 785, 1577, 3525, 6699, 4802, 4331, 9389, 7274, 5256, 8379, 3779, 147, 7791, 2636, 9617, 9351, 5260, 8591, 5008, 5242, 7637, 4481, 7332, 8330, 1182, 2604, 7921, 9526, 4528, 9823, 255, 3508, 5798, 7084, 5193, 5040, 6938, 7462, 5213, 6100, 3076, 2916, 4443, 5318, 1658, 7818, 4067, 5404, 6216, 9224, 8536, 2093, 852, 5128, 1595, 4479, 7226, 2712, 3638, 2681, 1995, 5665, 35, 1855, 8252, 7055, 3656, 3635, 2522, 3575, 9349, 5154, 2973, 7464, 5319, 1711, 620, 2839, 362, 9179, 6151, 5779, 6077, 1857, 8358, 2666, 1200, 3558, 8448, 7436, 3835, 3115, 4983, 3917, 7461, 5160, 3291, 4304, 7958, 1480, 8391, 4415, 3834, 3062, 2174, 5145, 2496, 2197, 6364, 7061, 3974, 475, 5161, 3344, 7113, 6730, 6445, 1347, 1342, 1077, 7046, 3179, 8375, 3567, 8925, 2696, 2790, 7772, 1629, 6281, 2662, 988, 2329, 3353, 7590, 1990, 5400, 6004, 7995, 3441, 2247, 9014, 7413, 2616, 8557, 3206, 9806, 9361, 5790, 6660, 2735, 4857, 7246, 3772, 9783, 8142, 1225, 4883, 8624, 6757, 7876, 7141, 8214, 5041, 6991, 264, 3985, 1058, 6039, 9850, 1686, 9302, 2663, 1041, 5138, 2125, 2548, 4953, 2327, 3247, 1972, 4446, 5477, 78, 4134, 8955, 4286, 7004, 953, 474, 5108, 535, 8341, 1765, 3482, 4420, 4099, 7100, 6041, 9956, 7304, 6846, 2586, 6967, 8999, 6618, 509, 6963, 8787, 5389, 5421, 7117, 6942, 7674, 6442, 1188, 2922, 4761, 2158, 4297, 7587, 1831, 6980, 9688, 3107};
    // int p = 53;
    long long m = 10000 + 7;
    unsigned long long hash = 0;
    for (int i = 0; i < strlen(string); i++)
    {
        hash = (hash + (string[i] - 'a' + 1) * p_pow[i]) % m;
    }
    return hash;
}

typedef struct Nodo
{
    int hash;
    char string[CMD_LEN];
    struct Nodo *successore;
} Nodo;

Nodo *inserisci(Nodo *testa, int hash, char *string)
{

    Nodo *temp = testa;
    int stop = 0;

    while (temp != NULL && stop != 1)
    {
        if (strcmp(temp->string, string) == 0)
            stop = 1;

        temp = temp->successore;
    }
    if (stop == 0)
    {
        Nodo *nuovo_nodo = (Nodo *)malloc(sizeof(Nodo));
        nuovo_nodo->hash = hash;
        strcpy(nuovo_nodo->string, string);

        nuovo_nodo->successore = testa;
        testa = nuovo_nodo;
    }
    return testa;
}

void stampa(Nodo *testa)
{
    while (testa != NULL)
    {
        printf("%s, HASH:%d\n", testa->string, testa->hash);
        testa = testa->successore;
    }
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

    if (a->hash <= b->hash)
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

int main()
{

    char comando[CMD_LEN];
    int clock = 0; // istanti di tempo della simulazione

    char nome_ricetta[CMD_LEN];
    char nome_ingrediente[CMD_LEN];

    int qta = 0;
    int scadenza = 0;
    int controllo = 0;
    // int qta_necessaria = 0;
    char separatore = 'a';

    // acquisisco parametri corriere
    int periodo, capienza;

    controllo = scanf("%d %d", &periodo, &capienza);
    // printf("Il periodo e: %d \nLa capienza del corriere e: %d \n", periodo, capienza);
    controllo = scanf("%c", &separatore);

    Nodo *ricette = NULL;
    // Nodo *ingrediente = NULL;

    while (separatore == '\n' && controllo != -1)
    {
        // printf("CLOCK: %d\n", clock);

        //----------------------------- ACQUISISCO COMANDO -------------------//

        controllo = scanf("%s", comando);

        //----------------------------- GESTIONE COMANDI -------------------//

        // AGGIUNGI_RICETTA
        if (strcmp(comando, "aggiungi_ricetta") == 0)
        {
            controllo = scanf("%s", nome_ricetta);
            controllo = scanf("%c", &separatore);

            

            while (separatore != '\n')
            {
                controllo = scanf("%s", nome_ingrediente);
                controllo = scanf("%d", &qta);
                controllo = scanf("%c", &separatore);
            }

            // printf("Nome ricetta:%s\n", nome_ricetta);

            nome_ricetta[0] = 0;
        }

        // RIMUOVI_RICETTA
        else if (strcmp(comando, "rimuovi_ricetta") == 0)
        {
            controllo = scanf("%s", nome_ricetta);
            controllo = scanf("%c", &separatore);

            // printf("Nome ricetta:%s\n", nome_ricetta);
        }

        // ORDINE
        else if (strcmp(comando, "ordine") == 0)
        {

            controllo = scanf("%c", &separatore);
            while (separatore != '\n')
            {

                controllo = scanf("%s", nome_ricetta);
                controllo = scanf("%d", &qta);
                controllo = scanf("%c", &separatore);
                // printf("Ordine:%s,qta:%d\n", nome_ricetta, qta)
            }
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

                ricette = inserisci(ricette, hash(nome_ingrediente), nome_ingrediente);
            }
        }

        // AGGIUSTAMENTI
        comando[0] = 0;
        clock++;
    }

    merge_sort(&ricette);
    stampa(ricette);

    // faccio le free
    return 0;
}