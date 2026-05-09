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

## Ejemplo de uso

Paso 1: Cargar catalogo de peliculas.
Primero, el usuario carga el archivo CSV que contiene todas las peliculas desponibles.
<img width="293" height="252" alt="image" src="https://github.com/user-attachments/assets/10bded5a-6200-4b46-8b09-e37e96ce247f" />
El sistema procesa el archivo CSV y almacena la informacion en memoria utilizando mapas y listas.

Paso 2: Buscar una pelicula por genero.
Ahora el usuario desea buscar peliculas del genero "Drama".
<img width="253" height="170" alt="image" src="https://github.com/user-attachments/assets/8ab24ec2-9b16-4944-bccb-ac77624e58ea" />
El sistema muestra todas las peliculas asociadas a ese genero.
<img width="350" height="221" alt="image" src="https://github.com/user-attachments/assets/5c61e96a-86a8-44b3-92c8-d7ea1236a636" />

Paso 3: Agregar una pelicula a la Watchlist.
El usuario decide guardar una pelicula para verla mas tarde.
<img width="294" height="111" alt="image" src="https://github.com/user-attachments/assets/cf318af7-f91c-40d5-800c-f3a4cc18471c" />
El sistema agrega correctamente la pelicula a la watchlist.

Paso 4: Calificar una pelicula.
Luego de ver la pelicula, el usuario decide ponerle nota a la pelicula.
<img width="232" height="74" alt="image" src="https://github.com/user-attachments/assets/c522d0b1-a141-4860-9a60-75564fc48777" />

Paso 5: Mostrar la watchlist
El usuario finalmente decide revisar las peliculas guardadas de su lista
<img width="256" height="200" alt="image" src="https://github.com/user-attachments/assets/8deb8fb7-1508-48c3-8ab6-099feb0c8fa9" />

