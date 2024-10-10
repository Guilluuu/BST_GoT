#include "abb.h"
#include "juegodetronos.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    char opcion;
    TABB arbol;
    crearAbb(&arbol);
    cargar_archivo(&arbol, argc, argv);
    do
    {
        printf("Escoge una opcion\n\n");
        printf("A/a:    Añadir personaje\n");
        printf("L/l     Listado alfabético de personajes\n");
        printf("E/e     Eliminar personaje\n");
        printf("S/s     Salir\n");
        printf("\nOpcion: ");
        scanf(" %c", &opcion);
        printf("\n");
        switch (opcion)
        {
            case 'A':
            case 'a':
                anhadirPersonaje(&arbol);
                break;
            case 'L':
            case 'l':
                listarPersonajes(arbol);
                break;
            case 'E':
                break;
            case 'e':
                eliminarPersonaje(&arbol);
                break;
            case 'S':
                opcion = 's';
            case 's':
                printf("Gracias por usar nuestro sistema GoT\n\n");
                destruirAbb(&arbol);
                break;
            default:
                printf("Opcion incorrecta");
                break;
        }
        actualizar_archivo(arbol, argc, argv);
    } while (opcion != 's');
    return 0;
}
