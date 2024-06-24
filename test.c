#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int n = 1;
    char comando[256];
    char dati[256];
    char nome_ricetta[256];
    char temp;


    while (n > 0)
    {
        scanf("%s",comando);
        n = strlen(comando);

        scanf("%s",nome_ricetta);

        scanf("%c", &temp);
        scanf("%[^\n]s",dati);
        scanf("%c", &temp);
        printf("Comando: %s\n", comando);
        printf("Nome ricetta: %s\n", nome_ricetta);
        printf("Dati: %s", dati);
        
        comando[0] = 0;
    }
    return 0;
}