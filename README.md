# Evaluación Intermedia
Implementación de redes de área amplia y servicios distribuidos

- A01424875 - Marlon Yahir Martínez Chacón
- A01734997 - Christian Flores Alberto
- A01735331 - Sarai Santiago Lozano
- A01736353 - Alejandro Daniel Moctezuma Cruz

## Wiki

Si deseas acceder a la Wiki puedes hacer clic [aquí](https://github.com/AlejandroMoc/ImplementaciónRedes/wiki).

## Archivos a revisar

### Interfaz de usuario

Para abrir la interfaz de usuario y seleccionar la modificación
aplicada a una imagen, ejecutar el siguiente comando:

    cd UI/
    python interface_design.py

### Multiprocesamiento de imágenes

Para ejecutar la interfaz:

    python image_processing_interface.py

Para ejecutar el archivo escrito en C de forma independiente
es necesario colocar las imágenes en la carpeta
"Images/Original" y ejecutar:

    gcc -fopenmp image_processing.c
    ./a.out

<!-- ## Licencia

Este proyecto cuenta con la licensia GPLv3, la cual estipula que no se puede vender ni demandar por la auditoría del programa. -->