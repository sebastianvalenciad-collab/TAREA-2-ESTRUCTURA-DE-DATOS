## Descripcion
Este programa permite a los usuarios gestionar sus pendientes, ayudándolos a optimizar su tiempo. En él, se puede elegir la categoría a la cual pertenece cada tarea, así como registrar nuevos pendientes. Además, permite visualizar todas las tareas y categorías ingresadas, y también eliminar tanto tareas como categorías.

## Como ejecutar el codigo
Este programa fue realizado en replit asi que les explicare como hacerlo funcionar en esta plataforma.
Para ejecutar la tarea1 primero debemos seguir los siguientes pasos:

1. Accede al repositorio de este codigo, y crea un nuevo repositorio usando el template
2. Coloca en la opción Import code or design. Luego importa desde Github.
3. Pega la dirección de tu repositorio y luego impórtalo (Import from Github).
4. Agrega una pestaña “Shell”
5. y ejecuta los siguientes codigos:
````
gcc tdas/*.c tarea1.c -Wno-unused-result -o tarea1
````

Y luego ejecutar:
````
./tarea1
````

## Funcionalidades

En mi código utilicé una lista para manejar las categorías y otra lista, utilizada como cola, para operar sobre las tareas.
Tambien desarrolle las siguientes funcionalidades, las cuales todas estan operativas:

- Cargar catalogo
- Buscar por ID
- Buscar por genero
- Buscar por director
- Buscar por Decada
- Busqueda avanzada
- Gestionar mi watchlist
- Calificar pelicula
