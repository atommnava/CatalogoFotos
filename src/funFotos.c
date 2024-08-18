/*file funFotos.c
 * @brief Este programa contiene las funciones para el programa principal.
 * 
 *  @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez
 *
 **/

#include "deftipo.h" // Libreria

/**
 *  @brief Inserta una nueva categoría en una LDC de categorias.
 *
 *  Esta funcion crea un nuevo nodo de categoria con la informacion proporcionada y lo inserta en orden alfabético dentro de la lista circular doblemente enlazada
 *  Si la lista esta vacía, el nuevo nodo se convierte en el unico nodo de la lista.
 *
 *  @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 *  @param inicio Pdoble puntero que apunta al inicio de la lista de las 
categorias.
 *  @param datos Estructura que contiene los datos de la categoria.
 * 
 */

extern void insertarCategoria(datCategoria **inicio, datCategoria datos, nodo **raiz) 
{
    // Verificar si la categoría ya existe
    if (buscarCategoria(*inicio, datos.nombre) != NULL) 
    {
        printf("La categoría %s ya existe.\n", datos.nombre);
        return;
    }
    // Crear el nuevo nodo de categoría
    datCategoria *nuevo = (datCategoria*)malloc(sizeof(datCategoria));
    if (!nuevo) 
    {
        printf("Error al asignar memoria\n");
        return;
    }
    // Copiar los datos proporcionados al nuevo nodo
    strcpy(nuevo->nombre, datos.nombre);
    nuevo->numFotos = 0;
    nuevo->inicioFoto = nuevo->finFoto = NULL;

    if (*inicio == NULL) 
    {
        nuevo->izq = nuevo->der = nuevo;
        *inicio = nuevo;
    } 
    else 
    {
        datCategoria *actual = *inicio;
        do {
            if (strcmp(datos.nombre, actual->nombre) < 0) 
            {
                // Insertar antes de 'actual'
                nuevo->der = actual;
                nuevo->izq = actual->izq;
                actual->izq->der = nuevo;
                actual->izq = nuevo;
                if (actual == *inicio) 
                {
                    *inicio = nuevo; // Actualizar inicio si es el menor
                }
                return;
            }
            actual = actual->der;
        } while (actual != *inicio);

        // Si no se insertó en el ciclo, se inserta al final (antes del inicio)
        nuevo->der = *inicio;
        nuevo->izq = (*inicio)->izq;
        (*inicio)->izq->der = nuevo;
        (*inicio)->izq = nuevo;
    }
    printf("\nCategoria Creada\n");
    // Inserción de la clave única de la primera foto de la categoría en el árbol binario
if (datos.numFotos > 0 && datos.inicioFoto != NULL) 
{
    *raiz = insertarNodArbol(*raiz, *datos.inicioFoto, &datos);
}
  return;
}
/**
 * @brief Inserta una nueva foto en una LCS de fotos a una categoria.
 *
 *  Esta función crea un nuevo nodo de foto con la información 
proporcionada y lo inserta en orden ascendente según la clave única
 *  dentro de la LCS, Si la lista esta vacia, el nuevo nodo se convierte 
en el unico nodo de la lista.
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param categoria Puntero de la categoría a la que se agregara la nueva 
foto.
 * @param datos Estructura que contiene los datos de la nueva foto a 
insertar.
 * 
 */
extern void insertarFoto(datCategoria *categoria, datFoto datos, nodo **raiz) 
{
    datFoto *nuevo;
    datFoto *prev;
    datFoto *actual;

    nuevo = (datFoto*)malloc(sizeof(datFoto));
    if (!nuevo) 
    {
        printf("Error al asignar memoria\n");
        return;
    }
    nuevo->claveUnica = datos.claveUnica;
    strcpy(nuevo->descripcion, datos.descripcion);
    strcpy(nuevo->archivo, datos.archivo);
    nuevo->baja = datos.baja;
    nuevo->ant = NULL;

    if (buscarFotoPorClaveUnica(categoria, datos.claveUnica) != NULL) 
    {
        printf("La clave única %d ya existe en la categoría.\n", datos.claveUnica);
        free(nuevo);
        return;
    }

    if (categoria->inicioFoto == NULL) 
    {
        nuevo->sig = nuevo;
        categoria->inicioFoto = categoria->finFoto = nuevo;
    } else 
    {
        actual = categoria->inicioFoto;
        do 
        {
            if (datos.claveUnica < actual->claveUnica) 
            {
                // Insertar antes de 'actual'
                nuevo->sig = actual;
                nuevo->ant = actual->ant;
                actual->ant = nuevo;
                if (actual == categoria->inicioFoto) 
                {
                    categoria->inicioFoto = nuevo;
                } else 
                {
                    prev->sig = nuevo;
                }
                categoria->numFotos++; // Incrementar el contador de fotos
                return;
            }
            prev = actual;
            actual = actual->sig;
        } while (actual != categoria->inicioFoto);

        // Si no se insertó en el ciclo, se inserta al final
        prev->sig = nuevo;
        nuevo->sig = categoria->inicioFoto;
        nuevo->ant = prev;
        categoria->finFoto = nuevo;
    }
    categoria->numFotos++;

    // Inserción de la clave única de la nueva foto en el árbol binario
    *raiz = insertarNodArbol(*raiz, datos, categoria);
}

/**
 * @brief Elimina una categoria junto con todas sus fotos de la galeria.
 *
 * Esta funcion elimina la categoría con el nombre especificado del inicio 
de la lista circular doble, junto con todas las fotos asociadas a esa 
categoría
 * Si la categoria a eliminar es la única categoría en la galeria, la 
lista circular doble se establece en NULL.
 * 
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param inicio Doble puntero al inicio de la lista circular doble que 
contiene las categorías.
 * @param nombre Nombre de la categoría que se desea eliminar.
 */
extern void eliminarCategoria(datCategoria **inicio, char *nombre) 
{
    datCategoria *actual, *anterior;
    datFoto *fotoActual, *temp;
    actual = *inicio;
    anterior = NULL;

    if (*inicio == NULL) return;
    
    do {
        if (strcmp(actual->nombre, nombre) == 0) 
        {
            // Eliminar todas las fotos en la categoría
            fotoActual = actual->inicioFoto;
            while (fotoActual != actual->finFoto) 
            {
                temp = fotoActual;
                fotoActual = fotoActual->sig;
                free(temp);
            }
            free(actual->finFoto);

            // Eliminar la categoría
            if (actual->izq == actual) 
            {
                *inicio = NULL;
            } 
            else 
            {
                actual->izq->der = actual->der;
                actual->der->izq = actual->izq;
                if (*inicio == actual) *inicio = actual->der; // Cambiar inicio si necesario
            }
            free(actual);
            return;
        }
        actual = actual->der;
    } while (actual != *inicio);

  return;
}
/**
 * @brief Elimina una foto de una categoria en la galeria.
 *
 * Esta función elimina una foto con la clave única especificada de la 
lista de fotos asociada a una categoría en particular. Si la foto a 
eliminar es la única en la lista,
 * la lista se establece en NULL.
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param categoria Puntero de la categoria a la que pertenece la foto.
 * @param claveUnica Clave unica de la foto que se desea eliminar.
 */
extern void eliminarFoto(datCategoria *categoria, int claveUnica) 
{

    datFoto *actual, *anterior;

    if (categoria == NULL || categoria->inicioFoto == NULL) return;

    actual = categoria->inicioFoto;
    anterior = categoria->finFoto;
    do {
        if (actual->claveUnica == claveUnica) 
        {
            if (actual == categoria->inicioFoto && actual == categoria->finFoto) 
            {
                categoria->inicioFoto = categoria->finFoto = NULL;
            } 
            else 
            {
                anterior->sig = actual->sig;
                if (actual == categoria->inicioFoto) categoria->inicioFoto = actual->sig;
                if (actual == categoria->finFoto) categoria->finFoto = anterior;
            }
            free(actual);
            categoria->numFotos--;
            return;
        }
        anterior = actual;
        actual = actual->sig;
    } while (actual != categoria->inicioFoto);

  return;
}
/**
 * @brief Modifica los datos de una foto en una categoria de la galeria.
 *
 * Esta funcion modifica los datos de una foto en la lista de fotos 
asociada a una categoria específica,
 * La modificación se realiza mediante la actualización de la descripción, 
el nombre del archivo y el estado de baja de la foto con la clave única 
especificada.
 * 
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param categoria Puntero a la categoria a la que pertenece la foto.
 * @param datos Estructura de datos que contiene la informacion de la 
foto.
 * 
 */
extern void modificarFoto(datCategoria *categoria, datFoto datos) 
{

    datFoto *actual;

    if (categoria == NULL) return;

    actual = categoria->inicioFoto;
    do {
        if (actual->claveUnica == datos.claveUnica) 
        {
            strcpy(actual->descripcion, datos.descripcion);
            strcpy(actual->archivo, datos.archivo);
            actual->baja = datos.baja;
            return;
        }
        actual = actual->sig;
    } while (actual != categoria->inicioFoto);

  return;
}
/**
 * @brief Imprime las categorías de la galería junto con el número de 
fotos en cada una.
 *
 * Esta funcion imprime en la consola la lista de categorias de la 
galeria, mostrando el nombre de cada categoria y el numero de fotos que 
contiene,
 * Si no hay categorias disponibles, muestra un mensaje indicando que no 
hay categorias.
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * 
 * @param inicio Puntero al inicio de la lista de categorías.
 */
extern void imprimirCategorias(datCategoria *inicio) 
{

    datCategoria * actual;

    if (inicio == NULL) 
    {
        printf("No hay categorías disponibles.\n");
        return;
    }
    actual = inicio;
    do 
    {
        printf("Categoría: %s, Número de Fotos: %d\n\n", actual->nombre, actual->numFotos);
        actual = actual->der;
    } while (actual != inicio);

  return;
}
/**
 * @brief Imprime las fotos de una categoria con su informacion.
 *
 * Esta funcion imprime en la consola la lista de fotos de una categoria 
especifica, mostrando el ID de cada foto, su descripcion,
 * el nombre del archivo asociado y su estado (activo o dado de baja), Si 
no hay fotos en la categoria o no, muestra un mensaje
 * indicando que no hay fotos disponibles.
 * 
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 *
 * @param categoria Puntero de la categoria de la cual se desean imprimir las fotos.
 */
extern void imprimirFotos(datCategoria *categoria) 
{

    datFoto *actual;

    if (categoria == NULL || categoria->inicioFoto == NULL) 
    {
        printf("\nNo hay fotos en esta categoría.\n");
        return;
    }
    actual = categoria->inicioFoto;
    do 
    {
        printf("\nFoto ID: %d, Descripción: %s, Archivo: %s, Estado: %s\n\n", actual->claveUnica, actual->descripcion, actual->archivo, actual->baja ? "Dada de baja" : "Activa");
        actual = actual->sig;
    } while (actual != categoria->inicioFoto);

  return;
}
/**
 * @brief Carga los datos de categorias y fotos desde un archivo binario.
 *
 * Esta funcion carga los datos de categorias y sus respectivas fotos 
desde un archivo binario
 * proporcionado, Utiliza el nombre del archivo binario y un puntero al 
inicio de la lista de
 * categorias, Si no se puede abrir el archivo, muestra un mensaje de 
error.
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param inicio Doble puntero del inicio de la lista de categorias.
 * @param nombreArchivo Nombre del archivo binario que contiene los datos.
 */
extern void cargarDesdeArchivoBinario(datCategoria **inicio, nodo **raiz, char *nombreArchivo)
{

    datCategoria categoriaTemp;
    datFoto fotoTemp;

    FILE *file = fopen(nombreArchivo, "rb");
    if (file == NULL) 
    {
        printf("\nError al abrir el archivo %s para lectura.\n", nombreArchivo);
        return;
    }

    while (fread(&categoriaTemp, sizeof(datCategoria), 1, file) == 1) 
    {
        categoriaTemp.inicioFoto = categoriaTemp.finFoto = NULL; // Limpiar punteros
       insertarCategoria(inicio, categoriaTemp, raiz);

        for (int i = 0; i < categoriaTemp.numFotos; i++) 
        {
            if (fread(&fotoTemp, sizeof(datFoto), 1, file) == 1) 
            {
                insertarFoto(buscarCategoria(*inicio, categoriaTemp.nombre), fotoTemp, raiz);
            }
            else 
            {
                printf("\nError al leer la foto desde el archivo.\n");
                break;
            }
        }
    }
    fclose(file);

    return;
}
/**
 * @brief Guarda los datos de categorias y fotos en un archivo binario.
 *
 * Esta función guarda los datos de categorias y sus respectivas fotos en un archivo binario
 * especificado, Utiliza el nombre del archivo binario y un puntero al inicio de la lista decategorias 
 * Si no hay categorias para guardar o si no se puede abrir el archivo para escritura,
 * muestra un mensaje de error, Después de guardar los datos, cierra el archivo.
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param inicio Puntero del inicio de la lista de categorias.
 * @param nombreArchivo Nombre del archivo binario donde se guardaran los datos.
 */
extern void guardarEnArchivoBinario(datCategoria *inicio, char *nombreArchivo) 
{

    datCategoria *actual = NULL;
    FILE *file;
    datFoto *fotoActual;
    actual = inicio;

    if (inicio == NULL) 
    {
        printf("\nNo hay categorías para guardar.\n");
        return;
    }

    file = fopen(nombreArchivo, "wb");
    if (file == NULL) 
    {
        printf("\nError al abrir el archivo %s para escritura.\n", nombreArchivo);
        return;
    }
    do 
    {
        fwrite(actual, sizeof(datCategoria), 1, file);
        fotoActual = actual->inicioFoto;
        if (fotoActual) 
        {
            do 
            {
                fwrite(fotoActual, sizeof(datFoto), 1, file);
                fotoActual = fotoActual->sig;
            } while (fotoActual != actual->inicioFoto);
        }
        actual = actual->der;
    } while (actual != inicio);

    fclose(file);
    printf("Datos guardados correctamente en %s.\n", nombreArchivo);

    return;
}
/**
 * @brief Busca una categoria en la lista de categorias por su nombre.
 *
 * Esta funcion busca una categoria en la lista de categorias por su nombre, Toma como
 * parametros un puntero al inicio de la lista de categorias y el nombre de la categoria
 * que se desea buscar, Si la categoria se encuentra, devuelve un puntero a la estructura
 * de datos de esa categoria. Si la categoria no se encuentra o si la lista de categorias
 * está vacia, devuelve NULL.
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param inicio Puntero al inicio de la lista de categorias.
 * @param nombreCategoria Nombre de la categoría que se desea buscar.
 * @return Puntero a la estructura de datos de la categoría si se 
encuentra, NULL de lo contrario.
 */
datCategoria* buscarCategoria(datCategoria *inicio, char *nombreCategoria) 
{

    datCategoria *actual = NULL;

    if (inicio == NULL) 
    {
        return NULL;
    }
    actual = inicio;
    do {
        if (strcmp(actual->nombre, nombreCategoria) == 0) 
        {
            return actual;
        }
        actual = actual->der;
    } while (actual != inicio);

    return NULL;  // No se encontró la categoría
}
/**
 * @brief Imprime el contenido de un archivo binario que almacena datos de 
categorías y fotos.
 *
 * Esta funcion abre un archivo binario en modo lectura y lee su contenido, que debe ser una serie de datos de categorías seguidos de datos de fotos. Para cada categoría, imprime su nombre
 * y el número de fotos que contiene. Luego, para cada foto dentro de la categoría, imprime su ID, descripción,
 * nombre de archivo y estado. Si ocurre algún error durante la lectura del archivo, muestra un mensaje de error correspondiente.
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param nombreArchivo Nombre del archivo binario que se desea imprimir.
 */
extern void imprimirArchivoBinario(char *nombreArchivo) 
{
    int i;
    FILE *file = fopen(nombreArchivo, "rb");
    datCategoria categoria;
    datFoto foto;

    if (file == NULL) 
    {
        printf("Error al abrir el archivo %s para lectura.\n", nombreArchivo);
        return;
    }

    // Leer categorías una por una
    while (fread(&categoria, sizeof(datCategoria), 1, file) == 1) 
    {
        // Imprimir detalles de la categoría
        printf("\nCategoría: %s, Número de Fotos: %d\n", categoria.nombre, categoria.numFotos);

        // Leer fotos dentro de cada categoría
        for (i = 0; i < categoria.numFotos; i++) 
        {
            if (fread(&foto, sizeof(datFoto), 1, file) == 1) 
            {
                // Imprimir detalles de cada foto
                printf("\nFoto ID: %d, Descripción: %s, Archivo: %s, Estado: %d\n", foto.claveUnica, foto.descripcion, foto.archivo, foto.baja);
            } else 
            {
                printf("\nError al leer la foto desde el archivo.\n");
                break;
            }
        }
    }
    fclose(file);

    return;
}
/**
 * @brief Solicita al usuario si desea guardar los datos en un archivo 
binario y realiza la respuesta.
 *
 * Esta funcion muestra un mensaje pidiendo al usuario que confirme si desea guardar los datos en un archivo binario,
 * Si la respuesta es "si", solicita al usuario que ingrese el nombre del archivo binario y llama a la función
 * "guardarEnArchivoBinario" para guardar los datos en dicho archivo, Si la respuesta es "no", muestra un mensaje
 * indicando que los datos no se guardaron. Si la respuesta no es válida, muestra un mensaje de error.
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param inicio Puntero del inicio de la lista de categorías.
 */
extern void guardarDatosBin(datCategoria *inicio) 
{
    char respuesta[50];

    printf("\n¿Desea guardar los datos en un archivo binario? (si o no): ");
    scanf(" %s", respuesta);
    if (strcmp(respuesta, "si") == 0) 
    {
        char nombreArchivo[100];
        printf("\nIngrese el nombre del archivo binario: ");
        scanf(" %[^\n]", nombreArchivo);
        guardarEnArchivoBinario(inicio, nombreArchivo);
        printf("\nDatos guardados correctamente en %s.\n", nombreArchivo);
    }
    else if (strcmp(respuesta, "no") == 0) 
    {
        printf("\nNo se guardaron los datos.\n");
    }
    else 
    {
        printf("\nRespuesta no valida.\n");
    }

    return;
}
/**
 * @brief Busca una foto en una categoria por su clave unica.
 *
 * Esta funcion busca una foto en la lista de fotos de una categoria por su clave unica.
 * Si la foto se encuentra, devuelve un puntero a la estructura de datos de esa foto.
 * Si la foto no se encuentra o si la lista de fotos está vacia, devuelve NULL.
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param categoria Puntero a la categoria en la que se realizara la busqueda.
 * @param claveUnica Clave única de la foto que se desea buscar.
 * @return Puntero de la estructura de datos de la foto si se encuentra NULL.
 */
datFoto* buscarFotoPorClaveUnica(datCategoria *categoria, int claveUnica) 
{
    
    datFoto *actual;

    if (categoria == NULL || categoria->inicioFoto == NULL) 
    {
        return NULL;
    }
    actual = categoria->inicioFoto;
    do 
    {
        if (actual->claveUnica == claveUnica) 
        {
            return actual;
        }
        actual = actual->sig;
    } while (actual != categoria->inicioFoto);

    return NULL;  // No se encontró la foto
}
/**
 * @brief Navegador de fotos.
 *
 * Esta función permite navegar por las fotos de una categoría, mostrando la descripción y el archivo de cada foto. Permite avanzar a la siguiente foto, retroceder a la anterior,
 * cambiar de categoría o salir de la navegación.
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param inicioFoto Puntero del inicio de la lista de fotos de la categoria.
 * @param nombreCategoria Nombre de la categoria en la que se están navegando las fotos.
 * @param numFotosCategoria Numero total de fotos en la categoria.
 * @param categoria Puntero a la estructura de datos de la categoria.
 */
extern void navegadorFotos(datFoto *inicioFoto, char *nombreCategoria, int numFotosCategoria, datCategoria *categoria) 
{
    char opcion[50];
    int fotoActual = 1; // Inicializamos la foto actual como la primera
    datFoto *actual = inicioFoto;

    if (inicioFoto == NULL) 
    {
        printf("No hay fotos disponibles en la categoría %s.\n", nombreCategoria);
        return;
    }
    
    do 
    {
        printf("\n<< Categoría: %s >>\n", nombreCategoria);
        printf("<< Foto %d de %d >>\n", fotoActual, numFotosCategoria); // Muestra el número de la foto actual y el total de fotos en la categoría
        printf("\nDescripción: %s\n", actual->descripcion);
        printf("Archivo: %s\n", actual->archivo);

        printf("\n<< Opciones >>\n");
        printf("\nSiguiente Foto (>), Foto Anterior (<), Cambiar de Categoría (categoria), Salir de la Navegación (salir): ");
        printf("\n\nIngresa tu opción: ");
        scanf(" %s", opcion);

        if (strcmp(opcion, ">") == 0) 
        {
            if (actual->sig != categoria->inicioFoto) 
            {
                actual = actual->sig;
                fotoActual++;
            } else 
            {
                system("clear");
                printf("Ya estás en la última foto de esta categoría.\n");
                printf("\nPresiona Enter para continuar ");
                getchar();
                getchar();
                system("clear");
            }
        }
        else if (strcmp(opcion, "<") == 0) 
        {
            if (fotoActual > 1) 
            {
                actual = actual->ant;
                fotoActual--;
            } 
            else 
            {
                system("clear");
                printf("Ya estás en la primera foto de esta categoría.\n");
                printf("\nPresiona Enter para continuar ");
                getchar();
                getchar();
                system("clear");
            }
        }
        else if (strcmp(opcion, "categoria") == 0) 
        {
            break; // Salir del bucle y permitir cambiar de categoria
        }
        else if (strcmp(opcion, "salir") == 0) 
        {
            system("clear");
            printf("Saliendo del navegador de fotos...\n");
            printf("\nPresiona enter para continuar ");
            getchar();
            getchar();
            system("clear");
            break;
        }
        else 
        {
            system("clear");
            printf("Opción no válida. Por favor, ingresa una opción válida.\n");
            printf("Presiona Enter para continuar ");
            getchar();
            getchar();
        }
    } while (1);
}
/**
 * @brief Navegador de categorias y fotos.
 *
 * Esta funcion permite navegar por las categorias y las fotos de cada categoria, mostrando las opciones disponibles y permitiendo al usuario seleccionar una categoría
 * para ver sus fotos o moverse entre las categorias disponibles.
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param inicio Puntero del inicio de la lista de categorias.
 */
void navegadorCategoriasFotos(datCategoria *inicio) 
{
    char opcion[50];
    char nombreCategoria[50];

    if (inicio == NULL) 
    {
        printf("No hay categorías disponibles.\n");
        return;
    }

    do 
    {
        printf("\n<< Opciones >>\n");
        printf("\n- Navegar por las categorías (>)");
        printf("\n- Navegar por las fotos (<)");
        printf("\n- Salir (salir)");
        printf("\n\nIngresa tu opción: ");
        scanf(" %s", opcion);

        if (strcmp(opcion, ">") == 0) 
        {
            // Navegar por las categorías
            datCategoria *actual = inicio;

            do 
            {
                system("clear");
                printf("\nCategoría: %s\n", actual->nombre);
                printf("Número de fotos: %d\n", actual->numFotos);
                printf("\n<< Opciones de categorías >>\n");
                printf("\n- ¿Desea revisar las fotos de esta categoría? (si o no)");
                printf("\n- Salir de la navegación de categorías (salir)");
                printf("\n\nIngresa tu opción: ");
                scanf(" %s", opcion);

                if (strcmp(opcion, "si") == 0) 
                {
                    system("clear");
                    printf("\ncategoria: %s\n", actual->nombre);
                    imprimirFotos(actual);
                }
                else if (strcmp(opcion, "salir") == 0) 
                {
                    system("clear");
                    printf("Saliendo de la navegación de categorías...\n");
                    break;
                }

                printf("\n¿Desea avanzar (>) o retroceder (<) en las categorías, o Salir de la navegación (salir)?: ");
                scanf(" %s", opcion);

                if (strcmp(opcion, ">") == 0) 
                {
                    actual = actual->der;
                } 
                else if (strcmp(opcion, "<") == 0) 
                {
                    actual = actual->izq;
                }
                else if (strcmp(opcion, "salir") == 0) 
                {
                    printf("Saliendo del navegador de categorías...\n");
                    printf("\nPresiona enter para continuar ");
                    getchar();
                    getchar();
                    system("clear");
                    break;
                }
            } while (strcmp(opcion, "no") != 0);
        }
else if (strcmp(opcion, "<") == 0) 
{
    // Navegar por las fotos
    system("clear");
    printf("\n<< Categorias >>\n\n");
    imprimirCategorias(inicio);
    printf("\nIngresa el nombre de la categoría: ");
    scanf(" %s", nombreCategoria);

    datCategoria *categoriaSeleccionada = buscarCategoria(inicio, nombreCategoria);
    if (categoriaSeleccionada != NULL) 
    {
        system("clear");
        printf("\nNavegando por las fotos de la categoría << %s >>...\n", nombreCategoria);
        navegadorFotos(categoriaSeleccionada->inicioFoto, categoriaSeleccionada->nombre, categoriaSeleccionada->numFotos, categoriaSeleccionada);

    } 
    else 
    {
        printf("\nLa categoría seleccionada no existe.\n");
    }
}
        else if (strcmp(opcion, "salir") == 0) 
        {
            system("clear");
            printf("\nSaliendo del navegador...\n");
            break;
        }
        else 
        {
            printf("Opción no válida. Por favor, ingresa una opción válida.\n");
        }
    } while (strcmp(opcion, "salir") != 0);
}
/**
 * @brief Inserta un nodo en un árbol binario.
 *
 * Esta Funcion inserta un nuevo nodo en un arbol binario basado en su clave unica, Si el arbol estasta vacio, el nuevo nodo se convierte en la raiz del arbol.
 *
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param pt Puntero de la raiz del arbol.
 * @param datos Estructura que contiene los datos de la foto a insertar.
 * @param categoria Puntero a la categoria a la que pertenece la foto.
 * @return pt Puntero a la raiz del árbol actualizada.
 */
extern nodo* insertarNodArbol(nodo* pt, datFoto datos, datCategoria *categoria) 
{
    // Declaración de variables al inicio de la función
    nodo *nuevo, *aux;
    aux = pt;
    int salir = 1;

    // Asignación de memoria para el nuevo nodo
    nuevo = (nodo *)malloc(sizeof(nodo));
    if (nuevo == NULL) {
        printf("\nNo hay memoria disponible.");
        exit(1);
    }

    // Inicializar los datos del nuevo nodo
    nuevo->datos = datos;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    nuevo->categoria = categoria; // Asignar la categoria al nuevo nodo

    // Si el arbol está vacio
    if (aux == NULL) 
    {
        pt = nuevo;
        return pt;
    }

    // Bucle para insertar el nuevo nodo en la posición correcta del árbol
    while (aux != NULL && salir != 0) {
        if (datos.claveUnica > aux->datos.claveUnica)
         {
            // Mover a la derecha
            if (aux->der != NULL) 
            {
                aux = aux->der;
            } 
            else 
            {
                aux->der = nuevo;
                salir = 0;
            }
        } 
        else 
        {
            // Mover a la izquierda
            if (aux->izq != NULL) 
            {
                aux = aux->izq;
            } 
            else 
            {
                aux->izq = nuevo;
                salir = 0;
            }
        }
    }

    // Retornar la raiz del arbol actualizada
    return pt;
}
/**
 * @brief Imprime el arbol binario en orden.
 *
 * Esta funcion recorre recursivamente el arbol binario en orden y muestra los datos de la foto y la categoria de cada nodo.
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param aux Puntero a un nodo del arbol.
 */
void imprimirArbol(nodo *aux)
 {
    if (aux != NULL) 
    {
        // Recorrer el izquierdo
        imprimirArbol(aux->izq);
        
        // Imprimir los datos de la foto y la categoría
        printf("Categoría: %s, Foto ID: %d, Descripción: %s, Archivo: %s, Estado: %s\n", aux->categoria->nombre, aux->datos.claveUnica, aux->datos.descripcion, aux->datos.archivo, aux->datos.baja ? "Dada de baja" : "Activa");
        
        // Recorrer el derecho
        imprimirArbol(aux->der);
    }
}
/**
 * @brief Busca un nodo en un arbol binario por su clave unica.
 *
 * Esta funcion busca un nodo en un arbol binario utilizando su clave unica, Recorre el arbol comparando la clave unica del nodo actual con la clave buscada.
 *
 * @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez.
 * 
 * @param aux Puntero a un nodo del arbol a partir del cual iniciar la busqueda.
 * @param busca La clave unica que se quiere buscar.
 * @return 1 si se encontro la clave única, 0 si no se encontró.
 */
extern int buscarDirecto(nodo *aux, int busca)
{
    int flag = 0;

    while (aux != NULL && flag == 0)
    {
        if (aux->datos.claveUnica < busca)
        {
            // Si la clave del nodo actual es menor, moverse a la derecha.
            aux = aux->der;
        }
        else if (aux->datos.claveUnica > busca)
        {
            // Si la clave del nodo actual es mayor, moverse a la izquierda.
            aux = aux->izq;
        }
        else
        {
            // Clave unica encontrada
            flag = 1;
        }
    }
    
    // Devuelve el valor del flag (1 si se encontró la clave, 0 si no).
    return flag;
}










