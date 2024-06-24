#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int n = 1;
    char comando[256];
    char *dati;
    char nome_ricetta[256];
    char temp;

    int periodo, capienza;

    scanf("%d %d", &periodo, &capienza);
    printf("Il periodo e: %d \nLa capienza del corriere e: %d \n", periodo, capienza);

    while (n > 0)
    {
        scanf("%c", &temp);
        scanf("%s", comando);
        n = strlen(comando);

        scanf("%s", nome_ricetta);

        scanf("%c", &temp);
        scanf("%m[^\n]s", &dati);

        printf("Comando: %s\n", comando);
        printf("Nome ricetta: %s\n", nome_ricetta);
        printf("Dati: %s\n", dati);

        comando[0] = 0;
    }
    return 0;
}