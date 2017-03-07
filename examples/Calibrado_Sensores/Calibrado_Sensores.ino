#include <HC_SR04.h>
#include <ORC.h>
// Ejemplo Librería para el kit de robótica de la ORC 2.17 Organizada por Makers UPV
// http://www.makersupv.com
// Incluye la librería de Ultrasonidos HC-SR04: http://playground.arduino.cc/Code/SR04
// Codificado por Guillermo Orts y por Germán Torres

//Límites de motores
const int MOT_L_MIN = -255;
const int MOT_L_MAX = 255;
const int MOT_R_MIN = -255;
const int MOT_R_MAX = 255;

const int limite = 512; //Límite para distinguir línea de blanco

#define tipo_sensor 6//1: IR Lineas analógico/2: Líneas digital/3: Ultrasonidos/4: barrido/5: extremos
int retardo_muestras = 100;

void setup() {
//Inicializa la librería
  inicializar();
  Serial.println("Calibrado de Sensores");
}

void loop() {
  int distancia = 0;
  bool* linea_b;
  int* linea;
  int* valores_distancias;
  switch (tipo_sensor)
  {
    case 1: //Linea analógico

      linea = lee_linea();
      Serial.print("Sensor L:");
      Serial.print(linea[0]);
      Serial.print(" Sensor R:");
      Serial.print(linea[1]);
      break;

    case 2:  //Linea digital

      linea_b = lee_linea_bin(limite);
      Serial.print("Sensor L:");
      Serial.print(linea_b[0]);
      Serial.print(" Sensor R:");
      Serial.print(linea_b[1]);
      break;

    case 3://Distancia

      distancia = lee_distancia();
      Serial.print("Distancia: ");
      Serial.print(distancia);
      Serial.print(" cm");
      break;

    case 4://Barrido
      valores_distancias = barrido(90, 4);
      for (int i = 0; i < 14; i++)
      {
        if ((i % 2) == 0)
        {
          Serial.print("Angulo: ");
        }
        else
        {
          Serial.print("Distancia: ");
        }
        Serial.print(valores_distancias[i]);
        Serial.println("");
      }
      break;
    case 5:
      valores_distancias = extremos();
      Serial.print("Min pos: ");
      Serial.print(valores_distancias[0]);

      Serial.print(" Min: ");
      Serial.print(valores_distancias[1]);

      Serial.print(" Max pos: ");
      Serial.print(valores_distancias[2]);

      Serial.print(" Max: ");
      Serial.print(valores_distancias[3]);
      break;
  }
  Serial.println();
  delay(retardo_muestras); //Espera 1 segundo
}
