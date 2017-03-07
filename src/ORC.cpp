#include "ORC.h"
HC_SR04 ultrasonido(PIN_SONAR_TRIGGER, PIN_SONAR_ECHO, 0,500);
Servo servoRobot;

void inicializar()
{//Inicializa librerías y servo
  ultrasonido.begin();
  Serial.begin(57600);
  servoRobot.attach(PIN_SERVO);
  servoRobot.write(90);
  //Mapeo de pines
  pinMode(PIN_IN1,OUTPUT);
  pinMode(PIN_IN2,OUTPUT);
  pinMode(PIN_IN3,OUTPUT);
  pinMode(PIN_IN4,OUTPUT);
  pinMode(PIN_LED_1,OUTPUT);
  pinMode(PIN_LED_2,OUTPUT);
  pinMode(PIN_LED_3,OUTPUT);
  pinMode(PIN_LINE_L,INPUT);
  pinMode(PIN_LINE_R,INPUT);
  pinMode(PIN_PULSADOR,INPUT_PULLUP);
  pinMode(PIN_FC1,INPUT_PULLUP);
  pinMode(PIN_FC2,INPUT_PULLUP);
//conversor A/D rápido
  #if FASTADC
  sbi(ADCSRA, ADPS2) ;
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
  #endif

  //Enciende el LED rojo
LEDs(true,false,false);
}

void LEDs(bool R, bool Y, bool G)
{//Autoexplanatorio
digitalWrite(PIN_LED_1,R);
digitalWrite(PIN_LED_2,Y);
digitalWrite(PIN_LED_3,G);
}

void motores(int mot_L, int mot_R, int* limites)
{
  //Restringe el rango de la entrada
  mot_L=constrain(mot_L,-255,255);
  mot_R=constrain(mot_R,-255,255);
  //Mapea la entrada a los rangos máximos de los motores para asegurar proporcionalidad
  int MOT_L_MIN=limites[0];
  int MOT_L_MAX=limites[1];
  int MOT_R_MIN=limites[2];
  int MOT_R_MAX=limites[3];

  mot_L=map(mot_L,-255,255,MOT_L_MIN,MOT_L_MAX);
  mot_R=map(mot_R,-255,255,MOT_R_MIN,MOT_R_MAX);
  //Lógica de decisión:
  if(mot_L>0)
  {
    analogWrite(PIN_IN1,mot_L);
    analogWrite(PIN_IN2,0);
  }
  else if(mot_L==0)
  {
    analogWrite(PIN_IN1,0);
    analogWrite(PIN_IN2,0);
  }
  else
  {
    analogWrite(PIN_IN1,0);
    analogWrite(PIN_IN2,-1*mot_L);
  }

  if(mot_R>0)
  {
    analogWrite(PIN_IN3,mot_R);
    analogWrite(PIN_IN4,0);
  }
  else if(mot_R==0)
  {
    analogWrite(PIN_IN3,0);
    analogWrite(PIN_IN4,0);
  }
  else
  {
    analogWrite(PIN_IN3,0);
    analogWrite(PIN_IN4,-1*mot_R);
  }
}

void espera()
{//Se espera a que el usuario pulse el botón
  while(digitalRead(PIN_PULSADOR));
  //Parpadea el LED amarillo 3 veces
LEDs(false,true,false);
delay(500);
LEDs(false,false,false);
delay(500);
LEDs(false,true,false);
delay(500);
LEDs(false,false,false);
delay(500);
LEDs(false,true,false);
delay(500);
LEDs(false,false,false);
delay(500);
//Enciende el LED verde y vuelve al programa
LEDs(false,false,true);
}

int* lee_linea()
{ /*
  La función se usa de la siguiente manera:
  int* vector_de_lineas;
  vector_de_lineas=lee_linea();
  //A partir de aquí en vector_de_lineas están disponibles los valores.
  vector_de_lineas[0];//Izquierda (entero 0-1023)
  vector_de_lineas[1];//Derecha (entero 0-1023)
  */
    //Autoexplanatorio
 static int valores_linea[2];
  valores_linea[0]= analogRead(PIN_LINE_L);
  valores_linea[1]= analogRead(PIN_LINE_R);
  return valores_linea;
}
bool* lee_linea_bin(int limite)
{/*
  La función se usa de la siguiente manera:
  bool* vector_de_lineas;
  vector_de_lineas=lee_linea_bin();
  //A partir de aquí en vector_de_lineas están disponibles los valores.
  vector_de_lineas[0];//Izquierda (lógico)
  vector_de_lineas[1];//Derecha (lógico)
  */
    //Autoexplanatorio
  static int valores_linea_bin[2];
  static bool salida_bin[2];
  valores_linea_bin[0]= analogRead(PIN_LINE_L);
  valores_linea_bin[1]= analogRead(PIN_LINE_R);
  if(valores_linea_bin[0]>limite)
  {
    salida_bin[0]=false;
  }
  else{
    salida_bin[0]=true;
  }
  if(valores_linea_bin[1]>limite)
  {
    salida_bin[1]=false;
  }
  else{
    salida_bin[1]=true;
  }
  return salida_bin;
}

int lee_distancia()
{ //Autoexplanatorio
  static float dist_sonar;
ultrasonido.start();
  while(!ultrasonido.isFinished());
      dist_sonar = ultrasonido.getRange();

      return constrain(int(dist_sonar),0,200);
}

int* barrido(int semicono, int muestras)
{ /*
  La función se usa de la siguiente manera:
  int* vector_barrido;
  vector_barrido=barrido(semicono, muestras);
  "semicono" es el ángulo, medido hacia un sólo lado, que barre el servo
  está restringido de 0 a 90º
  "muestras" es el número de medidas que hace en ese ángulo, si se indica 1
  sólo lee un punto hacia delante. Está restringido de 1 a 4.
  //A partir de aquí en vector_barrido están disponibles los valores en el formato:
  vector_barrido[0]= posición en -semicono;
  vector_barrido[1]=distancia en posición -semicono;
  .
  .
  .
  vector_barrido[2*(2*(muestras-1))-2]= posición en +semicono;
  vector_barrido[2*(2*(muestras-1))-1]= distancia en posición -semicono;

  NOTA: el vector que devuelve la función siempre es de tamaño máximo, por lo que
  es posible acceder a las posiciones más allá de las pedidas a la función.
  */
  //Retardos para el programa
  int retardos=400;
  int retardo_inicial=1000;
  int retardo_final=450;
  //Límite de valores
  semicono=constrain(semicono,0,90);
  muestras=constrain(muestras,1,4);
  int angulo;
  bool primero=true;
  int k=0;
  static int salida_barrido[14]; //Primer elemento, ángulo, segundo distancia
  //Si sólo se pide una muestra:
if(muestras==1)
{
  servoRobot.write(90);
  delay(retardos);
  salida_barrido[0]=0;
  salida_barrido[1]=lee_distancia();
}
else //Para más de una muestra
{
  for(int i=-(muestras-1);i<muestras;i++)
  {
    angulo=int(90+i*(float)(semicono/(muestras-1)));
    //Debugging
    if(DEBUG_BARRIDO)
    {
      Serial.print("Angulo: ");
      Serial.println(angulo);
    }
    servoRobot.write(angulo);
    //Si es el primero, espera distinto
    if(primero)
    {
      delay(retardo_inicial);
      primero=false;
    }else if(i==(muestras-1))     //Si es el último, espera distinto
    {
      delay(retardo_final);
    }
    else{
    delay(retardos);
  }
    salida_barrido[k]=angulo-90;
    k++;
    salida_barrido[k]=lee_distancia();
    k++;
  }
}
//Vuelve a poner el servo mirando hacia delante
servoRobot.write(90);
  return salida_barrido;
}

int* extremos()
{/*
  La función se usa de la siguiente manera:
  int* vector_extremos;
  vector_extremos=extremos();
  //A partir de aquí en vector_extremos están disponibles los valores en el formato:
  vector_barrido[0]= posición del mínimo;
  vector_barrido[1]=distancia mínima;
  vector_barrido[2]= posición del máximo;
  vector_barrido[3]=distancia máxima;
  */
   static int salida_extremos[4];
  int* valores;
  valores=barrido(90,4);
  int min=500;
  volatile int i_min=0;
  int max=0;
  volatile int i_max=0;
  //Recorremos el barrido
  for(int i=1;i<14;i+=2)
  {
    if(DEBUG_EXTREMOS)
    {
      Serial.println(valores[i]);
    }
    if((valores[i]>max)&&(valores[i]!=-1))
    {
      max=valores[i];
      i_max=i;
    }
   else if((valores[i]<min)&&(valores[i]!=-1))
    {
      min=valores[i];
      i_min=i;
    }
  }
  //Debugging
  if(DEBUG_EXTREMOS)
  {
    Serial.print("I_min: ");
    Serial.print(i_min);
    Serial.print("  I_max: ");
    Serial.println(i_max);
  }
  salida_extremos[0]=valores[i_min-1];
  salida_extremos[1]=min;
  salida_extremos[2]=valores[i_max-1];
  salida_extremos[3]=max;
  return salida_extremos;
}
