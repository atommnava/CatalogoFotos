#include "deftipo.h" // Libreria

/**
 * @file catalogoFotos.c
 * @brief Este programa nos permite crear una galeria de fotos con 
multiples funcionalidades.
 *  
 *        A) Permite cargar categorías y fotos desde un archivo binario 
proporcionado en la línea de comandos o seleccionando desde el menu.
 *
 *        B) Respliega un menu de opciones para realizar las siguientes 
acciones.
 *           1. Insertar una nueva categoría.
 *           2. Insertar una nueva foto en una categoría existente.
 *           3. Eliminar una categoría.
 *           4. Eliminar una foto de una categoría.
 *           5. Modificar los detalles de una foto.
 *           6. Imprimir todas las categorías disponibles.
 *           7. Imprimir todas las fotos de una categoría específica.
 *           8. Guardar los datos en un archivo binario.
 *           9. Imprimir el contenido de un archivo binario.
 *          10. Navegar por Fotos y Categoria   
 *          11. Buscar por Clave Unica
 *           0. Salir del programa.
 *
 *        C) Al salir del programa, podemos guardar los datos en un archivo binario si el usuario lo requiere o no.
 *
 *        D) El programa nos da opciones de eliminar catalogos o fotos y poder modificar fotos.
 * 
 *        E) El programa permite navegar entre catalogos o fotos.
 * 
 *  @author Equipo de Muñoz Nava, Ruiz Hellemann, Roldan Marquez
 *
 *
 **/

int main(int argc, char *argv[]) // Funcion principal 
{
    // Declaracion de variables

    datCategoria *inicio = NULL;
    datCategoria *cat = NULL;
    datCategoria *actual = NULL;
    char nombreArchivo[100];
    char nombreCategoria[50];
    char opcionmove3[50], opcionmove2[50];
    char opcionmove4[50];
    int opcionmove;
    int opcionmoves;
    datFoto datosFoto;
    datCategoria datosCategoria;
    int opcion;
    int opcion2;
    char opcion3[50];
    int found;
    nodo *raiz;
    nodo *aux;
    FILE *fp;
    raiz = NULL;
    actual = inicio;
    int busca;
    int val;

    // Cargar desde un archivo binario si se proporciona en la línea de comandos
    if (argc > 1) 
    {
        strcpy(nombreArchivo, argv[1]);
        cargarDesdeArchivoBinario(&inicio, &raiz, nombreArchivo);

        // Cargar datos desde el archivo especificado
        fp = fopen(nombreArchivo, "r");
        if (fp == NULL) 
        {
            printf("\nError: Archivo no disponible.\n");
            exit(1);
        }
        
        // Leer datos del archivo e insertarlos en el arbol de categorías
        while (fscanf(fp, "%d\t%s\t%s\t%d\n", &datosFoto.claveUnica, datosFoto.descripcion, datosFoto.archivo, &datosFoto.baja) == 4) 
        {
            // Inserta los datos en el arbol de categorías
            raiz = insertarNodArbol(raiz, datosFoto, &datosCategoria);

        }
        // Cerrar el archivo después de leer los datos
        fclose(fp);

        printf("\nCategorias cargadas desde el archivo binario.\n\n");
        imprimirCategorias(inicio);

        printf("Ingrese el nombre de la categoría para imprimir sus fotos: ");
        scanf(" %s", nombreCategoria);
        imprimirFotos(buscarCategoria(inicio, nombreCategoria));

        printf("Presiona enter para continuar ");
        getchar();
        getchar();

        printf("\nCategorias cargadas desde el arbol binario.\n\n");
        imprimirArbol(raiz);

        printf("\n¿Clave de la FOTO que quieres buscar?: ");
        scanf("%d", &busca);
        val = buscarDirecto(raiz, busca);

        // Comprobar el resultado
        if (val == 1) 
        {
            printf("\nEl dato existe\n");
            printf("\nClave unica %d encontrada.\n", busca);
            printf("\nCategoría: %s, Foto ID: %d, Descripción: %s, Archivo: %s, Estado: %s\n", raiz->categoria->nombre, raiz->datos.claveUnica, raiz->datos.descripcion, raiz->datos.archivo, raiz->datos.baja ? "Dada de baja" : "Activa");
        } 
        else 
        {
            printf("\nClave unica %d no encontrada.\n", busca);
        }

        printf("\nPresiona enter para continuar ");
        getchar();
        getchar();
        system("clear");
    }

    // Bucle principal del menú
    do {
        system("clear");
        printf("\n << GALERIA DE FOTOS >> \n");
        printf("\nMenú de Opciones:\n\n");
        printf("1. Insertar Categoría\n");
        printf("2. Insertar Foto\n");
        printf("3. Eliminar Categoría\n");
        printf("4. Eliminar Foto\n");
        printf("5. Modificar Foto\n");
        printf("6. Imprimir Categorías\n");
        printf("7. Imprimir Fotos de una Categoría\n");
        printf("8. Guardar en Archivo Binario\n");
        printf("9. Imprimir Archivo Binario\n");
	    printf("10. Navegar por Fotos y Categorias \n");
        printf("11. Buscar Por Clave Unica\n");
        printf("0. Salir\n");
        printf("\nSeleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) // Opciones del menu para ejecutar
        {
            case 1:
                system("clear");
                printf("Ingrese el nombre de la nueva categoría: ");
                scanf(" %s", datosCategoria.nombre);
                system("clear");
                insertarCategoria(&inicio, datosCategoria, &raiz);
		actual = inicio;
                printf("\nPresiona enter para continuar ");
                getchar();
                getchar();
                break;
            case 2:
                system("clear");
                printf("Ingrese el nombre de la categoría donde añadir la foto: ");
                scanf(" %s", nombreCategoria);
                cat = inicio;
                found = 0;
                if (cat)
                {
                    do {
                        if (strcmp(cat->nombre, nombreCategoria) == 0)
                        {
                           do {
                                printf("\nIngrese la clave única: ");
                                scanf("%d", &datosFoto.claveUnica);

                                // Verificar si la clave única ya existe
                                if (buscarFotoPorClaveUnica(cat, datosFoto.claveUnica) != NULL)
                                  {
                                     printf("La clave única %d ya existe en la categoría, ingrese otra.\n", datosFoto.claveUnica);
                                  }
                               } while (buscarFotoPorClaveUnica(cat, datosFoto.claveUnica) != NULL);
                            printf("\nIngrese Descripción de la Foto: ");
                            scanf(" %[^\n]", datosFoto.descripcion);
                            printf("\nIngrese Nombre del Archivo: ");
                            scanf(" %[^\n]", datosFoto.archivo);
                            printf("\nIngrese Estado (1 para baja, 0 para activa): ");
                            scanf("%d", &datosFoto.baja);
                            system("clear");
                            insertarFoto(cat, datosFoto, &raiz);
                            printf("\nFoto Cargada\n");
                            printf("\nPresiona enter para continuar ");
                            getchar();
                            getchar();
                            found = 1;
                            break;
                        }
                        cat = cat->der;
                    } while (cat != inicio);
                    if (!found) 
                    {
                        printf("Categoría no encontrada.\n");
                    }
                } else 
                {
                    printf("No hay categorías disponibles. Añada una categoría primero.\n");
                }
                break;
            case 3:
                system("clear");
                printf("Ingrese el nombre de la categoría a eliminar: ");
                scanf(" %s", nombreCategoria);
                eliminarCategoria(&inicio, nombreCategoria);
                printf("\nCategoria eliminada.\n");
                printf("\nPresiona enter para continuar ");
                getchar();
                getchar();
                break;
            case 4:
                system("clear");
                printf("Ingrese el nombre de la categoría de la foto a eliminar: ");
                scanf(" %s", nombreCategoria);
                printf("\nIngrese Clave Única de la Foto a eliminar: ");
                scanf("%d", &datosFoto.claveUnica);
                eliminarFoto(buscarCategoria(inicio, nombreCategoria), datosFoto.claveUnica);
                printf("\nFoto eliminada.\n");
                printf("\nPresiona enter para continuar ");
                getchar();
                getchar();
                break;
            case 5:
                system("clear");
                printf("Ingrese el nombre de la categoría de la foto a modificar: ");
                scanf(" %s", nombreCategoria);
                printf("Ingrese Clave Única de la Foto: ");
                scanf("%d", &datosFoto.claveUnica);
                printf("Ingrese Nueva Descripción de la Foto: ");
                scanf(" %s", datosFoto.descripcion);
                printf("Ingrese Nuevo Nombre del Archivo: ");
                scanf(" %s", datosFoto.archivo);
                printf("Ingrese Nuevo Estado (1 para baja, 0 para activa): ");
                scanf("%d", &datosFoto.baja);
                modificarFoto(buscarCategoria(inicio,nombreCategoria),datosFoto);
                printf("\nFoto modificada.");
                printf("\nPresiona enter para continuar ");
                getchar();
                getchar();
                break;
            case 6:
                system("clear");
                imprimirCategorias(inicio);
                printf("\nPresiona Enter para continuar ");
                getchar();
                getchar();
                break;
            case 7:
                system("clear");
                printf("Ingrese el nombre de la categoría para imprimir fotos: ");
                scanf(" %s", nombreCategoria);
                system("clear");
                imprimirFotos(buscarCategoria(inicio, nombreCategoria));
                printf("\nPresiona enter para continuar ");
                getchar();
                getchar();
                break;
            case 8:
                system("clear");
                printf("Ingrese nombre del archivo binario para guardar: ");
                scanf(" %s", nombreArchivo);
                system("clear");
                guardarEnArchivoBinario(inicio, nombreArchivo);
                printf("\nPresiona enter para continuar ");
                getchar();
                getchar();
                break;
            case 9:
                system("clear");
                printf("Ingrese el nombre del archivo binario que desea imprimir: ");
                scanf("%s", nombreArchivo);
                imprimirArchivoBinario(nombreArchivo);
                printf("\nPresiona enter para continuar ");
                getchar();
                getchar();
                break;
            case 10:
                system("clear");
                navegadorCategoriasFotos(inicio);
                printf("\nPresiona enter para continuar ");
                getchar();
                getchar();
            break;
            case 11:
            system("clear");
            printf("\n¿Clave de la FOTO que quieres buscar?: ");
            scanf("%d", &busca);
            val = buscarDirecto(raiz, busca);

            // Comprobar el resultado
            if (val == 1) 
            {

                printf("\nEl dato existe\n");
                printf("\nClave unica %d encontrada.\n", busca);
                printf("\nCategoría: %s, Foto ID: %d, Descripción: %s, Archivo: %s, Estado: %s\n", raiz->categoria->nombre, raiz->datos.claveUnica, raiz->datos.descripcion, raiz->datos.archivo, raiz->datos.baja ? "Dada de baja" : "Activa");
            } 
            else 
            {
                printf("Clave unica %d no encontrada.\n", busca);
            }
                printf("\nPresiona enter para continuar ");
                getchar();
                getchar();
            break;
            case 0:
                guardarDatosBin(inicio);
                system("clear");
                printf("Saliendo...\n");
                break;
            default:
                system("clear");
                printf("Opción no válida. Por favor intente de nuevo.\n");
        } 
    } while (opcion != 0);

    // Liberar memoria y realizar la limpieza aquí

    return 0;
}







