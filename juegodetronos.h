#ifndef JUEGODETRONOS_H
#define JUEGODETRONOS_H

#include "abb.h"
#include "lista.h"

void anhadirPersonaje(TABB *arbol);
void listarPersonajes(TABB arbol);
void eliminarPersonaje(TABB *arbol);
void cargar_archivo(TABB *arbol, int argc, char **argv);

#endif // LISTA_H
