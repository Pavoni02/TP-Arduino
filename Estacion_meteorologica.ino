#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//Constantes y definiciones

#define DHTTYPE DHT11
LiquidCrystal_I2C lcd(0x27,16,2);

const int SENSOR_LLUVIA_PIN=A0;
const int LDR_PIN=A2;

const int GREEN=2;
const int BLUE=3;
const int RED=4;
const int PULSADOR_PIN=7;
const int DHTPIN=8;
const int LED_HUMEDAD=10;
const int LED_LDR=11;

DHT dht(DHTPIN, DHTTYPE);

//Variables

float humedad = 0;
float temperatura = 0;

int ldr = 0;
int sensor_lluvia = 0;
int contador_flancos = 0;

bool pulsador=false;
bool estado_actual = HIGH; // Estado actual del pulsador
bool estado_anterior = HIGH;

char texto[40];
char texto_flotante[40];
char texto_porcentaje[3] = "%";

unsigned long tiempo_inicial = 0;
unsigned long tiempo_inicial_2 = 0;
unsigned long tiempo_inicial_3 = 0;

//Funciones

void dht11();
void fotoresistencia();
void medicion_lluvia();
void led_rgb_verde();
void led_rgb_amarillo();
void led_rgb_rojo();
void display();
void alerta_temperatura();

void setup(){
  pinMode(SENSOR_LLUVIA_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(RED, OUTPUT);	
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(PULSADOR_PIN, INPUT_PULLUP);
  pinMode(DHTPIN, INPUT);
  pinMode(LED_HUMEDAD, OUTPUT);
  pinMode(LED_LDR, OUTPUT);

  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();

  tiempo_inicial=millis();
  tiempo_inicial_2=millis();
  tiempo_inicial_3=millis();
  dht11();
  fotoresistencia();
  medicion_lluvia();
  display();
  Serial.println("------------------------------");
}

void loop(){
  if (millis()-tiempo_inicial_2 >= 1000){
    lcd.scrollDisplayLeft();
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

  pulsador = digitalRead(PULSADOR_PIN);
  estado_actual = digitalRead(PULSADOR_PIN);

  //Detecta el flanco de high a low
  if (estado_anterior == HIGH && estado_actual == LOW && millis()-tiempo_inicial_3 >= 800) {
    tiempo_inicial_3=millis();
    contador_flancos++;
    Serial.print("Flanco detectado: ");
    Serial.println(contador_flancos);
    dht11();
    fotoresistencia();
    medicion_lluvia();
    display();
    Serial.println("------------------------------");
  }
  estado_anterior = estado_actual;
}

//Funciones

void dht11(){
  humedad = dht.readHumidity();
  temperatura = dht.readTemperature();
  //humedad = 65.00;
  if(humedad < 60.00){
    digitalWrite(LED_HUMEDAD, LOW);
  }
  else{
    analogWrite(LED_HUMEDAD, map(humedad, 58, 100, 0, 255));
  }
}

void fotoresistencia(){
  ldr = analogRead(LDR_PIN);
  if(ldr > 600){
    digitalWrite(LED_LDR, LOW);
  }
  else{
    analogWrite(LED_LDR, map(ldr, 0, 650, 255, 0));
  }
}

void medicion_lluvia(){
  sensor_lluvia = analogRead(SENSOR_LLUVIA_PIN);  //lectura digital de pin
}

void led_rgb_verde(){
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);
}

void led_rgb_amarillo(){
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);
}

void led_rgb_rojo(){
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
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
  //Se usa valor 5 o mayor para evitar falsas alarmas por salpicaduras menores
  if (sensor_lluvia >= 5) {
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