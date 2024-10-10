#include "juegodetronos.h"
#include "abb.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 2047

void imprimirLista(TLISTA lista) {

  if (!esListaVacia(lista)) {
    TIPOELEMENTOLISTA elemento;
    for (TPOSICION pos = primeroLista(lista); pos != finLista(lista);
         pos = siguienteLista(lista, pos)) {

      recuperarElementoLista(lista, pos, &elemento);
      printf("%s, ", elemento.name);
    }
    printf("\n");
  }
}

void printElemento(TIPOELEMENTOABB elemento) {
  printf("%s\n", elemento.name);
  printf("\tHouse: %s\n", elemento.house);
  printf("\tRoyal: ");
  if (elemento.royal == 0) {
    printf("No\n");
  } else {
    printf("Yes\n");
  }
  if (!esListaVacia(elemento.parents)) {
    printf("\tParents: ");
    imprimirLista(elemento.parents);
  }
  if (!esListaVacia(elemento.siblings)) {
    printf("\tSiblings: ");
    imprimirLista(elemento.siblings);
  }
  if (!esListaVacia(elemento.killed)) {
    printf("\tKilled: ");
    imprimirLista(elemento.killed);
  }
  printf("\n\n");
}

void inorden(TABB arbol) {
  if (!esAbbVacio(arbol)) {
    TIPOELEMENTOABB elemento;
    inorden(izqAbb(arbol));
    leerElementoAbb(arbol, &elemento);
    printElemento(elemento);
    inorden(derAbb(arbol));
  }
}

TIPOELEMENTOABB crearPersonaje() {

  TIPOELEMENTOLISTA aux;
  TIPOELEMENTOABB ret;

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

  while (strcmp(aux.name, "fin") != 0) {
    printf("\tParents\t (fin para terminar): ");
    scanf(" %[^\n]s", aux.name);
    if (strcmp(aux.name, "fin") != 0)
      insertarElementoLista(&ret.parents, finLista(ret.parents), aux);
  }
  aux.name[0] = '\0';
  while (strcmp(aux.name, "fin") != 0) {
    printf("\tSiblings\t (fin para terminar): ");
    scanf(" %[^\n]s", aux.name);
    if (strcmp(aux.name, "fin") != 0)
      insertarElementoLista(&ret.siblings, finLista(ret.siblings), aux);
  }

  aux.name[0] = '\0';
  while (strcmp(aux.name, "fin") != 0) {
    printf("\tKilled\t (fin para terminar): ");
    scanf(" %[^\n]s", aux.name);
    if (strcmp(aux.name, "fin") != 0)
      insertarElementoLista(&ret.killed, finLista(ret.killed), aux);
  }
  printf("\n\n");
  return ret;
}

void anhadirPersonaje(TABB *arbol) {
  TIPOELEMENTOABB elemento = crearPersonaje();
  insertarElementoAbb(arbol, elemento);
}

void listarPersonajes(TABB arbol) {
  if (esAbbVacio(arbol)) {
    printf("El árbol está vacío\n");
    return;
  }
  inorden(arbol);
}

void eliminarPersonaje(TABB *arbol) { // No necesito llamar a buscar nodo

  if (!esAbbVacio(*arbol)) {
    TIPOELEMENTOABB elemento, aux;
    printf("Introduce el nombre del personaje a eliminar: ");
    scanf(" %[^\n]s", aux.name);
    printf("ESTO ES UNA PRUEBA: %s\n", aux.name);
    buscarNodoAbb(*arbol, aux.name, &elemento);
    if (esMiembroAbb(*arbol, elemento)) {
      printf("Eliminando a %s de la base de datos.\n", elemento.name);
      suprimirElementoAbb(arbol, elemento);
    } else {
      printf("El personaje %s no forma parte de la base de datos.\n", aux.name);
    }
  }
}
void _strip_line(char *linea) { linea[strcspn(linea, "\r\n")] = 0; }

void _split_ByComa(char *cadena, TLISTA *lista) {
  char *start = cadena;
  char *end = cadena;
  TIPOELEMENTOLISTA elemento;
  while (*end != '\0') {
    if (*end == ',') {
      *end = '\0';
      strncpy(elemento.name, start, MAX);
      insertarElementoLista(lista, finLista(*lista), elemento);
      start = end + 1;
    }
    end++;
  }
}

void cargar_archivo(TABB *arbol, int argc, char **argv) {
  if (argc < 3 ||
      (strcmp(argv[1], "-f") != 0 && strcmp(argv[1], "--file") != 0)) {
    printf("Error al llamar a la función\n");
    return;
  }
  FILE *file = fopen(argv[2], "r");
  if (file == NULL) {
    printf("El archivo no se pudo abrir\n");
    return;
  }

  TIPOELEMENTOABB elemento;
  char aux[MAX_LINE];
  TIPOELEMENTOLISTA caux;

  crearLista(&elemento.siblings);
  crearLista(&elemento.parents);
  crearLista(&elemento.killed);

  fgets(aux, MAX_LINE, file);
  _strip_line(aux);

  char *name = strtok(aux, "|");
  char *house = strtok(NULL, "|");
  char *royal = strtok(NULL, "|");
  char *parents_list = strtok(NULL, "|");
  char *siblings_list = strtok(NULL, "|");
  char *killed_list = strtok(NULL, "|");
  strncpy(elemento.name, name, MAX);
  strncpy(elemento.house, house, MAX);
  elemento.royal = atoi(royal);
  _split_ByComa(siblings_list, &elemento.siblings);
  _split_ByComa(parents_list, &elemento.parents);
  _split_ByComa(killed_list, &elemento.killed);
  insertarElementoAbb(arbol, elemento);
}
