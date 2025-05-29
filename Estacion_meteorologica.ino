#include "DHT.h"

const int R=5;
const int B=6;
const int G=7;
int ldr = 0;
bool pulsador=false;
#define DHTTYPE DHT11
float humedad = 0;
float temperatura = 0;

const int DHTPin = 8;
DHT dht(DHTPin, DHTTYPE);

void setup(){
  pinMode(R, OUTPUT);	
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(A5, INPUT);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  pinMode(8, INPUT);
  dht.begin();
}

void loop(){
  //DHT-11
  humedad = dht.readHumidity();
  temperatura = dht.readTemperature();
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.print(" % ");
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" *C");


  //Led RGB
  digitalWrite(R, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(B, HIGH);
  
  //Fotoresistencia
  ldr = analogRead(A5);
  Serial.println(ldr);
  analogWrite(12, map(ldr, 0, 1023, 0, 255));
  
  //Pulsador
  Serial.println(pulsador);
  pulsador = digitalRead(4);
  if (pulsador == HIGH) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
    delay(5000);
  }
  
  //Delay
  delay(5000);
}