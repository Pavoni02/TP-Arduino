#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);

const int R=5;
const int B=6;
const int G=7;
int ldr = 0;
bool pulsador=false;
#define DHTTYPE DHT11
float humedad = 0;
float temperatura = 0;
int sensor_lluvia = 0;

bool estadoActual = HIGH;     // Estado actual del pulsador
bool estadoAnterior = HIGH;

int contadorFlancos = 0; 

char texto[40];
char texto_flotante[40];
char texto_porcentaje[3] = "%";

unsigned long int tiempo_inicial = 0;
unsigned long int tiempo_inicial_2 = 0;

const int DHTPin=8;
DHT dht(DHTPin, DHTTYPE);

void dht11();
void fotoresistencia();
void medicion_lluvia();
void led_rgb_verde();
void led_rgb_amarillo();
void led_rgb_rojo();
void display();
void alerta_temperatura();

void setup(){
  lcd.init();
  lcd.backlight();

  pinMode(A0, INPUT);
  pinMode(R, OUTPUT);	
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(A5, INPUT);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  pinMode(8, INPUT);
  dht.begin();

  tiempo_inicial=millis();
  tiempo_inicial_2=millis();
  dht11();
  fotoresistencia();
  medicion_lluvia();
  display();
  Serial.println("------------------------------");
}

void loop(){
  if (millis()-tiempo_inicial_2 >= 1000){
    lcd.scrollDisplayLeft();
    tiempo_inicial_2=millis();
  }

  if (millis()-tiempo_inicial >= 60000){
    dht11();
    fotoresistencia();
    medicion_lluvia();
    display();
    tiempo_inicial=millis();
    Serial.println("------------------------------");
  }
  if (temperatura>30 || temperatura<5){
    led_rgb_rojo();
    alerta_temperatura();
  }
  else{
    if(temperatura>25 || temperatura<15){
      led_rgb_amarillo();
    }
    else{
      led_rgb_verde();
    }
  }

  pulsador = digitalRead(4);
  estadoActual = digitalRead(4);

  //Detecta el flanco de high a low
  if (estadoAnterior == HIGH && estadoActual == LOW) {
    contadorFlancos++;
    Serial.print("Flanco detectado: ");
    Serial.println(contadorFlancos);
    dht11();
    fotoresistencia();
    medicion_lluvia();
    display();
    Serial.println("------------------------------");
  }
  estadoAnterior = estadoActual;
}

void dht11(){
  humedad = dht.readHumidity();
  temperatura = dht.readTemperature();
}

void fotoresistencia(){
  ldr = analogRead(A3);
  analogWrite(11, map(ldr, 0, 1023, 255, 0));
}

void medicion_lluvia(){
  sensor_lluvia = analogRead(A0);  //lectura digital de pin
}

void led_rgb_verde(){
  digitalWrite(R, LOW);
  digitalWrite(G, HIGH);
  digitalWrite(B, LOW);
}

void led_rgb_amarillo(){
  digitalWrite(R, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(B, LOW);
}

void led_rgb_rojo(){
  digitalWrite(R, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(B, LOW);
}

void display(){
  lcd.clear();
  lcd.setCursor(0, 0);
  dtostrf(temperatura, 2, 2, texto_flotante);
  sprintf(texto, "Temperatura: %s *C", texto_flotante);
  lcd.print(texto);
  Serial.println(texto);
  lcd.setCursor(0,1);
  dtostrf(humedad, 2, 2, texto_flotante);
  sprintf(texto, "Humedad: %s %s", texto_flotante, texto_porcentaje);
  lcd.print(texto);
  Serial.println(texto);
  sprintf(texto, "Sensor de lluvia: %d", sensor_lluvia);
  Serial.println(texto);
  sprintf(texto, "LDR: %d", ldr);
  Serial.println(texto);
  if (sensor_lluvia == HIGH) {
      Serial.println("Detectada lluvia");
  }
  else{
    Serial.println("No hay lluvia");
  }
  tiempo_inicial_2=millis();
}

void alerta_temperatura(){
  lcd.setCursor(0,1);
  lcd.print("Temperatura Extrema Detectada");
  Serial.println("Temperatura Extrema Detectada");
}