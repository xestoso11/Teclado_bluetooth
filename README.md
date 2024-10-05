# Teclado Bluetooth con ESP32

Este proyecto consiste en un teclado Bluetooth personalizado basado en el microcontrolador ESP32. Permite la simulación de pulsaciones de teclas a través de una conexión Bluetooth y también incluye la funcionalidad para actualizar la configuración de las teclas mediante un sistema de archivos SPIFFS.

## Descripción

El archivo **`teclado_bluetooth_fs.ino`** contiene el código que implementa las siguientes funcionalidades:

- **Conexión Bluetooth**: Utiliza la biblioteca `BleKeyboard` para establecer una conexión Bluetooth con dispositivos compatibles, simulando un teclado.
- **Teclado Matricial**: Integra un teclado matricial de 4x5, donde cada tecla está mapeada a un carácter específico.
- **Sistema de Archivos SPIFFS**: Permite almacenar y actualizar la configuración de las teclas en el sistema de archivos SPIFFS del ESP32.

## Requisitos

- Un microcontrolador ESP32.
- Un teclado matricial de 4x5 conectado a los pines definidos en el código.
- Biblioteca `BleKeyboard` instalada en el entorno de desarrollo de Arduino.
- El sistema de archivos SPIFFS debe estar habilitado.

## Configuración

### Hardware

1. Conectar el teclado matricial a los siguientes pines del ESP32:
   - Filas: 15, 2, 0, 4
   - Columnas: 13, 12, 14, 27, 26

### Software

1. Abrir el archivo `teclado_bluetooth_fs.ino` en el IDE de Arduino.
2. Asegurarse de que la biblioteca `BleKeyboard` esté instalada.
3. Cargar el código en el ESP32.

## Uso

- Al conectar el ESP32 a un dispositivo Bluetooth, este se comportará como un teclado Bluetooth.
- Al presionar las teclas del teclado matricial, se enviarán las pulsaciones correspondientes a través de Bluetooth.
- Puedes actualizar las teclas a través de comandos enviados por el puerto serie, usando los comandos:
  - **`setKey <index> <newKey>`**: Para establecer una nueva tecla en el índice especificado.
  - **`delete`**: Para eliminar el archivo de configuración de teclas.

## Notas

- Asegúrate de que el ESP32 esté conectado a un dispositivo compatible con Bluetooth.
- Verifica que los pines del teclado matricial estén conectados correctamente para un funcionamiento óptimo.
- El código incluye una implementación básica para el manejo de errores y mensajes de estado a través del puerto serie.

## Contribuciones

Las contribuciones son bienvenidas. Si encuentras un error o deseas mejorar la funcionalidad, no dudes en abrir un *issue* o un *pull request*.
