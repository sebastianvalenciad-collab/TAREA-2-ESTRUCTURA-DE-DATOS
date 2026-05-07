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


// Funcion para Mostrar  la lista de directores que hacen una pelicula.
void mostrar_director(Film *peli) 
{
  char *director = list_first(peli->directors); // usamos el primer director
  while(director != NULL) // si director es null entonces es porque no hay mas
  {
    printf("%s ", director); // printeamos
    director = list_next(peli->directors); // avanzamos al director siguiente, si no hay otro es NULL y se acaba el codigo
  }
  printf("\n");
}

// Funcion para Mostrar la lista de generos de una pelicula
void mostrar_generos(Film *peli)
{
  char *genero = list_first(peli->genres); // usamos el primer genero para recorrer la lista
  while(genero != NULL) // si el genero es NULL no hay mas generos en la lista
  {
    printf("%s ", genero); 
    genero = list_next(peli->genres); // avanzamos al siguiente genero
  }
  printf("\n");
}

// Funcion para mostrar los datos de una pelicula
void mostrar_pelicula(Film *peli) 
{
  // Solo printeamos los datos de la pelicula
  printf("========================================\n"); 
  printf("ID de la pelicula: %s\n", peli->id);
  printf("Titulo: %s\n", peli->title);
  printf("Año de lanzamiento: %d\n", peli->year);
  printf("Director/s: ");
  mostrar_director(peli);
  printf("Genero/s: ");
  mostrar_generos(peli);
  
  printf("Notas de los usuarios: "); 
  MapPair *nota_pair = map_first(peli->ratings); // obtenemos el primer par del mapa de notas
  if(nota_pair == NULL) // si es NULL entonces no hay notas en el mapa
  { 
    printf("No hay notas para esta pelicula.");
  }
  else // si hay mapas entonces recorremos el mapa hasta que encontremos un NULL
  {
    while(nota_pair != NULL)
    {
      Rating *nota = (Rating *) nota_pair->value; // rescatamos la nota
      printf("%s: %d | ", nota->username, nota->nota); // printeamos el nombre del usuario y la nota
      nota_pair = map_next(peli->ratings); // avanzamos en el mapa
    }
  }
  printf("\n========================================\n");
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
    char *director = list_first(peli->directors); // obtenemos el primer director de la lista de directores de la pelicula
    
    while(director != NULL) // recorremos todos los directores de la pelicula
    {
      MapPair *director_pair = map_search(pelis_bydirector, director); // Buscamos el director en el mapa de peliculas de directores

      if(director_pair == NULL) // si el director no esta
      {
        List *newlist = list_create(); // creamos una lista
        list_pushBack(newlist, peli); // agregamos la pelicula a la lista
        map_insert(pelis_bydirector, director, newlist); // y al mapa de directores le agregamos el director y la lista de peliculas
      }
      else // si el director esta agregamos la pelicula actual a la lista del director
      {
        List *director_list = (List *) director_pair->value;  
        list_pushBack(director_list, peli);
      }
      director = list_next(peli->directors); // Avanzamos al next si es que hay mas de 1 director
    }

    // DECADA
    int decada = (peli->year / 10) * 10; // obtenemos la decada 
    MapPair *decada_pair = map_search(pelis_bydecada, &decada); // buscamos la decada

    if(decada_pair == NULL) // si no esta la agregamos 
    {
      int *key = (int *) malloc(sizeof(int));
      *key = decada; // la llave es la decada
      List *newlist = list_create();
      list_pushBack(newlist, peli); // e insertamos la pelicula a la decada correspondiente
      map_insert(pelis_bydecada, key, newlist);
    }
    else
    {
      List *decada_list = (List *) decada_pair->value; // si la decada esta solo agregamos la pelicula
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
  char id[100]; // Buffer para almacenar el ID de la película

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
  char genero[100]; // variable que almacena el input del usuario
  printf("Ingresar genero: ");
  scanf(" %[^\n]", genero);

  MapPair *genero_pair = map_search(pelis_bygenres, genero); // buscamos si la entrada del usuario esta en el mapa de generos
  if(genero_pair == NULL) // si es NULL es porque no existe en el mapa
  {
    printf("No se encontraron peliculas de este genero.\n");
    return;
  }

  List *genero_lista = (List *) genero_pair->value; // obtenemos la lista de peliculas asociadas al genero
  Film *peli = list_first(genero_lista); //obtenemos la primera pelicula de la lista
  
  while(peli != NULL) // si la peli es NULL se acaba
  {
    mostrar_pelicula(peli); // printeamos los datos de la pelicula
    peli = list_next(genero_lista); // avanzamos a la siguiente pelicula
  }
}


// BUSCAR POR DIRECTOR
void buscar_por_director(Map *pelis_bydirector)
{
  char director[100]; // variable que almacena la entrada
  printf("Ingresar nombre del director: ");
  scanf(" %[^\n]", director); //

  MapPair *director_pair = map_search(pelis_bydirector, director); // Buscamos si la entrada el usuario esta en el mapa de directores
  
  if(director_pair == NULL) // si es NULL no existe en el mapa
  {
    printf("No se encontraron peliculas de este director\n");
    return;
  }

  List *directores_lista = (List *) director_pair->value; // obtenemos la lista de peliculas asociadas al director
  Film *peli = list_first(directores_lista); // obtyenemos la primera pelicula de la lista

  while(peli != NULL) // si es NULL se acaba el ciclo
  {
    mostrar_pelicula(peli); // mostramos los datos de las peliculas
    peli = list_next(directores_lista); // avanzamos al siguiente elemento de la lista de peliculas de directores
  }
}


//BUSCAR POR DECADA
void buscar_por_decada(Map *pelis_bydecada)
{
  int decada; // variable que guarda la decada
  printf("Ingresar decada a buscar: ");
  scanf("%d", &decada);
  
  printf("\n");
  
  MapPair *decada_pair = map_search(pelis_bydecada, &decada); // buscamos si la decada esta en el mapa de directores

  if(decada_pair == NULL) // si es NULL no existe esa decada en el mapa
  {
    printf("No se encontraron peliculas de esta decada\n");
    return;
  }

  List *decada_list = (List *) decada_pair->value; // obtenemos la lista de  peliculas asociadas a la decada
  Film *peli = list_first(decada_list); // obtenemos la primera pelicula de la lista

  while(peli != NULL)
  {
    mostrar_pelicula(peli); //  mostramos los datos de cada pelicula
    peli = list_next(decada_list); // avanzamos a la siguiente pelicula
  } 
}

//BUSQUEDA AVANZADA
void busqueda_avanzada(Map *pelis_bygenres, Map *pelis_bydecada)
{
  char genero[100]; // variable que guarda el genero
  int decada; // variable que guarda la decada

  printf("Ingresar genero: ");
  scanf(" %[^\n]", genero);

  printf("Ingresar decada: ");
  scanf("%d", &decada);

  MapPair *generos_pair = map_search(pelis_bygenres, genero); // buscamos si el genero esta en el mapa de generos
  MapPair *decada_pair = map_search(pelis_bydecada, &decada); // buscamos si la decada esta en el mapa de decadas

  if(generos_pair == NULL || decada_pair == NULL) // si alguno es NULL no se pueden encontrar peliculas
  {
    printf("No se encontraron peliculas\n");
    return;
  }

  List *genero_lista = (List *) generos_pair->value; // obtenemos la lista de peliculas asociadas al genero
  List *decada_lista = (List *) decada_pair->value; // obtenemos la lista de peliculas asociadas a la decada

  Film *peli = list_first(genero_lista); // obtenemos la primera pelicula de la lista de peliculas del genero
  while(peli != NULL) // recorremos hasta que sea NULL la pelicula
  {
    Film *decada_peliculas = list_first(decada_lista); // obtenemos la primera pelicula de la lista de decada
    while(decada_peliculas != NULL)
    {
      if(strcmp(peli->id, decada_peliculas->id) == 0) // comparamos ID's de la pelicula de generos y de decadadas si son iguales entran al if
      {
        mostrar_pelicula(peli); // mostramos la pelicula que esta en ambas listas
      }
      decada_peliculas = list_next(decada_lista); // avanzamos a la siguiente posicion
    }
    peli = list_next(genero_lista); // avanzamos a la siguiente posicion
  }
}

// AGREGAR PELICULA A WATCHLIST
void agregar_watchlist(Map *pelis_byid, List *watchlist)
{
  printf("\n");
  char id[100]; // variable que guarda la id
  printf("Ingresar ID de la pelicula a agregar: ");
  scanf("%s", id);

  MapPair *pelicula = map_search(pelis_byid, id); // buscamos la pelicula por el id
  if(pelicula == NULL) // si es NULL la pelicula no existe
  {
    printf("No se encontro la pelicula  o no existe\n");
    return;
  }
  
  Film *peli = list_first(watchlist); // tenemos el primer elemento de la lista watchlist
  Film *nueva = (Film *) pelicula->value; // obtenemos la pelicula del ID anterior

  while(peli != NULL) // VERIFICAMOS QUE NO SE REPITA LA PELICULA
  {
    if(strcmp(peli->id, id) == 0) // si son iguales significa que la pelicula ya fue agregada, entonces no se agrega
    {
      printf("La pelicula ya esta en la watchlist\n");
      return;
    }
    peli = list_next(watchlist); // avanzamos en la watchlist
  }
  list_pushBack(watchlist, nueva);  // si no retornamos, entonces agregamos la pelicula a la watchlist
  printf("Pelicula agregada correctamente a la watchlist.\n");
}


// MOSTRAR WATCHLIST
void mostrar_watchlist(List *watchlist)
{
  printf("\n");
  Film *peli = list_first(watchlist); // obtenemos el primer elem,ento de la lista
  if(peli == NULL) // Si es NULL Es porque no hay peliculas en la lista
  {
    printf("No hay peliculas guardadas\n");
    return;
  }

  while(peli != NULL) // recorremos y mostramos los datos de las peliculas de la watchlist
  {
    mostrar_pelicula(peli);
    peli = list_next(watchlist);
  }
}

// ELIMINAR WATCHLIST
void eliminar_watchlist(List *watchlist)
{
  char id[100]; // variable que guarda el id
  printf("Ingresar ID de la pelicula a ELIMINAR: ");
  scanf("%s", id);
  
  Film *peli = list_first(watchlist); // obtenemos la primera pelicula de la watchlist
  while(peli != NULL) 
  {
    if(strcmp(peli->id, id) == 0) // comparamos los ID's
    {
      list_popCurrent(watchlist); // si son iguales eliminamos la pelicula de la watchlist
      printf("Pelicula eliminada correctamente.\n");
      return;
    }
    peli = list_next(watchlist); // avanzamos una posicion
  }
  printf("La pelicula no esta en la watchlist.\n");
}

// MENU DE LA WATCHLIST
void gestionar_watchlist(Map *pelis_byid, List *watchlist)
{
  char opcion; // escoges una opcion entre 1 y 4 y haces la operacion que escogiste
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
  char id[100]; // Variable que guarda el ID
  char username[100]; // Variable que guarda el username
  int nota; // Variable que guarda la nota

  printf("Ingresar ID de la pelicula: ");
  scanf("%s", id);

  printf("Ingresar usuario: ");
  scanf(" %[^\n]", username);

  printf("Ingresar nota (1-10): ");
  scanf("%d",&nota);

  if(nota < 1 || nota > 10) // verifiqcamos que la nota este en el rango entre 1 y 10
  {
    printf("Nota invalida (fuera de rango)\n");
    return;
  }

  MapPair *pair = map_search(pelis_byid, id); // buscamos la pelicula
  if(pair == NULL) // verificamos si existe
  {
    printf("La pelicula NO existe.\n");
    return;
  }
  
  Film *peli = (Film *) pair->value; // obtenemos la pelicula
  
  MapPair *rating_pair = map_search(peli->ratings, username); // vemos si el usuario ya califico esta pelicula
  if(rating_pair != NULL) // si ya existe actualizamos la nota
  {
    Rating *rating = (Rating *) rating_pair->value;
    rating->nota = nota;
    printf("Nota actualizada!\n");
  }
  else // si no la califico, creamos una nueva nota y guardamos los datos
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
  Map *pelis_byid = map_create(is_equal_str); // mapa de ID's
  Map *pelis_bygenres = map_create(is_equal_str); // mapa de generos
  Map *pelis_bydirector = map_create(is_equal_str); // mapa de directores
  Map *pelis_bydecada = map_create(is_equal_int); // mapa de decadas
  
  List *watchlist = list_create(); // lista de watchlist
  
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
