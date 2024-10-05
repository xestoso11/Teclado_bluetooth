#include <BleKeyboard.h>
#include <FS.h>
#include "SPIFFS.h"

const int numRows = 4;
const int numCols = 5;
const int rowPins[] = {15, 2, 0, 4}; // Define los pines conectados a las filas del teclado matricial
const int colPins[] = {13, 12, 14, 27, 26}; // Define los pines conectados a las columnas del teclado matricial
char keyMap[numRows][numCols] = {
  {'1', '2', '3', 'A', 'B'},
  {'4', '5', '6', 'C', 'D'},
  {'7', '8', '9', 'G', 'H'},
  {'U', '0', 'V', 'J', 'K'}
};

BleKeyboard bleKeyboard;

void setup() {
  Serial.begin(115200);
  bleKeyboard.begin();
  if (!SPIFFS.begin(true)) {
    Serial.println("Error al formatear e inicializar SPIFFS");
    return;
  }
  if (!SPIFFS.exists("/keys.txt")) {
    File file = SPIFFS.open("/keys.txt", "w");
    if (file) {
      for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
          file.print(keyMap[i][j]);
        }
      }
      file.close();
      Serial.println("Archivo en SPIFFS creado");
    } else {
      Serial.println("Error al crear el archivo en SPIFFS");
    }
  }
}

void loop() {
  if (bleKeyboard.isConnected()) {
    for (int i = 0; i < numRows; i++) {
      pinMode(rowPins[i], OUTPUT);
      digitalWrite(rowPins[i], LOW);
      for (int j = 0; j < numCols; j++) {
        pinMode(colPins[j], INPUT_PULLUP);
        if (digitalRead(colPins[j]) == LOW) {
          delay(20); // Breve retardo para debounce
          if (digitalRead(colPins[j]) == LOW) { // Verificar si el botón sigue presionado
            char key = keyMap[i][j];
            bleKeyboard.press(KEY_LEFT_CTRL);
            bleKeyboard.press(KEY_LEFT_SHIFT);
            bleKeyboard.press(KEY_LEFT_ALT);
            bleKeyboard.write(key);
            bleKeyboard.releaseAll();
            while (digitalRead(colPins[j]) == LOW) {
              // Esperar hasta que el botón sea liberado para evitar múltiples activaciones
              delay(10);
            }
          }
        }
        pinMode(colPins[j], INPUT); // Liberar columna
      }
      digitalWrite(rowPins[i], HIGH); // Liberar fila
    }
    delay(50); // Reducir el retardo para hacer que el bucle sea más rápido
  }
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input.startsWith("setKey")) {
      updatekeys(input.substring(6));
    } else if (input.startsWith("delete")) {
      deleteFile("/keys.txt");
    } else {
      Serial.println("Comando invalido, solo sirve setKey y delete");
    }
  }
}

void updatekeys(String input) {
  if (input.length() >= 9) {
    int buttonIndex = input.charAt(0) - '0';
    char newKey = input.charAt(2);
    if (buttonIndex >= 0 && buttonIndex < numRows * numCols) {
      int rowIndex = buttonIndex / numCols;
      int colIndex = buttonIndex % numCols;
      keyMap[rowIndex][colIndex] = newKey;
      writeKeyToSPIFFS(rowIndex, colIndex, newKey);
      Serial.println("Tecla cambiada correctamente");
    } else {
      Serial.println("Boton no valido");
    }
  } else {
    Serial.println("Entrada invalida");
  }
}

char readKeyFromSPIFFS(int rowIndex, int colIndex) {
  File file = SPIFFS.open("/keys.txt", "r");
  if (file) {
    file.seek(rowIndex * numCols + colIndex);
    char key = file.read();
    file.close();
    return key;
  } else {
    Serial.println("Error al abrir el archivo en SPIFFS para lectura");
    return ' '; // Otra opción para manejar errores
  }
}

void writeKeyToSPIFFS(int rowIndex, int colIndex, char newKey) {
  File file = SPIFFS.open("/keys.txt", "r+");
  if (file) {
    file.seek(rowIndex * numCols + colIndex);
    file.write(newKey);
    file.close();
  } else {
    Serial.println("Error al abrir el archivo en SPIFFS para escritura");
  }
}

void deleteFile(const char* nombreArchivo) {
  if (SPIFFS.remove(nombreArchivo)) {
    Serial.println("Archivo eliminado con éxito");
  } else {
    Serial.println("Error al eliminar el archivo");
  }
}
