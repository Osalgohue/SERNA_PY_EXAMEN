<<<<<<< Updated upstream
# SERNA_PY_EXAMEN

# Generador de Exámenes

Aplicación de consola en C que permite generar, modificar y aplicar exámenes de opción múltiple.

**Materia:** Estructura de Datos I
**Proyecto Final:** Generador de exámenes

---

## Tabla de contenido

- [Requisitos](#requisitos)
- [Compilación](#compilación)
- [Uso](#uso)
- [Formato del archivo de examen](#formato-del-archivo-de-examen)
- [Estructuras de datos utilizadas](#estructuras-de-datos-utilizadas)
- [Limitaciones conocidas](#limitaciones-conocidas)

---

## Requisitos

- Un compilador de C
- **Windows:** MinGW 
- **Linux / macOS:** gcc 

## Compilación

> **Importante:** Siempre que crees un nuevo examen al final tienes que ponel el `.txt` sino no va a funcionar y guarda estos archivos desde donde se copila no a un lado del proyecto.

### Linux / macOS

```bash
cd ruta/a/la/carpeta/del/proyecto
gcc -Wall -o examen PY.c
./examen
```

### Windows (MinGW)

```bash
cd ruta\a\la\carpeta\del\proyecto
gcc -Wall -o examen.exe PY.c
examen.exe
```

El programa detecta automáticamente el sistema operativo en tiempo de compilación para el manejo de archivos y limpieza de pantalla — el mismo código fuente compila en ambos sistemas sin modificaciones.

## Uso

Al ejecutar el programa se muestra un menú con 4 opciones:

```
========================================
   GENERADOR DE EXAMENES
========================================
1. Generar examen
2. Modificar examen
3. Aplicar examen
4. Salir
Selecciona una opcion:
```

### 1. Generar examen

Pregunta, las 4 opciones de respuesta, cuál es la correcta (1-4) y los puntos asignados. Después de cada reactivo pregunta si quieres agregar otro (`S`/`N`). Al terminar, guarda todo en un archivo `.txt` con el nombre que elijas.

### 2. Modificar examen

Si no hay un examen cargado en memoria, primero muestra los archivos `.txt` disponibles en la carpeta y pide cuál cargar. Luego imprime la lista de reactivos numerados, pide cuál quieres modificar y si confirmas, vuelve a pedir todos los datos de ese reactivo. Guarda los cambios automáticamente al terminar.

### 3. Aplicar examen

Muestra los archivos `.txt` disponibles, carga el examen elegido y entra al modo quiz:

```
a = anterior | d = siguiente | 1-4 = responder | f = finalizar
```

- `a` / `d` — navega entre preguntas
- `1`-`4` — selecciona tu respuesta (puedes cambiarla mientras no hayas finalizado)
- `f` — finaliza el examen y muestra la calificación final

### 4. Salir

Libera toda la memoria reservada dinámicamente y cierra el programa.

## Formato del archivo de examen

Cada reactivo se guarda en texto plano con el siguiente formato:

```
:p;Texto de la pregunta
:op1;Texto de la opción 1
:op2;Texto de la opción 2
:op3;Texto de la opción 3
:op4;Texto de la opción 4
:r;opN
PUNTOS.
```

Donde `N` es el número de la opción correcta (1-4) 
Y`PUNTOS` es el valor numérico asignado al reactivo, seguido de un punto. 
Se pueden incluir varios reactivos seguidos en el mismo archivo.

**Nota:** el archivo debe tener la extensión `.txt` para que aparezca en el listado de exámenes disponibles.

## Estructuras de datos utilizadas

- **Lista doblemente enlazada** (`LISTA_EXAMEN` / `REACTIVO`): cada reactivo es un nodo con punteros `ANTERIOR` y `SIGUIENTE`, lo que permite recorrer el examen en ambas direcciones — usado tanto al editar reactivos como al navegar el quiz en Aplicar examen.
- **Memoria dinámica**: cada reactivo se reserva con `malloc` (`CREAR_REACTIVO_VACIO`) y toda la lista se libera explícitamente con `free` antes de cerrar el programa o antes de cargar un examen distinto (`LIBERAR_LISTA`), para evitar fugas de memoria.
- **Manejo de archivos**: lectura y escritura con `fopen`/`fprintf`/`fgets`/`fclose` usando el formato de texto descrito arriba.

## Limitaciones conocidas

- El nombre del archivo debe escribirse con la extensión `.txt` al guardar; si se omite, el archivo se guarda igual pero no aparece en el listado de "exámenes disponibles" (que filtra por `.txt`).
- El menú principal no valida que la entrada sea numérica — si se escribe una letra en vez de un número, puede comportarse de forma inesperada.
- Los nombres de archivo no pueden contener espacios, ya que se leen con `scanf("%s", ...)`.
- La función de listar archivos disponibles (`LISTAR_EXAMENES_DISPONIBLES`) usa `dirent.h` en Linux/macOS y la API de Windows (`FindFirstFile`/`FindNextFile`) en Windows, seleccionadas en tiempo de compilación con `#ifdef _WIN32`.
=======
# SERNA_PY_EXAMEN_Documentacion

Este codigo fue escrito en Linux distro Debian 13.5 pero esta hecho para funcionar en Windows

/*================ listar examenes funcional en windows y linux ======================*/

Este apartado esta hecho para poder soportar Windows y Linux
Para encotrar archivos en los 2 tipos de sistemas operativos 
>>>>>>> Stashed changes
