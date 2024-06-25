#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // int n = 1;
    char comando[256];
    char nome_ricetta[256];
    char ingrediente[256];
    char separatore = 'a';
    int qta;
    int continua=1;

    int periodo, capienza;
    int processo = 0; // processo = 1 se aggiungi ricetta, 2 se rimuovi ricetta, ...

    scanf("%d %d", &periodo, &capienza);
    printf("Il periodo e: %d \nLa capienza del corriere e: %d \n", periodo, capienza);

    while (continua)
    {
        separatore = getchar();
        if(separatore== EOF)
            continua=0;

        if (separatore == '\n')
        { // se separatore e ENTER allora ho finito un comando e me ne arriva un altro
            scanf("%s", comando);

            // ricetta
            if (strcmp(comando, "ricetta") == 0)
            {
                processo = 1;
                scanf("%s", nome_ricetta);
                printf("Nome ricetta:%s\n", nome_ricetta);
                // verifico se esiste e creo ricetta
            }
            // ordine
            if (strcmp(comando, "ordine") == 0)
            {
                printf("Ordine\n");
            }
            // e cosi via
        }
        else if (continua==1)
        {// se separatore non e EOF e nemmeno ENTER e uno spazio e sto processando dati di una delle funzionui della pasticceria

            if (processo == 1)//sto processando comando aggiungi ricetta e sto prendendo ingredienti
            {
                scanf("%s %d",ingrediente,&qta);
                printf("INGREDIENTE:%s,QTA:%d\n",ingrediente,qta);
            }
        }
    }
    printf("fine\n");
    return 0;
}