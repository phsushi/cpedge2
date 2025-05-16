#include <Wire.h> // Biblioteca utilizada para fazer a comunicação com o I2C
#include <LiquidCrystal_I2C.h> // Biblioteca utilizada para fazer a comunicação com o display 20x4 
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT22
#define DHTPIN 8
DHT_Unified dht(DHTPIN,DHTTYPE);

#define col 16 // Serve para definir o numero de colunas do display utilizado
#define lin  2 // Serve para definir o numero de linhas do display utilizado
#define ende  0x27 // Serve para definir o endereço do display.
LiquidCrystal_I2C lcd(ende,col,lin); // Chamada da funcação LiquidCrystal para ser usada com o I2C

int valorLumi = 0;

void checaTemperatura(){
  lcd.clear();
  lcd.setCursor(0,0);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    lcd.print("Erro na leitura");
  } else {
    float temp = event.temperature;
    if (temp >= 10.0 && temp <= 15.0) {
      lcd.print("Temperatura OK");
      lcd.setCursor(0, 1);
      lcd.print("Temp. = " + String(temp));
    } else if (temp < 10.0) {
      lcd.print("Temp. Baixa");
      lcd.setCursor(0, 1);
      lcd.print("Temp. = " + String(temp));
    } else if (temp > 15.0) {
      lcd.print("Temp. Alta");
      lcd.setCursor(0, 1);
      lcd.print("Temp. = " + String(temp));
    }
  }
  delay(3000);
}
void checaUmidade(){
  lcd.clear();
  lcd.setCursor(0,0);
  sensors_event_t event;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    lcd.print("Erro na leitura");
  } else {
    float umidade = event.relative_humidity;
    if(umidade > 70){
      lcd.print("Umidade Alta");
      lcd.setCursor(0,1);
      lcd.print("Umidade = "+String(umidade)+"%");
    }else if(umidade < 50){
      lcd.print("Umidade Baixa");
      lcd.setCursor(0,1);
      lcd.print("Umidade = "+String(umidade)+"%");
    }else if(umidade <= 70 && umidade >=50){
      lcd.print("Umidade OK");
      lcd.setCursor(0,1);
      lcd.print("Umidade = "+String(umidade)+"%");
    }
  }
  delay(3000);
}


void setup()
{
  dht.begin();
  lcd.init(); // Serve para iniciar a comunicação com o display já conectado
  lcd.backlight(); // Serve para ligar a luz do display
  lcd.clear(); // Serve para limpar a tela do display
  lcd.setCursor(0,0);           
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(A0,INPUT);
  pinMode(11,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  digitalWrite(11,HIGH);
  valorLumi = analogRead(A0);
  Serial.println(valorLumi);
  if(valorLumi >= 250){
    //valorLumi = analogRead(A0);
    digitalWrite(2,HIGH);
    digitalWrite(6,LOW);
    digitalWrite(4,LOW);
    digitalWrite(11,LOW);
    checaTemperatura();
    checaUmidade();
  }
  else if(valorLumi < 250 && valorLumi > 125){
    //valorLumi = analogRead(A0);
    digitalWrite(4,HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ambiente:"); 
    lcd.setCursor(0,1);
    lcd.print("a meia luz");
    digitalWrite(2,LOW);
    digitalWrite(6,LOW);
    digitalWrite(11,LOW);
    delay(3000);
    checaTemperatura();
    checaUmidade();
  }
  else if(valorLumi <= 125 ){
    //valorLumi = analogRead(A0);
    digitalWrite(6,HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ambiente:"); 
    lcd.setCursor(0,1);
    lcd.print("muito claro");
    digitalWrite(11,HIGH);
    digitalWrite(4,LOW);
    digitalWrite(2,LOW);
    delay(3000);
    checaTemperatura();
    checaUmidade();
  }
  lcd.clear();
}