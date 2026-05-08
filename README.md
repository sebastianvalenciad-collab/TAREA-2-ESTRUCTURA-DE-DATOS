## Descripcion
Este programa permite cargar un catálogo de películas y realizar búsquedas por género, director, década o ID. Además, incluye una watchlist donde el usuario puede agregar películas para ver más tarde, eliminarlas o visualizar las películas guardadas. Finalmente, el sistema permite calificar las películas vistas por los usuarios.

## Como ejecutar el codigo
Este programa fue realizado en replit asi que les explicare como hacerlo funcionar en esta plataforma.
Para ejecutar la tarea1 primero debemos seguir los siguientes pasos:

1. Accede al repositorio de este codigo, y crea un nuevo repositorio usando el template
````
https://github.com/sebastianvalenciad-collab/TAREA-2-ESTRUCTURA-DE-DATOS
````
2. Coloca en la opción Import code or design. Luego importa desde Github.
3. Pega la dirección de tu repositorio y luego impórtalo (Import from Github).
4. Agrega una pestaña “Shell”
5. y ejecuta los siguientes codigos:
````
gcc tdas/*.c tarea2.c -Wno-unused-result -o tarea2
````

Y luego ejecutar:
````
./tarea2
````

## Funcionalidades

En este codigo usamos mapas y listas, usamos un mapa para los ID's, generos, directores, decadas y en algunas funcionalidades mas pequeñas. Y usamos listas para la watchlist y para almacenar las peliculas en los mapas.
Tambien desarrollamos las siguientes funcionalidades, las cuales todas estan operativas:

- Cargar catalogo
- Buscar por ID
- Buscar por genero
- Buscar por director
- Buscar por Decada
- Busqueda avanzada
- Gestionar mi watchlist
- Calificar pelicula
