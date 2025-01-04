#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{

    char comando[256];
    char nome_ricetta[256];
    char separatore = 'a';
    char nome_ingrediente[256];
    int qta;
    int controllo = 0;

    int periodo, capienza;
    // int processo = 0; // processo = 1 se aggiungi ricetta, 2 se rimuovi ricetta, ...

    controllo = scanf("%d %d", &periodo, &capienza);
    printf("Il periodo e: %d \nLa capienza del corriere e: %d \n", periodo, capienza);
    controllo = scanf("%c", &separatore);

    while (separatore == '\n' &&  controllo != -1)
    {
        controllo = scanf("%s",comando);
        if (strcmp(comando, "ricetta") == 0)
        {
            controllo = scanf("%s", nome_ricetta);
            controllo = scanf("%c", &separatore);
            printf("Nome ricetta:%s\n", nome_ricetta);
            while (separatore != '\n')
            {
                controllo = scanf("%s", nome_ingrediente);
                controllo =scanf("%d", &qta);
                controllo = scanf("%c", &separatore);
                printf("Ingrediente:%s,qta:%d\n", nome_ingrediente, qta);
            }
        }
        else if (strcmp(comando, "ordine") == 0)
        {
            printf("Comando:%s\n", comando);
        }
    }
    return 0;
}
