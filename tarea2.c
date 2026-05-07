#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char title[100];
  List *genres;
  List *directors;
  int year;
  Map *ratings;
} Film;

typedef struct 
{
  char username[100];
  int nota;
} Rating;


void mostrar_director(Film *peli)
{
  char *director = list_first(peli->directors);
  while(director != NULL)
  {
    printf("%s ", director);
    director = list_next(peli->directors);
  }
  printf("\n");
}

void mostrar_generos(Film *peli)
{
  char *genero = list_first(peli->genres);
  while(genero != NULL)
  {
    printf("%s ", genero);
    genero = list_next(peli->genres);
  }
  printf("\n");
}

void mostrar_pelicula(Film *peli)
{
  printf("========================================");
  printf("ID de la pelicula: %s\n", peli->id);
  printf("Titulo: %s\n", peli->title);
  printf("Año de lanzamiento: %d\n", peli->year);
  printf("Director/s: ");
  mostrar_director(peli);
  printf("Genero/s: ");
  mostrar_generos(peli);
  
  printf("Notas de los usuarios: ");
  MapPair *nota_pair = map_first(peli->ratings);
  if(nota_pair == NULL)
  {
    printf("No hay notas para esta pelicula.");
  }
  else
  {
    while(nota_pair != NULL)
    {
      Rating *nota = (Rating *) nota_pair->value;
      printf("%s: %d | ", nota->username, nota->nota);
      nota_pair = map_next(peli->ratings);
    }
  }
  printf("========================================");
  printf("\n\n");
}


// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por ID");
  puts("3) Buscar por Genero");
  puts("4) Buscar por Director");
  puts("5) Buscar por Decada");
  puts("6) Busqueda Avanzada");
  puts("7) Gestionar Watchlist");
  puts("8) Calificar Pelicula");
  puts("9) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

/**
 * Carga películas desde un archivo CSV y las almacena en un mapa por ID.
 */
void cargar_peliculas(Map *pelis_byid, Map *pelis_bygenres, Map *pelis_bydirector, Map *pelis_bydecada) {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // Crea una nueva estructura Film y almacena los datos de cada película
    Film *peli = (Film *)malloc(sizeof(Film));
    peli->directors = split_string(campos[14], ","); // separamos directores
    strcpy(peli->id, campos[1]);        // Asigna ID
    strcpy(peli->title, campos[5]);     // Asigna título
    peli->genres = split_string(campos[11], ",");       // Inicializa la lista de géneros
    peli->year = atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero
    peli->ratings = map_create(is_equal_str); // inicializamos el mapa de los ratings
    
    // Inserta la película en el mapa usando el ID como clave
    map_insert(pelis_byid, peli->id, peli);
  
    // Obtiene el primer género de la lista de géneros de la película
    char *genre = list_first(peli->genres);
    // Itera sobre cada género de la película
    while (genre != NULL) {
        // Busca el género en el mapa pelis_bygenres
        MapPair *genre_pair = map_search(pelis_bygenres, genre);

        // Si el género no existe en el mapa, crea una nueva lista y agrégala al mapa
        if (genre_pair == NULL) {
            List *new_list = list_create();
            list_pushBack(new_list, peli);
            map_insert(pelis_bygenres, genre, new_list);
        } else {
            // Si el género ya existe en el mapa, obtén la lista y agrega la película
            List *genre_list = (List *)genre_pair->value;
            list_pushBack(genre_list, peli);
        }

        // Avanza al siguiente género en la lista
        genre = list_next(peli->genres);
    }
    
    // DIRECTORES
    char *director = list_first(peli->directors);

    while(director != NULL)
    {
      MapPair *director_pair = map_search(pelis_bydirector, director);

      if(director_pair == NULL)
      {
        List *newlist = list_create();
        list_pushBack(newlist, peli);
        map_insert(pelis_bydirector, director, newlist);
      }
      else
      {
        List *director_list = (List *) director_pair->value;
        list_pushBack(director_list, peli);
      }
      director = list_next(peli->directors); // Avanzamos al next si es que hay mas de 1 director
    }

    // DECADA
    int decada = (peli->year / 10) * 10;
    MapPair *decada_pair = map_search(pelis_bydecada, &decada);

    if(decada_pair == NULL)
    {
      int *key = (int *) malloc(sizeof(int));
      *key = decada;
      List *newlist = list_create();
      list_pushBack(newlist, peli);
      map_insert(pelis_bydecada, key, newlist);
    }
    else
    {
      List *decada_list = (List *) decada_pair->value;
      list_pushBack(decada_list, peli);
    }
    
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas


  // Itera sobre el mapa para mostrar las películas cargadas
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    printf("ID: %s, Título: %s, Año: %d\n", peli->id, peli->title,
           peli->year);

    printf("Géneros: ");
    for(char *genre = list_first(peli->genres); genre != NULL; genre = list_next(peli->genres))
      printf("%s, ", genre);
    printf("\n");
    
    pair = map_next(pelis_byid); // Avanza al siguiente par en el mapa
  }
}


/**
 * Busca y muestra la información de una película por su ID en un mapa.
 */
void buscar_por_id(Map *pelis_byid) {
  char id[10]; // Buffer para almacenar el ID de la película

  // Solicita al usuario el ID de la película
  printf("Ingrese el id de la película: ");
  scanf("%s", id); // Lee el ID del teclado

  // Busca el par clave-valor en el mapa usando el ID proporcionado
  MapPair *pair = map_search(pelis_byid, id);

  // Si se encontró el par clave-valor, se extrae y muestra la información de la
  // película
  if (pair != NULL) {
    Film *peli =
        pair->value; // Obtiene el puntero a la estructura de la película
    // Muestra el título y el año de la película
    mostrar_pelicula(peli);
  } else {
    // Si no se encuentra la película, informa al usuario
    printf("La película con id %s no existe\n", id);
  }
}

// ---------------------------------------------------------------------------- 

// BUSCAR POR GENERO
void buscar_por_genero(Map *pelis_bygenres)
{
  char genero[100];
  printf("Ingresar genero: ");
  scanf(" %[^\n]", genero);

  MapPair *genero_pair = map_search(pelis_bygenres, genero);
  if(genero_pair == NULL)
  {
    printf("No se encontraron peliculas de este genero.\n");
    return;
  }

  List *genero_lista = (List *) genero_pair->value;
  Film *peli = list_first(genero_lista);
  
  while(peli != NULL)
  {
    mostrar_pelicula(peli);
    peli = list_next(genero_lista);
  }
}


// BUSCAR POR DIRECTOR
void buscar_por_director(Map *pelis_bydirector)
{
  char director[100];
  printf("Ingresar nombre del director: ");
  scanf(" %[^\n]", director); // VERIFICAR

  MapPair *director_pair = map_search(pelis_bydirector, director);
  
  if(director_pair == NULL)
  {
    printf("No se encontraron peliculas de este director\n");
    return;
  }

  List *directores_lista = (List *) director_pair->value;
  Film *peli = list_first(directores_lista);

  while(peli != NULL)
  {
    printf("%s\n", peli->title);
    peli = list_next(directores_lista);
  }
}


//BUSCAR POR DECADA
void buscar_por_decada(Map *pelis_bydecada)
{
  int decada;
  printf("Ingresar decada a buscar: ");
  scanf("%d", &decada);
  
  printf("\n");
  
  MapPair *decada_pair = map_search(pelis_bydecada, &decada);

  if(decada_pair == NULL)
  {
    printf("No se encontraron peliculas de esta decada\n");
    return;
  }

  List *decada_list = (List *) decada_pair->value;
  Film *peli = list_first(decada_list);

  while(peli != NULL)
  {
    mostrar_pelicula(peli);
    peli = list_next(decada_list);
  }
}

//BUSQUEDA AVANZADA
void busqueda_avanzada(Map *pelis_bygenres, Map *pelis_bydecada)
{
  char genero[100];
  int decada;

  printf("Ingresar genero: ");
  scanf(" %[^\n]", genero);

  printf("Ingresar decada: ");
  scanf("%d", &decada);

  MapPair *generos_pair = map_search(pelis_bygenres, genero);
  MapPair *decada_pair = map_search(pelis_bydecada, &decada);

  if(generos_pair == NULL || decada_pair == NULL)
  {
    printf("No se encontraron peliculas\n");
    return;
  }

  List *genero_lista = (List *) generos_pair->value;
  List *decada_lista = (List *) decada_pair->value;

  Film *peli = list_first(genero_lista);
  while(peli != NULL)
  {
    Film *decada_peliculas = list_first(decada_lista);
    while(decada_peliculas != NULL)
    {
      if(strcmp(peli->id, decada_peliculas->id) == 0) // comparamos ID's
      {
        mostrar_pelicula(peli);
      }
      decada_peliculas = list_next(decada_lista);
    }
    peli = list_next(genero_lista);
  }
}

// AGREGAR PELICULA A WATCHLIST
void agregar_watchlist(Map *pelis_byid, List *watchlist)
{
  printf("\n");
  char id[100];
  printf("Ingresar ID de la pelicula a agregar: ");
  scanf("%s", id);

  MapPair *pelicula = map_search(pelis_byid, id);
  if(pelicula == NULL)
  {
    printf("No se encontro la pelicula  o no existe\n");
    return;
  }
  
  Film *peli = list_first(watchlist);
  Film *nueva = (Film *) pelicula->value;

  while(peli != NULL) // VERIFICAMOS QUE NO SE REPITA LA PELICULA
  {
    if(strcmp(peli->id, id) == 0)
    {
      printf("La pelicula ya esta en la watchlist\n");
      return;
    }
    peli = list_next(watchlist);
  }
  list_pushBack(watchlist, nueva);
  printf("Pelicula agregada correctamente a la watchlist.\n");
}


// MOSTRAR WATCHLIST
void mostrar_watchlist(List *watchlist)
{
  printf("\n");
  Film *peli = list_first(watchlist);
  if(peli == NULL)
  {
    printf("No hay peliculas guardadas\n");
    return;
  }

  while(peli != NULL)
  {
    mostrar_pelicula(peli);
    peli = list_next(watchlist);
  }
}

void eliminar_watchlist(List *watchlist)
{
  char id[100];
  printf("Ingresar ID de la pelicula a ELIMINAR: ");
  scanf("%s", id);
  
  Film *peli = list_first(watchlist);
  while(peli != NULL)
  {
    if(strcmp(peli->id, id) == 0)
    {
      list_popCurrent(watchlist);
      printf("Pelicula eliminada correctamente.\n");
      return;
    }
    peli = list_next(watchlist);
  }
  printf("La pelicula no esta en la watchlist.\n");
}

void gestionar_watchlist(Map *pelis_byid, List *watchlist)
{
  char opcion;
  do
  {
    printf("\n");
    printf("1) Agregar Pelicular\n");
    printf("2) Eliminar Pelicula\n");
    printf("3) Mostrar Watchlist\n");
    printf("4) Salir\n");
    printf("Ingresar opcion: ");
    scanf(" %c", &opcion);

    switch(opcion)
    {
      case '1':
        agregar_watchlist(pelis_byid, watchlist);
        break;
      case '2':
        eliminar_watchlist(watchlist);
        break;
      case '3':
        mostrar_watchlist(watchlist);
        break;
    }
    
  } while(opcion != '4');
}


// CALIFICAR PELICULA
void calificar_pelicula(Map *pelis_byid)
{
  printf("\n");
  char id[100];
  char username[100];
  int nota;

  printf("Ingresar ID de la pelicula: ");
  scanf("%s", id);

  printf("Ingresar usuario: ");
  scanf(" %[^\n]", username);

  printf("Ingresar nota (1-10): ");
  scanf("%d",&nota);

  if(nota < 1 || nota > 10)
  {
    printf("Nota invalida (fuera de rango)\n");
    return;
  }

  MapPair *pair = map_search(pelis_byid, id);
  if(pair == NULL)
  {
    printf("La pelicula NO existe.\n");
    return;
  }
  
  Film *peli = (Film *) pair->value;
  
  MapPair *rating_pair = map_search(peli->ratings, username);
  if(rating_pair != NULL)
  {
    Rating *rating = (Rating *) rating_pair->value;
    rating->nota = nota;
    printf("Nota actualizada!\n");
  }
  else
  {
    Rating *newrating = (Rating *) malloc(sizeof(Rating));
    strcpy(newrating->username, username);
    newrating->nota = nota;
    map_insert(peli->ratings, newrating->username, newrating);
    printf("Nota agregada correctamente!\n");
  }
}

int main() {
  char opcion; // Variable para almacenar una opción ingresada por el usuario
               // (sin uso en este fragmento)

  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *pelis_byid = map_create(is_equal_str);
  Map *pelis_bygenres = map_create(is_equal_str);
  Map *pelis_bydirector = map_create(is_equal_str);
  Map *pelis_bydecada = map_create(is_equal_int);
  
  List *watchlist = list_create();
  
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(pelis_byid, pelis_bygenres, pelis_bydirector, pelis_bydecada);
      break;
    case '2':
      buscar_por_id(pelis_byid);
      break;
    case '3':
      buscar_por_genero(pelis_bygenres);
      break;
    case '4':
      buscar_por_director(pelis_bydirector);
      break;
    case '5':
      buscar_por_decada(pelis_bydecada);
      break;
    case '6':
      busqueda_avanzada(pelis_bygenres, pelis_bydecada);
      break;
    case '7':
      gestionar_watchlist(pelis_byid, watchlist);
      break;
    case '8':
      calificar_pelicula(pelis_byid);
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '9');

  return 0;
}
