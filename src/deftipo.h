#include <stdio.h> // Libreria 
#include <stdlib.h> // Libreria
#include <string.h> // Libreria

/**
 * @file deftipo.h
 * @brief Este programa contiene los prototipos de funciones para el 
programa principal y las estructuras de datos de las fotos y categorias.
 * 
 *  @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 *
 **/

// Definición de las estructuras

typedef struct Foto // Estructura de las fotos
{
    int claveUnica;
    char descripcion[200];
    char archivo[50];
    int baja;
    struct Foto *sig, *ant;
} datFoto;

typedef struct categoria // Estructura de las categorias
{
    char nombre[50];
    int numFotos;
    datFoto *inicioFoto, *finFoto;
    struct categoria *izq, *der;
} datCategoria;

typedef struct nodo // Estructura para el arbol binario
{
    datFoto datos;
    struct nodo *izq, *der;
    datCategoria *categoria; // Campo para la categoría de la foto
} nodo;

// Prototipos de funciones
void insertarCategoria(datCategoria **inicio, datCategoria datos, nodo **raiz);
extern void insertarFoto(datCategoria *categoria, datFoto datos, nodo **raiz);
void eliminarCategoria(datCategoria **inicio, char *nombre);
void eliminarFoto(datCategoria *categoria, int claveUnica);
void modificarFoto(datCategoria *categoria, datFoto datos);
void imprimirCategorias(datCategoria *inicio);
void imprimirFotos(datCategoria *categoria);
void cargarDesdeArchivoBinario(datCategoria **inicio, nodo **raiz, char *nombreArchivo);
void guardarEnArchivoBinario(datCategoria *inicio, char *nombreArchivo);
void imprimirArchivoBinario(char *nombreArchivo);
void guardarDatosBin(datCategoria *inicio);
void navegadorCategoriasFotos(datCategoria *inicio);
void navegadorFotos(datFoto *inicioFoto, char *nombreCategoria, int numFotosCategoria, datCategoria *categoria);
void imprimirArbol(nodo *aux);
int buscarDirecto(nodo *aux, int claveUnica);
nodo* insertarNodArbol(nodo* pt, datFoto datos, datCategoria *categoria);
datCategoria *buscarCategoria(datCategoria *inicio, char *nombreCategoria);
datFoto* buscarFotoPorClaveUnica(datCategoria *categoria, int claveUnica);