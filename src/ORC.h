//Librería para el kit de robótica de la ORC 2.16 Organizada por Makers UPV
// http://www.makersupv.com
// Incluye la librería de Ultrasonidos HC-SR04: http://playground.arduino.cc/Code/SR04
// Codificado por Guillermo Orts y por Germán Torres

//Definición de pines y constantes
#ifndef ORC
#define ORC
#endif

//Librerías base
#include <Arduino.h>
#include <Servo.h>
#include <HC_SR04.h>

#define PIN_IN1 5
#define PIN_IN2 3
#define PIN_IN3 11
#define PIN_IN4 6
#define PIN_SERVO 13
#define PIN_LINE_L A0 //Analog 0
#define PIN_LINE_R A1 //Analog 1
#define PIN_COLOR_R A2 //Analog 2
#define PIN_COLOR_G A3 //Analog 3
#define PIN_COLOR_B A4//Analog 4
#define PIN_COLOR_SENSE A6//Analog 6
#define PIN_SONAR_TRIGGER 9
#define PIN_SONAR_ECHO 2
#define PIN_PULSADOR 4
#define PIN_FC1 7
#define PIN_FC2 8
#define PIN_LED_1 10
#define PIN_LED_2 12
#define PIN_LED_3 A5 //Analog 5

// Conversión A/D Rápida
#define FASTADC 1
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//Debugging por Serie
#define DEBUG_BARRIDO false
#define DEBUG_EXTREMOS false
#define DEBUG_ESTADISTICAS false
#define DEBUG_ESTADISTICAS1 false
#define DEBUG_ESTADISTICAS2 false

//Funciones disponibles:
void inicializar(); //Inicializa E/S y pone el ADC en modo rápido
void inicializar_calibracion_color(int* valores_r,int* valores_g,int* valores_b,int* valores_w,int* valores_black);
void LEDs(bool R,bool Y,bool G); //Enciende o apaga los LEDs de estado del robot.
void RGB(bool R,bool G,bool B); //Función auxiliar para el sensor de color, enciende el LED RGB en el color que toca.
void motores(int mot_L, int mot_R,int* limites); //Enciende los motores en la velocidad seleccionada por el usuario, recibe dos enteros de -255 a 255 y un vector de 4 enteros con los límites por motor y sentido de giro.
void espera();//Espera inicial del robot con cuenta atrás.
int* lee_color();// Devuelve un vector de 3 elementos con los valores de rojo, verde y azul del sensor de color.
int* lee_linea(); //Devuelve un vector de 2 elementos con los valores de los sensores de línea izquierdo y derecho.
bool* lee_linea_bin(int limite); //Devuelve un vector de 2 elementos binarios con que indican si hay línea o no basado en el límite pasado a la función.
int lee_distancia();// Devuelve la distancia en cm registrada por el sensor de ultrasonidos.
int* barrido(int semicono, int muestras); //Devuelve un vector de tamaño 2*(2*(muestras-1)) con las posiciones angulares y las distancias del barrido.
int* extremos();// Devuelve los caminos de mínima y máxima distancia y su orientación respecto al robot.
int lee_color_calibrado(int tolerancia_color); //Devuelve el color en función de la calibración inicial. Un valor de tolerancia razonable es 25.
//Nota: código de colores: -1: color no reconocido/0: color rojo/1: color verde/2: color azul/3: color blanco/4: color negro
