// ||===================================||
// ||      CREATED by FERY SETIAWAN     ||
// ||      Technology of Univercity     ||
// ||             YOGYAKARTA            ||
// ||===================================||
// 10/08/2024



#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#include "Page.h"


#define push_a 4
#define push_b 5

int antriA = 0;
int antriB = 0;
int jumlahAntri_A = 10; 
int jumlahAntri_B = 10; 
int sisaA;
int sisaB;

const char* ssid = "Fry_sn";
const char* password = "16062k04";

AsyncWebServer server(80);

String data_B() {
  String json = "{\"antriB\":";
  json += antriB;
  json += ",\"jumlahB\":";
  json += jumlahAntri_B;
  json += ",\"sisaB\":";
  json += sisaB;
  json += "}";
  return json;
}

String data_A() {
  String json = "{\"antriA\":";
  json += antriA;
  json += ",\"jumlahA\":";
  json += jumlahAntri_A;
  json += ",\"sisaA\":";
  json += sisaA;
  json += "}";
  return json;
}

String view(){
  String json = "{\"antriA\":";
  json += antriA;
  json += ",\"antriB\":";
  json += antriB;
  json += "}";
  return json;
}

int sisa_A(){
  int hasil = jumlahAntri_A - antriA;
  return hasil;
}

int sisa_B(){
  int hasil = jumlahAntri_B - antriB;
  return hasil;
}

void print_s(String perintah, String sesi, int nilai, int nilaisisa){
    Serial.print(perintah);
    Serial.print(" -> ");
    Serial.print(sesi);
    Serial.print(" -> ");
    Serial.print(nilai);
    Serial.print(" -> ");
    Serial.println(nilaisisa);
}

void lcd_standby(){
  lcd.setCursor(2, 0);
  lcd.print("Tekan Tombol");
}


void print_lcd(String sesi, int j ){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No. Antrian Anda");
  lcd.setCursor(0, 1);
  lcd.print(sesi);
  lcd.print(" - ");
  lcd.print(j);
  delay(2000);
  lcd.clear();
  lcd_standby();
}

void setup(){
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_page);
    });

    server.on("/antri_a", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", antri_a);
    });

    server.on("/antri_b", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", antri_b);
    });

    // server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
    //     request->send(200, "application/javascript", javaPage);
    // });
    
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/css", cssPage);
    });

    server.on("/next", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String button_id = request->getParam("buttonId")->value();

        if (button_id == "a"){
          antriA++;
          sisa_A();
          sisaA = sisa_A();
          print_s("Next", button_id, antriA, sisa_A());
        } else if (button_id == "b"){
          antriB++;
          sisa_B();
          sisaB = sisa_B();
          print_s("Next", button_id, antriB, sisa_B());
        } else {
          request->send(400, "text/plain", "Data Failed");
        }

        request->send(200, "text/plain", "Data Received");
    });


    server.on("/prev", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String button_id = request->getParam("buttonId")->value();

        if (button_id == "a"){
          antriA--;
          sisaA = sisaA + 1;
          print_s("Prev", button_id, antriA, sisaA);
        } else if (button_id == "b"){
          antriB--;
          sisaB = sisaB + 1;
          print_s("Prev", button_id, antriB, sisaB);
        } else {
          request->send(400, "text/plain", "Data Failed");
        }

        request->send(200, "text/plain", "Data Received");
      // request->send(400, "text/plain", "Parameter 'lamp' hilang");
    });

    server.on("/reset", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String buttom_id = request->getParam("buttomId")->value();

        Serial.print("Next -> ");
        Serial.println(buttom_id);
        request->send(200, "text/plain", "Data Received");
      // request->send(400, "text/plain", "Parameter 'lamp' hilang");
    });

    server.on("/update_a", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "application/json", data_A());
    });

    server.on("/update_b", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "application/json", data_B());
    });

    server.on("/update_view", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "application/json", view());
    });

    
    server.begin();

    pinMode(push_a, INPUT_PULLUP);
    pinMode(push_b, INPUT_PULLUP);

    lcd.begin();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Sistem Antrian");
    lcd.setCursor(0, 1);
    lcd.print("IP:");
    lcd.print(WiFi.localIP());
    delay(2000);
    lcd.clear();
    lcd_standby();
}

void loop(){
  int k_push_a = digitalRead(push_a);
  int k_push_b = digitalRead(push_b);
  if(k_push_a == LOW){
    delay(250);
    jumlahAntri_A++;
    sisa_A();
    sisaA = sisa_A();
    Serial.print("Jumlah Antrian A = ");
    Serial.println(jumlahAntri_A);
    print_lcd("A", jumlahAntri_A);
  }

  if(k_push_b == LOW){
    delay(250);
    jumlahAntri_B++;
    sisa_B();
    sisaB = sisa_B();
    Serial.print("Jumlah Antrian B = ");
    Serial.println(jumlahAntri_B);
    print_lcd("B", jumlahAntri_B);
  }
}



