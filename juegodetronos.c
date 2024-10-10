#include "juegodetronos.h"
#include "abb.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 2047

void imprimirLista(TLISTA lista)
{
    if (!esListaVacia(lista))
    {
        TIPOELEMENTOLISTA elemento;
        for (TPOSICION pos = primeroLista(lista); pos != finLista(lista);
             pos           = siguienteLista(lista, pos))
        {
            recuperarElementoLista(lista, pos, &elemento);
            printf("%s, ", elemento.name);
        }
        printf("\n");
    }
}

void printElemento(TIPOELEMENTOABB elemento)
{
    printf("%s\n", elemento.name);
    printf("\tHouse: %s\n", elemento.house);
    printf("\tRoyal: ");
    if (elemento.royal == 0)
    {
        printf("No\n");
    }
    else
    {
        printf("Yes\n");
    }
    if (!esListaVacia(elemento.parents))
    {
        printf("\tParents: ");
        imprimirLista(elemento.parents);
    }
    if (!esListaVacia(elemento.siblings))
    {
        printf("\tSiblings: ");
        imprimirLista(elemento.siblings);
    }
    if (!esListaVacia(elemento.killed))
    {
        printf("\tKilled: ");
        imprimirLista(elemento.killed);
    }
    printf("\n\n");
}

void inorden(TABB arbol)
{
    if (!esAbbVacio(arbol))
    {
        TIPOELEMENTOABB elemento;
        inorden(izqAbb(arbol));
        leerElementoAbb(arbol, &elemento);
        printElemento(elemento);
        inorden(derAbb(arbol));
    }
}

TIPOELEMENTOABB crearPersonaje()
{
    TIPOELEMENTOLISTA aux;
    TIPOELEMENTOABB   ret;

    crearLista(&ret.parents);
    crearLista(&ret.killed);
    crearLista(&ret.siblings);

    printf("Name: ");
    scanf(" %[^\n]s", ret.name);
    getchar(); // Leer el '\n' pendiente

    printf("\tHouse\t (? si es desconocido): ");
    scanf("%s", ret.house);

    printf("\tRoyal\t (0/1): ");
    scanf("%d", &ret.royal);

    while (strcmp(aux.name, "fin") != 0)
    {
        printf("\tParents\t (fin para terminar): ");
        scanf(" %[^\n]s", aux.name);
        if (strcmp(aux.name, "fin") != 0)
            insertarElementoLista(&ret.parents, finLista(ret.parents), aux);
    }
    aux.name[0] = '\0';
    while (strcmp(aux.name, "fin") != 0)
    {
        printf("\tSiblings\t (fin para terminar): ");
        scanf(" %[^\n]s", aux.name);
        if (strcmp(aux.name, "fin") != 0)
            insertarElementoLista(&ret.siblings, finLista(ret.siblings), aux);
    }

    aux.name[0] = '\0';
    while (strcmp(aux.name, "fin") != 0)
    {
        printf("\tKilled\t (fin para terminar): ");
        scanf(" %[^\n]s", aux.name);
        if (strcmp(aux.name, "fin") != 0)
            insertarElementoLista(&ret.killed, finLista(ret.killed), aux);
    }
    printf("\n\n");
    return ret;
}

void anhadirPersonaje(TABB *arbol)
{
    TIPOELEMENTOABB elemento = crearPersonaje();
    insertarElementoAbb(arbol, elemento);
}

void listarPersonajes(TABB arbol)
{
    if (esAbbVacio(arbol))
    {
        printf("El árbol está vacío\n");
        return;
    }
    inorden(arbol);
}

void eliminarPersonaje(TABB *arbol)
{
    if (!esAbbVacio(*arbol))
    {
        TIPOELEMENTOABB elemento, aux;
        printf("Introduce el nombre del personaje a eliminar: ");
        scanf(" %[^\n]s", aux.name);
        if (esMiembroAbb(*arbol, aux))
        {
            suprimirElementoAbb(arbol, aux);
        }
        else
        {
            printf("El personaje %s no forma parte de la base de datos.\n",
                   aux.name);
        }
    }
}


// ENTREGA 2


void _strip_line(char *linea) { linea[strcspn(linea, "\r\n")] = 0; }

void _split_ByComa(char *cadena, TLISTA *lista)
{
    char             *start = cadena;
    char             *end   = cadena;
    TIPOELEMENTOLISTA elemento;
    while (*end != '\0')
    {
        if (*end == ',')
        {
            *end = '\0';
            strncpy(elemento.name, start, MAX);
            insertarElementoLista(lista, finLista(*lista), elemento);
            start = end + 1;
        }
        end++;
    }
    if ((start != end) && start[0] != '-')
    {
        strncpy(elemento.name, start, MAX);
        insertarElementoLista(lista, finLista(*lista), elemento);
    }
}

void cargar_archivo(TABB *arbol, int argc, char **argv)
{
    if (argc < 3 ||
        (strcmp(argv[1], "-f") != 0 && strcmp(argv[1], "--file") != 0))
    {
        return;
    }
    FILE *file = fopen(argv[2], "r");
    if (file == NULL)
    {
        printf("El archivo no se pudo abrir\n");
        return;
    }

    TIPOELEMENTOABB elemento;
    char            aux[MAX_LINE];
    char *name, *house, *royal, *parents_list, *siblings_list, *killed_list;
    TIPOELEMENTOLISTA caux;

    while (fgets(aux, MAX_LINE, file) != NULL)
    {
        crearLista(&elemento.siblings);
        crearLista(&elemento.parents);
        crearLista(&elemento.killed);
        _strip_line(aux);

        name          = strtok(aux, "|");
        house         = strtok(NULL, "|");
        royal         = strtok(NULL, "|");
        parents_list  = strtok(NULL, "|");
        siblings_list = strtok(NULL, "|");
        killed_list   = strtok(NULL, "|");
        strncpy(elemento.name, name, MAX);
        strncpy(elemento.house, house, MAX);
        elemento.royal = atoi(royal);
        _split_ByComa(siblings_list, &elemento.siblings);
        _split_ByComa(parents_list, &elemento.parents);
        _split_ByComa(killed_list, &elemento.killed);
        insertarElementoAbb(arbol, elemento);
    }
}
void imprimirListaArchivo(TLISTA lista, FILE *file)
{
    if (esListaVacia(lista))
    {
        fprintf(file, "-|");
        return;
    }
    TIPOELEMENTOLISTA elemento;
    for (TPOSICION pos = primeroLista(lista); pos != finLista(lista);
         pos           = siguienteLista(lista, pos))
    {
        recuperarElementoLista(lista, pos, &elemento);
        fprintf(file, "%s", elemento.name);
        if (siguienteLista(lista, pos) != finLista(lista))
        {
            fprintf(file, ",");
        }
        else
        {
            fprintf(file, "|");
        }
    }
}
void printElementoArchivo(TIPOELEMENTOABB elemento, FILE *file)
{
    fprintf(file, "%s|", elemento.name);
    fprintf(file, "%s|", elemento.house);
    fprintf(file, "%d|", elemento.royal);
    imprimirListaArchivo(elemento.parents, file);
    imprimirListaArchivo(elemento.siblings, file);
    imprimirListaArchivo(elemento.killed, file);
    fprintf(file, "\n");
}
void postorden(TABB arbol, FILE *file)
{
    if (!esAbbVacio(arbol))
    {
        TIPOELEMENTOABB elemento;
        postorden(izqAbb(arbol), file);
        postorden(derAbb(arbol), file);
        leerElementoAbb(arbol, &elemento);
        printElementoArchivo(elemento, file);
    }
}
void actualizar_archivo(TABB arbol, int argc, char **argv)
{
    if (argc < 3 ||
        (strcmp(argv[1], "-f") != 0 && strcmp(argv[1], "--file") != 0))
    {
        return;
    }
    FILE *file = fopen("pruebapersonajes.txt", "w");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }
    postorden(arbol, file);
}
